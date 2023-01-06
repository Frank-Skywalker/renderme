#include "integrator.hpp"

#include <imgui/imgui.h>

namespace renderme
{

	Integrator::Integrator(std::unique_ptr<Shader> shader)
		:shader{std::move(shader)}
	{
	}

}