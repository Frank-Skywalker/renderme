#include "bvh.hpp"

namespace renderme
{
	BVH::BVH(std::vector<std::unique_ptr<Primitive>> primivites, unsigned int max_primitives_per_node, Strategy strategy)
		:primitives{ std::move(primitives) }, max_primitives_per_node{max_primitives_per_node}, strategy{strategy}
	{
		if (primitives.empty())
			return;



	}


	auto BVH::gl_draw(Shader const& shader) const noexcept -> void
	{

	}

	auto BVH::intersect() const noexcept ->bool
	{
		return false;
	}

	auto BVH::intersect_shadow() const noexcept ->bool
	{
		return false;
	}

	auto BVH::imgui_config() const noexcept ->void
	{

	}

	auto BVH::world_bounds() const noexcept -> Bounds3f const&
	{
		return Bounds3f();
	}
}