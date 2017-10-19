/*
 * PhaseVocoder
 *
 * Copyright (c) Terence M. Darwen - tmdarwen.com
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Application/PhaseVocoderMediator.h>
#include <Application/PhaseVocoderProcessor.h>
#include <Application/Transients.h>
#include <Signal/PhaseVocoder.h>
#include <Utilities/Exception.h>
#include <Utilities/Timer.h>
#include <thread>

PhaseVocoderMediator::PhaseVocoderMediator(const PhaseVocoderSettings& settings) : settings_{settings}
{
	InstantiateAudioFileObjects();
}

PhaseVocoderMediator::~PhaseVocoderMediator() { }

void PhaseVocoderMediator::InstantiateAudioFileObjects()
{
	if(!settings_.InputWaveFileGiven())
	{
		Utilities::ThrowException("No input wave file given to PhaseVocoderProcessor");
	}

	audioFileReader_.reset(new ThreadSafeAudioFile::Reader{settings_.GetInputWaveFile()});

	if(settings_.OutputWaveFileGiven())
	{
		std::size_t outputSampleRate{audioFileReader_->GetSampleRate()};
		if(settings_.ResampleValueGiven())
		{
			outputSampleRate = settings_.GetResampleValue();
		}
	
		audioFileWriter_.reset(new ThreadSafeAudioFile::Writer(settings_.GetOutputWaveFile(), 
																static_cast<uint16_t>(audioFileReader_->GetChannels()), 
																static_cast<uint32_t>(outputSampleRate), 
																static_cast<uint16_t>(audioFileReader_->GetBitsPerSample())));
	}
}
void PhaseVocoderMediator::Process()
{
	Utilities::Timer timer(Utilities::Timer::Action::START_NOW);

	if(audioFileReader_->GetChannels() == 1)
	{
		PhaseVocoderProcessor processor(0, settings_, audioFileReader_, audioFileWriter_);
		processor.Process();
		transients_.push_back(processor.GetTransients());
	}
	else if(audioFileReader_->GetChannels() == 2)
	{
		PhaseVocoderProcessor leftChannelProcessor(0, settings_, audioFileReader_, audioFileWriter_);
		PhaseVocoderProcessor rightChannelProcessor(1, settings_, audioFileReader_, audioFileWriter_);
	
		auto leftChannelThread{std::thread([&]{leftChannelProcessor.Process();})};
		auto rightChannelThread{std::thread([&]{rightChannelProcessor.Process();})};

		leftChannelThread.join();
		rightChannelThread.join();

		transients_.push_back(leftChannelProcessor.GetTransients());
		transients_.push_back(rightChannelProcessor.GetTransients());
	}
	else
	{
		Utilities::ThrowException("PhaseVocoder only handles mono or stereo audio data");	
	}

	totalProcessingTime_ = timer.Stop();
}

double PhaseVocoderMediator::GetTotalProcessingTime()
{
	return totalProcessingTime_;
}

std::size_t PhaseVocoderMediator::GetChannelCount() const
{
	return audioFileReader_->GetChannels();	
}

std::size_t PhaseVocoderMediator::GetMaxBufferedSamples()
{
	return audioFileWriter_->GetMaxBufferedSamples();
}

const std::vector<std::size_t>& PhaseVocoderMediator::GetTransients(std::size_t streamID)
{
	return transients_[streamID];
}
