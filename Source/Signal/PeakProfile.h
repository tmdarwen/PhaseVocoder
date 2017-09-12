#pragma once

#include <AudioData/AudioData.h>
#include <Signal/FrequencyDomain.h>
#include <vector>

namespace Signal  {

// Given a frequency domain signal, this will find all the "peak" bins
class PeakProfile
{
	public:
		PeakProfile(const FrequencyDomain& frequencyDomain);

		// This will return the closest peak for the given frequency bin
		std::size_t GetLocalPeakForBin(std::size_t bin);

		const std::vector<std::size_t>& GetAllPeakBins();

		std::pair<std::size_t, std::size_t> GetValleyBins(std::size_t peakBin);

	private:
		void CalculatePeaksAndValleys();
		
		// The following hold the bin numbers of peaks (high point of magnitude) and 
		// the low points (or "valleys") on both sides of the peaks.
		std::vector<std::size_t> peakBins_;
		std::vector<std::size_t> valleyBins_;

		// The frequency domain given by the user at construction
		FrequencyDomain frequencyDomain_;	
};

}
