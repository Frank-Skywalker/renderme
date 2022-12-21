#pragma once
#include "vector.hpp"
#include <limits>

namespace renderme
{
	template<unsigned int dim, typename T>
	struct Bounds;

	template<typename T>
	struct Bounds<2, T>
	{
		Bounds()
		{
			T max = std::numeric_limits<T>::max();
			T min = std::numeric_limits<T>::lowest();
			pmin = Point2<T>(min, min);
			pmax = Point2<T>(max, max);
		}

		Point2<T> pmin, pmax;
	};

	using Bounds2i = Bounds<2, int>;
	using Bounds2f = Bounds<2, Float>;

	template<typename T>
	struct Bounds<3, T>
	{
		Bounds()
		{
			T max = std::numeric_limits<T>::max();
			T min = std::numeric_limits<T>::lowest();
			pmin = Point3<T>(min, min, min);
			pmax = Point3<T>(max, max, max);
		}

		Point3<T> pmin, pmax;
	};

	using Bounds3i = Bounds<3, int>;
	using Bounds3f = Bounds<3, Float>;

}