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
