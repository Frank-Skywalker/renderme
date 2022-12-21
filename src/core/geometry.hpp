#pragma once
#include"type.hpp"

#include <cstring>

namespace renderme
{
	//////Vectors//////
	
	//template<class T, size_t dim>
	//struct Vector final
	//{
	//	Vector()
	//	{
	//		memset(v, 0, sizeof(T) * dim);
	//		//for (auto i = 0u; i < dim; ++i) {
	//		//	v[i] = 0;
	//		//}
	//	}

	//	Vector(T value[dim])
	//	{
	//		memcpy(v, value, sizeof(T) * dim);
	//	}
	//	T v[dim];
	//};

	enum struct Category
	{
		vector,
		point,
		normal
	};

	template<Category category, size_t dim, typename T> struct Vector;

	template<typename T>
	struct Vector<Category::vector, 2, T>
	{
		Vector():x{0}, y{0} {}
		Vector(T x, T y):x{x}, y{y} {}

		auto operator+(Vector const& v) const noexcept->Vector
		{
			return Vector(x + v.x, y + v.y);
		}

		auto operator+=(Vector const& v) -> Vector&
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		T x;
		T y;
	};

	using Vector2i = Vector<Category::vector, 2, int>;
	using Vector2f = Vector<Category::vector, 2, Float>;

	template<class T>
	struct Vector<Category::vector, 3, T> final
	{
		Vector():x{0}, y{0}, z{0} {}
		Vector(T x, T y, T z):x{x}, y{y}, z{z} {}
		T x;
		T y;
		T z;
	};

	using Vector3i = Vector<Category::vector, 3, int>;
	using Vector3f = Vector<Category::vector, 3, Float>;

	template<class T>
	struct Vector<Category::vector, 4, T> final
	{
		Vector():x{0}, y{0}, z{0}, w{0} {}
		Vector(T x, T y, T z, T w):x{x}, y{y}, z{z}, w{w} {}
		T x;
		T y;
		T z;
		T w;
	};

	using Vector4i = Vector<Category::vector, 4, int>;
	using Vector4f = Vector<Category::vector, 4, Float>;


	//////Points//////
	template<class T>
	struct Vector<Category::point, 2, T> final
	{
		Vector():x{0}, y{0} {}
		Vector(T x, T y):x{x}, y{y} {}
		T x;
		T y;
	};

	using Point2i = Vector<Category::point, 2, int>;
	using Point2f = Vector<Category::point, 2, Float>;


	template<class T>
	struct Vector<Category::point, 3, T> final
	{
		Vector():x{0}, y{0}, z{0} {}
		Vector(T x, T y, T z):x{x}, y{y}, z{z} {}
		T x;
		T y;
		T z;
	};

	using Point3ui = Vector<Category::point,3,unsigned int>;
	using Point3i = Vector<Category::point, 3, int>;
	using Point3f = Vector<Category::point, 3, Float>;


	//////Normals//////
	template<class T>
	struct Vector<Category::normal, 3, T> final
	{
		Vector():x{0}, y{0}, z{0} {}
		Vector(T x, T y, T z):x{x}, y{y}, z{z} {}
		T x;
		T y;
		T z;
	};

	using Normal3i = Vector<Category::normal, 3, int>;
	using Normal3f = Vector<Category::normal, 3, Float>;
}

#include "geometry.inl"