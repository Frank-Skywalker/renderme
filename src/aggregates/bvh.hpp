#pragma once
#include <core/primitive.hpp>

namespace renderme
{
	struct BVH : Aggregate
	{
		BVH(std::vector<std::unique_ptr<Primitive>> primivites);

	private:
		std::vector<std::unique_ptr<Primitive>> primitives;
	};

}