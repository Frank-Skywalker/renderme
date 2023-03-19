#pragma once
#include <core/primitive.hpp>
#include <vector>
namespace renderme
{
	enum struct Strategy
	{
		sah,
	};

	struct BVH final: Aggregate
	{
		BVH(std::vector<std::unique_ptr<Primitive>> primivites, unsigned int max_primitives_per_node = 1, Strategy strategy = Strategy::sah);

		auto gl_draw(Shader const& shader) const noexcept -> void override;
		auto intersect() const noexcept ->bool override;
		auto intersect_shadow() const noexcept ->bool override;
		auto imgui_config() const noexcept ->void override;

		auto world_bounds() const noexcept -> Bounds3f const& override;

	private:
		std::vector<std::unique_ptr<Primitive>> primitives;
		unsigned int max_primitives_per_node{ 1 };
		Strategy strategy{ Strategy::sah };
	};

}