#pragma once
#include "vector.hpp"
#include <limits>

namespace deprecated
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
	static_assert(std::is_standard_layout_v<Bounds2i>);

	using Bounds2f = Bounds<2, float>;
	static_assert(std::is_standard_layout_v<Bounds2f>);

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
	static_assert(std::is_standard_layout_v<Bounds3i>);

	using Bounds3f = Bounds<3, float>;
	static_assert(std::is_standard_layout_v<Bounds3f>);

}
