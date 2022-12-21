#include "transform.hpp"

namespace renderme
{

	Transform::Transform(Float const value[4][4])
		:m{value}
	{
		m_inv = m.inverse();
	}

	Transform::Transform(
		Float t00, Float t01, Float t02, Float t03,
		Float t10, Float t11, Float t12, Float t13, 
		Float t20, Float t21, Float t22, Float t23, 
		Float t30, Float t31, Float t32, Float t33)
		:m{ t00,  t01,  t02,  t03,
		 t10,  t11,  t12,  t13,
		 t20,  t21,  t22,  t23,
		 t30,  t31,  t32,  t33}
	{
		m_inv = m.inverse();
	}

	Transform::Transform(Matrix4f const& _m)
		:m{_m}, m_inv{_m.inverse()}
	{

	}
	
	Transform::Transform(Matrix4f const& _m, Matrix4f const& _m_inv)
		:m{_m},m_inv{_m_inv}
	{

	}

	auto Transform::matrix() const noexcept ->Matrix4f const&
	{
		return m;
	}

	auto Transform::inverse_matrix() const noexcept ->Matrix4f const&
	{
		return m_inv;
	}

	auto Transform::print() const noexcept ->void
	{
		m.print();
	}

	auto Transform::is_identity() const noexcept -> bool
	{
		return (m.m[0][0] == 1.f && m.m[0][1] == 0.f && m.m[0][2] == 0.f &&
			m.m[0][3] == 0.f && m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
			m.m[1][2] == 0.f && m.m[1][3] == 0.f && m.m[2][0] == 0.f &&
			m.m[2][1] == 0.f && m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
			m.m[3][0] == 0.f && m.m[3][1] == 0.f && m.m[3][2] == 0.f &&
			m.m[3][3] == 1.f);
	}

	auto Transform::transpose() const noexcept->Transform
	{
		return Transform{m.transpose(), m_inv.transpose()};
	}

	auto Transform::inverse() const noexcept->Transform
	{
		return Transform{m_inv, m};
	}

	auto Transform::operator*(Transform const& rhs) const noexcept -> Transform
	{
		return Transform{m * rhs.m, rhs.m_inv * m_inv};
	}

	auto Transform::operator==(Transform const& rhs) const noexcept->bool
	{
		return m == rhs.m && m_inv == rhs.m_inv;
	}

	auto Transform::operator!=(Transform const& rhs) const noexcept->bool
	{
		return m != rhs.m || m_inv != rhs.m_inv;
	}


	auto operator<<(std::ostream& os, Transform const& m)->std::ostream&
	{
		return os << m.m;
	}

}