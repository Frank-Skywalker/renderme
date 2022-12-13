#pragma once

#include "primitive.hpp"
#include "camera.hpp"
#include "light.hpp"

#include<vector>
#include<memory>
#include<string>

namespace renderme
{
	struct Scene final: Only_Movable
	{

	private:
		std::string name;
		std::unique_ptr<Camera> camera;
		std::vector<std::unique_ptr<Primitive>> primitives;
		std::vector<std::unique_ptr<Light>> lights;
	};
}