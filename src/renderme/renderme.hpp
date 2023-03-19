#pragma once
#include<core/util.hpp>
#include<core/scene.hpp>
#include<core/integrator.hpp>
#include<core/film.hpp>

//explicitly disable inclusion of the development environment header of glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


#include<string>
#include<memory>


#define MAX_FILE_NAME_LENGTH 1000

namespace renderme
{
	struct Renderme: Singleton<Renderme>, Non_Transferable
	{
		enum struct State
		{
			uninit,
			ready
		};

		enum GL_Draw_Mode
		{
			fill = 0,
			line,
			point
		};

		struct Config
		{
			GL_Draw_Mode gl_draw_mode{GL_Draw_Mode::fill};
			float line_width_point_size;

			bool raytrace{false};
			bool show_imgui_demo_window{false};
			bool enable_io{true};
			unsigned int scene_index{0};
			unsigned int integrator_index{0};
			unsigned int camera_index{0};

			//Metadata
			glm::ivec2 framebuffer_size{1280, 720};
			glm::vec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
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
		Runtime_Path app_path{ "/config/app.json" };
		State state{State::uninit};
		std::vector<std::unique_ptr<Scene>> scenes;
		std::vector<std::unique_ptr<Integrator>> integrators;
		std::vector<std::unique_ptr<Camera>> cameras;
		std::unique_ptr<Film> film;
	};

}
