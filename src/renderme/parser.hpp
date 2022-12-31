#pragma once
#include <core/util.hpp>
#include<core/runtime-path.hpp>
#include<core/primitive.hpp>
#include<core/integrator.hpp>

#include<core/transform.hpp>
#include<core/camera.hpp>
#include<core/light.hpp>


#include <assimp/scene.h>
#include<vector>
#include<memory>

namespace renderme
{
	struct Parser final: Singleton<Parser>
	{
		//////Parsing//////
		//auto parse_file(Runtime_Path path)->void;
		auto parse_integrator(Runtime_Path path)->std::unique_ptr<Integrator>;
		auto parse_scene(Runtime_Path path)->std::unique_ptr<Scene>;

	private:
		auto parse_obj(Runtime_Path path)->bool;
		auto parse_ainode(aiScene const* aiscene, aiNode const* ainode) -> bool;
		auto parse_aimesh(aiScene const* aiscene, aiMesh const* aimesh) -> bool;
		auto parse_aimaterial(aiScene const* aiscene, aiMaterial const* aimaterial) -> bool;

		auto clean_parsing_cache()->void;

	private:
		//Transform must not move in memory, so use unique_ptr to store Transforms on stack
		std::vector<std::unique_ptr<Transform>> parsing_transforms;
		std::vector<std::unique_ptr<Primitive>> parsing_primitives;
		std::vector<std::unique_ptr<Light>> parsing_lights;
		std::vector<std::unique_ptr<Camera>> parsing_cameras;
	};
}