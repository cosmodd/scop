#pragma once
#include <cmath>

namespace maths
{
	float radians(float degrees);
	float degrees(float radians);
	float clamp(float value, float min, float max);
}