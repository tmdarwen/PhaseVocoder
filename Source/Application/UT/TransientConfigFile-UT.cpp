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
#include <Application/TransientConfigFile.h>
#include <Utilities/Exception.h>

TEST(TransientConfigFile, TestNonExistantFile)
{
	EXPECT_THROW(TransientConfigFile("InvalidFilename"), Utilities::Exception);
}

TEST(TransientConfigFile, TestIllFormattedFile)
{
	EXPECT_THROW(TransientConfigFile("IncorrectTransientConfigFile.yaml"), Utilities::Exception);
}

TEST(TransientConfigFile, TestGettingTransients)
{
	TransientConfigFile transientConfigFile("TransientConfigFile.yaml");
	auto transients{transientConfigFile.GetTransients()};

	EXPECT_EQ(5, transients.size());
	if(transients.size() == 5)
	{
		EXPECT_EQ(100, transients[0]);
		EXPECT_EQ(200, transients[1]);
		EXPECT_EQ(300, transients[2]);
		EXPECT_EQ(400, transients[3]);
		EXPECT_EQ(500, transients[4]);
	}
}

TEST(TransientConfigFile, TestGettingChannelSpecificTransients)
{
	TransientConfigFile transientConfigFile("ChannelSpecificTransientConfigFile.yaml");

	auto leftChannelTransients{transientConfigFile.GetLeftChannelTransients()};

	EXPECT_EQ(8, leftChannelTransients.size());
	if(leftChannelTransients.size() == 8)
	{
		EXPECT_EQ(100, leftChannelTransients[0]);
		EXPECT_EQ(200, leftChannelTransients[1]);
		EXPECT_EQ(275, leftChannelTransients[2]);
		EXPECT_EQ(300, leftChannelTransients[3]);
		EXPECT_EQ(400, leftChannelTransients[4]);
		EXPECT_EQ(445, leftChannelTransients[5]);
		EXPECT_EQ(500, leftChannelTransients[6]);
		EXPECT_EQ(550, leftChannelTransients[7]);
	}

	auto rightChannelTransients{transientConfigFile.GetRightChannelTransients()};

	EXPECT_EQ(7, rightChannelTransients.size());
	if(rightChannelTransients.size() == 7)
	{
		EXPECT_EQ(100, rightChannelTransients[0]);
		EXPECT_EQ(150, rightChannelTransients[1]);
		EXPECT_EQ(200, rightChannelTransients[2]);
		EXPECT_EQ(300, rightChannelTransients[3]);
		EXPECT_EQ(340, rightChannelTransients[4]);
		EXPECT_EQ(400, rightChannelTransients[5]);
		EXPECT_EQ(500, rightChannelTransients[6]);
	}
}
