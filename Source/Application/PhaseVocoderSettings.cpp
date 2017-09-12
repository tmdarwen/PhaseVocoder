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

void PhaseVocoderSettings::SetTransientCallback(std::function<void(std::size_t)> callback)
{
	transientCallback_ = callback;
	transientCallbackGiven_ = true;
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

bool PhaseVocoderSettings::TransientCallbackGiven() const
{
	return transientCallbackGiven_;
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

std::function<void(std::size_t)> PhaseVocoderSettings::GetTransientCallback() const
{
	return transientCallback_;
}

double PhaseVocoderSettings::GetValleyToPeakRatio() const
{
	return valleyToPeakRatio_;
}