#pragma once
#include <core/type.hpp>
#include <core/log.hpp>

#include <cstring>
#include <cmath>

namespace deprecated
{
	using namespace renderme;

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

		//operator- and operator-= between Points are different from Vectors and Normals
		//Thus must be written separately

		//auto operator-(Vec const& rhs) const noexcept->Vec
		//{
		//	return Vec(x - rhs.x, y - rhs.y);
		//}

		//auto operator-=(Vec const& rhs)->Vec&
		//{
		//	x -= rhs.x;
		//	y -= rhs.y;
		//	return *this;
		//}

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

	template<typename T>
	auto operator-(Vector2<T> const& lhs, Vector2<T> const& rhs) ->Vector2<T>
	{
		return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	template<typename T>
	auto operator-=(Vector2<T>& lhs, Vector2<T> const& rhs) ->Vector2<T>&
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}

	template<typename T>
	inline auto length_squared(Vector2<T> const& v)->Float
	{
		return v.x * v.x + v.y * v.y;
	}

	template<typename T>
	inline auto length(Vector2<T> const& v)->Float
	{
		return std::sqrt(length_squared(v));
	}

	template<typename T>
	inline auto normalize(Vector2<T> const& v)->Vector2<T>
	{
		return v / length(v);
	}
	
	using Vector2i = Vector2<int>;
	static_assert(std::is_standard_layout_v<Vector2i>);
	static_assert(offsetof(Vector2i, y) == sizeof(int));

	using Vector2f = Vector2<Float>;
	static_assert(std::is_standard_layout_v<Vector2f>);
	static_assert(offsetof(Vector2f, y) == sizeof(Float));


	template<typename T>
	using Point2 = Vec<2, Category::point, T>;

	//Minus between two Points yields a Vector
	template<typename T>
	auto operator-(Point2<T> const& lhs, Point2<T> const& rhs) ->Vector2<T>
	{
		return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	//No operator-= between Points
	//template<typename T>
	//auto operator-=(Vector2<T>& lhs, Vector2<T> const& rhs) ->Vector2<T>&
	//{
	//	lhs.x -= rhs.x;
	//	lhs.y -= rhs.y;
	//	return lhs;
	//}

	template<typename T, typename U>
	auto operator+(Point2<T> const& p, Vector2<U> const& v)-> Point2<T>
	{
		return Point2<T>(p.x + v.x, p.y + v.y);
	}

	template<typename T, typename U>
	auto operator+=(Point2<T>& p, Vector2<U> const& v)-> Point2<T>&
	{
		p.x += v.x;
		p.y += v.y;
		return p;
	}

	template<typename T, typename U>
	auto operator-(Point2<T> const& p, Vector2<U> const& v)-> Point2<T>
	{
		return Point2<T>(p.x - v.x, p.y - v.y);
	}

	template<typename T, typename U>
	auto operator-=(Point2<T>& p, Vector2<U> const& v)-> Point2<T>&
	{
		p.x -= v.x;
		p.y -= v.y;
		return p;
	}

	using Point2i = Point2<int>;
	static_assert(std::is_standard_layout_v<Point2i>);
	static_assert(offsetof(Point2i, y) == sizeof(int));

	using Point2ui = Point2<unsigned int>;
	static_assert(std::is_standard_layout_v<Point2ui>);
	static_assert(offsetof(Point2ui, y) == sizeof(unsigned int));

	using Point2f = Point2<Float>;
	static_assert(std::is_standard_layout_v<Point2f>);
	static_assert(offsetof(Point2f, y) == sizeof(Float));



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

		//operator- and operator-= between Points are different from Vectors and Normals
        //Thus must be written separately

		//auto operator-(Vec const& rhs) const noexcept->Vec
		//{
		//	return Vec(x - rhs.x, y - rhs.y, z - rhs.z);
		//}

		//auto operator-=(Vec const& rhs)->Vec&
		//{
		//	x -= rhs.x;
		//	y -= rhs.y;
		//	z -= rhs.z;
		//	return *this;
		//}

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

	template<typename T>
	auto operator-(Vector3<T> const& lhs, Vector3<T> const& rhs) ->Vector3<T>
	{
		return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template<typename T>
	auto operator-=(Vector3<T>& lhs, Vector3<T> const& rhs) ->Vector3<T>&
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	template<typename T>
	inline auto length_squared(Vector3<T> const& v)->Float
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	template<typename T>
	inline auto length(Vector3<T> const& v)->Float
	{
		return std::sqrt(length_squared(v));
	}

	template<typename T>
	inline auto normalize(Vector3<T> const& v)->Vector3<T>
	{
		return v / length(v);
	}

	template<typename T>
	inline auto cross(Vector3<T> const& lhs, Vector3<T> const& rhs)->Vector3<T>
	{
		T v1x = lhs.x, v1y = lhs.y, v1z = lhs.z;
		T v2x = rhs.x, v2y = rhs.y, v2z = rhs.z;
		return Vector3<T>(
			(v1y * v2z) - (v1z * v2y), 
			(v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x)
			);
	}



	using Vector3i = Vector3<int>;
	static_assert(std::is_standard_layout_v<Vector3i>);
	static_assert(offsetof(Vector3i, y) == sizeof(int));
	static_assert(offsetof(Vector3i, z) == sizeof(int) * 2);

	using Vector3f = Vector3<Float>;
	static_assert(std::is_standard_layout_v<Vector3f>);
	static_assert(offsetof(Vector3f, y) == sizeof(Float));
	static_assert(offsetof(Vector3f, z) == sizeof(Float) * 2);

	
	template<typename T>
	using Point3 = Vec<3, Category::point, T>;

	//Minus between two Points yields a Vector
	template<typename T>
	auto operator-(Point3<T> const& lhs, Point3<T> const& rhs) ->Vector3<T>
	{
		return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	//No operator-= between Points
	//template<typename T>
	//auto operator-=(Vector3<T>& lhs, Vector3<T> const& rhs) ->Vector3<T>&
	//{
	//	lhs.x -= rhs.x;
	//	lhs.y -= rhs.y;
	//	lhs.z -= rhs.z;
	//	return lhs;
	//}

	template<typename T, typename U>
	auto operator+(Point3<T> const& p, Vector3<U> const& v)-> Point3<T>
	{
		return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
	}

	template<typename T, typename U>
	auto operator+=(Point3<T>& p, Vector3<U> const& v)-> Point3<T>&
	{
		p.x += v.x;
		p.y += v.y;
		p.z += v.z;
		return p;
	}

	template<typename T, typename U>
	auto operator-(Point3<T> const& p, Vector3<U> const& v)-> Point3<T>
	{
		return Point3<T>(p.x - v.x, p.y - v.y, p.z - v.z);
	}

	template<typename T, typename U>
	auto operator-=(Point3<T>& p, Vector3<U> const& v)-> Point3<T>&
	{
		p.x -= v.x;
		p.y -= v.y;
		p.z -= v.z;
		return p;
	}

	using Point3ui = Point3<unsigned int>;
	static_assert(std::is_standard_layout_v<Point3ui>);
	static_assert(offsetof(Point3ui, y) == sizeof(unsigned int));
	static_assert(offsetof(Point3ui, z) == sizeof(unsigned int) * 2);

	using Point3i = Point3<int>;
	static_assert(std::is_standard_layout_v<Point3i>);
	static_assert(offsetof(Point3i, y) == sizeof(int));
	static_assert(offsetof(Point3i, z) == sizeof(int) * 2);

	using Point3f = Point3<Float>;
	static_assert(std::is_standard_layout_v<Point3f>);
	static_assert(offsetof(Point3f, y) == sizeof(Float));
	static_assert(offsetof(Point3f, z) == sizeof(Float) * 2);


	template<typename T>
	using Normal3= Vec<3, Category::normal, T>;

	template<typename T>
	auto operator-(Normal3<T> const& lhs, Normal3<T> const& rhs) ->Normal3<T>
	{
		return Normal3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template<typename T>
	auto operator-=(Normal3<T>& lhs, Normal3<T> const& rhs) ->Normal3<T>&
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	template<typename T>
	inline auto length_squared(Normal3<T> const& v)->Float
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	template<typename T>
	inline auto length(Normal3<T> const& v)->Float
	{
		return std::sqrt(length_squared(v));
	}

	template<typename T>
	inline auto normalize(Normal3<T> const& v)->Normal3<T>
	{
		return v / length(v);
	}

	template<typename T>
	inline auto cross(Vector3<T> const& lhs, Normal3<T> const& rhs)->Vector3<T>
	{
		T v1x = lhs.x, v1y = lhs.y, v1z = lhs.z;
		T v2x = rhs.x, v2y = rhs.y, v2z = rhs.z;
		return Vector3<T>(
			(v1y * v2z) - (v1z * v2y),
			(v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x)
			);
	}

	template<typename T>
	inline auto cross(Normal3<T> const& lhs, Vector3<T> const& rhs)->Vector3<T>
	{
		T v1x = lhs.x, v1y = lhs.y, v1z = lhs.z;
		T v2x = rhs.x, v2y = rhs.y, v2z = rhs.z;
		return Vector3<T>(
			(v1y * v2z) - (v1z * v2y),
			(v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x)
			);
	}



	using Normal3i = Normal3<int>;
	static_assert(std::is_standard_layout_v<Normal3i>);
	static_assert(offsetof(Normal3i, y) == sizeof(int));
	static_assert(offsetof(Normal3i, z) == sizeof(int) * 2);

	using Normal3f = Normal3<Float>;
	static_assert(std::is_standard_layout_v<Normal3f>);
	static_assert(offsetof(Normal3f, y) == sizeof(Float));
	static_assert(offsetof(Normal3f, z) == sizeof(Float) * 2);




	//template<Category category, typename T>
	//struct Vec<4, category, T> final
	//{
	//	Vec():x{0}, y{0}, z{0}, w{0} {}
	//	Vec(T x, T y, T z, T w):x{x}, y{y}, z{z}, w{w} {}

	//	auto operator+(Vec const& rhs) const noexcept->Vec
	//	{
	//		return Vec(x + rhs.x, y + rhs.y, z + rhs.z, w+ rhs.w);
	//	}

	//	auto operator+=(Vec const& rhs) -> Vec&
	//	{
	//		x += rhs.x;
	//		y += rhs.y;
	//		z += rhs.z;
	//		w += rhs.w;
	//		return *this;
	//	}

	//	auto operator-(Vec const& rhs) const noexcept->Vec
	//	{
	//		return Vec(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	//	}

	//	auto operator-=(Vec const& rhs)->Vec&
	//	{
	//		x -= rhs.x;
	//		y -= rhs.y;
	//		z -= rhs.z;
	//		w -= rhs.w;
	//		return *this;
	//	}

	//	auto operator==(Vec const& rhs) const noexcept ->bool
	//	{
	//		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	//	}

	//	auto operator!=(Vec const& rhs) const noexcept ->bool
	//	{
	//		return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
	//	}

	//	template<typename U>
	//	auto operator*(U const& rhs) const noexcept->Vec
	//	{
	//		return Vec(x * rhs, y * rhs, z * rhs, w * rhs);
	//	}

	//	template<typename U>
	//	auto operator*=(U const& rhs) ->Vec&
	//	{
	//		x *= rhs;
	//		y *= rhs;
	//		z *= rhs;
	//		w *= rhs;
	//		return *this;
	//	}

	//	template<typename U>
	//	auto operator/(U const& rhs) const noexcept->Vec
	//	{
	//		return Vec(x / rhs, y / rhs, z / rhs, w / rhs);
	//	}

	//	template<typename U>
	//	auto operator/=(U const& rhs) ->Vec&
	//	{
	//		x /= rhs;
	//		y /= rhs;
	//		z /= rhs;
	//		w /= rhs;
	//		return *this;
	//	}

	//	auto operator-() const noexcept->Vec
	//	{
	//		return Vec(-x, -y, -z, -w);
	//	}

	//	auto operator[](int i) -> T&
	//	{
	//		if (i < 0 || i >= 3) {
	//			log(Status::fatal, "Vec: index out of range");
	//		}
	//		if (i == 0) {
	//			return x;
	//		} 
	//		else if (i == 1) {
	//			return y;
	//		}
	//		else if (i == 2) {
	//			return z;
	//		}
	//		return w;
	//	}


	//	T x;
	//	T y;
	//	T z;
	//	T w;
	//};

	//template<typename T>
	//using Vector4 = Vec<4, Category::vector, T>;
	//template<typename T>
	//inline auto length_squared(Vector4<T> const& v)->Float
	//{
	//	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	//}

	//template<typename T>
	//inline auto length(Vector4<T> const& v)->Float
	//{
	//	return std::sqrt(length_squared(v));
	//}

	//template<typename T>
	//inline auto normalize(Vector4<T> const& v)->Vector4<T>
	//{
	//	return v / length(v);
	//}


	//using Vector4i = Vector4<int>;
	//static_assert(std::is_standard_layout_v<Vector4i>);
	//static_assert(offsetof(Vector4i, y) == sizeof(int));
	//static_assert(offsetof(Vector4i, z) == sizeof(int) * 2);
	//static_assert(offsetof(Vector4i, w) == sizeof(int) * 3);

	//using Vector4f = Vector4<Float>;
	//static_assert(std::is_standard_layout_v<Vector4f>);
	//static_assert(offsetof(Vector4f, y) == sizeof(Float));
	//static_assert(offsetof(Vector4f, z) == sizeof(Float) * 2);
	//static_assert(offsetof(Vector4f, w) == sizeof(Float) * 3);

}

#include "vector.inl"