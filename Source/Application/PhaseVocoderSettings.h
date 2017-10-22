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

#include <string>
#include <memory>
#include <functional>

class PhaseVocoderSettings
{
	public:
		// Typical setter methods
		void SetInputWaveFile(const std::string& filename);
		void SetOutputWaveFile(const std::string& filename);
		void SetStretchFactor(double stretchFactor);
		void SetResampleValue(std::size_t resampleValue);
		void SetPitchShiftValue(double pitchShiftValue);
		void SetTransientConfigFilename(const std::string& transientConfgFilename);
		void SetDisplayTransients();
		void SetValleyToPeakRatio(double valleyToPeakRatio);

		// Methods to check if a value was actually given
		bool InputWaveFileGiven() const;
		bool OutputWaveFileGiven() const;
		bool StretchFactorGiven() const;
		bool ResampleValueGiven() const;
		bool PitchShiftValueGiven() const;
		bool TransientConfigFilenameGiven() const;
		bool DisplayTransients() const;
		bool ValleyToPeakRatioGiven() const;

		// Typical getter methods
		const std::string& GetInputWaveFile() const;
		const std::string& GetOutputWaveFile() const;
		double GetStretchFactor() const;
		std::size_t GetResampleValue() const;
		double GetPitchShiftValue() const;
		const std::string& GetTransientConfigFilename() const;
		double GetValleyToPeakRatio() const;

	private:
		std::string inputWaveFilename_;
		bool inputWaveFilenameGiven_{false};

		std::string outputWaveFilename_;
		bool outputWaveFilenameGiven_{false};

		double stretchFactor_;
		bool stretchFactorGiven_{false};

		std::size_t resampleValue_;
		bool resampleValueGiven_{false};

		double pitchShiftValue_;
		bool pitchShiftValueGiven_{false};

		bool displayTransients_{false};

		std::string transientConfigFilename_;
		bool transientConfigFilenameGiven_{false};

		double valleyToPeakRatio_{1.5};
		bool valleyToPeakRatioGiven_{false};
};