#pragma once

#include <core/material.hpp>

namespace renderme
{
	struct Phong_Material final: Material
	{
		auto gl_draw() const noexcept ->void;

	};
}