#pragma once

#include <AudioData/AudioData.h>
#include <Signal/FrequencyDomain.h>
#include <vector>

namespace Signal  {

namespace Fourier
{
	bool IsPowerOfTwo(std::size_t number);

	Signal::FrequencyDomain ApplyDFT(const AudioData& timeDomainSignal);
	AudioData ApplyInverseDFT(const Signal::FrequencyDomain& frequencyDomainData);

	Signal::FrequencyDomain ApplyFFT(const AudioData& timeDomainSignal);
	AudioData ApplyInverseFFT(const Signal::FrequencyDomain& frequencyDomainData);
}

}
