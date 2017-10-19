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

#include <iostream>
#include <string>
#include <algorithm>
#include <Utilities/Exception.h>
#include <Application/PhaseVocoderMediator.h>
#include <Application/CommandLineArguments.h>
#include <Application/Usage.h>

const uint32_t SUCCESS{0};
const uint32_t FAILURE{1};

void CheckCommandLineArguments(CommandLineArguments& commandLineArguments)
{
	if(!commandLineArguments.IsValid())
	{
		std::cout << commandLineArguments.GetErrorMessage() << std::endl;
		std::cout << std::endl;
		DisplayShortHelp();
		exit(FAILURE);
	}

	if(commandLineArguments.Help())
	{
		DisplayShortHelp();
		exit(SUCCESS);
	}

	if(commandLineArguments.LongHelp())
	{
		DisplayLongHelp();
		exit(SUCCESS);
	}

	if(commandLineArguments.Version())
	{
		DisplayVersion();
		exit(SUCCESS);
	}
}

std::unique_ptr<PhaseVocoderMediator> GetPhaseVocoderMediator(const CommandLineArguments& commandLineArguments)
{
	PhaseVocoderSettings phaseVocoderSettings;

	if(commandLineArguments.InputFilenameGiven())
	{
		phaseVocoderSettings.SetInputWaveFile(commandLineArguments.GetInputFilename());	
	}

	if(commandLineArguments.OutputFilenameGiven())
	{
		phaseVocoderSettings.SetOutputWaveFile(commandLineArguments.GetOutputFilename());	
	}

	if(commandLineArguments.StretchFactorGiven())
	{
		phaseVocoderSettings.SetStretchFactor(commandLineArguments.GetStretchFactor());
	}

	if(commandLineArguments.ResampleSettingGiven())
	{
		phaseVocoderSettings.SetResampleValue(commandLineArguments.GetResampleSetting());
	}

	if(commandLineArguments.PitchSettingGiven())
	{
		phaseVocoderSettings.SetPitchShiftValue(commandLineArguments.GetPitchSetting());
	}

	if(commandLineArguments.TransientConfigFileGiven())
	{
		phaseVocoderSettings.SetTransientConfigFilename(commandLineArguments.GetTransientConfigFilename());
	}

	if(commandLineArguments.ValleyPeakRatioGiven())
	{
		phaseVocoderSettings.SetValleyToPeakRatio(commandLineArguments.GetValleyPeakRatio());
	}

	return std::unique_ptr<PhaseVocoderMediator>{new PhaseVocoderMediator(phaseVocoderSettings)};
}

void DisplayTransients(const std::unique_ptr<PhaseVocoderMediator>& phaseVocoderMediator)
{
	auto printSamplePosition{[](const std::size_t& samplePosition) { std::cout << " " << samplePosition; }};

	if(phaseVocoderMediator->GetChannelCount() == 1)
	{
		auto transients{phaseVocoderMediator->GetTransients(0)};

		if(transients.size() == 0)
		{
			std::cout << "No transients found" << std::endl;	
		}
		else
		{
			std::cout << "Transient sample positions:";
			std::for_each(transients.begin(), transients.end(), printSamplePosition);
		}
	}
	else if(phaseVocoderMediator->GetChannelCount() == 2)
	{
		auto leftTransients{phaseVocoderMediator->GetTransients(0)};
		auto rightTransients{phaseVocoderMediator->GetTransients(1)};

		if(leftTransients.size() == 0 && rightTransients.size() == 0)
		{
			std::cout << "No transients found" << std::endl;	
		}

		if(leftTransients.size())
		{
			std::cout << "Left channel transient sample positions:";
			std::for_each(leftTransients.begin(), leftTransients.end(), printSamplePosition);
			std::cout << std::endl;
		}

		if(rightTransients.size())
		{
			std::cout << "Right channel transient sample positions:";
			std::for_each(rightTransients.begin(), rightTransients.end(), printSamplePosition);
		}
	}
}

int PerformPhaseVocoding(CommandLineArguments& commandLineArguments)
{
	try
	{
		auto phaseVocoderMediator{GetPhaseVocoderMediator(commandLineArguments)};
		phaseVocoderMediator->Process();

		std::cout << "Total Processing Time: " << phaseVocoderMediator->GetTotalProcessingTime() << std::endl;
		if(phaseVocoderMediator->GetChannelCount() == 2)
		{
			std::cout << "Write Buffer Highwater Mark: " << phaseVocoderMediator->GetMaxBufferedSamples() << std::endl;
		}

		if(commandLineArguments.ShowTransients())
		{
			DisplayTransients(phaseVocoderMediator);
		}
		
		std::cout << std::endl;  // Newline so prompt displays below output
	}
	catch(Utilities::Exception& exception)
	{
		std::cerr << "Error: " << exception.what() << std::endl;
		return FAILURE;
	}

	return SUCCESS;
}

int main(int argc, char* argv[])
{
	CommandLineArguments commandLineArguments(argc, argv);

	CheckCommandLineArguments(commandLineArguments);
	return PerformPhaseVocoding(commandLineArguments);
}
