#include <gtest/gtest.h>
#include <Utilities/Exception.h>

void TestThrow()
{
	Utilities::Exception theException("Throwing");
	throw theException;
}

void TestThrowWithFilename()
{
	Utilities::Exception theException("Throwing", "SomeFilename");
	throw theException;
}

void TestThrowWithFilenameAndLineNumber()
{
	Utilities::Exception theException("Throwing", "SomeFilename", 99);
	throw theException;
}

TEST(UtilitiesException, TestThrow)
{
	// Since Utilities::Exception inherits from std::exception, it's considered both types
	ASSERT_THROW(TestThrow(), std::exception);
	ASSERT_THROW(TestThrow(), Utilities::Exception);
}

TEST(UtilitiesException, CatchWhat)
{
	try
	{
		TestThrow();
	}
	catch(std::exception& theException)
	{
		EXPECT_STREQ("Throwing", theException.what());
	}

	try
	{
		TestThrow();
	}
	catch(Utilities::Exception& theException)
	{
		EXPECT_STREQ("Throwing", theException.what());
	}
}

TEST(UtilitiesException, TestThrowWithFilename)
{
	try
	{
		TestThrowWithFilename();
	}
		catch(std::exception& theException)
	{
		EXPECT_STREQ("Throwing SomeFilename:0", theException.what());
	}

	try
	{
		TestThrowWithFilename();
	}
		catch(Utilities::Exception& theException)
	{
		EXPECT_STREQ("Throwing SomeFilename:0", theException.what());
	}
}

TEST(UtilitiesException, TestThrowWithFilenameAndLineNumber)
{
	try
	{
		TestThrowWithFilenameAndLineNumber();
	}
	catch(std::exception& theException)
	{
		EXPECT_STREQ("Throwing SomeFilename:99", theException.what());
	}

	try
	{
		TestThrowWithFilenameAndLineNumber();
	}
	catch(Utilities::Exception& theException)
	{
		EXPECT_STREQ("Throwing SomeFilename:99", theException.what());
	}
}
