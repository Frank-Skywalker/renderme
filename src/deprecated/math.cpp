#include "math.hpp"
#include <numbers>

namespace deprecated
{
	auto radians(float deg)->float
	{
		return  (std::numbers::pi / 180) * deg;
	}

	auto degrees(float rad)->float
	{
		return (180 / std::numbers::pi) * rad;
	}
}
