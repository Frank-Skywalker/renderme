#pragma once

#include <glm/glm-all.hpp>

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
			T min = std::numeric_limits<T>::min();
			pmin = glm::tvec2<T>(max, max);
			pmax = glm::tvec2<T>(min, min);
		}

		Bounds(glm::tvec2<T> const& p)
			:pmin{ p }, pmax{ p }{}

		Bounds(glm::tvec2<T> const& p1, glm::tvec2<T> const& p2)
			:pmin{ std::min(p1.x, p2.x), std::min(p1.y, p2.y) },
			pmax{ std::max(p1.x,p2.x), std::max(p1.y, p2.y) } {}

		auto eat(glm::tvec2<T> const& p)->void
		{
			pmin = glm::tvec2<T>(std::min(pmin.x, p.x), std::min(pmin.y, p.y));
			pmax = glm::tvec2<T>(std::max(pmax.x, p.x), std::max(pmax.y, p.y));
		}

		auto center() const noexcept -> glm::tvec2<T>
		{
			return (pmax + pmin) / 2.0f;
		}

		auto diagonal() const noexcept -> glm::tvec2<T>
		{ 
			return pmax - pmin; 
		}


		glm::tvec2<T> pmin, pmax;
	};

	template<typename T>
	using Bounds2 = Bounds<2, T>;

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
			T min = std::numeric_limits<T>::min();
			pmin = glm::tvec3<T>(max, max, max);
			pmax = glm::tvec3<T>(min, min, min);
		}

		Bounds(glm::tvec3<T> const& p)
			:pmin{ p }, pmax{ p } {}

		Bounds(glm::tvec3<T> const& p1, glm::tvec3<T> const& p2)
			:pmin{ std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z) },
			pmax{ std::max(p1.x,p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z) } {}

		auto center() const noexcept -> glm::tvec2<T>
		{
			return (pmax + pmin) / 2.0f;
		}

		auto eat(glm::tvec3<T> const& p) -> void
		{
			pmin = glm::tvec3<T>(std::min(pmin.x, p.x), std::min(pmin.y, p.y), std::min(pmin.z, p.z));
			pmax = glm::tvec3<T>(std::max(pmax.x, p.x), std::max(pmax.y, p.y), std::max(pmax.z, p.z));
		}

		auto diagonal() const noexcept -> glm::tvec3<T>
		{
			return pmax - pmin;
		}

		glm::tvec3<T> pmin, pmax;
	};

	template<typename T>
	using Bounds3 = Bounds<3, T>;

	using Bounds3i = Bounds<3, int>;
	static_assert(std::is_standard_layout_v<Bounds3i>);

	using Bounds3f = Bounds<3, float>;
	static_assert(std::is_standard_layout_v<Bounds3f>);
}
