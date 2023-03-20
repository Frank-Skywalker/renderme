#pragma once
#include "shader.hpp"
#include "primitive.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "texture.hpp"

#include<vector>
#include<memory>
#include<string>

namespace renderme
{
	struct Scene final : Only_Movable
	{
		Scene(
			std::string name,
			std::vector<std::unique_ptr<Transform>> transforms,
			std::vector<std::unique_ptr<Texture>> textures,
			std::vector<std::unique_ptr<Shape>> shapes,
			std::vector<std::unique_ptr<Material>> materials,
			std::vector<std::unique_ptr<Primitive>> primitives,
			std::vector<std::unique_ptr<Light>> lights
		);
		auto gl_draw(Shader const& shader) const noexcept -> void;
		auto imgui_config() -> void;
		auto intersect() const noexcept->bool;
		auto intersect_shadow() const noexcept->bool;


		std::string name;
		std::vector<std::unique_ptr<Transform>> transforms;
		std::vector<std::unique_ptr<Texture>> textures;
		std::vector<std::unique_ptr<Shape>> shapes;
		std::vector<std::unique_ptr<Material>> materials;
		std::vector<std::unique_ptr<Primitive>> primitives;
		std::vector<std::unique_ptr<Light>> lights;
	};
}
