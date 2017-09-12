#pragma once

#include <cstdint>

namespace WaveFile { 

struct WaveFileHeader
{
	unsigned char chunkID_[4];
	unsigned char chunkSize_[4];
	unsigned char format_[4];
	unsigned char subChunk1ID_[4];
	unsigned char subChunk1Size_[4];
	unsigned char audioFormat_[2];
	unsigned char channels_[2];
	unsigned char sampleRate_[4];
	unsigned char byteRate_[4];
	unsigned char blockAlign_[2];
	unsigned char bitsPerSample_[2];
	unsigned char subChunk2ID_[4];
	unsigned char subChunk2Size_[4];
};

static const std::size_t WAVE_FILE_HEADER_SIZE{44};

}
