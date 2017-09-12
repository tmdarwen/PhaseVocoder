#include <string>
#include <memory>
#include <functional>

class PhaseVocoderSettings
{
	public:
		// Typical setter methods
		void SetInputWaveFile(const std::string& filename);
		void SetOutputWaveFile(const std::string& filename);
		void SetStretchFactor(double stretchFactor);
		void SetResampleValue(std::size_t resampleValue);
		void SetPitchShiftValue(double pitchShiftValue);
		void SetTransientConfigFilename(const std::string& transientConfgFilename);
		void SetTransientCallback(std::function<void(std::size_t)> callback);
		void SetValleyToPeakRatio(double valleyToPeakRatio);

		// Methods to check if a value was actually given
		bool InputWaveFileGiven() const;
		bool OutputWaveFileGiven() const;
		bool StretchFactorGiven() const;
		bool ResampleValueGiven() const;
		bool PitchShiftValueGiven() const;
		bool TransientConfigFilenameGiven() const;
		bool TransientCallbackGiven() const;
		bool ValleyToPeakRatioGiven() const;

		// Typical getter methods
		const std::string& GetInputWaveFile() const;
		const std::string& GetOutputWaveFile() const;
		double GetStretchFactor() const;
		std::size_t GetResampleValue() const;
		double GetPitchShiftValue() const;
		const std::string& GetTransientConfigFilename() const;
		std::function<void(std::size_t)> GetTransientCallback() const;
		double GetValleyToPeakRatio() const;

	private:
		std::string inputWaveFilename_;
		bool inputWaveFilenameGiven_{false};

		std::string outputWaveFilename_;
		bool outputWaveFilenameGiven_{false};

		double stretchFactor_;
		bool stretchFactorGiven_{false};

		std::size_t resampleValue_;
		bool resampleValueGiven_{false};

		double pitchShiftValue_;
		bool pitchShiftValueGiven_{false};

		std::function<void(std::size_t)> transientCallback_;
		bool transientCallbackGiven_{false};

		std::string transientConfigFilename_;
		bool transientConfigFilenameGiven_{false};

		double valleyToPeakRatio_{1.5};
		bool valleyToPeakRatioGiven_{false};
};