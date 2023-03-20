#pragma once

#include <glm/glm-all.hpp>

#include <limits>

namespace renderme
{
	enum struct Axis
	{
		x,
		y,
		z,
		invalid,
	};

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
			:pmin{ glm::min(p1, p2)},
			pmax{ glm::max(p1, p2)} {}

		Bounds(Bounds const& b1, Bounds const& b2)
			:pmin{ glm::min(b1.pmin, b2.pmin) },
			pmax{ glm::max(b1.pmax, b2.pmax) } {}

		auto eat(glm::tvec3<T> const& p) -> void
		{
			pmin = glm::min(pmin, p);
			pmax = glm::max(pmax, p);
		}

		auto eat(Bounds const& b) -> void
		{
			pmin = glm::min(pmin, b.pmin);
			pmax = glm::max(pmax, b.pmax);
		}

		auto center() const noexcept -> glm::tvec2<T>
		{
			return (pmax + pmin) / 2.0f;
		}

		auto diagonal() const noexcept -> glm::tvec2<T>
		{ 
			return pmax - pmin; 
		}

		auto max_extent() const noexcept -> Axis
		{
			auto dia = diagonal();
			if (dia.x >= dia.y) {
				return Axis::x;
			}
			return Axis::y;
		}

		auto offset(glm::tvec2<T> const& p) const noexcept ->glm::tvec2<T>
		{
			return (p - pmin) / (pmax - pmin);
		}

		glm::tvec2<T> pmin;
		glm::tvec2<T> pmax;
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
			:pmin{ glm::min(p1, p2) },
			pmax{ glm::max(p1, p2) } {}

		Bounds(Bounds const& b1, Bounds const& b2)
			:pmin{ glm::min(b1.pmin, b2.pmin) },
			pmax{ glm::max(b1.pmax, b2.pmax) } {}

		auto eat(glm::tvec3<T> const& p) -> void
		{
			pmin = glm::min(pmin, p);
			pmax = glm::max(pmax, p);
		}

		auto eat(Bounds const& b) -> void
		{
			pmin = glm::min(pmin, b.pmin);
			pmax = glm::max(pmax, b.pmax);
		}

		auto center() const noexcept -> glm::tvec3<T>
		{
			return (pmax + pmin) / 2.0f;
		}

		auto diagonal() const noexcept -> glm::tvec3<T>
		{
			return pmax - pmin;
		}

		auto max_extent() const noexcept -> Axis
		{
			auto dia = diagonal();
			if (dia.x >= dia.y && dia.x>=dia.z) {
				return Axis::x;
			}
			else if (dia.y >= dia.z) {
				return Axis::y;
			}
			else {
				return Axis::z;
			}
		}

		auto offset(glm::tvec3<T> const& p) const noexcept ->glm::tvec3<T>
		{
			return (p - pmin) / (pmax - pmin);
		}

		auto surface_area() const noexcept->T
		{
			auto dia = diagonal();
			return 2 * (dia.x * dia.y + dia.y * dia.z + dia.x * dia.z);
		}

		glm::tvec3<T> pmin;
		glm::tvec3<T> pmax;
	};

	template<typename T>
	using Bounds3 = Bounds<3, T>;

	using Bounds3i = Bounds<3, int>;
	static_assert(std::is_standard_layout_v<Bounds3i>);

	using Bounds3f = Bounds<3, float>;
	static_assert(std::is_standard_layout_v<Bounds3f>);
}
