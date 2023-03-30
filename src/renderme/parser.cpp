#include "parser.hpp"
#include "renderme.hpp"

#include<core/log.hpp>
#include<shapes/triangle.hpp>
#include<cameras/perspective.hpp>
#include<integrators/zbuffer.hpp>
#include<integrators/path-tracer.hpp>
#include<integrators/ray-caster.hpp>
#include<samplers/uniform.hpp>
#include<samplers/jitter.hpp>
#include<samplers/random.hpp>
#include<aggregates/bvh.hpp>
#include<materials/phong.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace renderme
{
	auto parse_film(nlohmann::json const& j)->std::unique_ptr<Film>
	{
		return std::make_unique<Film>();
	}


	auto parse_camera(nlohmann::json const& j)->std::unique_ptr<Camera>
	{
		std::unique_ptr<Camera> camera;

		std::string type = j.at("type");
		if (type == "perspective") {
			if (j.contains("position")) {
				auto position = glm::vec3(j.at("position").at(0), j.at("position").at(1), j.at("position").at(2));
				float yaw = j.at("yaw");
				float pitch = j.at("pitch");
				float fov = j.at("fov");
				camera = std::make_unique<Perspective_Camera>(position, yaw, pitch, fov);
			}
			else {
				camera = std::make_unique<Perspective_Camera>();
			}
			return camera;
		}

		throw std::logic_error("Invalid camera type");
		return nullptr;
	}

	auto parse_integrator(nlohmann::json const& j)->std::unique_ptr<Integrator>
	{
		std::string type = j.at("type");

		if (type == "zbuffer") {
			return std::make_unique<ZBuffer_Integrator>();
		}
		else if (type == "pathtracer") {
			return std::make_unique<Path_Tracer>();
		}
		else if (type == "raycaster") {
			return std::make_unique<Ray_Caster>();
		}

		throw std::logic_error("Invalid integrator type");
		return nullptr;
	}

	auto parse_sampler(nlohmann::json const& j) -> std::unique_ptr<Sampler>
	{
		std::string type = j.at("type");

		if (type == "uniform") {
			return std::make_unique<Uniform_Sampler>();
		}
		else if (type == "jitter") {
			return std::make_unique<Jitter_Sampler>();
		}
		else if (type == "random") {
			return std::make_unique<Random_Sampler>();
		}

		throw std::logic_error("Invalid integrator type");
		return nullptr;
	}



	auto parse_scene(nlohmann::json const& j)->std::unique_ptr<Scene>
	{
		Scene_Parse_Cache cache;

		if (!parse_obj_into(cache, std::string(j.at("obj")))) {
			throw std::logic_error("Invalid obj path");
			return nullptr;
		}

		for (auto const& json : j.at("lights")) {
			parse_light_into(cache, json);
		}

		// Create BVH
		auto parsing_primitives = std::move(cache.parsing_gl_draw_primitives);
		auto bvh = std::make_unique<BVH>(std::move(cache.parsing_render_primitives));
		parsing_primitives.push_back(std::move(bvh));

		return std::make_unique<Scene>(
			j.at("name"),
			std::move(cache.parsing_transforms),
			std::move(cache.parsing_textures),
			std::move(cache.parsing_shapes),
			std::move(cache.parsing_materials),
			std::move(parsing_primitives),
			std::move(cache.parsing_lights)
		);

	}


	auto parse_light_into(Scene_Parse_Cache& cache, nlohmann::json const& json) -> void
	{
		std::string mat_name = json.at("material");
		if (auto iter = cache.name_to_material.find(mat_name); iter != cache.name_to_material.end()) {
			// Parse radiance
			glm::vec3 radiance;
			radiance.r = json.at("radiance").at(0);
			radiance.g = json.at("radiance").at(1);
			radiance.b = json.at("radiance").at(2);
			//radiance = radiance / glm::vec3(255.f, 255.f, 255.f);

			for (auto const& primitive : cache.parsing_render_primitives) {
				auto shape_prim = dynamic_cast<Shape_Primitive const*>(primitive.get());
				if (shape_prim->material->name == mat_name) {
					auto light = std::make_unique<Area_Light>(iter->second, radiance, shape_prim->shape);
					cache.parsing_lights.push_back(std::move(light));
				}
			}
			// TODO
			auto material = dynamic_cast<Phong_Material*>(iter->second);
			material->_emition = radiance;
		}
		else {
			throw std::logic_error("Invalid material name for light");
		}
	}

	// loads a model with supported ASSIMP extensions from file
	auto parse_obj_into(Scene_Parse_Cache& cache, Runtime_Path const& path)->bool
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		auto aiscene = importer.ReadFile(path.full_path(),
			//aiProcess_FlipUVs |
			//aiProcess_JoinIdenticalVertices |
			//aiProcess_SortByPType |
			aiProcess_GenSmoothNormals |
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace
		);

		// check for errors
		if ((aiscene == nullptr) ||
			(aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
			(aiscene->mRootNode == nullptr)
			) {
			log(Status::error, importer.GetErrorString());
			return false;
		}

		// process ASSIMP's root node recursively
		return parse_ainode_into(cache, path, aiscene, aiscene->mRootNode);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	auto parse_ainode_into(Scene_Parse_Cache& cache, Runtime_Path const& path, aiScene const* aiscene, aiNode const* ainode) -> bool
	{
		// process each mesh located at the current node
		for (auto i = 0u; i < ainode->mNumMeshes; ++i) {
			// the node object only contains indices to index the actual objects in the scene.
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			auto aimesh = aiscene->mMeshes[ainode->mMeshes[i]];
			if (!parse_aimesh_into(cache, path, aiscene, aimesh)) {
				return false;
			}
		}

		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (auto i = 0u; i < ainode->mNumChildren; ++i) {
			if (!parse_ainode_into(cache,path, aiscene, ainode->mChildren[i])) {
				return false;
			}
		}
		return true;
	}

	auto parse_aimesh_into(Scene_Parse_Cache& cache, Runtime_Path const& path, aiScene const* aiscene, aiMesh const* aimesh) -> bool
	{
		// data to fill
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<glm::uvec3> faces;


		// walk through each of the mesh's vertices
		for (auto i = 0u; i < aimesh->mNumVertices; ++i) {
			//positions
			positions.push_back(glm::vec3(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z));
			//normals
			if (aimesh->HasNormals()) {
				normals.push_back(glm::vec3(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z));
			}
			else {
				//normals.push_back(Normal3f());
				log(Status::error, "obj has no normal");
				return false;
			}
			//texture coordinates
			if (aimesh->mTextureCoords[0]) {
				//uv
				uvs.push_back(glm::vec2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y));
				//tangent
				tangents.push_back(glm::vec3(aimesh->mTangents[i].x, aimesh->mTangents[i].y, aimesh->mTangents[i].z));
				//bitangent
				bitangents.push_back(glm::vec3(aimesh->mBitangents[i].x, aimesh->mBitangents[i].y, aimesh->mBitangents[i].z));
			}
			else {
				//uvs.push_back(Point2f());
				//tangents.push_back(Vector3f());
				//bitangents.push_back(Vector3f());
				log(Status::error, "obj has no uv");
				return false;
			}
		}

		// walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (auto i = 0u; i < aimesh->mNumFaces; ++i) {
			auto aiface = aimesh->mFaces[i];
			if (aiface.mNumIndices != 3) {
				log(Status::error, "obj face is not triangle");
				return false;
			}
			faces.push_back(glm::uvec3(aiface.mIndices[0], aiface.mIndices[1], aiface.mIndices[2]));
		}

		//Process materials
		auto aimaterial = aiscene->mMaterials[aimesh->mMaterialIndex];
		auto material = parse_aimaterial_into(cache, path, aiscene, aimaterial);

		//Create primitives
		auto triangle_mesh = std::make_unique<Triangle_Mesh>(
			nullptr, nullptr,
			std::move(faces), std::move(positions), std::move(normals),
			std::move(uvs), std::move(tangents), std::move(bitangents)
		);
		cache.parsing_gl_draw_primitives.push_back(std::make_unique<Shape_Primitive>(triangle_mesh.get(), material));
		auto triangles = triangle_mesh->create_triangles();
		cache.parsing_shapes.push_back(std::move(triangle_mesh));

		for (auto& tri : triangles) {
			auto triangle = std::make_unique<Triangle>(std::move(tri));
			cache.parsing_render_primitives.push_back(std::make_unique<Shape_Primitive>(triangle.get(), material));
			cache.parsing_shapes.push_back(std::move(triangle));
		}
		return true;
	}


	auto parse_aimaterial_into(Scene_Parse_Cache& cache, Runtime_Path const& path, aiScene const* aiscene, aiMaterial const* aimaterial) -> Material*
	{
		// Need not create material if material has already been parsed
		if (auto iter = cache.name_to_material.find(aimaterial->GetName().C_Str()); iter != cache.name_to_material.end()) {
			return iter->second;
		}

		auto phong_material = std::make_unique<Phong_Material>(aimaterial->GetName().C_Str());

		aiColor4D diffuse;
		if (aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse) != aiReturn_SUCCESS) {
			log(Status::error, "No diffuse");
		}
		else {
			phong_material->_diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
		}

		aiColor4D specular;
		if (aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_SPECULAR, &specular) != aiReturn_SUCCESS) {
			log(Status::error, "No Specular");
		}
		else {
			phong_material->_specular = glm::vec3(specular.r, specular.g, specular.b);
		}

		if (aiGetMaterialFloat(aimaterial, AI_MATKEY_SHININESS, &phong_material->_specular_exponent)!=aiReturn_SUCCESS) {
			log(Status::error, "No Specular Factor");
		}

		aiColor4D transparent;
		if (aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_TRANSPARENT, &transparent) != aiReturn_SUCCESS) {
			log(Status::error, "No Transparent");
		}
		else {
			phong_material->_transparent = glm::vec3(transparent.r, transparent.g, transparent.b);
		}

		if (aiGetMaterialFloat(aimaterial, AI_MATKEY_REFRACTI, &phong_material->_refraction_index) != aiReturn_SUCCESS) {
			log(Status::error, "No Refraction Index");
		}


		//Textures part
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN
		std::array<Texture*, size_t(Texture_Type::invalid)> textures;
		textures.fill(nullptr);

		std::vector<std::pair<aiTextureType, Texture_Type>> types = {
			{aiTextureType_DIFFUSE, Texture_Type::diffuse},
			{aiTextureType_SPECULAR, Texture_Type::specular},
			{aiTextureType_NORMALS, Texture_Type::normal},
			{aiTextureType_AMBIENT, Texture_Type::ambient},
		};

		for (auto const& [aitype, type] : types) {
			for (auto i = 0u; i < aimaterial->GetTextureCount(aitype); ++i) {
				aiString tex_relative_path;
				aimaterial->GetTexture(aitype, i, &tex_relative_path);

				//Reuse same textures
				auto iter = cache.path_to_texture.find(tex_relative_path.C_Str());
				if (iter != cache.path_to_texture.end()) {
					textures[size_t(type)] = iter->second;
				}
				else {
					auto upper_dir = path.upper_directory();
					auto tex_path = Runtime_Path(upper_dir.relative_path() + "/" + tex_relative_path.C_Str());
					auto texture = std::make_unique<Texture>(type, tex_path);
					textures[size_t(type)] = texture.get();
					// Save texture to lookup buffer
					cache.path_to_texture[tex_relative_path.C_Str()] = texture.get();
					cache.parsing_textures.push_back(std::move(texture));
				}
			}
		}

		//// Generate pure color diffuse texture if no such
		//auto has_diffuse_texture = [&]()->bool {
		//	for (auto texture : textures) {
		//		if (texture->type == Texture_Type::diffuse) {
		//			return true;
		//		}
		//	}
		//	return false;
		//}();

		//if (!has_diffuse_texture) {
		//	auto texture = std::make_unique<Texture>(Texture_Type::diffuse, glm::vec3(phong_material->_diffuse + phong_material->_specular + phong_material->_transparent) / 3.0f);
		//	textures.push_back(texture.get());
		//	// Need not save this texture to lookup buffer
		//	cache.parsing_textures.push_back(std::move(texture));
		//}
		
		phong_material->textures = std::move(textures);

		// Save material to lookup buffer
		cache.name_to_material[aimaterial->GetName().C_Str()] = phong_material.get();
		cache.parsing_materials.push_back(std::move(phong_material));

		return cache.parsing_materials.back().get();
	}


}
