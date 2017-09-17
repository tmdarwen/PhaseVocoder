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

#include <gtest/gtest.h>
#include <WaveFile/WaveFileReader.h>
#include <Utilities/Exception.h>
#include <cmath>

TEST(WaveReaderTests, OpenFile)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
}

TEST(WaveReaderTests, FailedOpenFile)
{
	ASSERT_THROW(WaveFile::WaveFileReader("NonExistantWaveFile.wav"), Utilities::Exception);
}

TEST(WaveReaderTests, WaveFileSize)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	EXPECT_EQ(88244, waveReader.GetFileSize());
}

TEST(WaveReaderTests, ValidateReadData)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto header = waveReader.GetHeader();
	EXPECT_EQ('R', header.chunkID_[0]);
	EXPECT_EQ('I', header.chunkID_[1]);
	EXPECT_EQ('F', header.chunkID_[2]);
	EXPECT_EQ('F', header.chunkID_[3]);
}

TEST(WaveReaderTests, ValidateFormat)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto header = waveReader.GetHeader();
	EXPECT_EQ('W', header.format_[0]);
	EXPECT_EQ('A', header.format_[1]);
	EXPECT_EQ('V', header.format_[2]);
	EXPECT_EQ('E', header.format_[3]);
}

TEST(WaveReaderTests, ValidateChunkSize)
{
	// The document states the chunk size should be all of the following:
	// 1) 36 + SubChunk2Size
	// 2) 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	// 3) The size of the entire file minus 8 bytes

	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto header = waveReader.GetHeader();

	unsigned int headerChunkSize = header.chunkSize_[3];
	headerChunkSize <<= 8;
	headerChunkSize += header.chunkSize_[2];
	headerChunkSize <<= 8;
	headerChunkSize += header.chunkSize_[1];
	headerChunkSize <<= 8;
	headerChunkSize += header.chunkSize_[0];

	unsigned int subChunk1Size = header.subChunk1Size_[3];
	subChunk1Size <<= 8;
	subChunk1Size += header.subChunk1Size_[2];
	subChunk1Size <<= 8;
	subChunk1Size += header.subChunk1Size_[1];
	subChunk1Size <<= 8;
	subChunk1Size += header.subChunk1Size_[0];

	unsigned int subChunk2Size = header.subChunk2Size_[3];
	subChunk2Size <<= 8;
	subChunk2Size += header.subChunk2Size_[2];
	subChunk2Size <<= 8;
	subChunk2Size += header.subChunk2Size_[1];
	subChunk2Size <<= 8;
	subChunk2Size += header.subChunk2Size_[0];

	// Case 1: ChunkSize = 36 + SubChunk2Size
	// SubChunk2Size is said to be at bytes 40-43
	EXPECT_EQ(subChunk2Size + 36, headerChunkSize);

	// Case 2: ChunkSize = 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	// SubChunk1Size is said to be at bytes 16-20
	// SubChunk2Size is said to be at bytes 40-43
	EXPECT_EQ(4 + (8 + subChunk1Size ) + (8 + subChunk2Size), headerChunkSize);

	// Case 3: ChunkSize = The size of the entire file minus 8 bytes
	EXPECT_EQ(waveReader.GetFileSize() - 8, headerChunkSize);

	// Also validate GetChunkSize() returns the same value we've retrieved
	EXPECT_EQ(waveReader.GetChunkSize(), headerChunkSize);
}

TEST(WaveReaderTests, ValidateSubchunkID)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto header = waveReader.GetHeader();
	EXPECT_EQ('f', header.subChunk1ID_[0]);
	EXPECT_EQ('m', header.subChunk1ID_[1]);
	EXPECT_EQ('t', header.subChunk1ID_[2]);
	EXPECT_EQ(' ', header.subChunk1ID_[3]);
}

TEST(WaveReaderTests, AudioFormat)
{
	// Audio format is said to have a value of 1 for PCM.  A
	// value other than 1 indicates a form of compression.
	// We'll error on those for now as we only support PCM
	// currently.
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto header = waveReader.GetHeader();
	unsigned int audioFormat = (header.audioFormat_[1] << 8) + header.audioFormat_[0];
	EXPECT_EQ(1, audioFormat);
}

TEST(WaveReaderTests, Channels)
{
	// The channel count is at bytes 22-23.  We're only going to 
	// support mono (1) and stereo (2) for now.
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto header = waveReader.GetHeader();

	int channels = (header.channels_[1] << 8) + header.channels_[0];
	
	EXPECT_GT(channels, 0);  // Translates to "expect audioFormat > 0
	EXPECT_LT(channels, 3);  // Translates to "expect audioFormat < 3

	EXPECT_GT(waveReader.GetChannels(), static_cast<unsigned int>(0)); 
	EXPECT_LT(waveReader.GetChannels(), static_cast<unsigned int>(3));

	EXPECT_EQ(waveReader.GetChannels(), channels);  // We expect both values should match
}

TEST(WaveReaderTests, SampleRate)
{
	// This is a tough one to write a test for.  Typically the sample rate will be from 
	// say 8000 Hz to 192 KHz, (so 8,000 to 192,000), but theoritically the sample rate could
	// be between 1 and ??? skys the limit.
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	EXPECT_GT(waveReader.GetSampleRate(), static_cast<unsigned int>(0));
	EXPECT_LT(waveReader.GetSampleRate(), static_cast<unsigned int>(pow(2, 32) - 1));
}

TEST(WaveReaderTests, BitsPerSample)
{
	// This will usually be 8, 16, 24 or 32 but might also be 64
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	EXPECT_TRUE(waveReader.GetBitsPerSample() == 8 ||
				waveReader.GetBitsPerSample() == 16 ||
				waveReader.GetBitsPerSample() == 24 ||
				waveReader.GetBitsPerSample() == 32 ||
				waveReader.GetBitsPerSample() == 64);
}


TEST(WaveReaderTests, ByteRate)
{
	// The byte rate is at bytes 28-31.  And is said to be equal to
	// SampleRate * NumChannels * BitsPerSample/8
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	EXPECT_EQ(waveReader.GetSampleRate() * waveReader.GetChannels() * waveReader.GetBitsPerSample() / 8, waveReader.GetByteRate());
}

TEST(WaveReaderTests, BlockAlign)
{
	// According to the spec, BlockAlign are bytes 32-33 and are 
	// equal to Channels * BitsPerSample/8
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	EXPECT_EQ(waveReader.GetChannels() * waveReader.GetBitsPerSample()/8, waveReader.GetBlockAlign());
}

TEST(WaveReaderTests, ValidateSubchunk2ID)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto header = waveReader.GetHeader();
	EXPECT_EQ('d', header.subChunk2ID_[0]);
	EXPECT_EQ('a', header.subChunk2ID_[1]);
	EXPECT_EQ('t', header.subChunk2ID_[2]);
	EXPECT_EQ('a', header.subChunk2ID_[3]);
}

TEST(WaveReaderTests, ValidateSubchunk2Size)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");

	// From the spec, we can easily conclude that this should be the size 
	// of the file minus the size of the wave header.
	EXPECT_EQ(waveReader.GetFileSize() - waveReader.GetWaveHeaderSize(), waveReader.GetSubChunk2Size());
}

TEST(WaveReaderTests, GetAudioData)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav");
	auto audioData = waveReader.GetAudioData();

	EXPECT_EQ(waveReader.GetSampleCount(), waveReader.GetSubChunk2Size() / 2);

	EXPECT_NEAR(0.00003, audioData[0], 0.0001);
	EXPECT_NEAR(0.03140, audioData[1], 0.0001);
	EXPECT_NEAR(0.06259, audioData[2], 0.0001);

	// TestWaveFile.wav is a sine wave and I wanted to test a negative value so I'm
	// skipping down to sample 59
	EXPECT_NEAR(-0.26495, audioData[59], 0.0001);
}

TEST(WaveReaderTests, TestGettingCacheWhenAudioDataIsNotCached)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav", false);

	// If WaveReader is not doing internal buffering (i.e. bufferSmallFile=false) it should throw 
	// if the user attempts to get a raw audio buffer to the data.
	ASSERT_THROW(waveReader.GetAudioData(), Utilities::Exception);
}

TEST(WaveReaderTests, GetNonCachedAudioData)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav", false);
	auto audioData{waveReader.GetAudioData(3)};

	auto audioDataBuffer{audioData.GetData()};

	EXPECT_EQ(3, audioData.GetSize());

	if(audioData.GetSize() == 3)
	{
		EXPECT_NEAR(0.00003, audioDataBuffer[0], 0.0001);
		EXPECT_NEAR(0.03140, audioDataBuffer[1], 0.0001);
		EXPECT_NEAR(0.06259, audioDataBuffer[2], 0.0001);
	}
}

TEST(WaveReaderTests, GetNonCachedAudioDataFromParticularLocation)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav", false);
	auto audioData{waveReader.GetAudioData(43298, 3)};

	auto audioDataBuffer{audioData.GetData()};

	EXPECT_EQ(3, audioData.GetSize());

	if(audioData.GetSize() == 3)
	{
		EXPECT_NEAR(-0.0057983, audioDataBuffer[0], 0.0001);
		EXPECT_NEAR(0.0256653, audioDataBuffer[1], 0.0001);
		EXPECT_NEAR(0.0569458, audioDataBuffer[2], 0.0001);
	}
}

TEST(WaveReaderTests, GetNonCachedAudioDataSuccessiveReads)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav", false);

	{
		auto audioData{waveReader.GetAudioData(10)};
		auto audioDataBuffer{audioData.GetData()};
		EXPECT_EQ(10, audioData.GetSize());
		if(audioData.GetSize() == 10)
		{
			EXPECT_NEAR(0.00003, audioDataBuffer[0], 0.0001);
			EXPECT_NEAR(0.03140, audioDataBuffer[1], 0.0001);
			EXPECT_NEAR(0.06259, audioDataBuffer[2], 0.0001);
			EXPECT_NEAR(0.09360, audioDataBuffer[3], 0.0001);
			EXPECT_NEAR(0.12433, audioDataBuffer[4], 0.0001);
			EXPECT_NEAR(0.15451, audioDataBuffer[5], 0.0001);
			EXPECT_NEAR(0.18405, audioDataBuffer[6], 0.0001);
			EXPECT_NEAR(0.21286, audioDataBuffer[7], 0.0001);
			EXPECT_NEAR(0.24091, audioDataBuffer[8], 0.0001);
			EXPECT_NEAR(0.26794, audioDataBuffer[9], 0.0001);
		}
	}
	{
		auto audioData{waveReader.GetAudioData(10)};
		auto audioDataBuffer{audioData.GetData()};
		EXPECT_EQ(10, audioData.GetSize());

		if(audioData.GetSize() == 10)
		{
			EXPECT_NEAR(0.29395, audioDataBuffer[0], 0.0001);
			EXPECT_NEAR(0.31885, audioDataBuffer[1], 0.0001);
			EXPECT_NEAR(0.34244, audioDataBuffer[2], 0.0001);
			EXPECT_NEAR(0.36469, audioDataBuffer[3], 0.0001);
			EXPECT_NEAR(0.38550, audioDataBuffer[4], 0.0001);
			EXPECT_NEAR(0.40482, audioDataBuffer[5], 0.0001);
			EXPECT_NEAR(0.42252, audioDataBuffer[6], 0.0001);
			EXPECT_NEAR(0.43857, audioDataBuffer[7], 0.0001);
			EXPECT_NEAR(0.45291, audioDataBuffer[8], 0.0001);
			EXPECT_NEAR(0.46545, audioDataBuffer[9], 0.0001);
		}
	}
}

TEST(WaveReaderTests, GetNonCachedAudioDataParticularPointSuccessiveReads)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav", false);

	{
		auto audioData{waveReader.GetAudioData(6690, 10)};
		auto audioDataBuffer{audioData.GetData()};
		EXPECT_EQ(10, audioData.GetSize());
		if(audioData.GetSize() == 10)
		{
			EXPECT_NEAR(-0.50116, audioDataBuffer[0], 0.0001);
			EXPECT_NEAR(-0.50049, audioDataBuffer[1], 0.0001);
			EXPECT_NEAR(-0.49789, audioDataBuffer[2], 0.0001);
			EXPECT_NEAR(-0.49332, audioDataBuffer[3], 0.0001);
			EXPECT_NEAR(-0.48679, audioDataBuffer[4], 0.0001);
			EXPECT_NEAR(-0.47842, audioDataBuffer[5], 0.0001);
			EXPECT_NEAR(-0.46808, audioDataBuffer[6], 0.0001);
			EXPECT_NEAR(-0.45593, audioDataBuffer[7], 0.0001);
			EXPECT_NEAR(-0.44205, audioDataBuffer[8], 0.0001);
			EXPECT_NEAR(-0.42633, audioDataBuffer[9], 0.0001);
		}
	}
	{
		auto audioData{waveReader.GetAudioData(20)};
		auto audioDataBuffer{audioData.GetData()};
		EXPECT_EQ(20, audioData.GetSize());

		if(audioData.GetSize() == 20)
		{
			EXPECT_NEAR(-0.40903, audioDataBuffer[0], 0.0001);
			EXPECT_NEAR(-0.39005, audioDataBuffer[1], 0.0001);
			EXPECT_NEAR(-0.36963, audioDataBuffer[2], 0.0001);
			EXPECT_NEAR(-0.34766, audioDataBuffer[3], 0.0001);
			EXPECT_NEAR(-0.32434, audioDataBuffer[4], 0.0001);
			EXPECT_NEAR(-0.29984, audioDataBuffer[5], 0.0001);
			EXPECT_NEAR(-0.27408, audioDataBuffer[6], 0.0001);
			EXPECT_NEAR(-0.24719, audioDataBuffer[7], 0.0001);
			EXPECT_NEAR(-0.21945, audioDataBuffer[8], 0.0001);
			EXPECT_NEAR(-0.19080, audioDataBuffer[9], 0.0001);
			EXPECT_NEAR(-0.16134, audioDataBuffer[10], 0.0001);
			EXPECT_NEAR(-0.13132, audioDataBuffer[11], 0.0001);
			EXPECT_NEAR(-0.10080, audioDataBuffer[12], 0.0001);
			EXPECT_NEAR(-0.06982, audioDataBuffer[13], 0.0001);
			EXPECT_NEAR(-0.03851, audioDataBuffer[14], 0.0001);
			EXPECT_NEAR(-0.00720, audioDataBuffer[15], 0.0001);
			EXPECT_NEAR(0.024200, audioDataBuffer[16], 0.0001);
			EXPECT_NEAR(0.055481, audioDataBuffer[17], 0.0001);
			EXPECT_NEAR(0.086670, audioDataBuffer[18], 0.0001);
			EXPECT_NEAR(0.117371, audioDataBuffer[19], 0.0001);
		}
	}
}


TEST(WaveReaderTests, GetNonCachedAudioDataTestEOF)
{
	WaveFile::WaveFileReader waveReader("TestWaveFile.wav", false);

	{
		auto audioData{waveReader.GetAudioData(44095, 10)};
		auto audioDataBuffer{audioData.GetData()};
		EXPECT_EQ(5, audioData.GetSize());

		if(audioData.GetSize() == 5)
		{
			EXPECT_NEAR(-0.15460, audioDataBuffer[0], 0.0001);
			EXPECT_NEAR(-0.12445, audioDataBuffer[1], 0.0001);
			EXPECT_NEAR(-0.09372, audioDataBuffer[2], 0.0001);
			EXPECT_NEAR(-0.06268, audioDataBuffer[3], 0.0001);
			EXPECT_NEAR(-0.03149, audioDataBuffer[4], 0.0001);
		}
	}

	// The TestWaveFile has 44100 samples in it.  Once we've read to the end-of-data we 
	// should no longer get any additional data on subsequent reads.
	{
		auto audioData{waveReader.GetAudioData(10)};
		EXPECT_EQ(0, audioData.GetSize());
	}
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

