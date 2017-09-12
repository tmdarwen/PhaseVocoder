#include <string>
#include <memory>
#include <functional>
#include <AudioData/AudioData.h>
#include <Application/PhaseVocoderSettings.h>

namespace WaveFile
{
	class WaveFileReader;
	class WaveFileWriter;
}

namespace Signal
{
	class PhaseVocoder;
	class Resampler;
}

class Transients;

class PhaseVocoderMediator
{
	public:
		PhaseVocoderMediator(const PhaseVocoderSettings& settings);
		virtual ~PhaseVocoderMediator();

		void Process();

	private:
		void HandleSilenceInInput(std::size_t sampleCount);

		void ProcessAudioSection(std::size_t startSamplePosition, std::size_t endSamplePosition);

		AudioData GetAudioInput(std::size_t startSample, std::size_t length);

		void HandleLeadingSilence();

		AudioData FlushPhaseVocoderOutput(std::size_t samplesNeeded);

		void InstantiateWaveFileObjects();
		void InstantiatePhaseVocoder(std::size_t sampleLengthOfAudioToProcess);
		void InstantiateResampler();

		void ProcessInput(const AudioData& audioInputData);
		void FinalizeAudioSection(std::size_t totalInputSamples);
		AudioData ProcessAudioWithPhaseVocoder(const AudioData& audioInputData);
		AudioData ProcessAudioWithResampler(const AudioData& audioInputData);

		double GetPitchShiftRatio();
		double GetResampleRatio();

		std::size_t samplesOutputFromCurrentPhaseVocoder_{0};

		AudioData transientSectionOverlap_;
		std::size_t transientSectionOverlapSampleCount_{64};  // The number of samples to crossfade-mix between output transient sections

		PhaseVocoderSettings settings_;

		std::size_t bufferSize_{8192};

		void ObtainTransients();

		std::unique_ptr<Transients> transients_;
		std::unique_ptr<Signal::PhaseVocoder> phaseVocoder_;
		std::unique_ptr<Signal::Resampler> resampler_;
		std::unique_ptr<WaveFile::WaveFileReader> waveReader_;
		std::unique_ptr<WaveFile::WaveFileWriter> waveWriter_;

};