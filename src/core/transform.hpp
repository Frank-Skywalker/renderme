#pragma once
#include "vector.hpp"
#include <iostream>

namespace renderme
{
	template<unsigned int xdim, unsigned int ydim, typename T> 
	struct Mat;

	template<typename T>
	struct Mat<4,4,T> final
	{
		Mat()
		{
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
			m[0][1] = m[0][2] = m[0][3] = 0;
			m[1][0] = m[1][2] = m[1][3] = 0;
			m[2][0] = m[2][1] = m[2][3] = 0;
			m[3][0] = m[3][1] = m[3][2] = 0;
		}

		Mat(T const value[4][4])
		{
			for (auto i = 0; i < 4; ++i) {
				for (auto j = 0; j < 4; ++j) {
					m[i][j] = value[i][j];
				}
			}
		}

		Mat(T t00, T t01, T t02, T t03,
			T t10, T t11, T t12, T t13,
			T t20, T t21, T t22, T t23,
			T t30, T t31, T t32, T t33)
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
		
		auto print() const noexcept -> void
		{
			std::cout << "[" << std::endl;
			for (auto i = 0; i < 4; ++i) {
				std::cout << "[";
				for (auto j = 0; j < 4; ++j) {
					std::cout << " " << m[i][j];
				}
				std::cout << " ]" << std::endl;;
			}
			std::cout << "]" << std::endl;
		}

		auto transpose() const noexcept->Mat
		{
			return Mat(
				m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]
			);
		}

		auto inverse() const noexcept->Mat
		{
			int indxc[4], indxr[4];
			int ipiv[4] = {0, 0, 0, 0};
			T minv[4][4];
			memcpy(minv, m, 4 * 4 * sizeof(T));
			for (auto i = 0; i < 4; ++i) {
				int irow = 0, icol = 0;
				T big = 0.f;
				// Choose pivot
				for (auto j = 0; j < 4; ++j) {
					if (ipiv[j] != 1) {
						for (auto k = 0; k < 4; ++k) {
							if (ipiv[k] == 0) {
								if (std::abs(minv[j][k]) >= big) {
									big = T(std::abs(minv[j][k]));
									irow = j;
									icol = k;
								}
							} else if (ipiv[k] > 1)
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
				T pivinv = 1. / minv[icol][icol];
				minv[icol][icol] = 1.;
				for (auto j = 0; j < 4; ++j)
					minv[icol][j] *= pivinv;

				// Subtract this row from others to zero out their columns
				for (auto j = 0; j < 4; ++j) {
					if (j != icol) {
						T save = minv[j][icol];
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
			return Mat(minv);
		}

		auto operator*(Mat const& rhs) const noexcept->Mat
		{
			Mat r;
			for (auto i = 0; i < 4; ++i)
				for (auto j = 0; j < 4; ++j)
					r.m[i][j] =
					m[i][0] * rhs.m[0][j] +
					m[i][1] * rhs.m[1][j] +
					m[i][2] * rhs.m[2][j] +
					m[i][3] * rhs.m[3][j];
			return r;
		}

		auto operator==(Mat const& rhs) const noexcept->bool
		{
			for (auto i = 0; i < 4; ++i)
				for (auto j = 0; j < 4; ++j)
					if (m[i][j] != rhs.m[i][j])
						return false;
			return true;
		}

		auto operator!=(Mat const& rhs) const noexcept->bool
		{
			for (auto i = 0; i < 4; ++i)
				for (auto j = 0; j < 4; ++j)
					if (m[i][j] != rhs.m[i][j])
						return true;
			return false;
		}

		friend auto operator<<(std::ostream& os, Mat const& m)->std::ostream&
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

		T m[4][4];
	};

	template<typename T>
	using Matrix4 = Mat<4, 4, T>;
	using Matrix4f = Matrix4<Float>;
	static_assert(std::is_standard_layout_v<Matrix4f>);

	struct Transform final
	{
		Transform() = default;
		Transform(Float const value[4][4]);
		Transform(Float t00, Float t01, Float t02, Float t03,
			Float t10, Float t11, Float t12, Float t13,
			Float t20, Float t21, Float t22, Float t23,
			Float t30, Float t31, Float t32, Float t33);
		Transform(Matrix4f const& _m);
		Transform(Matrix4f const& _m, Matrix4f const& _m_inv);

		auto matrix() const noexcept ->Matrix4f const&;
		auto inverse_matrix() const noexcept ->Matrix4f const&;
		auto print() const noexcept ->void;
		auto is_identity() const noexcept -> bool;
		auto transpose() const noexcept->Transform;
		auto inverse() const noexcept->Transform;
		auto operator*(Transform const& rhs) const noexcept -> Transform;
		auto operator==(Transform const& rhs) const noexcept->bool;
		auto operator!=(Transform const& rhs) const noexcept->bool;

		friend auto operator<<(std::ostream& os, Transform const& m)->std::ostream&;

	private:
		Matrix4f m;
		Matrix4f m_inv;
	};

	auto translate(Vector3f const& delta)-> Transform;
	auto translate(Float x, Float y, Float z)->Transform;
	auto scale(Vector3f const& _scale)->Transform;
	auto scale(Float x, Float y, Float z)->Transform;
	auto rotate_x(Float theta)->Transform;
	auto rotate_y(Float theta)->Transform;
	auto rotate_z(Float theta)->Transform;
	auto rotate(Vector3f const& axis, Float theta);
	auto lookat(Point3f const& position, Point3f const& lookat, Vector3f const& up);
	auto orthographic(Float znear, Float zfar)->Transform;
	auto perspective(Float znear, Float zfar, Float fov)->Transform;

}