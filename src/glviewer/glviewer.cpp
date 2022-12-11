#include "glviewer.hpp"
#include <GL/glew.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <cstdio>
namespace renderme
{

    GL_Viewer::GL_Viewer()
    {
        ///////////////GLFW Init//////////////////////////
        // Setup window
        glfwSetErrorCallback(
            [] (int error, char const* description) {
                fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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
        if (err!=GLEW_OK) {
            /* Problem: glewInit failed, something is seriously wrong. */
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
            return;
        }
        fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


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


    GL_Viewer::~GL_Viewer()
    {
        ////////////ImGui Cleanup///////////
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();


        ////////////GLFW Cleanup///////////
        glfwDestroyWindow(window);
        glfwTerminate();
    }


    auto GL_Viewer::main_loop()->void
    {
        while (!glfwWindowShouldClose(window)) {

            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            //glfwPollEvents();
            glfwWaitEvents();
            //Poll for and process events

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);

            double time = glfwGetTime();

            render_scene();
            show_imgui_menu();

            //Swap front and back buffers
            glfwSwapBuffers(window);



        }
    }

    auto GL_Viewer::show_imgui_menu()->void
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Imgui content
        bool show=true;
        ImGui::ShowDemoWindow(&show);

        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    auto GL_Viewer::render_scene()->void
    {
        //GLuint vertex;
        //glGenBuffers(1, &vertex);
    }


}