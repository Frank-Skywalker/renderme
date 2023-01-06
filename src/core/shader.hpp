#pragma once
#include "runtime-path.hpp"

#include<glm/glm-all.hpp>

#include <string>
namespace renderme
{
	enum struct Shader_Type
	{
		vertex,
		fragment
	};

	struct Shader final
	{
		struct Config
		{
			std::string vertex_path;
			std::string frag_path;
		};


		Shader(Runtime_Path vertex_path, Runtime_Path fragment_path);
		~Shader();

		auto use()->void;
		auto imgui_config()->void;

		auto id() const noexcept ->unsigned int;
		auto use() const noexcept -> void;
		auto set_uniform_bool(std::string const& name, bool value) const noexcept ->void;
		auto set_uniform_int(std::string const& name, int value) const noexcept ->void;
		auto set_uniform_float(std::string const& name, float value) const noexcept ->void;
		auto set_uniform_vec2(std::string const& name,  glm::vec2 value) const noexcept ->void;
		auto set_uniform_vec2(std::string const& name, float x, float y) const noexcept ->void;
		auto set_uniform_vec3(std::string const& name, glm::vec3 value) const noexcept ->void;
		auto set_uniform_vec3(std::string const& name, float x, float y, float z) const noexcept ->void;
		auto set_uniform_vec4(std::string const& name, float x, glm::vec4 value) const noexcept ->void;
		auto set_uniform_vec4(std::string const& name, float x, float y, float z, float w) const noexcept ->void;
		auto set_uniform_mat4(std::string const& name, glm::mat4 value) const noexcept ->void;

	private:
		unsigned int _id;
		Config config;
	};
}