#include <gtest/gtest.h>
#include <Signal/Resampler.h>
#include <Signal/SignalConversion.h>
#include <WaveFile/WaveFileReader.h>
#include <WaveFile/WaveFileWriter.h>
#include <Utilities/File.h>

void DoResampling(const std::string& inputFilename, const std::string& outputFilename, std::size_t newSampleRate)
{
	WaveFile::WaveFileReader inputWaveFile{inputFilename};

	double resampleRatio{static_cast<double>(newSampleRate) / static_cast<double>(inputWaveFile.GetSampleRate())};

	Signal::Resampler resampler{inputWaveFile.GetSampleRate(), resampleRatio};

	resampler.SubmitAudioData(AudioData(inputWaveFile.GetAudioData(), inputWaveFile.GetSampleCount()));

	WaveFile::WaveFileWriter waveWriter(outputFilename, inputWaveFile.GetChannels(), newSampleRate, inputWaveFile.GetBitsPerSample());
	waveWriter.AppendAudioData(resampler.FlushAudioData().GetData());
}

TEST(ResamplerTests, SineWaveResampled)
{
	DoResampling("100HzSineWaveAt32768Hz.wav", "100HzSineWaveAt32768HzResampledCurrentResult.wav", 38000);
 	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("100HzSineWaveAt32768HzResampled.wav", "100HzSineWaveAt32768HzResampledCurrentResult.wav"));
}

TEST(ResamplerTests, PianoKeyResampled)
{
	DoResampling("SinglePianoKey.wav", "SinglePianoKeyResampledCurrentResult.wav", 24123);
 	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("SinglePianoKeyResampled.wav", "SinglePianoKeyResampledCurrentResult.wav"));
}

TEST(ResamplerTests, TestLowPassFilterAt2000Hz)
{
	DoResampling("400HzSineAnd2121HzSine.wav", "400HzSineAnd2121HzSineResampledCurrentResult.wav", 2000);
 	EXPECT_TRUE(Utilities::File::CheckIfFilesMatch("400HzSineAnd2121HzSineResampled.wav", "400HzSineAnd2121HzSineResampledCurrentResult.wav"));
}

TEST(ResamplerTests, TestLowPassFilterAt5000Hz)
{
	DoResampling("222HzSineAnd19000HzSine.wav", "222HzSineAnd19000HzSineResampledCurrentResult.wav", 5000);
 	EXPECT_TRUE(Utilities::File::CheckIfFilesMatch("222HzSineAnd19000HzSineResampled.wav", "222HzSineAnd19000HzSineResampledCurrentResult.wav"));
}

TEST(ResamplerTests, TestLowPassFilterAt15000Hz)
{
	DoResampling("5000HzSineAnd9797HzSine.wav", "5000HzSineAnd9797HzSineResampledCurrentResult.wav", 15000);
 	EXPECT_TRUE(Utilities::File::CheckIfFilesMatch("5000HzSineAnd9797HzSineResampled.wav", "5000HzSineAnd9797HzSineResampledCurrentResult.wav"));
}
