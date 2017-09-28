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
#include <WaveFile/WaveFileReader.h>
#include <WaveFile/WaveFileWriter.h>
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

void TransientCallback(std::size_t transient)
{
	std::cout << "Transient at sample position: " << transient << std::endl;
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

	if(commandLineArguments.ShowTransients())
	{
		phaseVocoderSettings.SetTransientCallback(TransientCallback);
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

int PerformPhaseVocoding(CommandLineArguments& commandLineArguments)
{
	try
	{
		auto phaseVocoderMediator{GetPhaseVocoderMediator(commandLineArguments)};
		phaseVocoderMediator->Process();

		std::cout << "Total Processing Time: " << phaseVocoderMediator->GetTotalProcessingTime() << std::endl;
		std::cout << "Transient Processing Time: " << phaseVocoderMediator->GetTransientProcessingTime() << std::endl;
		std::cout << "PhaseVocoder Processing Time: " << phaseVocoderMediator->GetPhaseVocoderProcessingTime() << std::endl;
		std::cout << "Resampler Processing Time: " << phaseVocoderMediator->GetResamplerProcessingTime() << std::endl;
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
