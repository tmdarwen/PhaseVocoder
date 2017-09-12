#include <string>
#include <map>

class CommandLineArguments
{
	public:
		CommandLineArguments(int argc, char** argv);

		bool IsValid() const;

		const std::string& GetErrorMessage() const;

		bool InputFilenameGiven() const;
		const std::string GetInputFilename() const;

		bool OutputFilenameGiven() const;
		const std::string GetOutputFilename() const;

		bool StretchFactorGiven() const;
		double GetStretchFactor() const;

		bool PitchSettingGiven() const;
		double GetPitchSetting() const;

		bool ResampleSettingGiven() const;
		std::size_t GetResampleSetting() const;

		bool ValleyPeakRatioGiven() const;
		double GetValleyPeakRatio() const;

		bool ShowTransients() const;
		bool TransientConfigFileGiven() const;
		const std::string GetTransientConfigFilename() const;
		bool Help() const;
		bool LongHelp() const;
		bool Version() const;

	private:
		bool ParseArguments(int argc, char** argv);
		void ValidateArguments();
		bool ValidateStretchSetting();
		bool ValidatePitchSetting();
		bool ValidateResampleSetting();

		bool valid_{true};
		std::string errorMessage_;

		std::map<std::string, std::string> argumentsGiven_;

		// The stretch factor must be between 0.01 and 10.0
		const double minimumStretchFactor_{0.01};
		const double maximumStretchFactor_{10.0};

		// The pitch shift must be between -24.0 and +24.0 semitones
		const double minimumPitchShift_{-24.0};
		const double maximumPitchShift_{24.0};

		// The stretch factor must be between 0.01 and 10.0
		const std::size_t minimumResampleFrequency_{1000};
		const std::size_t maximumResampleFrequency_{192000};

		struct ArgumentTraits
		{
			ArgumentTraits() : acceptsValue_{false}, requiresValue_{false} { }
			ArgumentTraits(const std::string& shortArgument) : shortArgument_{shortArgument}, acceptsValue_{false}, requiresValue_{false} { }
			ArgumentTraits(const std::string& shortArgument, bool acceptsValue, bool requiresValue) : 
				shortArgument_{shortArgument}, acceptsValue_{acceptsValue}, requiresValue_{requiresValue} { }
			std::string shortArgument_;
			bool acceptsValue_;
			bool requiresValue_;
		};

		std::map<std::string, ArgumentTraits> possibleArguments_;
};