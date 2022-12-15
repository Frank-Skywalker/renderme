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
			std::string file_path;
			bool raytrace{false};
			bool show_imgui_demo_window{false};
		};

		Renderme();
		~Renderme();


		//////Frontend//////
	public:
		auto main_loop()->void;
	private:
		auto show_imgui_menu()->void;
		auto show_scene()->void;

	private:
		GLFWwindow* window;
		ImGuiIO* io;
		//Metadata
		const ImVec2 window_size{1280, 720};
		const ImVec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
		Config config;


		//////Backend//////
	private:
		auto parse_obj(std::string const& path)->void;
		auto parse_from_file(std::string const& path)->void;
		auto gl_draw() const noexcept->void;
		auto render() const noexcept->void;

	private:
		State state{State::uninit};
		//std::vector<Scene> scenes;
		std::unique_ptr<Scene> scene;
		//std::vector<Integrator> integrators;
		std::unique_ptr<Integrator> integrator;

	};

}