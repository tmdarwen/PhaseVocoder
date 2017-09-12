#include <gtest/gtest.h>
#include <Signal/LowPassFilter.h>
#include <WaveFile/WaveFileReader.h>
#include <WaveFile/WaveFileWriter.h>
#include <Utilities/Exception.h>
#include <Utilities/File.h>

void DoLowPassFiltering(const std::string& inputFilename, const std::string& outputFilename, std::size_t cutoffFrequency)
{
	WaveFile::WaveFileReader inputWaveFile{inputFilename};

	double cutoffRatio{static_cast<double>(cutoffFrequency) / static_cast<double>(inputWaveFile.GetSampleRate())};
	Signal::LowPassFilter lowPassFilter{cutoffRatio};

	lowPassFilter.SubmitAudioData(AudioData(inputWaveFile.GetAudioData(), inputWaveFile.GetSampleCount()));

	WaveFile::WaveFileWriter waveWriter(outputFilename, inputWaveFile.GetChannels(), inputWaveFile.GetSampleRate(), inputWaveFile.GetBitsPerSample());
	waveWriter.AppendAudioData(lowPassFilter.FlushAudioData().GetData());
}

TEST(LowPassFilterTests, TestInvalidCutoff)
{
	EXPECT_THROW(Signal::LowPassFilter(0.000001), Utilities::Exception);
	EXPECT_THROW(Signal::LowPassFilter(0.6), Utilities::Exception);
}

TEST(LowPassFilterTests, TestMinimumSamplesNeededForProcessing)
{
	{
		Signal::LowPassFilter lowPassFilter{0.25};
		EXPECT_EQ(100, lowPassFilter.MinimumSamplesNeededForProcessing());
	}

	{
		Signal::LowPassFilter lowPassFilter{0.25, 125};
		EXPECT_EQ(125, lowPassFilter.MinimumSamplesNeededForProcessing());
	}

	{
		Signal::LowPassFilter lowPassFilter{0.25, 75};
		EXPECT_EQ(75, lowPassFilter.MinimumSamplesNeededForProcessing());
	}
}

TEST(LowPassFilterTests, LowPassAt1000Hz)
{
	DoLowPassFiltering("400HzSineAnd2121HzSine.wav", "400HzSineAnd2121HzSineLowPassFilteredAt1000HzCurrentResults.wav", 1000);
 	EXPECT_TRUE(Utilities::File::CheckIfFilesMatch("400HzSineAnd2121HzSineLowPassFilteredAt1000Hz.wav", 
															"400HzSineAnd2121HzSineLowPassFilteredAt1000HzCurrentResults.wav"));
}

TEST(LowPassFilterTests, LowPassAt6000Hz)
{
	DoLowPassFiltering("5000HzSineAnd9797HzSine.wav", "5000HzSineAnd9797HzSineLowPassFilteredAt6000HzCurrentResults.wav", 6000);
 	EXPECT_TRUE(Utilities::File::CheckIfFilesMatch("5000HzSineAnd9797HzSineLowPassFilteredAt6000Hz.wav", 
															"5000HzSineAnd9797HzSineLowPassFilteredAt6000HzCurrentResults.wav"));
}

TEST(LowPassFilterTests, LowPassAt8000Hz)
{
	DoLowPassFiltering("222HzSineAnd19000HzSine.wav", "222HzSineAnd19000HzSineLowPassFilteredAt8000HzCurrentResults.wav", 8000);
 	EXPECT_TRUE(Utilities::File::CheckIfFilesMatch("222HzSineAnd19000HzSineLowPassFilteredAt8000Hz.wav", 
															"222HzSineAnd19000HzSineLowPassFilteredAt8000HzCurrentResults.wav"));
}