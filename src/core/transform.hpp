#pragma once
#include"renderme.hpp"
#include <iostream>

namespace renderme
{
	struct Matrix final
	{
		Matrix();
		Matrix(Float const value[4][4]);
		Matrix(Float t00, Float t01, Float t02, Float t03,
			Float t10, Float t11, Float t12, Float t13,
			Float t20, Float t21, Float t22, Float t23,
			Float t30, Float t31, Float t32, Float t33);
		
		auto print() const noexcept -> void;
		auto transpose() const noexcept->Matrix;
		auto inverse() const noexcept->Matrix;
		auto operator*(Matrix const& rhs) const noexcept->Matrix;
		auto operator==(Matrix const& rhs) const noexcept->bool;
		auto operator!=(Matrix const& rhs) const noexcept->bool;

		friend auto operator<<(std::ostream& os, Matrix const& m)->std::ostream&;

		Float m[4][4];
	};

	struct Transform final
	{
		Transform() = default;
		Transform(Float const value[4][4]);
		Transform(Float t00, Float t01, Float t02, Float t03,
			Float t10, Float t11, Float t12, Float t13,
			Float t20, Float t21, Float t22, Float t23,
			Float t30, Float t31, Float t32, Float t33);
		Transform(Matrix const& _m);
		Transform(Matrix const& _m, Matrix const& _m_inv);

		auto matrix() const noexcept ->Matrix const&;
		auto inverse_matrix() const noexcept ->Matrix const&;
		auto print() const noexcept ->void;
		auto is_identity() const noexcept -> bool;
		auto transpose() const noexcept->Transform;
		auto inverse() const noexcept->Transform;
		auto operator*(Transform const& rhs) const noexcept -> Transform;
		auto operator==(Transform const& rhs) const noexcept->bool;
		auto operator!=(Transform const& rhs) const noexcept->bool;

		friend auto operator<<(std::ostream& os, Transform const& m)->std::ostream&;

	private:
		Matrix m;
		Matrix m_inv;

	};
}