#pragma once
#include<core/util.hpp>
#include<core/scene.hpp>
#include<core/integrator.hpp>

//explicitly disable inclusion of the development environment header of glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


#include<string>
#include<memory>


#define MAX_FILE_NAME_LENGTH 1000

namespace renderme
{
	struct Renderme: Singleton<Renderme>
	{
		enum struct State
		{
			uninit,
			ready
		};

		struct Config
		{
			char scene_path[MAX_FILE_NAME_LENGTH];
			char integrator_path[MAX_FILE_NAME_LENGTH];
			bool raytrace{false};
			bool show_imgui_demo_window{false};
			bool enable_io{false};
			unsigned int scene_index;
			unsigned int integrator_index;

			//Metadata
			ImVec2 window_size{1280, 720};
			ImVec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
		};

		struct Info
		{
			bool first_frame{true};
			double time{0};
			double delta_time{0};

			double cursor_xpos{0};
			double cursor_ypos{0};
			double cursor_xdelta{0};
			double cursor_ydelta{0};

			double scroll_xdelta{0};
			double scroll_ydelta{0};
		};

		Renderme();
		~Renderme();


		//////Frontend//////
	public:
		auto main_loop()->void;
	private:
		auto update()->void;
		auto process_io()->void;
		auto show_imgui_menu()->void;
		auto show_scene()->void;
	private:
		GLFWwindow* window;
		ImGuiIO* io;
		Config config;
		Info info;


		//////Backend//////
	private:
		auto gl_draw() const noexcept->void;
		auto render() const noexcept->void;
		auto imgui_config()->void;
	private:
		State state{State::uninit};
		std::vector<std::unique_ptr<Scene>> scenes;
		std::vector<std::unique_ptr<Integrator>> integrators;

	};

}