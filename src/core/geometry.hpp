#pragma once
#include"type.hpp"

namespace renderme
{
	//////Vectors//////
	template<typename T>
	struct Vector2 final
	{
		Vector2():x{0}, y{0} {}
		Vector2(T _x, T _y):x{_x}, y{_y} {}
		T x;
		T y;
	};


	template<typename T>
	struct Vector3 final
	{
		Vector3():x{0}, y{0}, z{0} {}
		Vector3(T _x, T _y, T _z):x{_x}, y{_y}, z{_z} {}
		T x;
		T y;
		T z;
	};


	template<typename T>
	struct Vector4 final
	{
		Vector4():x{0}, y{0}, z{0}, w{0} {}
		Vector4(T _x, T _y, T _z, T _w):x{_x}, y{_y}, z{_z}, w{_w} {}
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
		T x;
		T y;
	};


	template<typename T>
	struct Point3 final
	{
		T x;
		T y;
		T z;
	};


	template<typename T>
	struct Point4 final
	{
		T x;
		T y;
		T z;
		T w;
	};

	using Point2i = Point2<int>;
	using Point3i = Point3<int>;
	using Point4i = Point4<int>;

	using Point2f = Point2<Float>;
	using Point3f = Point3<Float>;
	using Point4f = Point4<Float>;



	//////Normals//////
	template<typename T>
	struct Normal2 final
	{
		T x;
		T y;
	};


	template<typename T>
	struct Normal3 final
	{
		T x;
		T y;
		T z;
	};


	template<typename T>
	struct Normal4 final
	{
		T x;
		T y;
		T z;
		T w;
	};

	using Normal2i = Normal2<int>;
	using Normal3i = Normal3<int>;
	using Normal4i = Normal4<int>;

	using Normal2f = Normal2<Float>;
	using Normal3f = Normal3<Float>;
	using Normal4f = Normal4<Float>;


}

#include "geometry.inl"