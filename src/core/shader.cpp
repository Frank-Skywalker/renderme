#include "shader.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include<GL/glew.h>

namespace renderme
{
	Shader::Shader(Runtime_Path vertex_path, Runtime_Path fragment_path)
	{
		auto read_shader_from_file = [] (std::string const& shader_path)-> std::string {
			std::stringstream shader_stream;
			try {
				std::ifstream shader_file;
				// ensure ifstream objects can throw exceptions:
				shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				// open files
				shader_file.open(shader_path);
				// read file's buffer contents into streams
				shader_stream << shader_file.rdbuf();
				// close file handlers
				shader_file.close();
			}
			catch (std::ifstream::failure const& e) {
				std::cerr << "file read error" << e.what() << std::endl;
				exit(0);
			}
			return shader_stream.str();
		};

		// retrieve the shader source code from file
		auto vshader_code=read_shader_from_file(vertex_path.path());
		auto fshader_code=read_shader_from_file(fragment_path.path());

		
		auto compile_shader = [] (Shader_Type type, std::string shader_code) -> unsigned int {

			auto check_compile_error = [] (unsigned int shader, Shader_Type type)->void {
				int success;
				char infolog[1024];
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, 1024, nullptr, infolog);
					std::cerr << "error: " << infolog << std::endl;
					exit(0);
				}
			};

			auto vsc = shader_code.c_str();
			unsigned int shader;
			switch (type) {
				case Shader_Type::vertex:
					shader = glCreateShader(GL_VERTEX_SHADER);
					break;
				case Shader_Type::fragment:
					shader = glCreateShader(GL_VERTEX_SHADER);
					break;
			}
			glShaderSource(shader, 1, &vsc, nullptr);
			glCompileShader(shader);
			check_compile_error(shader, type);
			return shader;
		};

		// compile shaders
		auto vshader = compile_shader(Shader_Type::vertex, std::move(vshader_code));
		auto fshader = compile_shader(Shader_Type::fragment, std::move(fshader_code));


		auto check_link_error= [](unsigned int program)->void {
			int success;
			char infolog[1024];
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(program, 1024, nullptr, infolog);
				std::cerr << "error: " << infolog << std::endl;
				exit(0);
			}
		};

		// link shader Program
		_id = glCreateProgram();
		glAttachShader(_id, vshader);
		glAttachShader(_id, fshader);
		glLinkProgram(_id);
		check_link_error(_id);

		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vshader);
		glDeleteShader(fshader);

	}

	Shader::~Shader()
	{
		glDeleteProgram(_id);
	}


	auto Shader::imgui_config()->void
	{
	}


	auto Shader::id() const noexcept ->unsigned int
	{
		return _id;
	}

	auto Shader::use() const noexcept ->void
	{
		glUseProgram(_id);
	}

	auto Shader::set_uniform_bool(std::string const& name, bool value) const noexcept ->void
	{
		glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
	}
	auto Shader::set_uniform_int(std::string const& name, int value) const noexcept ->void
	{
		glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
	}
	auto Shader::set_uniform_float(std::string const& name, float value) const noexcept ->void
	{
		glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
	}
	auto Shader::set_uniform_vec2(std::string const& name, Vector2f value) const noexcept ->void
	{
		glUniform2f(glGetUniformLocation(_id, name.c_str()), static_cast<float>(value.x), static_cast<float>(value.y) );
	}
	auto Shader::set_uniform_vec2(std::string const& name, float x, float y) const noexcept ->void
	{
		glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
	}
	auto Shader::set_uniform_vec3(std::string const& name, Vector3f value) const noexcept ->void
	{
		glUniform3f(glGetUniformLocation(_id, name.c_str()), static_cast<float>(value.x), static_cast<float>(value.y), static_cast<float>(value.z));
	}
	auto Shader::set_uniform_vec3(std::string const& name, float x, float y, float z) const noexcept ->void
	{
		glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
	}
	auto Shader::set_uniform_vec4(std::string const& name, float x, Vector4f value) const noexcept ->void
	{
		glUniform4f(glGetUniformLocation(_id, name.c_str()), static_cast<float>(value.x), static_cast<float>(value.y), static_cast<float>(value.z), static_cast<float>(value.w));
	}
	auto Shader::set_uniform_vec4(std::string const& name, float x, float y, float z, float w) const noexcept ->void
	{
		glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
	}

}