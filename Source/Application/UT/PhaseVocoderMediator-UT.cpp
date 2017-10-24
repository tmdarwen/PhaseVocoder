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

#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <Application/PhaseVocoderMediator.h>
#include <Utilities/Exception.h>
#include <Utilities/File.h>

namespace PhaseVocoderMediatorUT {

void Stretch(const std::string& inputFile, const std::string& outputFile, double stretchFactor)
{
	PhaseVocoderSettings phaseVocoderSettings;
	phaseVocoderSettings.SetInputWaveFile(inputFile);
	phaseVocoderSettings.SetOutputWaveFile(outputFile);
	phaseVocoderSettings.SetStretchFactor(stretchFactor);

	PhaseVocoderMediator phaseVocoderMediator(phaseVocoderSettings);
	phaseVocoderMediator.Process();
}

void Resample(const std::string& inputFile, const std::string& outputFile, std::size_t newSampleRate)
{
	PhaseVocoderSettings phaseVocoderSettings;
	phaseVocoderSettings.SetInputWaveFile(inputFile);
	phaseVocoderSettings.SetOutputWaveFile(outputFile);
	phaseVocoderSettings.SetResampleValue(newSampleRate);

	PhaseVocoderMediator phaseVocoderMediator(phaseVocoderSettings);
	phaseVocoderMediator.Process();
}

void PitchShift(const std::string& inputFile, const std::string& outputFile, double pitchChange)
{
	PhaseVocoderSettings phaseVocoderSettings;
	phaseVocoderSettings.SetInputWaveFile(inputFile);
	phaseVocoderSettings.SetOutputWaveFile(outputFile);
	phaseVocoderSettings.SetPitchShiftValue(pitchChange);

	PhaseVocoderMediator phaseVocoderMediator(phaseVocoderSettings);
	phaseVocoderMediator.Process();
}

std::vector<std::size_t> SpecificValleyToPeakRatio(const std::string& inputFile, double valleyToPeakRatio)
{
	PhaseVocoderSettings phaseVocoderSettings;
	phaseVocoderSettings.SetInputWaveFile(inputFile);
	phaseVocoderSettings.SetValleyToPeakRatio(valleyToPeakRatio);
	phaseVocoderSettings.SetDisplayTransients();

	PhaseVocoderMediator phaseVocoderMediator(phaseVocoderSettings);
	phaseVocoderMediator.Process();

	return phaseVocoderMediator.GetTransients(0);
}

// I'm disabling these for MSVC debug builds b/c they will take minites.  It's the FFT that appears to be really slow in the debug build.  
// In a release build, running these tests just take a few seconds.
#ifndef _DEBUG
TEST(PhaseVocoderMediator, StretchTest1)
{
	PhaseVocoderMediatorUT::Stretch("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResult1.25.wav", 1.25);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev1.25.wav", "BuiltToSpillBeatAbbrevCurrentResult1.25.wav"));
}

TEST(PhaseVocoderMediator, StretchTest2)
{
	PhaseVocoderMediatorUT::Stretch("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResult1.50.wav", 1.50);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev1.50.wav", "BuiltToSpillBeatAbbrevCurrentResult1.50.wav"));
}

TEST(PhaseVocoderMediator, StretchTest3)
{
	PhaseVocoderMediatorUT::Stretch("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResult1.75.wav", 1.75);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev1.75.wav", "BuiltToSpillBeatAbbrev1.75.wav"));
}

TEST(PhaseVocoderMediator, CompressTest1)
{
	PhaseVocoderMediatorUT::Stretch("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResult0.75.wav", 0.75);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev0.75.wav", "BuiltToSpillBeatAbbrevCurrentResult0.75.wav"));
}

TEST(PhaseVocoderMediator, CompressTest2)
{
	PhaseVocoderMediatorUT::Stretch("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResult0.50.wav", 0.50);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev0.50.wav", "BuiltToSpillBeatAbbrevCurrentResult0.50.wav" ));
}

TEST(PhaseVocoderMediator, CompressTest3)
{
	PhaseVocoderMediatorUT::Stretch("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResult0.25.wav", 0.25);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev0.25.wav", "BuiltToSpillBeatAbbrevCurrentResult0.25.wav"));
}
#endif

TEST(PhaseVocoderMediator, ResampleTest1)
{
	PhaseVocoderMediatorUT::Resample("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResample48000.wav", 48000);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrevResample48000.wav", "BuiltToSpillBeatAbbrevCurrentResample48000.wav"));
}

TEST(PhaseVocoderMediator, ResampleTest2)
{
	PhaseVocoderMediatorUT::Resample("BuiltToSpillBeatAbbrev.wav", "BuiltToSpillBeatAbbrevCurrentResample32123.wav", 32123);
	EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrevResample32123.wav", "BuiltToSpillBeatAbbrevCurrentResample32123.wav"));
}


// TODO: Will add these and more UTs after additional enhancements (like low pass filter on Resampler) are added.

/*
TEST(PhaseVocoderMediator, PitchShift2)
{
	PhaseVocoderMediatorUT::PitchShift("M:\\NonSys\\DarwenAudio\\TransientTestAudio\\PianoMiddleC.wav", "p2.wav", 3.0);
	//EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev0.25.wav", "BuiltToSpillBeatAbbrevCurrentResult0.25.wav"));
}
*/

/*
TEST(PhaseVocoderMediator, PitchShiftResampleAndStretchTest1)
{
	//PhaseVocoderMediatorUT::PitchShiftResampleAndStretch("SinglePianoKey.wav", "SinglePianoKeyOut.wav", 12.0, 48000, 1.5);
	//EXPECT_EQ(true, Utilities::File::CheckIfFilesMatch("BuiltToSpillBeatAbbrev0.25.wav", "BuiltToSpillBeatAbbrevCurrentResult0.25.wav"));

	PhaseVocoderMediatorUT::PitchShift("M:\\NonSys\\DarwenAudio\\TransientTestAudio\\TeenSpirit.wav", "out.wav", -2.0);
}
*/

/*
TEST(PhaseVocoderMediator, PitchShift3)
{
	PhaseVocoderMediator phaseVocoderMediator;
	phaseVocoderMediator.SetInputWaveFile("M:\\NonSys\\DarwenAudio\\TransientTestAudio\\4GhostsI.wav");
	phaseVocoderMediator.SetOutputWaveFile("out.wav");
	phaseVocoderMediator.SetPitchShiftValue(2.0);
	//phaseVocoderMediator.SetTransientConfigFile("c:\\tmp\\GetLuckyTransients.yaml");
	phaseVocoderMediator.Process();
}
*/



TEST(TransientDetectorTests, DefaultValleyToPeakRatio)
{
	PhaseVocoderSettings phaseVocoderSettings;
	phaseVocoderSettings.SetInputWaveFile("SweetEmotion.wav");
	phaseVocoderSettings.SetDisplayTransients();

	PhaseVocoderMediator phaseVocoderMediator(phaseVocoderSettings);
	phaseVocoderMediator.Process();

	auto transientPositions{phaseVocoderMediator.GetTransients(0)};

	EXPECT_EQ(8, transientPositions.size());
	if(transientPositions.size() == 8)
	{
		EXPECT_EQ(0, transientPositions[0]);
		EXPECT_EQ(28288, transientPositions[1]);
		EXPECT_EQ(56416, transientPositions[2]);
		EXPECT_EQ(84032, transientPositions[3]);
		EXPECT_EQ(97472, transientPositions[4]);
		EXPECT_EQ(111296, transientPositions[5]);
		EXPECT_EQ(125184, transientPositions[6]);
		EXPECT_EQ(139040, transientPositions[7]);
	}
}

TEST(TransientDetectorTests, SpecificValleyToPeakRatioSameAsDefault)
{
	auto transientPositions{SpecificValleyToPeakRatio("SweetEmotion.wav", 1.5)};

	EXPECT_EQ(8, transientPositions.size());
	if(transientPositions.size() == 8)
	{
		EXPECT_EQ(0, transientPositions[0]);
		EXPECT_EQ(28288, transientPositions[1]);
		EXPECT_EQ(56416, transientPositions[2]);
		EXPECT_EQ(84032, transientPositions[3]);
		EXPECT_EQ(97472, transientPositions[4]);
		EXPECT_EQ(111296, transientPositions[5]);
		EXPECT_EQ(125184, transientPositions[6]);
		EXPECT_EQ(139040, transientPositions[7]);
	}
}

TEST(TransientDetectorTests, SpecifcTransientRatioTestStrict)
{
	auto transientPositions{SpecificValleyToPeakRatio("SweetEmotion.wav", 0.5)};

	EXPECT_EQ(17, transientPositions.size());
	if(transientPositions.size() == 17)
	{
		EXPECT_EQ(0, transientPositions[0]);
		EXPECT_EQ(27968, transientPositions[1]);
		EXPECT_EQ(43040, transientPositions[2]);
		EXPECT_EQ(56224, transientPositions[3]);
		EXPECT_EQ(64416, transientPositions[4]);
		EXPECT_EQ(66080, transientPositions[5]);
		EXPECT_EQ(79744, transientPositions[6]);
		EXPECT_EQ(84032, transientPositions[7]);
		EXPECT_EQ(91744, transientPositions[8]);
		EXPECT_EQ(97376, transientPositions[9]);
		EXPECT_EQ(102144, transientPositions[10]);
		EXPECT_EQ(111104, transientPositions[11]);
		EXPECT_EQ(118368, transientPositions[12]);
		EXPECT_EQ(121344, transientPositions[13]);
		EXPECT_EQ(125120, transientPositions[14]);
		EXPECT_EQ(139040, transientPositions[15]);
		EXPECT_EQ(153408, transientPositions[16]);
	}
}

TEST(TransientDetectorTests, SpecifcTransientRatioRelaxed)
{
	auto transientPositions{SpecificValleyToPeakRatio("SweetEmotion.wav", 2.0)};

	EXPECT_EQ(2, transientPositions.size());
	if(transientPositions.size() == 2)
	{
		EXPECT_EQ(0, transientPositions[0]);
		EXPECT_EQ(28512, transientPositions[1]);
	}
}

}
