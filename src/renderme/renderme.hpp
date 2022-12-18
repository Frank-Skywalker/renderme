#pragma once
#include<core/util.hpp>
#include<core/scene.hpp>
#include<core/integrator.hpp>

//explicitly disable inclusion of the development environment header of glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <assimp/scene.h>

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
			char file_path[MAX_FILE_NAME_LENGTH];
			bool raytrace{false};
			bool show_imgui_demo_window{false};
			unsigned int scene_index;
			unsigned int integrator_index;

			//Metadata
			const ImVec2 window_size{1280, 720};
			const ImVec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
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
		Config config;


		//////Backend//////
	private:
		auto gl_draw() const noexcept->void;
		auto render() const noexcept->void;
	private:
		State state{State::uninit};
		std::vector<Scene> scenes;
		std::vector<Integrator> integrators;

		//////Parsing//////
	private:
		auto parse_file()->void;
	    auto parse_obj(std::string const& path)->bool;
		auto parse_ainode(aiScene const* aiscene, aiNode const* ainode) -> bool;
		auto parse_aimesh(aiScene const* aiscene, aiMesh const* aimesh) -> bool;

	private:
		std::vector<std::unique_ptr<Transform>> parsing_transforms;
		std::vector<std::unique_ptr<Primitive>> parsing_primitives;
		std::vector<std::unique_ptr<Light>> parsing_lights;
		std::vector<std::unique_ptr<Camera>> parsing_cameras;
	};

}