#pragma once

namespace renderme
{


	struct Material
	{
		virtual auto gl_draw() const noexcept ->void = 0;
	};
}