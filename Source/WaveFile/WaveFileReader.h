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
#include <fstream>
#include <vector>
#include <unordered_map>
#include <WaveFile/WaveFileHeader.h>
#include <AudioData/AudioData.h>

namespace WaveFile {

class WaveFileReader
{
	public:
  		WaveFileReader(const std::string& filename);
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

		std::vector<AudioData> GetAudioData();
		std::vector<AudioData> GetAudioData(std::size_t samplesToRead);
		std::vector<AudioData> GetAudioData(std::size_t samplesStartPosition, std::size_t samplesToRead);

	private:
		void ReadHeader();
		void ValidateHeader();

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
		std::size_t sampleCount_;

		std::ifstream inputFileStream_;
		void OpenFile();
};

} // End of namespace