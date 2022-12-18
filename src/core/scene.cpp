#include "scene.hpp"

namespace renderme
{

	Scene::Scene(std::string _name, std::vector<std::unique_ptr<Transform>> _transforms,
		std::vector<std::unique_ptr<Primitive>> _primitives,
		std::vector<std::unique_ptr<Light>> _lights)
		:name{std::move(_name)}, primitives{std::move(_primitives)},
		lights{std::move(_lights)}, transforms{std::move(_transforms)}
	{}


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