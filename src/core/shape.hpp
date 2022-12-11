#pragma once

namespace renderme
{
	struct Shape
	{
		virtual auto gl_draw(void)->void = 0;
		virtual auto intersect(void)->void = 0;
		virtual auto intersect_shadow(void)->void = 0;
	};
}