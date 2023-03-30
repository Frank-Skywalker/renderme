#include "random.hpp"
#include <random>
namespace renderme
{
	namespace {
		std::random_device rd;  // Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<int> distribute(0, std::numeric_limits<int>::max());
	}

	auto random01() -> float
	{
		return float(distribute(gen)) / float(std::numeric_limits<int>::max());
	}

}