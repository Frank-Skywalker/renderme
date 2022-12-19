#pragma once
#include <core/camera.hpp>
#include <core/geometry.hpp>
namespace renderme
{
	struct Perspective_Camera final: Camera
	{
		struct Config
		{
			Point3f position;
			Vector3f direction;
			Vector3f up;
		};


		auto generate_ray() const noexcept ->Float;
		auto imgui_config() ->void;

		Config config;
	};
}