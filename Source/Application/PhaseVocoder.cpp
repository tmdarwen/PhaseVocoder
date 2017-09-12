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
