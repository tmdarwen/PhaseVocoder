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
	TransientConfigFile transientConfigFile("CorrectTransientConfigFile.yaml");
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
