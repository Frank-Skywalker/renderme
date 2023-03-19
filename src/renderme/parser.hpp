#pragma once
#include <core/util.hpp>
#include<core/runtime-path.hpp>
#include<core/primitive.hpp>
#include<core/integrator.hpp>
#include<core/transform.hpp>
#include<core/camera.hpp>
#include<core/light.hpp>
#include<core/texture.hpp>
#include<core/film.hpp>

#include<vector>
#include<unordered_map>
#include<memory>

#include <assimp/scene.h>
#include <nlohmann/json.hpp>

namespace renderme
{
	struct Scene_Parse_Cache
	{
		// Scene
		// Transform must not move in memory, so use unique_ptr to store Transforms on stack
		std::vector<std::unique_ptr<Transform>> parsing_transforms;
		std::vector<std::unique_ptr<Texture>> parsing_textures;
		std::vector<std::unique_ptr<Shape>> parsing_shapes;
		std::vector<std::unique_ptr<Material>> parsing_materials;
		std::vector<std::unique_ptr<Primitive>> parsing_gl_draw_primitives;
		std::vector<std::unique_ptr<Primitive>> parsing_render_primitives;
		std::vector<std::unique_ptr<Light>> parsing_lights;

		// Lookup buffers
		std::unordered_map<std::string, Material*> name_to_material;
		std::unordered_map<std::string, Texture*> path_to_texture;
	};


	//////Parsing//////
	auto parse_film(nlohmann::json const& json) -> std::unique_ptr<Film>;
	auto parse_camera(nlohmann::json const& json) -> std::unique_ptr<Camera>;
	auto parse_integrator(nlohmann::json const& json) -> std::unique_ptr<Integrator>;
	auto parse_scene(nlohmann::json const& json) -> std::unique_ptr<Scene>;


	// Scene parsing functions
	auto parse_obj_into(Scene_Parse_Cache& cache, Runtime_Path const& path) -> bool;
	auto parse_ainode_into(Scene_Parse_Cache& cache, Runtime_Path const& path, aiScene const* aiscene, aiNode const* ainode) -> bool;
	auto parse_aimesh_into(Scene_Parse_Cache& cache, Runtime_Path const& path, aiScene const* aiscene, aiMesh const* aimesh) -> bool;
	auto parse_aimaterial_into(Scene_Parse_Cache& cache, Runtime_Path const& path, aiScene const* aiscene, aiMaterial const* aimaterial) -> Material*;
	auto parse_light_into(Scene_Parse_Cache& cache, nlohmann::json const& json) -> void;

}
