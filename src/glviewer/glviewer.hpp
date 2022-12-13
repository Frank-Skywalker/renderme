#pragma once
#include <core/util.hpp>
#include <core/scene.hpp>

//explicitly disable inclusion of the development environment header of glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

namespace renderme
{
	struct GL_Viewer final: Singleton<GL_Viewer>
	{
		GL_Viewer();
		~GL_Viewer();
		auto main_loop()->void;

	private:
		auto show_imgui_menu()->void;
		auto render_scene()->void;

	private:
		std::vector<Scene> scenes;

		GLFWwindow* window;
		ImGuiIO* io;

		//Metadata
		const ImVec2 window_size{1280, 720};
		const ImVec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
	};
}