#include <string>
#include <memory>
#include <functional>
#include <vector>

class TransientSettings
{
	public:
		// Typical setter methods
		void SetInputWaveFile(const std::string& filename);
		void SetTransientConfigFilename(const std::string& transientConfgFilename);
		void SetTransientCallback(std::function<void(std::size_t)> callback);
		void SetTransientValleyToPeakRatio(double valleyToPeakRatio);

		// Methods to check if a value was actually given
		bool InputWaveFileGiven() const;
		bool TransientConfigFilenameGiven() const;
		bool TransientCallbackGiven() const;

		// Typical getter methods
		const std::string& GetInputWaveFile() const;
		const std::string& GetTransientConfigFilename() const;
		std::function<void(std::size_t)> GetTransientCallback() const;
		double GetTransientValleyToPeakRatio() const;

	private:
		std::string inputWaveFilename_;
		bool inputWaveFilenameGiven_{false};

		std::function<void(std::size_t)> transientCallback_;
		bool transientCallbackGiven_{false};

		std::string transientConfigFilename_;
		bool transientConfigFilenameGiven_{false};

		double valleyToPeakRatio_{1.5};
};

class Transients
{
	public:
		Transients(const TransientSettings& settings);
		virtual ~Transients();

		const std::vector<std::size_t>& GetTransients();

	private:
		void GetTransientPositionsFromAudioFile();
		void GetTransientPositionsFromConfigFile();
		void HandleCallback();

		TransientSettings settings_;
		std::vector<std::size_t> transients_;
		bool transientsProcessed_{false};	
};