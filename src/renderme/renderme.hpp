#pragma once
#include<core/util.hpp>
#include<core/scene.hpp>
#include<core/integrator.hpp>
#include<core/file-system.hpp>

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
		std::vector<Scene> scenes;
		std::vector<std::unique_ptr<Integrator>> integrators;

		
		//////Parsing//////
	private:
		auto parse_file(Runtime_Path path)->void;
		auto parse_obj(Runtime_Path path)->bool;
		auto parse_ainode(aiScene const* aiscene, aiNode const* ainode) -> bool;
		auto parse_aimesh(aiScene const* aiscene, aiMesh const* aimesh) -> bool;
		auto parse_aimaterial(aiScene const* aiscene, aiMaterial const* aimaterial) -> bool;

	private:
		//Transform must not move in memory, so use unique_ptr to store Transforms on stack
		std::vector<std::unique_ptr<Transform>> parsing_transforms;
		std::vector<std::unique_ptr<Primitive>> parsing_primitives;
		std::vector<std::unique_ptr<Light>> parsing_lights;
		std::vector<std::unique_ptr<Camera>> parsing_cameras;
	};

}