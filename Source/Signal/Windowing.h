#pragma once

#include <vector>

namespace Signal {

void BlackmanWindow(std::vector<double>& inputSignal, double startPercent=0.0, double endPercent=100.0);
void InverseBlackmanWindow(std::vector<double>& inputSignal, double startPercent=0.0, double endPercent=100.0);
void ReverseBlackmanWindow(std::vector<double>& inputSignal, double startPercent=0.0, double endPercent=100.0);

void LinearFadeInOut(std::vector<double>& inputSignal);

}
