#include <string>
#include <Application/CommandLineArguments.h>
#include <Utilities/Stringify.h>
#include <Utilities/Exception.h>
#include <cstdlib>

CommandLineArguments::CommandLineArguments(int argc, char** argv)
{
	possibleArguments_["--help"] = ArgumentTraits{"-h", false, false};
	possibleArguments_["--input"] = ArgumentTraits{"-i", true, true};
	possibleArguments_["--output"] = ArgumentTraits{"-o", true, true};
	possibleArguments_["--stretch"] = ArgumentTraits{"-s", true, true};
	possibleArguments_["--pitch"] = ArgumentTraits{"-p", true, true};
	possibleArguments_["--resample"] = ArgumentTraits{"-r", true, true};
	possibleArguments_["--showtransients"] = ArgumentTraits{"-t", false, false};
	possibleArguments_["--transientconfig"] = ArgumentTraits{"-c", true, true};
	possibleArguments_["--valleypeakratio"] = ArgumentTraits{"-a", true, true};
	possibleArguments_["--longhelp"] = ArgumentTraits{"-l", false, false};
	possibleArguments_["--version"] = ArgumentTraits{"-v", false, false};

	if(ParseArguments(argc, argv))
	{
		ValidateArguments();
	}
}

// Returns false if a problem with given parameters is encountered
bool CommandLineArguments::ParseArguments(int argc, char** argv)
{
	for(int i{1}; i < argc; ++i)
	{
		std::string argument{argv[i]};
		bool argumentFound{false};
		for(auto possibleArgument : possibleArguments_)
		{
			std::string posArg{possibleArgument.first};
			if(argument == possibleArgument.first || argument == possibleArgument.second.shortArgument_)
			{
				if(possibleArgument.second.requiresValue_ && i + 1 == argc)
				{
					valid_ = false;
					errorMessage_ = "No value given for argument requiring value";
					return false;
				}

				if(possibleArgument.second.acceptsValue_)
				{
					argumentsGiven_[possibleArgument.first] = argv[i + 1];
					++i;
				}
				else
				{
					argumentsGiven_[possibleArgument.first] = "";
				}

				argumentFound = true;
			}
		}

		if(!argumentFound)
		{
			valid_ = false;		
			errorMessage_ = "Invalid parameter given: ";
			errorMessage_.append(argument);
			return false;
		}
	}

	return true;
}

bool CommandLineArguments::InputFilenameGiven() const
{
	auto element = argumentsGiven_.find("--input");
	if(element == argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

bool CommandLineArguments::OutputFilenameGiven() const
{
	auto element = argumentsGiven_.find("--output");
	if(element == argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

bool CommandLineArguments::StretchFactorGiven() const
{
	auto element = argumentsGiven_.find("--stretch");
	if(element == argumentsGiven_.end())
	{
		return false;
	}

	return true;

}

bool CommandLineArguments::PitchSettingGiven() const
{
	auto element = argumentsGiven_.find("--pitch");
	if(element == argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

bool CommandLineArguments::ResampleSettingGiven() const
{
	auto element = argumentsGiven_.find("--resample");
	if(element == argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

bool CommandLineArguments::ValleyPeakRatioGiven() const
{
	auto element = argumentsGiven_.find("--valleypeakratio");
	if(element == argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

void CommandLineArguments::ValidateArguments()
{
	if(argumentsGiven_.size() == 0)
	{
		argumentsGiven_["--help"] = "";
		return;
	}

	if(Help() || LongHelp() || Version())
	{
		return;
	}

	if(!InputFilenameGiven())
	{
		valid_ = false;		
		errorMessage_ = "No input file given.";
		return;
	}

	if(InputFilenameGiven() && !StretchFactorGiven() && !PitchSettingGiven() && !ResampleSettingGiven() && !ShowTransients())
	{
		valid_ = false;
		errorMessage_ = "Nothing to do.  No action specified.";
		return;
	}

	if(!ValidateStretchSetting() || !ValidatePitchSetting() || !ValidateResampleSetting())
	{
		valid_ = false;
		return;
	}

	if(OutputFilenameGiven() && !StretchFactorGiven() && !PitchSettingGiven() && !ResampleSettingGiven())
	{
		valid_ = false;
		errorMessage_ = "Output file given but no stretch, pitch or resample setting given.";
		return;
	}
}

bool CommandLineArguments::ValidateStretchSetting()
{
	auto element = argumentsGiven_.find("--stretch");
	if(element != argumentsGiven_.end() && GetOutputFilename().size() == 0)
	{
		valid_ = false;		
		errorMessage_ = "Stretch factor given, but no output file given.";
		return false;
	}
	else if(element != argumentsGiven_.end())
	{
		auto stretchFactor{atof(element->second.c_str())};
		if(stretchFactor < minimumStretchFactor_ || stretchFactor > maximumStretchFactor_)
		{
			errorMessage_ = Utilities::CreateString(" ", "Given stretch factor out of range.  Min:", minimumStretchFactor_, " Max:", maximumStretchFactor_);
			return false;
		}
	}

	return true;
}

bool CommandLineArguments::ValidatePitchSetting()
{
	auto element = argumentsGiven_.find("--pitch");
	if(element != argumentsGiven_.end() && GetOutputFilename().size() == 0)
	{
		valid_ = false;		
		errorMessage_ = "Pitch setting given, but no output file given.";
		return false;
	}
	else if(element != argumentsGiven_.end())
	{
		auto pitchSetting{atof(element->second.c_str())};
		if(pitchSetting < minimumPitchShift_ || pitchSetting > maximumPitchShift_)
		{
			errorMessage_ = Utilities::CreateString(" ", "Given pitch setting out of range.  Min:", minimumPitchShift_, " Max:", maximumPitchShift_);
			return false;
		}
	}

	return true;
}

bool CommandLineArguments::ValidateResampleSetting()
{
	auto element = argumentsGiven_.find("--resample");
	if(element != argumentsGiven_.end() && GetOutputFilename().size() == 0)
	{
		valid_ = false;		
		errorMessage_ = "Resample setting given, but no output file given.";
		return false;
	}
	else if(element != argumentsGiven_.end())
	{
		auto resampleSetting{atof(element->second.c_str())};
		if(resampleSetting < minimumResampleFrequency_ || resampleSetting > maximumResampleFrequency_)
		{
			errorMessage_ = Utilities::CreateString(" ", "Given resample setting out of range.  Min:", minimumResampleFrequency_, " Max:", maximumResampleFrequency_);
			return false;
		}
	}

	return true;
}

bool CommandLineArguments::IsValid() const
{
	return valid_;
}

const std::string CommandLineArguments::GetInputFilename() const
{
	auto element = argumentsGiven_.find("--input");
	if(element == argumentsGiven_.end())
	{
		return "";
	}

	return element->second;
}

const std::string CommandLineArguments::GetOutputFilename() const
{
	auto element = argumentsGiven_.find("--output");
	if(element == argumentsGiven_.end())
	{
		return "";
	}

	return element->second;
}

double CommandLineArguments::GetStretchFactor() const
{
	auto element = argumentsGiven_.find("--stretch");
	if(element == argumentsGiven_.end())
	{
		return 0.0;
	}

	return atof(element->second.c_str());
}

double CommandLineArguments::GetPitchSetting() const
{
	auto element = argumentsGiven_.find("--pitch");
	if(element == argumentsGiven_.end())
	{
		return 0.0;
	}

	return atof(element->second.c_str());
}

std::size_t CommandLineArguments::GetResampleSetting() const
{
	auto element = argumentsGiven_.find("--resample");
	if(element == argumentsGiven_.end())
	{
		return 0;
	}

	return atoi(element->second.c_str());
}

bool CommandLineArguments::ShowTransients() const
{
	if(argumentsGiven_.find("--showtransients")== argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

bool CommandLineArguments::TransientConfigFileGiven() const
{
	if(GetTransientConfigFilename().size())
	{
		return true;
	}

	return false;
}

const std::string CommandLineArguments::GetTransientConfigFilename() const
{
	auto element = argumentsGiven_.find("--transientconfig");
	if(element == argumentsGiven_.end())
	{
		return "";
	}

	return element->second;
}

double CommandLineArguments::GetValleyPeakRatio() const
{
	auto element = argumentsGiven_.find("--valleypeakratio");
	if(element == argumentsGiven_.end())
	{
		return 0.0;
	}

	return atof(element->second.c_str());
}

const std::string& CommandLineArguments::GetErrorMessage() const
{
	return errorMessage_;
}

bool CommandLineArguments::Help() const
{
	if(argumentsGiven_.find("--help")== argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

bool CommandLineArguments::LongHelp() const
{
	if(argumentsGiven_.find("--longhelp")== argumentsGiven_.end())
	{
		return false;
	}

	return true;
}

bool CommandLineArguments::Version() const
{
	if(argumentsGiven_.find("--version")== argumentsGiven_.end())
	{
		return false;
	}

	return true;
}
