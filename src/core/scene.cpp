#include "scene.hpp"

namespace renderme
{

	auto Scene::gl_draw(Shader const& shader) const noexcept -> void
	{
		for (auto const& primitive : primitives) {
			primitive->gl_draw(shader);
		}
	}


	auto Scene::intersect() const noexcept->bool
	{
		auto result = false;
		for (auto const& primitive : primitives) {
			result |= primitive->intersect();
		}
		return result;
	}


	auto Scene::intersect_shadow() const noexcept->bool
	{
		auto result = false;
		for (auto const& primitive : primitives) {
			result |= primitive->intersect_shadow();
		}
		return result;
	}
}