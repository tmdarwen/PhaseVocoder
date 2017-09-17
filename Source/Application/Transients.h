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
#include <vector>

class TransientSettings
{
	public:
		// Typical setter methods
		void SetInputWaveFile(const std::string& filename);
		void SetTransientConfigFilename(const std::string& transientConfgFilename);
		void SetTransientCallback(std::function<void(std::size_t)> callback);
		void SetTransientValleyToPeakRatio(double valleyToPeakRatio);

		// Methods to check if a value was actually given
		bool InputWaveFileGiven() const;
		bool TransientConfigFilenameGiven() const;
		bool TransientCallbackGiven() const;

		// Typical getter methods
		const std::string& GetInputWaveFile() const;
		const std::string& GetTransientConfigFilename() const;
		std::function<void(std::size_t)> GetTransientCallback() const;
		double GetTransientValleyToPeakRatio() const;

	private:
		std::string inputWaveFilename_;
		bool inputWaveFilenameGiven_{false};

		std::function<void(std::size_t)> transientCallback_;
		bool transientCallbackGiven_{false};

		std::string transientConfigFilename_;
		bool transientConfigFilenameGiven_{false};

		double valleyToPeakRatio_{1.5};
};

class Transients
{
	public:
		Transients(const TransientSettings& settings);
		virtual ~Transients();

		const std::vector<std::size_t>& GetTransients();

	private:
		void GetTransientPositionsFromAudioFile();
		void GetTransientPositionsFromConfigFile();
		void HandleCallback();

		TransientSettings settings_;
		std::vector<std::size_t> transients_;
		bool transientsProcessed_{false};	
};