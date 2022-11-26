#pragma once
#include <core/util.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

namespace renderme::glviewer
{
	struct GL_Viewer final: util::Singleton<GL_Viewer>
	{
		GL_Viewer();
		~GL_Viewer();
		auto main_loop()->void;


	private:
		auto show_imgui_menu()->void;
		auto render_scene()->void;

	private:
		GLFWwindow* window;
		ImGuiIO* io;

		//Metadata
		const ImVec2 window_size{1280, 720};
		const ImVec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
	};
}