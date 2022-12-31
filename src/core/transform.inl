#pragma once
#include "transform.hpp"


namespace renderme
{

	template<typename T>
	auto Transform::operator()(Point3<T> const& p) const noexcept -> Point3<T>
	{
		T x = p.x, y = p.y, z = p.z;
		T xp = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
		T yp = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
		T zp = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
		T wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];

		if (wp == 1)
			return Point3<T>(xp, yp, zp);
		else
			return Point3<T>(xp, yp, zp) / wp;
	}

	template<typename T>
	auto Transform::operator()(Vector3<T> const& v) const noexcept -> Vector3<T>
	{
		T x = v.x, y = v.y, z = v.z;
		return Vector3<T>(
			m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z,
			m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z,
			m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z
			);
	}

	template<typename T>
	auto Transform::operator()(Normal3<T> const& n) const noexcept -> Normal3<T>
	{
		T x = n.x, y = n.y, z = n.z;
		return Normal3<T>(
			m_inv.m[0][0] * x + m_inv.m[1][0] * y + m_inv.m[2][0] * z,
			m_inv.m[0][1] * x + m_inv.m[1][1] * y + m_inv.m[2][1] * z,
			m_inv.m[0][2] * x + m_inv.m[1][2] * y + m_inv.m[2][2] * z
			);
	}

}