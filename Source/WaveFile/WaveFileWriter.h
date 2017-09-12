#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

namespace WaveFile {

class WaveFileWriter
{
	public:
		WaveFileWriter(const std::string& filename, std::size_t channels, std::size_t sampleRate, std::size_t bitsPerSample);
		~WaveFileWriter();

		void AppendAudioData(const std::vector<double>& audioData);
		std::size_t GetSampleCount();

	private:
		void WriteWaveFileHeader();

		std::string filename_;
		std::size_t channels_;
		std::size_t sampleRate_;
		std::size_t bitsPerSample_;
		std::size_t sampleCount_;
		std::ofstream fileStream_;

		static const uint32_t SIZE_OF_SUBCHUNK1{16};
		static const uint16_t PCM_AUDIO_FORMAT{1};
};

}