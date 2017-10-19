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

#include <Application/PhaseVocoderProcessor.h>
#include <Application/Transients.h>
#include <WaveFile/WaveFileReader.h>
#include <WaveFile/WaveFileWriter.h>
#include <Signal/PhaseVocoder.h>
#include <Signal/Resampler.h>
#include <ThreadSafeAudioFile/Reader.h>
#include <ThreadSafeAudioFile/Writer.h>
#include <Utilities/Exception.h>
#include <iostream>
#include <cmath>
#include <algorithm>

PhaseVocoderProcessor::PhaseVocoderProcessor(std::size_t streamID, 
	const PhaseVocoderSettings& settings, 
	std::shared_ptr<ThreadSafeAudioFile::Reader> audioFileReader, 
	std::shared_ptr<ThreadSafeAudioFile::Writer> audioFileWriter) :
		streamID_{streamID}, 
		settings_{settings}, 
		audioFileReader_{audioFileReader}, 
		audioFileWriter_{audioFileWriter}
{

}

PhaseVocoderProcessor::~PhaseVocoderProcessor()
{

}

void PhaseVocoderProcessor::Process()
{
	InstantiateResampler();

	if(settings_.StretchFactorGiven() || settings_.PitchShiftValueGiven() || settings_.DisplayTransients())
	{
		// Even if we don't stretch the audio, and are just pitch shifting, the pitch shifter requires 
		// stretching, and if we're stretching, we want to properly handle transients so the audio 
		// quality doesn't suffer.
		ObtainTransients();
	}

	// If we're not stretching, pitch shifting or resampling the audio, we must have only been 
	// displaying transients (via the transient callback) so we're done processing at this point.
	if(!settings_.StretchFactorGiven() && !settings_.PitchShiftValueGiven() && !settings_.ResampleValueGiven())
	{
		return;
	}

	// We process audio by transient sections.  If no stretching of audio is happening (i.e. we're just  
	// resampling), we just treat the audio as one single transient section.
	if(settings_.StretchFactorGiven() || settings_.PitchShiftValueGiven() || settings_.DisplayTransients())
	{
		// This handles any leading silence given in the input.
		HandleLeadingSilence();

		std::size_t transientStartIndex{0};
		auto transientPositions{transients_->GetTransients()};
		while(transientStartIndex < (transientPositions.size() - 1))
		{
			ProcessAudioSection(transientPositions[transientStartIndex], transientPositions[transientStartIndex + 1]);	
			++transientStartIndex;
		}

		// Handle the last transient section to the end of input
		ProcessAudioSection(transientPositions[transientStartIndex], audioFileReader_->GetSampleCount());
	}
	else
	{
		// If we're here, we are just resampling the audio.  No transients, no stretching.
		ProcessAudioSection(0, audioFileReader_->GetSampleCount());
	}

	// Flush the Resampler (if we're using it)
	if(settings_.ResampleValueGiven() || settings_.PitchShiftValueGiven())
	{
		auto audioData{resampler_->FlushAudioData()};
		audioFileWriter_->WriteAudioStream(streamID_, audioData.GetData());
	}
}

void PhaseVocoderProcessor::ObtainTransients()
{
	TransientSettings transientSettings;

	transientSettings.SetStreamID(streamID_);

	if(settings_.InputWaveFileGiven())
	{
		transientSettings.SetAudioFile(audioFileReader_);
	}

	if(settings_.TransientConfigFilenameGiven())
	{
		transientSettings.SetTransientConfigFilename(settings_.GetTransientConfigFilename());
	}

	transientSettings.SetTransientValleyToPeakRatio(settings_.GetValleyToPeakRatio());

	transients_.reset(new Transients(transientSettings));
	transients_->GetTransients();
}

void PhaseVocoderProcessor::HandleLeadingSilence()
{
	auto transientPositions{transients_->GetTransients()};

	if(transientPositions.size() == 0)
	{
		HandleSilenceInInput(audioFileReader_->GetSampleCount());
	}
	else if(transientPositions[0] != 0)
	{
		HandleSilenceInInput(transientPositions[0]);
	}
}

AudioData PhaseVocoderProcessor::GetAudioInput(std::size_t startSample, std::size_t length)
{
	return audioFileReader_->ReadAudioStream(streamID_, startSample, length);
}

void PhaseVocoderProcessor::HandleSilenceInInput(std::size_t sampleCount)
{
	std::size_t samplesToOutput{static_cast<std::size_t>(static_cast<double>(sampleCount) * settings_.GetStretchFactor() + 0.5)};

	std::size_t currentSamplePosition{0};
	while(currentSamplePosition < samplesToOutput)
	{
		std::size_t currentWriteAmount{std::min(bufferSize_, samplesToOutput - currentSamplePosition)};

		AudioData silentAudioData;
		silentAudioData.AddSilence(currentWriteAmount);

		audioFileWriter_->WriteAudioStream(streamID_, silentAudioData.GetData());

		currentSamplePosition += currentWriteAmount;
	}
}

void PhaseVocoderProcessor::ProcessAudioSection(std::size_t startSamplePosition, std::size_t endSamplePosition)
{
	std::size_t totalSamplesToRead{endSamplePosition - startSamplePosition};

	InstantiatePhaseVocoder(endSamplePosition - startSamplePosition);
	samplesOutputFromCurrentPhaseVocoder_ = 0;

	std::size_t currentSamplePosition{0};
	while(currentSamplePosition < totalSamplesToRead)
	{
		std::size_t samplesToRead{std::min(bufferSize_, totalSamplesToRead - currentSamplePosition)};
		auto audioInputData{GetAudioInput(startSamplePosition + currentSamplePosition, samplesToRead)};
		ProcessInput(audioInputData);
		currentSamplePosition += samplesToRead;
	}

	FinalizeAudioSection(totalSamplesToRead);
}

void PhaseVocoderProcessor::ProcessInput(const AudioData& audioInputData)
{
	AudioData resultingAudio;

	if(settings_.PitchShiftValueGiven() || (settings_.StretchFactorGiven() && settings_.ResampleValueGiven()))
	{
		resultingAudio = ProcessAudioWithResampler(ProcessAudioWithPhaseVocoder(audioInputData));
	}
	else if(settings_.StretchFactorGiven() && !settings_.PitchShiftValueGiven())
	{
		resultingAudio = ProcessAudioWithPhaseVocoder(audioInputData);
	}
	else if(settings_.ResampleValueGiven() && !settings_.PitchShiftValueGiven())
	{
		resultingAudio = ProcessAudioWithResampler(audioInputData);
	}
	else
	{
		Utilities::ThrowException("PhaseVocoderProcessor has no action to perform");
	}

	audioFileWriter_->WriteAudioStream(streamID_, resultingAudio.GetData());
}

void PhaseVocoderProcessor::FinalizeAudioSection(std::size_t totalInputSamples)
{
	AudioData audioData;

	if(settings_.StretchFactorGiven() || settings_.PitchShiftValueGiven())
	{
		std::size_t totalOutputSamplesNeeded{static_cast<std::size_t>(totalInputSamples * phaseVocoder_->GetStretchFactor() + 0.5)};
		std::size_t samplesStillNeeded{totalOutputSamplesNeeded - samplesOutputFromCurrentPhaseVocoder_};

		audioData = FlushPhaseVocoderOutput(samplesStillNeeded);
	}

	if(audioData.GetSize() && (settings_.ResampleValueGiven() || settings_.PitchShiftValueGiven()))
	{
		resampler_->SubmitAudioData(audioData);
	}
	else if(audioData.GetSize())
	{
		audioFileWriter_->WriteAudioStream(streamID_, audioData.GetData());
	}
}

AudioData PhaseVocoderProcessor::ProcessAudioWithPhaseVocoder(const AudioData& audioInputData)
{
	phaseVocoder_->SubmitAudioData(audioInputData);

	AudioData dataToReturn;

	while(phaseVocoder_->OutputSamplesAvailable())
	{
		dataToReturn.Append(phaseVocoder_->GetAudioData(std::min(bufferSize_, phaseVocoder_->OutputSamplesAvailable())));
	}

	// If transient overlap data exist, mix it with this output
	if(transientSectionOverlap_.GetSize() && (dataToReturn.GetSize() >= transientSectionOverlap_.GetSize()))
	{
		dataToReturn = LinearCrossfade(transientSectionOverlap_, dataToReturn);
		transientSectionOverlap_.Clear();	
	}

	samplesOutputFromCurrentPhaseVocoder_ += dataToReturn.GetSize();

	return dataToReturn;
}

AudioData PhaseVocoderProcessor::FlushPhaseVocoderOutput(std::size_t samplesNeeded)
{
	AudioData audioToReturn;
	auto flushedOutput{phaseVocoder_->FlushAudioData()};

	if(samplesNeeded)
	{
		if(samplesNeeded > flushedOutput.GetSize())
		{
			Utilities::ThrowException("Flushed output has less samples than still needed", samplesNeeded, flushedOutput.GetSize());
		}

		audioToReturn = flushedOutput.RetrieveRemove(samplesNeeded);
  
		// If transient overlap data exist, mix it with this output
		if(transientSectionOverlap_.GetSize())
		{
			audioToReturn = LinearCrossfade(transientSectionOverlap_, audioToReturn);
			transientSectionOverlap_.Clear();	
		}
	}

	// Save off transient overlap samples for clean mix/transition to next transient
	if(flushedOutput.GetSize() >= transientSectionOverlapSampleCount_)
	{
		transientSectionOverlap_.Append(flushedOutput.Retrieve(transientSectionOverlapSampleCount_));
	}

	return audioToReturn;
}

AudioData PhaseVocoderProcessor::ProcessAudioWithResampler(const AudioData& audioInputData)
{
	resampler_->SubmitAudioData(audioInputData);

	AudioData dataToReturn;

	while(resampler_->OutputSamplesAvailable())
	{
		dataToReturn.Append(resampler_->GetAudioData(std::min(bufferSize_, resampler_->OutputSamplesAvailable())));
	}

	return dataToReturn;
}

void PhaseVocoderProcessor::InstantiatePhaseVocoder(std::size_t sampleLengthOfAudioToProcess)
{
	if(!settings_.GetStretchFactor() && !settings_.PitchShiftValueGiven())
	{
		// No Phase Vocoder needed
		return;
	}

	double stretchFactor{1.0};
	if(settings_.StretchFactorGiven())
	{
		stretchFactor = settings_.GetStretchFactor();	
	}

	if(settings_.PitchShiftValueGiven())
	{
		stretchFactor *= GetPitchShiftRatio();
	}

	phaseVocoder_.reset(new Signal::PhaseVocoder(audioFileReader_->GetSampleRate(), sampleLengthOfAudioToProcess, stretchFactor));
}

void PhaseVocoderProcessor::InstantiateResampler()
{
	if(!settings_.ResampleValueGiven() && !settings_.PitchShiftValueGiven())
	{
		// No Resampler needed
		return;
	}

	resampler_.reset(new Signal::Resampler(audioFileReader_->GetSampleRate(), GetResampleRatio()));
}

double PhaseVocoderProcessor::GetPitchShiftRatio()
{
	// Google tells me the ratio of a semitone change in pitch can be found by 2^(semitone/12)
	return pow(2.0, settings_.GetPitchShiftValue() / 12.0);
}

double PhaseVocoderProcessor::GetResampleRatio()
{
	double resampleRatio{1.0};

	if(settings_.ResampleValueGiven())
	{
		resampleRatio = static_cast<double>(settings_.GetResampleValue()) / static_cast<double>(audioFileReader_->GetSampleRate());
	}

	if(settings_.PitchShiftValueGiven())
	{
		resampleRatio = resampleRatio / GetPitchShiftRatio();
	}

	return resampleRatio;
}

const std::vector<std::size_t>& PhaseVocoderProcessor::GetTransients() const
{
	if(transients_.get()) return transients_->GetTransients();
	else return noTransients_;
}
