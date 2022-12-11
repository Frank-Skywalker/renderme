#pragma once

#include<core/geometry.hpp>

#include <string>
namespace renderme
{
	enum struct Shader_Type
	{
		vertex,
		fragment
	};

	struct Shader
	{
		Shader(std::string const& vertex_path, std::string const& fragment_path);
		auto id() const noexcept ->unsigned int;
		auto use() const noexcept -> void;
		auto set_uniform_bool(std::string const& name, bool value) const noexcept ->void;
		auto set_uniform_int(std::string const& name, int value) const noexcept ->void;
		auto set_uniform_float(std::string const& name, float value) const noexcept ->void;
		auto set_uniform_vec2(std::string const& name,  Vector2f value) const noexcept ->void;
		auto set_uniform_vec2(std::string const& name, float x, float y) const noexcept ->void;
		auto set_uniform_vec3(std::string const& name, Vector3f value) const noexcept ->void;
		auto set_uniform_vec3(std::string const& name, float x, float y, float z) const noexcept ->void;
		auto set_uniform_vec4(std::string const& name, float x, Vector4f value) const noexcept ->void;
		auto set_uniform_vec4(std::string const& name, float x, float y, float z, float w) const noexcept ->void;
		//auto set_uniform_mat2(std::string const& name, bool value) const noexcept ->void;
		//auto set_uniform_mat3(std::string const& name, bool value) const noexcept ->void;
		//auto set_uniform_mat4(std::string const& name, bool value) const noexcept ->void;

	private:
		unsigned int _id;
	};
}