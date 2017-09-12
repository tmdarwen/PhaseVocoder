#pragma once

#include <cstddef>

// See chapter 16 of "The Scientists' and Engineers Guide to DSP" for more on the "Windowed-Sinc Filters"

namespace Signal {

	const double SINC_SAMPLES_PER_X_INTEGER{224.0};
	const std::size_t SINC_VALUE_SIZE{9857};  //4928 on both sides plus 1.0 in the middle
	const std::size_t SINC_CENTER_POINT{4928};
	const double MAX_X_POSITION_POSITIVE{4928.0};
	const double MIN_X_POSITION_POSITIVE{-4928.0};

	double GetSincValue(double xPosition);

}
