#pragma once

#include <AudioData/AudioData.h>
#include <vector>
#include <cstdint>

namespace Signal {

std::vector<int16_t> ConvertFloat64ToSigned16(const std::vector<double>& signal);
std::vector<int16_t> ConvertFloat64ToSigned16(const std::vector<double>& signal, std::size_t sampleCount);

std::vector<double> ConvertSigned16ToFloat64(const std::vector<int16_t>& signal);
std::vector<double> ConvertSigned16ToFloat64(const std::vector<int16_t>& signal, std::size_t sampleCount);

std::vector<int16_t> ConvertAudioDataToInterleavedSigned16(const AudioData& leftChannel, const AudioData& rightChannel);

}
