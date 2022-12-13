#include "transform.hpp"
#include"log.hpp"

#include<iostream>

namespace renderme
{

	Matrix::Matrix()
	{
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
		m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = 0.0f;
	}

	Matrix::Matrix(Float const value[4][4])
	{
		for (auto i = 0; i < 4; ++i) {
			for (auto j = 0; j < 4; ++j) {
				m[i][j] = value[i][j];
			}
		}
	}

	Matrix::Matrix(Float t00, Float t01, Float t02, Float t03, Float t10,
		Float t11, Float t12, Float t13, Float t20, Float t21,
		Float t22, Float t23, Float t30, Float t31, Float t32,
		Float t33)
	{
		m[0][0] = t00;
		m[0][1] = t01;
		m[0][2] = t02;
		m[0][3] = t03;
		m[1][0] = t10;
		m[1][1] = t11;
		m[1][2] = t12;
		m[1][3] = t13;
		m[2][0] = t20;
		m[2][1] = t21;
		m[2][2] = t22;
		m[2][3] = t23;
		m[3][0] = t30;
		m[3][1] = t31;
		m[3][2] = t32;
		m[3][3] = t33;
	}

	auto Matrix::print() const noexcept -> void
	{
		std::cout<<"["<<std::endl;
		for (auto i = 0; i < 4; ++i) {
			std::cout<<"[";
			for (auto j = 0; j < 4; ++j) {
				std::cout<<" " << m[i][j];
			}
			std::cout << " ]" << std::endl;;
		}
		std::cout << "]" << std::endl;
	}
	

	auto Matrix::transpose() const noexcept->Matrix
	{
		return Matrix(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		);
	}



	auto Matrix::inverse() const noexcept->Matrix
	{
		int indxc[4], indxr[4];
		int ipiv[4] = {0, 0, 0, 0};
		Float minv[4][4];
		memcpy(minv, m, 4 * 4 * sizeof(Float));
		for (auto i = 0; i < 4; ++i) {
			int irow = 0, icol = 0;
			Float big = 0.f;
			// Choose pivot
			for (auto j = 0; j < 4; ++j) {
				if (ipiv[j] != 1) {
					for (auto k = 0; k < 4; ++k) {
						if (ipiv[k] == 0) {
							if (std::abs(minv[j][k]) >= big) {
								big = Float(std::abs(minv[j][k]));
								irow = j;
								icol = k;
							}
						} 
						else if (ipiv[k] > 1)
							log(Status::fatal, "Singular matrix in MatrixInvert");
					}
				}
			}
			++ipiv[icol];
			// Swap rows _irow_ and _icol_ for pivot
			if (irow != icol) {
				for (auto k = 0; k < 4; ++k)
					std::swap(minv[irow][k], minv[icol][k]);
			}
			indxr[i] = irow;
			indxc[i] = icol;
			if (minv[icol][icol] == 0.f) 
				log(Status::fatal, "Singular matrix in MatrixInvert");

			// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
			Float pivinv = 1. / minv[icol][icol];
			minv[icol][icol] = 1.;
			for (auto j = 0; j < 4; ++j) 
				minv[icol][j] *= pivinv;

			// Subtract this row from others to zero out their columns
			for (auto j = 0; j < 4; ++j) {
				if (j != icol) {
					Float save = minv[j][icol];
					minv[j][icol] = 0;
					for (auto k = 0; k < 4; ++k)
						minv[j][k] -= minv[icol][k] * save;
				}
			}
		}
		// Swap columns to reflect permutation
		for (auto j = 3; j >= 0; --j) {
			if (indxr[j] != indxc[j]) {
				for (auto k = 0; k < 4; ++k)
					std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
			}
		}
		return Matrix(minv);
	}

	auto Matrix::operator*(Matrix const& rhs) const noexcept->Matrix
	{
		Matrix r;
		for (auto i = 0; i < 4; ++i)
			for (auto j = 0; j < 4; ++j)
				r.m[i][j] =
				m[i][0] * rhs.m[0][j] +
				m[i][1] * rhs.m[1][j] +
				m[i][2] * rhs.m[2][j] +
				m[i][3] * rhs.m[3][j];
		return r;
	}

	auto Matrix::operator==(Matrix const& rhs) const noexcept->bool
	{
		for (auto i = 0; i < 4; ++i)
			for (auto j = 0; j < 4; ++j)
				if (m[i][j] != rhs.m[i][j]) 
					return false;
		return true;
	}

	auto Matrix::operator!=(Matrix const& rhs) const noexcept->bool
	{
		for (auto i = 0; i < 4; ++i)
			for (auto j = 0; j < 4; ++j)
				if (m[i][j] != rhs.m[i][j])
					return true;
		return false;
	}

	auto operator<<(std::ostream& os, Matrix const& m)->std::ostream&
	{
		os << "[" << std::endl;
		for (auto i = 0; i < 4; ++i) {
			os << "[";
			for (auto j = 0; j < 4; ++j) {
				os << " " << m.m[i][j];
			}
			os << " ]" << std::endl;;
		}
		os << "]" << std::endl;
		return os;
	}



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

	Transform::Transform(Matrix const& _m)
		:m{_m}, m_inv{_m.inverse()}
	{

	}
	
	Transform::Transform(Matrix const& _m, Matrix const& _m_inv)
		:m{_m},m_inv{_m_inv}
	{

	}

	auto Transform::matrix() const noexcept ->Matrix const&
	{
		return m;
	}

	auto Transform::inverse_matrix() const noexcept ->Matrix const&
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