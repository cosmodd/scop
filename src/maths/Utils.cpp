#include "maths/Utils.hpp"

namespace maths
{
	float radians(float degrees)
	{
		return degrees * M_PI / 180.0f;
	}

	float degrees(float radians)
	{
		return radians * 180.0f / M_PI;
	}

	float clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}
}