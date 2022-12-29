#include "math.hpp"
#include <numbers>

namespace renderme
{
	auto radians(Float deg)->Float
	{
		return  (std::numbers::pi / 180) * deg;
	}

	auto degrees(Float rad)->Float
	{
		return (180 / std::numbers::pi) * rad;
	}
}