#include "renderme.hpp"
#include"parser.hpp"

#include<core/log.hpp>

#include <GL/glew.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <fstream>

namespace renderme
{
	namespace {
		//Believe me I don't wanna do this.
		//GLFW doesn't have a function to query scroll pos.
		//Only scroll callback is provided and it cannot access Renderme private data.
		bool scroll{ false };
		double scroll_xdelta;
		double scroll_ydelta;
	}

	Renderme::Renderme()
	{
		///////////////GLFW Init//////////////////////////
		// Setup window
		glfwSetErrorCallback(
			[](int error, char const* description) {
				//fprintf(stderr, "Glfw Error %d: %s\n", error, description);
				std::string info{ description };
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
		window = glfwCreateWindow(config.framebuffer_size.x, config.framebuffer_size.y, "renderme", NULL, NULL);
		if (window == nullptr) {
			glfwTerminate();
			return;
		}
		//Make the window and OpenGL's context current
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync

		//Callbacks
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xdelta, double ydelta) {
			scroll = true;
			scroll_xdelta = xdelta;
			scroll_ydelta = ydelta;
			});


		/////////////GLEW Init///////////////
		auto err = glewInit();
		if (err != GLEW_OK) {
			/* Problem: glewInit failed, something is seriously wrong. */
			//fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			std::string info{ (const char*)glewGetErrorString(err) };
			log(Status::fatal, info);
			return;
		}
		//fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		//std::string info{(const char*)glewGetString(GLEW_VERSION)};
		//log(Status::log, info);

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


		////////////OpengGL Configuration//////////////
		glEnable(GL_DEPTH_TEST);

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


	auto Renderme::parse_from_file(Runtime_Path path) -> void
	{
		///////////Data Members Init/////////////////
		try {
			std::ifstream f(path.full_path());
			nlohmann::json j = nlohmann::json::parse(f);

			config.framebuffer_size.x = j.at("width");
			config.framebuffer_size.y = j.at("height");
			glfwSetWindowSize(window, config.framebuffer_size.x, config.framebuffer_size.y);

			film = parse_film(j.at("film"));

			for (auto const& json : j.at("scenes")) {
				scenes.push_back(parse_scene(json));
			}

			for (auto const& json : j.at("cameras")) {
				cameras.push_back(parse_camera(json));
			}

			for (auto const& json : j.at("integrators")) {
				integrators.push_back(parse_integrator(json));
			}

			for (auto const& json : j.at("samplers")) {
				samplers.push_back(parse_sampler(json));
			}
		}
		catch (std::exception const& e) {
			log(Status::fatal, e.what());
		}

		if (film != nullptr)
			film->reset_resolution(config.framebuffer_size);
		for (auto& camera : cameras) {
			camera->reset_aspect(static_cast<float>(config.framebuffer_size.x) / static_cast<float>(config.framebuffer_size.y));
		}
		for (auto& sampler : samplers) {
			sampler->reset_sample_space(config.framebuffer_size);
		}
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
			//Poll for and process events
			glfwPollEvents();
			//glfwWaitEvents();

			update();

			if (config.enable_io) {
				process_io();
			}

			glClearColor(config.clear_color.x * config.clear_color.w, config.clear_color.y * config.clear_color.w, config.clear_color.z * config.clear_color.w, config.clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (config.gl_draw_mode == fill) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else if (config.gl_draw_mode == line) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glLineWidth(config.line_width_point_size);
			}
			else if (config.gl_draw_mode == point) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				glPointSize(config.line_width_point_size);
			}

			show_scene();
			show_imgui_menu();

			//Swap front and back buffers
			glfwSwapBuffers(window);
		}
	}

	auto Renderme::update()->void
	{
		//Update renderme state
		if (integrators.size() > 0 && scenes.size() > 0 && cameras.size() > 0 && samplers.size() > 0) {
			state = State::ready;
		}
		else {
			state = State::uninit;
		}

		//Update time
		double now_time = glfwGetTime();
		info.delta_time = now_time - info.time;
		info.time = now_time;

		// Update clear color to film
		if (film != nullptr) {
			film->set_clear_color(config.clear_color);
		}

		// Update film size and camera aspect with framebuffer size
		glm::ivec2 new_size;
		glfwGetFramebufferSize(window, &new_size.x, &new_size.y);
		if (new_size != config.framebuffer_size) {
			config.framebuffer_size = new_size;
			if (film != nullptr) {
				film->reset_resolution(config.framebuffer_size);
			}
			for (auto& camera : cameras) {
				camera->reset_aspect(static_cast<float>(config.framebuffer_size.x) / static_cast<float>(config.framebuffer_size.y));
			}
			for (auto& sampler : samplers) {
				sampler->reset_sample_space(config.framebuffer_size);
			}
		}

		//Update cursor position
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (info.first_frame) {
			info.cursor_xpos = xpos;
			info.cursor_ypos = ypos;
			info.first_frame = false;
		}
		info.cursor_xdelta = xpos - info.cursor_xpos;
		info.cursor_ydelta = ypos - info.cursor_ypos;
		info.cursor_xpos = xpos;
		info.cursor_ypos = ypos;

		//Update cursor scroll
		if (scroll) {
			info.scroll_xdelta = scroll_xdelta;
			info.scroll_ydelta = scroll_ydelta;
			scroll = false;
		}
		else {
			info.scroll_xdelta = 0;
			info.scroll_ydelta = 0;
		}
	}


	auto Renderme::process_io()->void
	{
		//Close
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		if (state != Renderme::State::ready) {
			return;
		}

		//Camera position movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameras[config.camera_index]->process_keyboard(Camera_Movement::forward, info.delta_time);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameras[config.camera_index]->process_keyboard(Camera_Movement::backward, info.delta_time);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameras[config.camera_index]->process_keyboard(Camera_Movement::left, info.delta_time);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameras[config.camera_index]->process_keyboard(Camera_Movement::right, info.delta_time);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			cameras[config.camera_index]->process_keyboard(Camera_Movement::up, info.delta_time);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			cameras[config.camera_index]->process_keyboard(Camera_Movement::down, info.delta_time);
		}

		//Caemra facing movement
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			cameras[config.camera_index]->process_cursor(info.cursor_xdelta, info.cursor_ydelta);
		}

		//Camera zoom in/out
		if (info.scroll_ydelta != 0) {
			cameras[config.camera_index]->process_scroll(info.scroll_ydelta);
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
			auto& io = ImGui::GetIO();
			ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Renderme Config")) {
				imgui_config();
			}

			if (ImGui::CollapsingHeader("Cameras Config")) {
				//for (auto i = 0u; i < cameras.size(); ++i) {
				//    std::string id = "Camera" + std::to_string(i);
				//    if (ImGui::TreeNode(id.c_str())) {
				//        cameras[i]->imgui_config();
				//        ImGui::TreePop();
				//    }
				//}
				cameras[config.camera_index]->imgui_config();
			}

			if (ImGui::CollapsingHeader("Integrators Config")) {
				for (auto i = 0u; i < integrators.size(); ++i) {
					std::string id = "Integrator" + std::to_string(i);
					if (ImGui::TreeNode(id.c_str())) {
						integrators[i]->imgui_config();
						ImGui::TreePop();
					}
				}
			}

			if (ImGui::CollapsingHeader("Scenes Config")) {
				for (auto i = 0u; i < scenes.size(); ++i) {
					std::string id = "Scene" + std::to_string(i);
					if (ImGui::TreeNode(id.c_str())) {
						scenes[i]->imgui_config();
						ImGui::TreePop();
					}
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
		assert(config.integrator_index < integrators.size() && config.scene_index < scenes.size() && config.sampler_index < samplers.size());
		integrators[config.integrator_index]->gl_draw(*scenes[config.scene_index], cameras[config.camera_index].get());
	}

	auto Renderme::render() const noexcept->void
	{
		assert(config.integrator_index < integrators.size() && config.scene_index < scenes.size() && config.sampler_index < samplers.size());
		integrators[config.integrator_index]->render(*scenes[config.scene_index], cameras[config.camera_index].get(), samplers[config.sampler_index].get(), film.get());
		film->gl_display();
	}

	auto Renderme::imgui_config()->void
	{
		ImGui::Checkbox("Path Tracing", &config.raytrace);
		ImGui::Checkbox("Enable IO", &config.enable_io);
		if (scenes.size() > 0) {
			ImGui::SliderInt("Scene", (int*)&config.scene_index, 0, scenes.size() - 1);
		}
		if (integrators.size() > 0) {
			ImGui::SliderInt("Integrator", (int*)&config.integrator_index, 0, integrators.size() - 1);
		}
		if (samplers.size() > 0) {
			ImGui::SliderInt("Sampler", (int*)&config.sampler_index, 0, samplers.size() - 1);
		}
		//if (cameras.size() > 0) {
		//    ImGui::SliderInt("Camera", (int*)&config.camera_index, 0, cameras.size() - 1);
		//}
		if (cameras.size() > config.scene_index) {
			config.camera_index = config.scene_index;
		}

		//ImGui::Checkbox("Show ImGUI demo window", &config.show_imgui_demo_window);
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(config.clear_color));

		if (ImGui::BeginCombo("GL Draw Mode", std::to_string(config.gl_draw_mode).c_str())) {
			for (auto mode = GL_Draw_Mode::fill; mode <= GL_Draw_Mode::point; mode = (GL_Draw_Mode)(mode + 1)) {
				const bool is_selected = (mode == config.gl_draw_mode);
				if (ImGui::Selectable(std::to_string(mode).c_str(), is_selected)) {
					config.gl_draw_mode = mode;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SliderFloat("Line Width/Point Size", &config.line_width_point_size, 0.0f, 10.0f);

	}




}
