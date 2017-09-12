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
