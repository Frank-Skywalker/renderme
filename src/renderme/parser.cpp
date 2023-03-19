#include "parser.hpp"

#include<core/log.hpp>
#include<shapes/triangle.hpp>
#include<cameras/perspective.hpp>
#include<materials/phong.hpp>
#include<integrators/zbuffer.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


namespace renderme
{

	auto Parser::parse_film(Runtime_Path const& path)->std::unique_ptr<Film>
	{
		clean_parsing_cache();
		return std::make_unique<Film>();
	}


	auto Parser::parse_camera(Runtime_Path const& path)->std::unique_ptr<Camera>
	{
		clean_parsing_cache();
		return std::make_unique<Perspective_Camera>();
	}

	auto Parser::parse_integrator(Runtime_Path const& path)->std::unique_ptr<Integrator>
	{
		if (path.full_path() == Runtime_Path::renderme_root_path()) {
			log(Status::error, "empty path");
			clean_parsing_cache();
			return nullptr;
		}

		auto create_new_integrator = [&]() -> std::unique_ptr<Integrator> {
			//auto camera = std::make_unique<Perspective_Camera>();
			//auto shader = std::make_unique<Shader>("src/shaders/phong.vert.glsl", "src/shaders/phong.frag.glsl");
			//return std::make_unique<Sample_Integrator>(
			//        std::move(camera),
			//        std::move(shader)
			//        );
			return std::make_unique<ZBuffer_Integrator>();
		};

		auto integrator = create_new_integrator();
		clean_parsing_cache();

		return integrator;
	}


	auto Parser::parse_scene(Runtime_Path const& path)->std::unique_ptr<Scene>
	{
		if (!parse_obj(path)) {
			clean_parsing_cache();
			return nullptr;
		}

		auto scene = std::make_unique<Scene>("test",
			std::move(parsing_transforms),
			std::move(parsing_textures),
			std::move(parsing_shapes),
			std::move(parsing_materials),
			std::move(parsing_gl_draw_primitives),
			std::move(parsing_render_primitives),
			std::move(parsing_lights)
		);

		clean_parsing_cache();

		return scene;
	}


	// loads a model with supported ASSIMP extensions from file
	auto Parser::parse_obj(Runtime_Path const& path)->bool
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		auto aiscene = importer.ReadFile(path.full_path(),
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
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
		return parse_ainode(path, aiscene, aiscene->mRootNode);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	auto Parser::parse_ainode(Runtime_Path const& path, aiScene const* aiscene, aiNode const* ainode) -> bool
	{
		// process each mesh located at the current node
		for (auto i = 0u; i < ainode->mNumMeshes; ++i) {
			// the node object only contains indices to index the actual objects in the scene.
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			auto aimesh = aiscene->mMeshes[ainode->mMeshes[i]];
			if (!parse_aimesh(path, aiscene, aimesh)) {
				return false;
			}
		}

		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (auto i = 0u; i < ainode->mNumChildren; ++i) {
			if (!parse_ainode(path, aiscene, ainode->mChildren[i])) {
				return false;
			}
		}
		return true;
	}

	auto Parser::parse_aimesh(Runtime_Path const& path, aiScene const* aiscene, aiMesh const* aimesh) -> bool
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
		auto material = parse_aimaterial(path, aiscene, aimaterial);

		//Create primitives
		auto triangle_mesh = std::make_unique<Triangle_Mesh>(
			nullptr, nullptr,
			std::move(faces), std::move(positions), std::move(normals),
			std::move(uvs), std::move(tangents), std::move(bitangents)
		);
		parsing_gl_draw_primitives.push_back(std::make_unique<Shape_Primitive>(triangle_mesh.get(), material));
		auto triangles = triangle_mesh->create_triangles();
		parsing_shapes.push_back(std::move(triangle_mesh));

		for (auto& tri : triangles) {
			auto triangle = std::make_unique<Triangle>(std::move(tri));
			parsing_render_primitives.push_back(std::make_unique<Shape_Primitive>(triangle.get(), material));
			parsing_shapes.push_back(std::move(triangle));
		}
		return true;
	}


	auto Parser::parse_aimaterial(Runtime_Path const& path, aiScene const* aiscene, aiMaterial const* aimaterial) -> Material*
	{
		// Need not create material if material has already been parsed
		if (auto iter = name_to_material.find(aimaterial->GetName().C_Str()); iter != name_to_material.end()) {
			return iter->second;
		}

		auto phong_material = std::make_unique<Phong_Material>(aimaterial->GetName().C_Str());

		aiColor4D diffuse;
		if (aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse) != aiReturn_SUCCESS) {
			log(Status::error, "No diffuse");
		}
		else {
			phong_material->diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
		}

		aiColor4D specular;
		if (aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_SPECULAR, &specular) != aiReturn_SUCCESS) {
			log(Status::error, "No Specular");
		}
		else {
			phong_material->specular = glm::vec3(specular.r, specular.g, specular.b);
		}

		if (aiGetMaterialFloat(aimaterial, AI_MATKEY_SHININESS, &phong_material->specular_exponent)!=aiReturn_SUCCESS) {
			log(Status::error, "No Specular Factor");
		}

		aiColor4D transparent;
		if (aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_TRANSPARENT, &transparent) != aiReturn_SUCCESS) {
			log(Status::error, "No Transparent");
		}
		else {
			phong_material->transparent = glm::vec3(transparent.r, transparent.g, transparent.b);
		}

		if (aiGetMaterialFloat(aimaterial, AI_MATKEY_REFRACTI, &phong_material->refraction_index) != aiReturn_SUCCESS) {
			log(Status::error, "No Refraction Index");
		}


		//Textures part
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN
		std::vector<Texture*> textures;

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
				auto iter = path_to_texture.find(tex_relative_path.C_Str());
				if (iter != path_to_texture.end()) {
					textures.push_back(iter->second);
				}
				else {
					auto upper_dir = path.upper_directory();
					auto tex_path = Runtime_Path(upper_dir.relative_path() + "/" + tex_relative_path.C_Str());
					auto texture = std::make_unique<Texture>(type, tex_path);
					textures.push_back(texture.get());
					// Save texture to lookup buffer
					path_to_texture[tex_relative_path.C_Str()] = texture.get();
					parsing_textures.push_back(std::move(texture));
				}
			}
		}
		// Generate pure color diffuse texture if no such
		auto has_diffuse_texture = [&]()->bool {
			for (auto texture : textures) {
				if (texture->type == Texture_Type::diffuse) {
					return true;
				}
			}
			return false;
		}();

		if (!has_diffuse_texture) {
			auto texture = std::make_unique<Texture>(Texture_Type::diffuse, glm::vec3(phong_material->diffuse + phong_material->specular + phong_material->transparent) / 3.0f);
			textures.push_back(texture.get());
			// Need not save this texture to lookup buffer
			parsing_textures.push_back(std::move(texture));
		}
		
		phong_material->textures = std::move(textures);

		// Save material to lookup buffer
		name_to_material[aimaterial->GetName().C_Str()] = phong_material.get();
		parsing_materials.push_back(std::move(phong_material));

		return parsing_materials.back().get();
	}


	auto Parser::clean_parsing_cache()->void
	{
		parsing_cameras.clear();

		parsing_textures.clear();
		parsing_materials.clear();
		parsing_transforms.clear();
		parsing_shapes.clear();
		parsing_gl_draw_primitives.clear();
		parsing_render_primitives.clear();
		parsing_lights.clear();

		name_to_material.clear();
		path_to_texture.clear();
	}

}
