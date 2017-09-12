#include <gtest/gtest.h>
#include <string>
#include <Application/CommandLineArguments.h>
#include <Utilities/Exception.h>

// This class constructs command line args as they typically would be by the system before main is called
class FakeCommandLineArguments
{
	public:
		FakeCommandLineArguments() { }

		~FakeCommandLineArguments()
		{
			Deallocate();
		}

		void PushArgument(const std::string& argument)
		{
			arguments_.push_back(argument);	
		}

		int GetArgc()
		{
			return static_cast<int>(arguments_.size()) + 1;	
		}

		char** GetArgv()
		{
			Deallocate();

			char** argv = new char*[GetArgc()];
			argv[0] = new char[64];
			MyStringCopy(argv[0], "ExeName", 63);

			std::size_t index{1};
			for(auto argument : arguments_)
			{
				argv[index] = new char[argument.size() + 1];
				MyStringCopy(argv[index], argument.c_str(), argument.size());
				++index;
			}

			return argv;
		}

		void Deallocate()
		{
			if(argv_ == nullptr)
			{
				return;
			}

			for(std::size_t i{0}; i < arguments_.size() + 1; ++i)
			{
				delete [] argv_[i];
			}

			delete [] argv_;
		}

		void MyStringCopy(char* destination, const char* source, std::size_t length)
		{
			
			std::size_t i{0};
			for( ; i < length; ++i)
			{
				destination[i] = source[i];
			}

			destination[i] = 0;
		}

	private:
		char** argv_{nullptr};
		std::vector<std::string> arguments_;
};

CommandLineArguments CreateCommandLineArguments(const std::string& commandLine)
{
	FakeCommandLineArguments fakeCommandLineArguments;
	for(auto arg : Utilities::DelimitedStringToVectorOfStrings(commandLine, ' '))
	{
		fakeCommandLineArguments.PushArgument(arg);
	}

	return CommandLineArguments(fakeCommandLineArguments.GetArgc(), fakeCommandLineArguments.GetArgv());
}

void VerifyTypicalUsage(const CommandLineArguments& commandLineArguments)
{
	EXPECT_TRUE(commandLineArguments.IsValid());
	EXPECT_STREQ("InputFileName.wav", commandLineArguments.GetInputFilename().c_str());
	EXPECT_STREQ("OutputFileName.wav", commandLineArguments.GetOutputFilename().c_str());
	EXPECT_EQ(1.25, commandLineArguments.GetStretchFactor());
	EXPECT_FALSE(commandLineArguments.ShowTransients());
	EXPECT_FALSE(commandLineArguments.TransientConfigFileGiven());
	EXPECT_STREQ("", commandLineArguments.GetTransientConfigFilename().c_str());
	EXPECT_FALSE(commandLineArguments.Help());
	EXPECT_FALSE(commandLineArguments.LongHelp());
}

TEST(CommandLineArguments, TestTypicalUsage)
{
	VerifyTypicalUsage(CreateCommandLineArguments("--input InputFileName.wav --output OutputFileName.wav --stretch 1.25"));
	VerifyTypicalUsage(CreateCommandLineArguments("-i InputFileName.wav -o OutputFileName.wav -s 1.25"));
}

void VerifyNoActionGiven(const CommandLineArguments& commandLineArguments)
{
	EXPECT_FALSE(commandLineArguments.IsValid());
	EXPECT_STREQ("Nothing to do.  No action specified.", commandLineArguments.GetErrorMessage().c_str());
}

TEST(CommandLineArguments, TestNoStretchFactorGiven)
{
	VerifyNoActionGiven(CreateCommandLineArguments("--input InputFileName.wav --output OutputFileName.wav"));
	VerifyNoActionGiven(CreateCommandLineArguments("-i InputFileName.wav -o OutputFileName.wav"));
}

void VerifyInputFileOnly(const CommandLineArguments& commandLineArguments)
{
	EXPECT_FALSE(commandLineArguments.IsValid());
	EXPECT_STREQ("Nothing to do.  No action specified.", commandLineArguments.GetErrorMessage().c_str());
}

TEST(CommandLineArguments, TestInputFileOnly)
{
	VerifyNoActionGiven(CreateCommandLineArguments("--input InputFileName.wav"));
	VerifyNoActionGiven(CreateCommandLineArguments("-i InputFileName.wav"));
}

// Giving no command line parameters shows the "short" help
TEST(CommandLineArguments, TestNoParametersGiven)
{
	FakeCommandLineArguments fakeCommandLineArguments;

	CommandLineArguments commandLineArguments(fakeCommandLineArguments.GetArgc(), fakeCommandLineArguments.GetArgv());

	EXPECT_TRUE(commandLineArguments.IsValid());
	EXPECT_STREQ("", commandLineArguments.GetInputFilename().c_str());
	EXPECT_STREQ("", commandLineArguments.GetOutputFilename().c_str());
	EXPECT_EQ(0.00, commandLineArguments.GetStretchFactor());
	EXPECT_FALSE(commandLineArguments.TransientConfigFileGiven());
	EXPECT_FALSE(commandLineArguments.ShowTransients());
	EXPECT_STREQ("", commandLineArguments.GetTransientConfigFilename().c_str());
	EXPECT_TRUE(commandLineArguments.Help());
	EXPECT_FALSE(commandLineArguments.LongHelp());
}

void VerifyJustGettingTransients(const CommandLineArguments& commandLineArguments)
{
	EXPECT_TRUE(commandLineArguments.IsValid());
	EXPECT_STREQ("InputFileName.wav", commandLineArguments.GetInputFilename().c_str());
	EXPECT_STREQ("", commandLineArguments.GetOutputFilename().c_str());
	EXPECT_EQ(0.00, commandLineArguments.GetStretchFactor());
	EXPECT_FALSE(commandLineArguments.TransientConfigFileGiven());
	EXPECT_TRUE(commandLineArguments.ShowTransients());
	EXPECT_STREQ("", commandLineArguments.GetTransientConfigFilename().c_str());
	EXPECT_FALSE(commandLineArguments.Help());
	EXPECT_FALSE(commandLineArguments.LongHelp());
}

// Just getting the transient positions of an input file is valid
TEST(CommandLineArguments, TestJustGettingTransients)
{
	VerifyJustGettingTransients(CreateCommandLineArguments("--input InputFileName.wav --showtransients"));
	VerifyJustGettingTransients(CreateCommandLineArguments("-i InputFileName.wav -t"));
}

void VerifyStretchWithNoOutputFile(const CommandLineArguments& commandLineArguments)
{
	EXPECT_FALSE(commandLineArguments.IsValid());
	EXPECT_STREQ("Stretch factor given, but no output file given.", commandLineArguments.GetErrorMessage().c_str());
}

TEST(CommandLineArguments, TestStretchWithNoOutputFile)
{
	VerifyStretchWithNoOutputFile(CreateCommandLineArguments("--input InputFileName.wav --stretch 1.10 --showtransients"));
	VerifyStretchWithNoOutputFile(CreateCommandLineArguments("-i InputFileName.wav -s 1.10 -t"));
}

void VerifyLongHelp(const CommandLineArguments& commandLineArguments)
{
	EXPECT_TRUE(commandLineArguments.IsValid());
	EXPECT_STREQ("", commandLineArguments.GetInputFilename().c_str());
	EXPECT_STREQ("", commandLineArguments.GetOutputFilename().c_str());
	EXPECT_EQ(0.00, commandLineArguments.GetStretchFactor());
	EXPECT_FALSE(commandLineArguments.TransientConfigFileGiven());
	EXPECT_FALSE(commandLineArguments.ShowTransients());
	EXPECT_STREQ("", commandLineArguments.GetTransientConfigFilename().c_str());
	EXPECT_FALSE(commandLineArguments.Help());
	EXPECT_TRUE(commandLineArguments.LongHelp());
}

TEST(CommandLineArguments, TestLongHelp)
{
	VerifyLongHelp(CreateCommandLineArguments("--longhelp"));
	VerifyLongHelp(CreateCommandLineArguments("-l"));
}

TEST(CommandLineArguments, TestInvalidParameter)
{
	FakeCommandLineArguments fakeCommandLineArguments;
	fakeCommandLineArguments.PushArgument("MyInvalidParameter");

	CommandLineArguments commandLineArguments(fakeCommandLineArguments.GetArgc(), fakeCommandLineArguments.GetArgv());

	EXPECT_FALSE(commandLineArguments.IsValid());
	EXPECT_STREQ("Invalid parameter given: MyInvalidParameter", commandLineArguments.GetErrorMessage().c_str());
}

void VerifyTooSmallStretchFactor(const CommandLineArguments& commandLineArguments)
{
	EXPECT_FALSE(commandLineArguments.IsValid());
	EXPECT_STREQ("Given stretch factor out of range.  Min: 0.010000  Max: 10.000000", commandLineArguments.GetErrorMessage().c_str());
}

TEST(CommandLineArguments, TestTooSmallStretchFactor)
{
	VerifyTooSmallStretchFactor(CreateCommandLineArguments("--input InputFileName.wav --output OutputFileName.wav --stretch 0.0009"));
	VerifyTooSmallStretchFactor(CreateCommandLineArguments("-i InputFileName.wav -o OutputFileName.wav -s 0.0009"));
}

void VerifyTooLargeStretchFactor(const CommandLineArguments& commandLineArguments)
{
	EXPECT_FALSE(commandLineArguments.IsValid());
	EXPECT_STREQ("Given stretch factor out of range.  Min: 0.010000  Max: 10.000000", commandLineArguments.GetErrorMessage().c_str());
}

TEST(CommandLineArguments, TestTooLargeStretchFactor)
{
	VerifyTooLargeStretchFactor(CreateCommandLineArguments("--input InputFileName.wav --output OutputFileName.wav --stretch 11.0"));
	VerifyTooLargeStretchFactor(CreateCommandLineArguments("-i InputFileName.wav -o OutputFileName.wav -s 11.0"));
}

void VerifyValleyToPeakRatio(const CommandLineArguments& commandLineArguments)
{
	EXPECT_TRUE(commandLineArguments.IsValid());
	EXPECT_EQ(1.75, commandLineArguments.GetValleyPeakRatio());
}

TEST(CommandLineArguments, TestValleyToPeakRatio)
{
	VerifyValleyToPeakRatio(CreateCommandLineArguments("--input InputFileName.wav --valleypeakratio 1.75 --showtransients"));
	VerifyValleyToPeakRatio(CreateCommandLineArguments("-i InputFileName.wav -a 1.75 -t"));
}

void VerifyNoValueGivenForRequiredArgument(const CommandLineArguments& commandLineArguments)
{
	EXPECT_FALSE(commandLineArguments.IsValid());
	EXPECT_STREQ("No value given for argument requiring value", commandLineArguments.GetErrorMessage().c_str());
}

TEST(CommandLineArguments, TestNoValueGivenForRequiredArgument)
{
	VerifyNoValueGivenForRequiredArgument(CreateCommandLineArguments("--input InputFileName.wav --output OutputFileName.wav --stretch"));
	VerifyNoValueGivenForRequiredArgument(CreateCommandLineArguments("-i InputFileName.wav -o OutputFileName.wav -s"));
}

