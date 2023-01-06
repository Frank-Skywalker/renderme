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

    auto Parser::parse_integrator(Runtime_Path const& path, Film* film)->std::unique_ptr<Integrator>
    {
        if (path.full_path() == Runtime_Path::renderme_root_path()) {
            log(Status::error, "empty path");
            clean_parsing_cache();
            return nullptr;
        }

        auto create_new_integrator = [&] () -> std::unique_ptr<Integrator> {
            //auto camera = std::make_unique<Perspective_Camera>();
            //auto shader = std::make_unique<Shader>("src/shaders/phong.vert.glsl", "src/shaders/phong.frag.glsl");
            //return std::make_unique<Sample_Integrator>(
            //        std::move(camera),
            //        std::move(shader)
            //        );
            return std::make_unique<ZBuffer_Integrator>(film);
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

        auto create_new_scene = [&] ()->std::unique_ptr<Scene> {
            //Create a new Scene and Integrator
            std::vector<std::unique_ptr<Transform>> tmp_transforms;
            std::vector<std::unique_ptr<Primitive>> tmp_gl_draw_primitives;
            std::vector<std::unique_ptr<Primitive>> tmp_render_primitives;
            std::vector<std::unique_ptr<Light>> tmp_lights;
            std::vector<std::unique_ptr<Camera>> tmp_cameras;

            for (auto& transform : parsing_transforms) {
                tmp_transforms.push_back(std::move(transform));
            }
            for (auto& primitive : parsing_gl_draw_primitives) {
                tmp_gl_draw_primitives.push_back(std::move(primitive));
            }
            for (auto& primitive : parsing_render_primitives) {
                tmp_render_primitives.push_back(std::move(primitive));
            }
            for (auto& light : parsing_lights) {
                tmp_lights.push_back(std::move(light));
            }
            for (auto& camera : parsing_cameras) {
                tmp_cameras.push_back(std::move(camera));
            }
            return std::make_unique<Scene>("test", std::move(tmp_transforms), std::move(tmp_gl_draw_primitives), std::move(tmp_render_primitives), std::move(tmp_lights));
        };

        auto scene = create_new_scene();
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
            } else {
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
            } else {
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
        auto triangles = triangle_mesh->create_triangles();
        auto triangle_mesh_primitive = std::make_unique<Shape_Primitive>(std::move(triangle_mesh), material);
        parsing_gl_draw_primitives.push_back(std::move(triangle_mesh_primitive));

        for (auto& tri : triangles) {
            auto triangle = std::make_unique<Triangle>(std::move(tri));
            auto triangle_primitive = std::make_unique<Shape_Primitive>(std::move(triangle), material);
            parsing_render_primitives.push_back(std::move(triangle_primitive));
        }

        return true;
    }


    auto Parser::parse_aimaterial(Runtime_Path const& path, aiScene const* aiscene, aiMaterial const* aimaterial) -> std::shared_ptr<Material>
    {
        //Textures part
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        std::vector<std::shared_ptr<Texture>> textures;

        std::vector<std::pair<aiTextureType, Texture_Type>> types = {
            {aiTextureType_DIFFUSE, Texture_Type::diffuse},
            {aiTextureType_SPECULAR, Texture_Type::specular},
            {aiTextureType_NORMALS, Texture_Type::normal},
            {aiTextureType_AMBIENT, Texture_Type::ambient},
        };

        for (auto const& [aitype, type]: types) {
            for (auto i = 0u; i < aimaterial->GetTextureCount(aitype); ++i) {
                aiString tex_relative_path;
                aimaterial->GetTexture(aitype,i, &tex_relative_path);
                
                //Reuse same textures
                auto iter = parsing_textures.find(tex_relative_path.C_Str());
                if (iter != parsing_textures.end()) {
                    textures.push_back(iter->second);
                }
                else {
                    auto upper_dir = path.upper_directory();
                    auto tex_path = Runtime_Path(upper_dir.relative_path() + "/" + tex_relative_path.C_Str());
                    auto texture = std::make_shared<Texture>(tex_path, type);
                    parsing_textures.emplace(tex_relative_path.C_Str(), texture);
                    textures.push_back(texture);
                }
            }
        }

        return std::make_shared<Phong_Material>(std::move(textures));
    }


    auto Parser::clean_parsing_cache()->void
    {
        parsing_transforms.clear();
        parsing_gl_draw_primitives.clear();
        parsing_render_primitives.clear();
        parsing_lights.clear();
        parsing_cameras.clear();
        parsing_textures.clear();
    }

}