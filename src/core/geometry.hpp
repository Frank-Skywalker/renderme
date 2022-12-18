#pragma once
#include"type.hpp"

namespace renderme
{
	//////Vectors//////
	template<typename T>
	struct Vector2 final
	{
		Vector2():x{0}, y{0} {}
		Vector2(T x, T y):x{x}, y{y} {}
		T x;
		T y;
	};


	template<typename T>
	struct Vector3 final
	{
		Vector3():x{0}, y{0}, z{0} {}
		Vector3(T x, T y, T z):x{x}, y{y}, z{z} {}
		T x;
		T y;
		T z;
	};


	template<typename T>
	struct Vector4 final
	{
		Vector4():x{0}, y{0}, z{0}, w{0} {}
		Vector4(T x, T y, T z, T w):x{x}, y{y}, z{z}, w{w} {}
		T x;
		T y;
		T z;
		T w;
	};

	using Vector2i = Vector2<int>;
	using Vector3i = Vector3<int>;
	using Vector4i = Vector4<int>;

	using Vector2f = Vector2<Float>;
	using Vector3f = Vector3<Float>;
	using Vector4f = Vector4<Float>;


	//////Points//////
	template<typename T>
	struct Point2 final
	{
		Point2():x{0}, y{0} {}
		Point2(T x, T y):x{x}, y{y} {}
		T x;
		T y;
	};


	template<typename T>
	struct Point3 final
	{
		Point3():x{0}, y{0}, z{0} {}
		Point3(T x, T y, T z):x{x}, y{y}, z{z} {}
		T x;
		T y;
		T z;
	};

	using Point2i = Point2<int>;
	using Point3i = Point3<int>;
	using Point3ui = Point3<unsigned int>;

	using Point2f = Point2<Float>;
	using Point3f = Point3<Float>;


	//////Normals//////
	template<typename T>
	struct Normal3 final
	{
		Normal3():x{0}, y{0}, z{0} {}
		Normal3(T x, T y, T z):x{x}, y{y}, z{z} {}
		T x;
		T y;
		T z;
	};

	using Normal3f = Normal3<Float>;
}

#include "geometry.inl"