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

#include <string>
#include <memory>
#include <functional>
#include <AudioData/AudioData.h>
#include <Application/PhaseVocoderSettings.h>
#include <Application/Transients.h>

namespace Signal
{
	class PhaseVocoder;
	class Resampler;
}

namespace ThreadSafeAudioFile
{
	class Reader;
	class Writer;
}

class PhaseVocoderProcessor
{
	public:
		PhaseVocoderProcessor(std::size_t streamID, const PhaseVocoderSettings& settings, 
								std::shared_ptr<ThreadSafeAudioFile::Reader> audioFileReader, 
								std::shared_ptr<ThreadSafeAudioFile::Writer> audioFileWriter);
		virtual ~PhaseVocoderProcessor();

		void Process();

		const std::vector<std::size_t>& GetTransients() const;

	private:
		void HandleSilenceInInput(std::size_t sampleCount);

		void ProcessAudioSection(std::size_t startSamplePosition, std::size_t endSamplePosition);

		AudioData GetAudioInput(std::size_t startSample, std::size_t length);

		void HandleLeadingSilence();

		AudioData FlushPhaseVocoderOutput(std::size_t samplesNeeded);

		void InstantiatePhaseVocoder(std::size_t sampleLengthOfAudioToProcess);
		void InstantiateResampler();

		void ProcessInput(const AudioData& audioInputData);
		void FinalizeAudioSection(std::size_t totalInputSamples);
		AudioData ProcessAudioWithPhaseVocoder(const AudioData& audioInputData);
		AudioData ProcessAudioWithResampler(const AudioData& audioInputData);

		double GetPitchShiftRatio();
		double GetResampleRatio();

		std::size_t samplesOutputFromCurrentPhaseVocoder_{0};

		AudioData transientSectionOverlap_;
		std::size_t transientSectionOverlapSampleCount_{64};  // The number of samples to crossfade-mix between output transient sections

		std::size_t streamID_;

		PhaseVocoderSettings settings_;

		std::size_t bufferSize_{8192};

		void ObtainTransients();

		std::unique_ptr<Transients> transients_;
		std::unique_ptr<Signal::PhaseVocoder> phaseVocoder_;
		std::unique_ptr<Signal::Resampler> resampler_;
		std::shared_ptr<ThreadSafeAudioFile::Reader> audioFileReader_;
		std::shared_ptr<ThreadSafeAudioFile::Writer> audioFileWriter_;

		double transientProcessingTime_{0.0};
		double phaseVocoderProcessingTime_{0.0};
		double resamplerProcessingTime_{0.0};

		std::vector<std::size_t> noTransients_;

};