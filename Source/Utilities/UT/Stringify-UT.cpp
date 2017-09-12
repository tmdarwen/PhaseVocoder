#include <gtest/gtest.h>
#include <Utilities/Stringify.h>

TEST(Stringify, TestStringify)
{
	int intValue(3);
	std::string theString("The magic number is " + Utilities::Stringify(intValue));
	EXPECT_STREQ("The magic number is 3", theString.c_str());
}

TEST(Stringify, StringifyCharacters)
{
	// Unfortunately, I have not yet figured out how to Stringify characters.  The following line does not work:
	//std::string theString(Utilities::Stringify('H') + Utilities::Stringify('e'));
	std::string theString(Utilities::Stringify("H") + Utilities::Stringify("e"));
	EXPECT_STREQ("He", theString.c_str());
}

TEST(Stringify, TestCreateStringWithSpaceDelimiter)
{
	std::string theString = Utilities::CreateString(" ", "Hello", "World", 1, 2, "Three");
	EXPECT_STREQ("Hello World 1 2 Three", theString.c_str());
}

TEST(Stringify, TestCreateStringWithPipeDelimiter)
{
	std::string theString = Utilities::CreateString("|", 1, "Two", 3, "Four", 5);
	EXPECT_STREQ("1|Two|3|Four|5", theString.c_str());
}

TEST(Stringify, TestDelimitedStringToVectorOfStrings)
{
	std::string theString("Hello-World-One-Two");
	auto parsedResult = Utilities::DelimitedStringToVectorOfStrings(theString, '-');

	EXPECT_EQ(4, parsedResult.size());
	EXPECT_STREQ("Hello", parsedResult[0].c_str());
	EXPECT_STREQ("World", parsedResult[1].c_str());
	EXPECT_STREQ("One", parsedResult[2].c_str());
	EXPECT_STREQ("Two", parsedResult[3].c_str());
}

TEST(Stringify, TestToUpper)
{
	std::string theString("Hello World");
	auto uc = Utilities::ConvertStringToUppercase(theString);

	EXPECT_STREQ("HELLO WORLD", uc.c_str());
}
