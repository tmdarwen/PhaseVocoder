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
#include <iostream>
#include <Signal/SignalConversion.h>
#include <vector>

// Note that we test clipping here too (note, the value over 1.0 or under -1.0
TEST(SignalConversionTests, ConvertFrom64BitTo16Bit)
{
	std::vector<double>  inputSignal{0, 0.25, 0.5, 0.75, 1.0, 1.4, -0.25, -0.5, -0.75, -1.0, -1.2};

	{
		auto outputSignal{Signal::ConvertFloat64ToSigned16(inputSignal)};

		EXPECT_EQ(inputSignal.size(), outputSignal.size());
		EXPECT_EQ(0, outputSignal[0]);
		EXPECT_EQ(8192, outputSignal[1]);
		EXPECT_EQ(16384, outputSignal[2]);
		EXPECT_EQ(24575, outputSignal[3]);
		EXPECT_EQ(32767, outputSignal[4]);
		EXPECT_EQ(32767, outputSignal[5]);
		EXPECT_EQ(-8192, outputSignal[6]);
		EXPECT_EQ(-16384, outputSignal[7]);
		EXPECT_EQ(-24576, outputSignal[8]);
		EXPECT_EQ(-32768, outputSignal[9]);
		EXPECT_EQ(-32768, outputSignal[10]);
	}

	{
		auto outputSignal{Signal::ConvertFloat64ToSigned16(inputSignal, 3)};

		EXPECT_EQ(3, outputSignal.size());
		EXPECT_EQ(0, outputSignal[0]);
		EXPECT_EQ(8192, outputSignal[1]);
		EXPECT_EQ(16384, outputSignal[2]);
	}
}

TEST(SignalConversionTests, ConvertFrom16BitTo64Bit)
{
	std::vector<int16_t>  inputSignal{0, 8192, 16384, 24575, 32767, -8192, -16384, -24576, -32768};

	{
		auto outputSignal{Signal::ConvertSigned16ToFloat64(inputSignal)};

		EXPECT_EQ(inputSignal.size(), outputSignal.size());
		EXPECT_NEAR(0, outputSignal[0], 0.0001);
		EXPECT_NEAR(0.25, outputSignal[1], 0.0001);
		EXPECT_NEAR(0.5, outputSignal[2], 0.0001);
		EXPECT_NEAR(0.75, outputSignal[3], 0.0001);
		EXPECT_NEAR(1.0, outputSignal[4], 0.0001);
		EXPECT_NEAR(-0.25, outputSignal[5], 0.0001);
		EXPECT_NEAR(-0.5, outputSignal[6], 0.0001);
		EXPECT_NEAR(-0.75, outputSignal[7], 0.0001);
		EXPECT_NEAR(-1.0, outputSignal[8], 0.0001);
	}

	{
		auto outputSignal{Signal::ConvertSigned16ToFloat64(inputSignal, 5)};

		EXPECT_EQ(5, outputSignal.size());
		EXPECT_NEAR(0, outputSignal[0], 0.0001);
		EXPECT_NEAR(0.25, outputSignal[1], 0.0001);
		EXPECT_NEAR(0.5, outputSignal[2], 0.0001);
		EXPECT_NEAR(0.75, outputSignal[3], 0.0001);
		EXPECT_NEAR(1.0, outputSignal[4], 0.0001);
	}
}

TEST(SignalConversionTests, ConvertFrom64BitStereoTo16BitInterleaved)
{
	AudioData leftChannel({0, 0.25, 0.5, 0.75, 1.0, 1.4, -0.25, -0.5, -0.75, -1.0, -1.2});
	AudioData rightChannel({0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0});

	auto outputSignal{Signal::ConvertAudioDataToInterleavedSigned16(leftChannel, rightChannel)};

	EXPECT_EQ(0, outputSignal[0]);
	EXPECT_EQ(0, outputSignal[1]);
	EXPECT_EQ(8192, outputSignal[2]);
	EXPECT_EQ(3277, outputSignal[3]);
	EXPECT_EQ(16384, outputSignal[4]);
	EXPECT_EQ(6553, outputSignal[5]);
	EXPECT_EQ(24575, outputSignal[6]);
	EXPECT_EQ(9830, outputSignal[7]);
	EXPECT_EQ(32767, outputSignal[8]);
	EXPECT_EQ(13107, outputSignal[9]);
	EXPECT_EQ(32767, outputSignal[10]);
	EXPECT_EQ(16384, outputSignal[11]);
	EXPECT_EQ(-8192, outputSignal[12]);
	EXPECT_EQ(19660, outputSignal[13]);
	EXPECT_EQ(-16384, outputSignal[14]);
	EXPECT_EQ(22937, outputSignal[15]);
	EXPECT_EQ(-24576, outputSignal[16]);
	EXPECT_EQ(26214, outputSignal[17]);
	EXPECT_EQ(-32768, outputSignal[18]);
	EXPECT_EQ(29490, outputSignal[19]);
	EXPECT_EQ(-32768, outputSignal[20]);
	EXPECT_EQ(32767, outputSignal[21]);
}
