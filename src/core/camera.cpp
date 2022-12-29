#include"camera.hpp"

namespace renderme
{
	auto Camera::gl_draw(Shader const& shader) const noexcept -> void
	{
		shader.set_uniform_mat4("camera", camera_to_world.matrix());
	}

}