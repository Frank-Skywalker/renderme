#include "renderme.hpp"

#include<core/log.hpp>

#include <GL/glew.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <cstdio>

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
        window = glfwCreateWindow(window_size.x, window_size.y, "renderme", NULL, NULL);
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
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
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

        if (ImGui::Begin("Config")) {
            ImGui::Checkbox("Show ImGUI demo window", &config.show_imgui_demo_window);
            if (ImGui::Button("load")) {
                parse_file("data/");
            }
            ImGui::Checkbox("raytrace", &config.raytrace);
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
		if (state != Renderme::State::ready) {
			return;
		}
		integrator->gl_draw(*scene);
	}

	auto Renderme::render() const noexcept->void
	{
		if(state!=Renderme::State::ready){
			return;
		}
		integrator->render(*scene);
	}



    //////Parsing//////
    auto Renderme::parse_file(std::string const& path)->void
    {
        parse_obj(path);
    }


    // loads a model with supported ASSIMP extensions from file
    auto Renderme::parse_obj(std::string const& path)->void
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        auto aiscene = importer.ReadFile(path,
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
            return;
        }

        // process ASSIMP's root node recursively
        parse_ainode(aiscene, aiscene->mRootNode);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    auto Renderme::parse_ainode(aiScene const* aiscene, aiNode const* ainode) -> void
    {
        // process each mesh located at the current node
        for (auto i = 0u; i < ainode->mNumMeshes; ++i) {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            auto aimesh = aiscene->mMeshes[ainode->mMeshes[i]];
            parse_aimesh(aiscene, aimesh);
        }

        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (auto i = 0u; i < ainode->mNumChildren; ++i) {
            parse_ainode(aiscene, ainode->mChildren[i]);
        }
    }

    auto Renderme::parse_aimesh(aiScene const* aiscene, aiMesh const* aimesh) -> void
    {
        // data to fill
        std::vector<Point3f> vertices;
        std::vector<Normal3f> normals;

        // walk through each of the mesh's vertices
        for (auto i = 0u; i < aimesh->mNumVertices; ++i) {
        }
    }


}