#pragma once
#include"type.hpp"
#include "log.hpp"

#include <cstring>

namespace renderme
{
	//////Vectors//////
	enum struct Category
	{
		vector,
		point,
		normal
	};

	template<unsigned int dim, Category category, typename T>
	struct Vec;

	template<Category category, typename T>
	struct Vec<2, category, T> final
	{
		Vec():x{0}, y{0} {}
		Vec(T x, T y):x{x}, y{y} {}

		auto operator+(Vec const& rhs) const noexcept->Vec
		{
			return Vec(x + rhs.x, y + rhs.y);
		}

		auto operator+=(Vec const& rhs) -> Vec&
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		auto operator-(Vec const& rhs) const noexcept->Vec
		{
			return Vec(x - rhs.x, y - rhs.y);
		}

		auto operator-=(Vec const& rhs)->Vec&
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		auto operator==(Vec const& rhs) const noexcept ->bool
		{
			return x == rhs.x && y == rhs.y;
		}

		auto operator!=(Vec const& rhs) const noexcept ->bool
		{
			return x != rhs.x || y != rhs.y;
		}

		template<typename U>
		auto operator*(U const& rhs) const noexcept->Vec
		{
			return Vec(x * rhs, y * rhs);
		}

		template<typename U>
		auto operator*=(U const& rhs) ->Vec&
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		template<typename U>
		auto operator/(U const& rhs) const noexcept->Vec
		{
			return Vec(x / rhs, y / rhs);
		}

		template<typename U>
		auto operator/=(U const& rhs) ->Vec&
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}

		auto operator-() const noexcept->Vec
		{
			return Vec(-x, -y);
		}

		auto operator[](int i) -> T&
		{
			if (i < 0 || i >= 2) {
				log(Status::fatal, "Vec: index out of range");
			}
			if (i == 0) {
				return x;
			}
			return y;
		}

		T x;
		T y;
	};

	template<typename T>
	using Vector2= Vec<2, Category::vector, T>;
	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<Float>;

	template<typename T>
	using Point2 = Vec<2, Category::point, T>;
	using Point2i = Point2<int>;
	using Point2f = Point2<Float>;


	template<Category category, typename T>
	struct Vec<3, category, T> final
	{
		Vec():x{0}, y{0}, z{0} {}
		Vec(T x, T y, T z):x{x}, y{y}, z{z} {}

		auto operator+(Vec const& rhs) const noexcept->Vec
		{
			return Vec(x + rhs.x, y + rhs.y, z + rhs.z);
		}

		auto operator+=(Vec const& rhs) -> Vec&
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		auto operator-(Vec const& rhs) const noexcept->Vec
		{
			return Vec(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		auto operator-=(Vec const& rhs)->Vec&
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		auto operator==(Vec const& rhs) const noexcept ->bool
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}

		auto operator!=(Vec const& rhs) const noexcept ->bool
		{
			return x != rhs.x || y != rhs.y || z != rhs.z;
		}

		template<typename U>
		auto operator*(U const& rhs) const noexcept->Vec
		{
			return Vec(x * rhs, y * rhs, z * rhs);
		}

		template<typename U>
		auto operator*=(U const& rhs) ->Vec&
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		template<typename U>
		auto operator/(U const& rhs) const noexcept->Vec
		{
			return Vec(x / rhs, y / rhs, z / rhs);
		}

		template<typename U>
		auto operator/=(U const& rhs) ->Vec&
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}

		auto operator-() const noexcept->Vec
		{
			return Vec(-x, -y, -z);
		}

		auto operator[](int i) -> T&
		{
			if (i < 0 || i >= 3) {
				log(Status::fatal, "Vec: index out of range");
			}
			if (i == 0) {
				return x;
			} else if (i == 1) {
				return y;
			}
			return z;
		}


		T x;
		T y;
		T z;
	};

	template<typename T>
	using Vector3 = Vec<3, Category::vector, T>;
	using Vector3i = Vector3<int>;
	using Vector3f = Vector3<Float>;

	template<typename T>
	using Point3 = Vec<3, Category::point, T>;
	using Point3ui = Point3<unsigned int>;
	using Point3i = Point3<int>;
	using Point3f = Point3<Float>;

	template<typename T>
	using Normal3= Vec<3, Category::normal, T>;
	using Normal3i = Normal3<int>;
	using Normal3f = Normal3<Float>;


	template<Category category, typename T>
	struct Vec<4, category, T> final
	{
		Vec():x{0}, y{0}, z{0}, w{0} {}
		Vec(T x, T y, T z, T w):x{x}, y{y}, z{z}, w{w} {}

		auto operator+(Vec const& rhs) const noexcept->Vec
		{
			return Vec(x + rhs.x, y + rhs.y, z + rhs.z, w+ rhs.w);
		}

		auto operator+=(Vec const& rhs) -> Vec&
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		auto operator-(Vec const& rhs) const noexcept->Vec
		{
			return Vec(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		auto operator-=(Vec const& rhs)->Vec&
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		auto operator==(Vec const& rhs) const noexcept ->bool
		{
			return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
		}

		auto operator!=(Vec const& rhs) const noexcept ->bool
		{
			return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
		}

		template<typename U>
		auto operator*(U const& rhs) const noexcept->Vec
		{
			return Vec(x * rhs, y * rhs, z * rhs, w * rhs);
		}

		template<typename U>
		auto operator*=(U const& rhs) ->Vec&
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
			return *this;
		}

		template<typename U>
		auto operator/(U const& rhs) const noexcept->Vec
		{
			return Vec(x / rhs, y / rhs, z / rhs, w / rhs);
		}

		template<typename U>
		auto operator/=(U const& rhs) ->Vec&
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;
			return *this;
		}

		auto operator-() const noexcept->Vec
		{
			return Vec(-x, -y, -z, -w);
		}

		auto operator[](int i) -> T&
		{
			if (i < 0 || i >= 3) {
				log(Status::fatal, "Vec: index out of range");
			}
			if (i == 0) {
				return x;
			} 
			else if (i == 1) {
				return y;
			}
			else if (i == 2) {
				return z;
			}
			return w;
		}



		T x;
		T y;
		T z;
		T w;
	};

	template<typename T>
	using Vector4 = Vec<4, Category::vector, T>;
	using Vector4i = Vector4<int>;
	using Vector4f = Vector4<Float>;
}

#include "vector.inl"