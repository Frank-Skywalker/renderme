#pragma once
#include "util.hpp"
#include "transform.hpp"

namespace renderme
{
	struct Camera: Only_Movable
	{
		Camera(Transform t);

		Transform camera_to_world;
	};
}