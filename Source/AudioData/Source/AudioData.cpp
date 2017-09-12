#include <AudioData/AudioData.h>
#include <Utilities/Exception.h>
#include <iostream>

AudioData::AudioData() { }

AudioData::AudioData(const std::vector<double>& data) : data_(data) { }

AudioData::AudioData(const double* data, std::size_t samples) 
{
	for(std::size_t i{0}; i < samples; ++i)
	{
		data_.push_back(data[i]);
	}
}

AudioData::AudioData(const AudioData& audioData)
{
	data_ = audioData.data_;
}

AudioData::AudioData(AudioData&& audioData) : data_(std::move(audioData.data_)) { }

AudioData::~AudioData() { }

void AudioData::operator=(AudioData& audioData)
{
	data_ = audioData.data_;
}

AudioData& AudioData::operator=(AudioData&& audioData)
{
	data_ = audioData.data_;
	return *this;
}

void AudioData::AddSilence(uint64_t sampleCount)
{
	data_.resize(data_.size() + sampleCount, 0.0);
}

void AudioData::PushSample(double sample)
{
	data_.push_back(sample);
}

void AudioData::PushBuffer(double* buffer, std::size_t size)
{
	for(auto i = 0; i < size; ++i)
	{
		data_.push_back(buffer[i]);
	}
}

void AudioData::PushBuffer(const std::vector<double>& buffer)
{
	for(auto i = 0; i < buffer.size(); ++i)
	{
		data_.push_back(buffer[i]);
	}
}

void AudioData::PushBuffer(const std::vector<double>& buffer, std::size_t size)
{
	for(auto i = 0; i < size; ++i)
	{
		data_.push_back(buffer[i]);
	}
}

void AudioData::Append(const AudioData& audioData)
{
	for(auto sample : audioData.data_)
	{
		data_.push_back(sample);	
	}
}

AudioData AudioData::Retrieve(uint64_t samples) const
{
	return Retrieve(0, samples);
}

AudioData AudioData::RetrieveRemove(uint64_t samples)
{
	AudioData audioData{Retrieve(samples)};

	RemoveFrontSamples(samples);

	return audioData;
}

AudioData AudioData::Retrieve(uint64_t startPosition, uint64_t samples) const
{
	if((startPosition + samples) > GetSize())
	{
		Utilities::ThrowException("Attempting to retrieve more samples than exist", GetSize(), startPosition, samples);
	}

	AudioData audioData;

	std::size_t index{startPosition};
	std::size_t stopSample{startPosition + samples};
	while(index < stopSample)
	{
		audioData.data_.push_back(data_[index]);	
		++index;
	}

	return audioData;
}

// Moves the last given number of samples into the targetAudioData
void AudioData::MoveLastSamples(std::size_t samples, AudioData& targetAudioData)
{
	if(samples > GetSize())
	{
		Utilities::ThrowException("Attempting to move more samples than exist", GetSize(), samples);
	}

	for(auto i = GetSize() - samples; i < GetSize(); ++i)
	{
		targetAudioData.PushSample(data_[i]);
	}

	data_.resize(GetSize() - samples);	
}

void AudioData::MixInSamples(const double* buffer, std::size_t samples)
{
	auto shorterOfTwo = samples;
	if(GetSize() < samples)
	{
		shorterOfTwo = GetSize();
	}

	// Mix the buffers
	for(auto i = 0; i < shorterOfTwo; ++i)
	{
		data_[i] += buffer[i];
		if(data_[i] > 1.0)
		{
			data_[i] = 1.0;
		}
		else if(data_[i] < -1.0)
		{
			data_[i] = -1.0;
		}
	}

	// Add any remaining input samples
	if(shorterOfTwo < samples)
	{
		for(auto i = shorterOfTwo; i < samples; ++i)
		{
			data_.push_back(buffer[i]);
		}
	}	
}

void AudioData::MixInSamples(const AudioData& audioData)
{
	MixInSamples(&(audioData.data_[0]), audioData.GetSize());
}

void AudioData::RemoveFrontSamples(std::size_t samples)
{
	if(samples >= data_.size())
	{
		data_.clear();
		return;
	}

	data_.erase(data_.begin(), data_.begin() + samples);
}

std::size_t AudioData::GetSize() const
{
	return data_.size();
}

const std::vector<double>& AudioData::GetData() const
{
	return data_;
}

std::vector<double>& AudioData::GetDataWriteAccess()
{
	return data_;
}

void AudioData::Clear()
{
	data_.clear();
}

void AudioData::LinearCrossfade(AudioData& audioData)
{
	for(std::size_t i = 0; i < data_.size(); ++i)
	{
		auto percentComplete = static_cast<double>(i) / static_cast<double>(data_.size() - 1);
		auto newValue = (data_[i] * (1.0 - percentComplete)) + (audioData.data_[i] * percentComplete);
		if(newValue > 1.0)
		{
			data_[i] = 1.0;
		}
		else if(newValue < -1.0)
		{
			data_[i] = -1.0;
		}
		else
		{
			data_[i] = newValue;
		}
	}
}

void AudioData::Amplify(double ratio)
{
	std::size_t index{0};
	while(index < data_.size())
	{
		data_[index] *= ratio;

		if(data_[index] > 1.0)
		{
			data_[index] = 1.0;
		}
		else if(data_[index] < -1.0)
		{
			data_[index] = -1.0;
		}

		++index;
	}
}

void AudioData::Amplify(double beginRatio, double endRatio)
{
	std::size_t index{0};
	while(index < data_.size())
	{
		double ratio{beginRatio + ((endRatio - beginRatio) * (static_cast<double>(index) / static_cast<double>(data_.size() - 1)))};
		data_[index] *= ratio;

		if(data_[index] > 1.0)
		{
			data_[index] = 1.0;
		}
		else if(data_[index] < -1.0)
		{
			data_[index] = -1.0;
		}

		++index;
	}
}

void AudioData::Truncate(std::size_t newSize)
{
	if(newSize > data_.size())
	{
		return;
	}

	data_.resize(newSize);
}

// Possibilities:
// 1) audioDataLeft and audioDataRight are the same length = simple
// 2) audioDataLeft is longer than audioDataRight = mix audioDataRight in starting at index 0.  Crossfading 
//    will end at the audioDataRight.GetSize()
// 3) audioDataLeft is shorter than audioDataRight = mix audioDataRight in starting at index 0.  Crossfading 
//    will end at the end of audioDataLeft.GetSize()
AudioData LinearCrossfade(const AudioData& audioDataLeft, const AudioData& audioDataRight)
{
	AudioData audioDataToReturn;

	auto audioDataLeftBuffer{audioDataLeft.GetData()};
	auto audioDataRightBuffer{audioDataRight.GetData()};

	std::size_t crossfadeLength{audioDataLeft.GetSize()};
	if(crossfadeLength > audioDataRight.GetSize())
	{
		crossfadeLength = audioDataRight.GetSize();
	}

	for(std::size_t i = 0; i < crossfadeLength; ++i)
	{
		auto percentComplete{static_cast<double>(i) / static_cast<double>(crossfadeLength - 1)};
		auto newValue = (audioDataLeftBuffer[i] * (1.0 - percentComplete)) + (audioDataRightBuffer[i] * percentComplete);
		audioDataToReturn.PushSample(newValue);
	}

	// The following if/else handles scenario 2 and 3
	if(audioDataToReturn.GetSize() < audioDataLeft.GetSize())
	{
		audioDataToReturn.Append(audioDataLeft.Retrieve(audioDataToReturn.GetSize(), audioDataLeft.GetSize() - audioDataToReturn.GetSize()));
	}
	else if(audioDataToReturn.GetSize() < audioDataRight.GetSize())
	{
		audioDataToReturn.Append(audioDataRight.Retrieve(audioDataToReturn.GetSize(), audioDataRight.GetSize() - audioDataToReturn.GetSize()));
	}

	return audioDataToReturn;
}
