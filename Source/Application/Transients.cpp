#include <Application/Transients.h>
#include <Application/TransientConfigFile.h>
#include <Signal/TransientDetector.h>
#include <WaveFile/WaveFileReader.h>
#include <Utilities/Exception.h>
#include <Utilities/Stringify.h>
#include <yaml-cpp/yaml.h>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
// This first group of methods are TransientSettings

void TransientSettings::SetInputWaveFile(const std::string& filename)
{
	inputWaveFilename_ = filename;
	inputWaveFilenameGiven_ = true;
}

void TransientSettings::SetTransientValleyToPeakRatio(double valleyToPeakRatio)
{
	valleyToPeakRatio_ = valleyToPeakRatio;	
}

void TransientSettings::SetTransientCallback(std::function<void(std::size_t)> callback)
{
	transientCallback_ = callback;
	transientCallbackGiven_ = true;
}

void TransientSettings::SetTransientConfigFilename(const std::string& transientConfgFilename)
{
	transientConfigFilename_ = transientConfgFilename;
	transientConfigFilenameGiven_ = true;
}

bool TransientSettings::InputWaveFileGiven() const
{
	return inputWaveFilenameGiven_;
}

bool TransientSettings::TransientConfigFilenameGiven() const
{
	return transientConfigFilenameGiven_;
}

bool TransientSettings::TransientCallbackGiven() const
{
	return transientCallbackGiven_;
}

const std::string& TransientSettings::GetInputWaveFile() const
{
	return inputWaveFilename_;
}

const std::string& TransientSettings::GetTransientConfigFilename() const
{
	return transientConfigFilename_;
}

std::function<void(std::size_t)> TransientSettings::GetTransientCallback() const
{
	return transientCallback_;
}

double TransientSettings::GetTransientValleyToPeakRatio() const
{
	return valleyToPeakRatio_;
}

//////////////////////////////////////////////////////////////////////////
// Now the actual Transient Methods

Transients::Transients(const TransientSettings& settings) : settings_(settings) { }

Transients::~Transients() { }

const std::vector<std::size_t>& Transients::GetTransients()
{
	if(!transientsProcessed_)
	{
		if(settings_.TransientConfigFilenameGiven())
		{
			GetTransientPositionsFromConfigFile();
		}
		else
		{
			GetTransientPositionsFromAudioFile();
		}

		transientsProcessed_ = true;
	}

	return transients_;
}

// Uses the TransientDetector in our Signal lib to find transients
void Transients::GetTransientPositionsFromAudioFile()
{
	WaveFile::WaveFileReader waveReader{settings_.GetInputWaveFile()};
	Signal::TransientDetector transientDetector{waveReader.GetSampleRate()};
	transientDetector.SetValleyToPeakRatio(settings_.GetTransientValleyToPeakRatio());

	const std::size_t bufferSize{8192};

	std::size_t samplesLeft{waveReader.GetSampleCount()};
	while(samplesLeft)
	{
		std::size_t samplesToRead{bufferSize};
		if(samplesToRead > samplesLeft)
		{
			samplesToRead = samplesLeft;
		}

		auto audioData{waveReader.GetAudioData(samplesToRead)};

		std::vector<std::size_t> newTransients;
		if(transientDetector.FindTransients(audioData, newTransients))
		{
			transients_.insert(transients_.end(), newTransients.begin(), newTransients.end());
		}

		samplesLeft -= samplesToRead;
	}

	HandleCallback();
}


void Transients::GetTransientPositionsFromConfigFile()
{
	auto transientConfigFile{TransientConfigFile{settings_.GetTransientConfigFilename()}};	
	transients_ = transientConfigFile.GetTransients();
	HandleCallback();
}

void Transients::HandleCallback()
{
	if(settings_.TransientCallbackGiven())
	{
		std::for_each(transients_.begin(), transients_.end(), settings_.GetTransientCallback());
	}
}

