#pragma once
#include"renderme.hpp"

namespace renderme
{
	//////Vectors//////
	template<typename T>
	struct Vector2 final
	{
		Vector2();
		T x;
		T y;
	};


	template<typename T>
	struct Vector3 final
	{
		Vector3();
		T x;
		T y;
		T z;
	};


	template<typename T>
	struct Vector4 final
	{
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