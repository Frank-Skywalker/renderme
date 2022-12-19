#include "renderme.hpp"

#include<core/log.hpp>

#include <shapes/triangle.hpp>
#include <cameras/perspective-camera.hpp>

#include <GL/glew.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
namespace renderme
{
    Renderme::Renderme()
    {
        ///////////////GLFW Init//////////////////////////
        // Setup window
        glfwSetErrorCallback(
            [] (int error, char const* description) {
                //fprintf(stderr, "Glfw Error %d: %s\n", error, description);
                std::string info{description};
                log(Status::fatal, info);
            }
        );

        //Initialize the library
        if (!glfwInit())
            return;

        // Decide GL+GLSL versions
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";

        // require a minimum OpenGL context version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

        //Create a windowed mode window and its OpenGL context
        window = glfwCreateWindow(config.window_size.x, config.window_size.y, "renderme", NULL, NULL);
        if (window == nullptr) {
            glfwTerminate();
            return;
        }
        //Make the window and OpenGL's context current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        /////////////GLEW Init///////////////
        auto err = glewInit();
        if (err != GLEW_OK) {
            /* Problem: glewInit failed, something is seriously wrong. */
            //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
            std::string info{(const char*)glewGetErrorString(err)};
            log(Status::fatal, info);
            return;
        }
        //fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
        std::string info{(const char*)glewGetString(GLEW_VERSION)};
        log(Status::log, info);

        ///////////ImGui Init//////////////

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO();
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }


    Renderme::~Renderme()
    {
        ////////////ImGui Cleanup///////////
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();


        ////////////GLFW Cleanup///////////
        glfwDestroyWindow(window);
        glfwTerminate();
    }


    //////Frontend//////
    auto Renderme::main_loop()->void
    {
        while (!glfwWindowShouldClose(window)) {

            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            glfwPollEvents();
            //glfwWaitEvents();
            //Poll for and process events

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(config.clear_color.x * config.clear_color.w, config.clear_color.y * config.clear_color.w, config.clear_color.z * config.clear_color.w, config.clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);

            double time = glfwGetTime();

            show_scene();
            show_imgui_menu();

            //Swap front and back buffers
            glfwSwapBuffers(window);
        }
    }

    auto Renderme::show_imgui_menu()->void
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Imgui content
        if (config.show_imgui_demo_window) {
            ImGui::ShowDemoWindow(&config.show_imgui_demo_window);
        }

        if (ImGui::Begin("CONFIG")) {
            
            if (ImGui::CollapsingHeader("Renderme Config")) {
                imgui_config();
            }

            if (ImGui::CollapsingHeader("Integrator Config")) {
                if (config.integrator_index < integrators.size()) {
                    integrators[config.integrator_index]->imgui_config();
                }
            }

            if (ImGui::CollapsingHeader("Scene Config")) {
                if (config.scene_index < scenes.size()) {
                    scenes[config.scene_index].imgui_config();
                }
            }
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    auto Renderme::show_scene()->void
    {
        if (state != Renderme::State::ready) {
            return;
        }

        if (config.raytrace) {
            render();
        }
        else {
            gl_draw();
        }

    }

    //////Backend//////
	auto Renderme::gl_draw() const noexcept->void
	{
        assert(config.integrator_index < integrators.size() && config.scene_index < scenes.size());
		integrators[config.integrator_index]->gl_draw(scenes[config.scene_index]);
	}

	auto Renderme::render() const noexcept->void
	{
        assert(config.integrator_index < integrators.size() && config.scene_index < scenes.size());
        integrators[config.integrator_index]->render(scenes[config.scene_index]);
	}

    auto Renderme::imgui_config()->void
    {
        ImGui::Checkbox("Show ImGUI demo window", &config.show_imgui_demo_window);
        if (ImGui::Button("load")) {
            parse_file(config.file_path);
        }
        ImGui::SameLine();
        ImGui::InputText("load path", config.file_path, MAX_FILE_NAME_LENGTH);

        ImGui::Checkbox("raytrace", &config.raytrace);
        if (scenes.size() > 0) {
            ImGui::SliderInt("Scene", (int*)&config.scene_index, 0, scenes.size() - 1);
        }
        if (integrators.size() > 0) {
            ImGui::SliderInt("Integrator", (int*)&config.integrator_index, 0, integrators.size() - 1);
        }
    }


    //////Parsing//////
    auto Renderme::parse_file(Runtime_Path path)->void
    {
        auto clean_parsing_cache = [&] () {
            parsing_transforms.clear();
            parsing_primitives.clear();
            parsing_lights.clear();
            parsing_cameras.clear();
        };

        auto create_new_scene = [&] () {
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
            scenes.push_back(
                Scene("test", std::move(tmp_transforms), std::move(tmp_primitives), std::move(tmp_lights))
            );
        };

        auto create_new_integrator = [&] () {
            auto camera = std::make_unique<Perspective_Camera>();
            auto shader = std::make_unique<Shader>("src/shaders/temp.vert.glsl", "src/shaders/temp.frag.glsl");
            integrators.push_back(
                std::make_unique<Sample_Integrator>(
                    std::move(camera),
                    std::move(shader)
                )
            );
        };

        if (parse_obj(path)) {
            create_new_scene();
        }

        if (true) {
            create_new_integrator();
        }

        clean_parsing_cache();

        if (scenes.size() > 0 && integrators.size() > 0) {
            state = State::ready;
        }
    }


    // loads a model with supported ASSIMP extensions from file
    auto Renderme::parse_obj(Runtime_Path path)->bool
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
    auto Renderme::parse_ainode(aiScene const* aiscene, aiNode const* ainode) -> bool
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

    auto Renderme::parse_aimesh(aiScene const* aiscene, aiMesh const* aimesh) -> bool
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
            } 
            else {
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
            faces.push_back(Point3ui(aiface.mIndices[0], aiface.mIndices[1], aiface.mIndices[2]));
        }

        //process materials
        auto aimaterial = aiscene->mMaterials[aimesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN


        //Store mesh back to primitives
        auto triangle_mesh = std::make_unique<Triangle_Mesh>(
            nullptr, nullptr,
            std::move(faces), std::move(positions), std::move(normals),
            std::move(uvs), std::move(tangents), std::move(bitangents)
            );
        auto triangles =triangle_mesh->create_triangles();
        auto triangle_mesh_primitive = std::make_unique<Shape_Primitive>(std::move(triangle_mesh));
        parsing_primitives.push_back(std::move(triangle_mesh_primitive));

        for (auto& tri : triangles) {
            auto triangle = std::make_unique<Triangle>(std::move(tri));
            auto triangle_primitive = std::make_unique<Shape_Primitive>(std::move(triangle));
            parsing_primitives.push_back(std::move(triangle_primitive));
        }

        return true;
    }

}