#include "parser.hpp"

#include<shapes/triangle.hpp>
#include<cameras/perspective.hpp>
#include<materials/phong.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


namespace renderme
{

    auto Parser::parse_integrator(Runtime_Path path)->std::unique_ptr<Integrator>
    {
        if (path.empty()) {
            log(Status::error, "empty path");
            clean_parsing_cache();
            return nullptr;
        }

        auto create_new_integrator = [&] () -> std::unique_ptr<Sample_Integrator> {
            auto camera = std::make_unique<Perspective_Camera>();
            auto shader = std::make_unique<Shader>("src/shaders/phong.vert.glsl", "src/shaders/phong.frag.glsl");
            return std::make_unique<Sample_Integrator>(
                    std::move(camera),
                    std::move(shader)
                    );
        };

        auto integrator = create_new_integrator();
        clean_parsing_cache();

        return integrator;
    }


    auto Parser::parse_scene(Runtime_Path path)->std::unique_ptr<Scene>
    {
        if (!parse_obj(path)) {
            clean_parsing_cache();
            return nullptr;
        }

        auto create_new_scene = [&] ()->std::unique_ptr<Scene> {
            //Create a new Scene and Integrator
            std::vector<std::unique_ptr<Transform>> tmp_transforms;
            std::vector<std::unique_ptr<Primitive>> tmp_primitives;
            std::vector<std::unique_ptr<Light>> tmp_lights;
            std::vector<std::unique_ptr<Camera>> tmp_cameras;

            for (auto& transform : parsing_transforms) {
                tmp_transforms.push_back(std::move(transform));
            }
            for (auto& primitive : parsing_primitives) {
                tmp_primitives.push_back(std::move(primitive));
            }
            for (auto& light : parsing_lights) {
                tmp_lights.push_back(std::move(light));
            }
            for (auto& camera : parsing_cameras) {
                tmp_cameras.push_back(std::move(camera));
            }
            return std::make_unique<Scene>("test", std::move(tmp_transforms), std::move(tmp_primitives), std::move(tmp_lights));
        };

        auto scene = create_new_scene();
        clean_parsing_cache();

        return scene;
    }


    // loads a model with supported ASSIMP extensions from file
    auto Parser::parse_obj(Runtime_Path path)->bool
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        auto aiscene = importer.ReadFile(path.path(),
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
        return parse_ainode(aiscene, aiscene->mRootNode);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    auto Parser::parse_ainode(aiScene const* aiscene, aiNode const* ainode) -> bool
    {
        // process each mesh located at the current node
        for (auto i = 0u; i < ainode->mNumMeshes; ++i) {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            auto aimesh = aiscene->mMeshes[ainode->mMeshes[i]];
            if (!parse_aimesh(aiscene, aimesh)) {
                return false;
            }
        }

        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (auto i = 0u; i < ainode->mNumChildren; ++i) {
            if (!parse_ainode(aiscene, ainode->mChildren[i])) {
                return false;
            }
        }
        return true;
    }

    auto Parser::parse_aimesh(aiScene const* aiscene, aiMesh const* aimesh) -> bool
    {
        // data to fill
        std::vector<Point3f> positions;
        std::vector<Normal3f> normals;
        std::vector<Point2f> uvs;
        std::vector<Vector3f> tangents;
        std::vector<Vector3f> bitangents;
        std::vector<Point3ui> faces;


        // walk through each of the mesh's vertices
        for (auto i = 0u; i < aimesh->mNumVertices; ++i) {
            //positions
            positions.push_back(Point3f(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z));
            //normals
            if (aimesh->HasNormals()) {
                normals.push_back(Normal3f(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z));
            } else {
                //normals.push_back(Normal3f());
                log(Status::error, "obj has no normal");
                return false;
            }
            //texture coordinates
            if (aimesh->mTextureCoords[0]) {
                //uv
                uvs.push_back(Point2f(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y));
                //tangent
                tangents.push_back(Vector3f(aimesh->mTangents[i].x, aimesh->mTangents[i].y, aimesh->mTangents[i].z));
                //bitangent
                bitangents.push_back(Vector3f(aimesh->mBitangents[i].x, aimesh->mBitangents[i].y, aimesh->mBitangents[i].z));
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
            faces.push_back(Point3ui(aiface.mIndices[0], aiface.mIndices[1], aiface.mIndices[2]));
        }

        //process materials
        auto aimaterial = aiscene->mMaterials[aimesh->mMaterialIndex];
        parse_aimaterial(aiscene, aimaterial);


        //Store mesh back to primitives
        auto triangle_mesh = std::make_unique<Triangle_Mesh>(
            nullptr, nullptr,
            std::move(faces), std::move(positions), std::move(normals),
            std::move(uvs), std::move(tangents), std::move(bitangents)
            );
        auto triangles = triangle_mesh->create_triangles();
        auto triangle_mesh_primitive = std::make_unique<Shape_Primitive>(std::move(triangle_mesh), std::make_unique<Phong_Material>());
        parsing_primitives.push_back(std::move(triangle_mesh_primitive));

        for (auto& tri : triangles) {
            auto triangle = std::make_unique<Triangle>(std::move(tri));
            auto triangle_primitive = std::make_unique<Shape_Primitive>(std::move(triangle), std::make_unique<Phong_Material>());
            parsing_primitives.push_back(std::move(triangle_primitive));
        }

        return true;
    }


    auto Parser::parse_aimaterial(aiScene const* aiscene, aiMaterial const* aimaterial) -> bool
    {
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        return true;
    }


    auto Parser::clean_parsing_cache()->void
    {
        parsing_transforms.clear();
        parsing_primitives.clear();
        parsing_lights.clear();
        parsing_cameras.clear();
    }

}