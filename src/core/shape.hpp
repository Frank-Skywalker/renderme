#pragma once

namespace renderme
{
	struct Shape
	{
		virtual auto gl_draw() const noexcept ->void = 0;
		virtual auto intersect() const noexcept ->void = 0;
		virtual auto intersect_shadow() const noexcept ->void = 0;
	};
}