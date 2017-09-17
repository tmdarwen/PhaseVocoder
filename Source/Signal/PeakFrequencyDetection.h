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
