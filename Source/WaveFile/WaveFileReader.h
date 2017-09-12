#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <WaveFile/WaveFileHeader.h>
#include <AudioData/AudioData.h>

namespace WaveFile {

class WaveFileReader
{
	public:
  		WaveFileReader(const std::string& filename, bool bufferSmallFile=true, std::size_t maxBufferLimit=defaultBufferLimitInSamples_);
		~WaveFileReader();

		const WaveFile::WaveFileHeader& GetHeader();

		std::size_t GetFileSize();
		std::size_t GetChunkSize();
		std::size_t GetByteRate();
		std::size_t GetChannels();
		std::size_t GetSampleRate();
		std::size_t GetBitsPerSample();
		std::size_t GetBlockAlign();
		std::size_t GetSubChunk1Size();
		std::size_t GetSubChunk2Size() const;
		std::size_t GetWaveHeaderSize();

		std::size_t GetSampleCount() const;
		const double* GetAudioData() const;
		AudioData GetAudioData(std::size_t samplesToRead);
		AudioData GetAudioData(std::size_t samplesStartPosition, std::size_t samplesToRead);

	private:
		void ReadHeader();
		void ValidateHeader();
		void CacheAudioData();

		// Returns the byte position in the file where the audio sample data starts
		std::size_t GetBytePositionInFileWhereAudioSamplesStart();

		// Returns the number of bytes of audio (sample) data that exist in the file
		std::size_t GetByteCountOfSampleData();

		// Returns the position the file pointer is currently at
		std::size_t GetCurrentBytePositionOfFilePointer();

		// Returns the position the file pointer is currently at with respect to audio samples
		std::size_t GetCurrentSamplePositionOfFilePointer();

		// Sets inputFileStream_ to the given audio sample position
		void FilePointerSeekToSamplePosition(std::size_t samplesStartPosition);

		std::vector<uint8_t> Read(std::size_t bytes, std::size_t filePosition=0);

		std::string filename_;
		WaveFile::WaveFileHeader header_;
		static const unsigned int waveHeaderSize_{sizeof(WaveFile::WaveFileHeader)};
		double* audioData_{nullptr};
		std::size_t sampleCount_;

		bool bufferSmallFile_;
		std::size_t maxBufferLimit_;

		std::ifstream inputFileStream_;
		void OpenFile();

		// The default buffer limit is calculated as 30 seconds of 44100Hz sample rate (i.e. 30 * 44100 samples)
		constexpr static std::size_t defaultBufferLimitInSamples_{44100 * 30};
};

} // End of namespace