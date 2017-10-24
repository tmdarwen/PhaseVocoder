/*
 * PhaseVocoder
 *
 * Copyright (c) 2017 - Terence M. Darwen - tmdarwen.com
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

#pragma once

#include <AudioData/AudioData.h>
#include <mutex>

// This is an implementation of equation 16-4 (Windowed Sinc Filter) from the 
// book "The Scientist and Engineer's Guide to Digital Signal Processing" 2nd 
// edition by Steven W. Smith.
// 
// The cutoffRatio should be calculated as follows:
// cutoffRatio = OutputSampleRate/InputSampleRate * 0.5
// signal sample rate.  For example, if you're input is a 44100Hz signal and 
// you want to filter out everything above 32000Hz you would use a ratio of 
// 0.3628.

namespace Signal {

class LowPassFilter
{
	public:
		LowPassFilter(double cutoffRatio, std::size_t filterLength=100);

		// Clears internal buffers and counters to restart processing fresh
		void Reset();

		// Method to give the Phase Vocoder input audio for it to process
		void SubmitAudioData(const AudioData& audioData);

		// Method to retrieve output audio the Phase Vocoder has processed
		AudioData GetAudioData(uint64_t samples);

		// Method to check how many output samples are currently available
		std::size_t OutputSamplesAvailable();

		// Call this at the end of processing to get any and all samples remaining in the system
		AudioData FlushAudioData();

		// Returns the MinimumSamplesNeededForProcessing. This is the same as filterLength_.
		std::size_t MinimumSamplesNeededForProcessing();


	private:
		void CalculateFilterKernel();
		void Process();

		double cutoffRatio_;
		std::size_t filterLength_;
		std::vector<double> filterKernel_;

		AudioData audioInput_;
		AudioData audioOutput_;

		std::mutex mutex_;

		const double minCutoffRatioRange_{0.0001};
		const double maxCutoffRatioRange_{0.5000};
};

}
