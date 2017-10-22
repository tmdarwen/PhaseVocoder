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

#include <Application/PhaseVocoderSettings.h>

void PhaseVocoderSettings::SetInputWaveFile(const std::string& filename)
{
	inputWaveFilename_ = filename;
	inputWaveFilenameGiven_ = true;
}

void PhaseVocoderSettings::SetOutputWaveFile(const std::string& filename)
{
	outputWaveFilename_ = filename;
	outputWaveFilenameGiven_ = true;
}

void PhaseVocoderSettings::SetStretchFactor(double stretchFactor)
{
	stretchFactor_ = stretchFactor;	
	stretchFactorGiven_ = true;
}

void PhaseVocoderSettings::SetResampleValue(std::size_t resampleValue)
{
	resampleValue_ = resampleValue;
	resampleValueGiven_ = true;
}

void PhaseVocoderSettings::SetPitchShiftValue(double pitchShiftValue)
{
	pitchShiftValue_ = pitchShiftValue;
	pitchShiftValueGiven_ = true;
}

void PhaseVocoderSettings::SetValleyToPeakRatio(double valleyToPeakRatio)
{
	valleyToPeakRatio_ = valleyToPeakRatio;	
	valleyToPeakRatioGiven_ = true;
}

void PhaseVocoderSettings::SetDisplayTransients()
{
	displayTransients_ = true;
}

void PhaseVocoderSettings::SetTransientConfigFilename(const std::string& transientConfgFilename)
{
	transientConfigFilename_ = transientConfgFilename;
	transientConfigFilenameGiven_ = true;
}

bool PhaseVocoderSettings::InputWaveFileGiven() const
{
	return inputWaveFilenameGiven_;
}

bool PhaseVocoderSettings::OutputWaveFileGiven() const
{
	return outputWaveFilenameGiven_;
}

bool PhaseVocoderSettings::StretchFactorGiven() const
{
	return stretchFactorGiven_;
}

bool PhaseVocoderSettings::ResampleValueGiven() const
{
	return resampleValueGiven_;
}

bool PhaseVocoderSettings::PitchShiftValueGiven() const
{
	return pitchShiftValueGiven_;
}

bool PhaseVocoderSettings::ValleyToPeakRatioGiven() const
{
	return valleyToPeakRatioGiven_;
}

bool PhaseVocoderSettings::TransientConfigFilenameGiven() const
{
	return transientConfigFilenameGiven_;
}

bool PhaseVocoderSettings::DisplayTransients() const
{
	return displayTransients_;
}

const std::string& PhaseVocoderSettings::GetInputWaveFile() const
{
	return inputWaveFilename_;
}

const std::string& PhaseVocoderSettings::GetOutputWaveFile() const
{
	return outputWaveFilename_;
}

double PhaseVocoderSettings::GetStretchFactor() const
{
	return stretchFactor_;
}

std::size_t PhaseVocoderSettings::GetResampleValue() const
{
	return resampleValue_;
}

double PhaseVocoderSettings::GetPitchShiftValue() const
{
	return pitchShiftValue_;
}

const std::string& PhaseVocoderSettings::GetTransientConfigFilename() const
{
	return transientConfigFilename_;
}

double PhaseVocoderSettings::GetValleyToPeakRatio() const
{
	return valleyToPeakRatio_;
}