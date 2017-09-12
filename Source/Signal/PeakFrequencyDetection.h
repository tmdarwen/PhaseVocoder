#pragma once

#include <vector>

namespace Signal {

std::vector<double> GenerateSineWave(double sampleRate, std::size_t lengthInSamples, double signalFrequency, double phase=0.0);

// *******************************************************************************************************
// ****************** USE THIS "QUINN" ALGORITHM AND AVOID THE CORRELATION ONE BELOW *********************
// *******************************************************************************************************
// I got this algorithm from: http://dspguru.com/dsp/howtos/how-to-interpolate-fft-peak it's called "Quinn's Second Estimator".
// If there's any question about how well it performs compared to correlation, see the UT titled "TestPeakBinFrequencyAccuracy"
double GetPeakFrequencyByQuinn(std::size_t peakBin, const std::vector<double>& timeDomainSignal, double inputSignalSampleRate);
double GetPeakFrequencyByQuinn(std::size_t peakBin, std::size_t fourierSize, const std::vector<double>& real, const std::vector<double>& imaginary, double inputSignalSampleRate);

// Don't use this algorithm.  Use the Quinn algorithm below.  If you have any question which performs better see the TestPeakBinAccuracy UT.
double GetPeakFrequencyByCorrelation(std::size_t peakBin, const std::vector<double>& timeDomainSignal, double inputSignalSampleRate);

}
