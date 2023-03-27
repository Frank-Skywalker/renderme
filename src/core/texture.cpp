#include "texture.hpp"
#include"log.hpp"

#include<stb/stb_image.h>
#include<GL/glew.h>

namespace renderme
{
	Texture::Texture(Texture_Type type, Runtime_Path path)
		:type{type}
	{
		//Texture loading
		auto data = stbi_load(path.full_path().c_str(), &width, &height, &num_components, 0);
		if (!data) {
			stbi_image_free(data);
			log(Status::fatal, "texture load error");
		}

		if (num_components == 1) {
			format = GL_RED;
			log(Status::fatal, "unsuported format");
		}
		else if (num_components == 3) {
			format = GL_RGB;
		}
		else if (num_components == 4) {
			format = GL_RGBA;
			log(Status::fatal, "unsuported format");
		}
		else {
			log(Status::fatal, "unsuported format");
		}

		//Load texture into OpenGL
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		pixels = std::make_unique<glm::vec3[]>(width * height);
		auto ptr = static_cast<unsigned char*>(data);
		for (auto i = 0; i < width * height; ++i) {
			pixels[i].x = float(ptr[3 * i]) / 255.0f;
			pixels[i].y = float(ptr[3 * i + 1]) / 255.0f;
			pixels[i].z = float(ptr[3 * i + 2]) / 255.0f;
		}

		stbi_image_free(data);
	}

	Texture::Texture(Texture_Type type, glm::vec3 color)
		:type{type}
	{
		format = GL_RGB;
		num_components = 3;
		width = 1;
		height = 1;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, glm::value_ptr(color));

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	auto Texture::gl_draw(Shader const& shader, unsigned int i, unsigned int type_cnt) const noexcept -> void
	{
		// active proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name;
		if (type == Texture_Type::diffuse) {
			name = "texture_diffuse";
		}
		else if (type == Texture_Type::specular) {
			name = "texture_specular";
		}
		else if (type == Texture_Type::normal) {
			name = "texture_normal";
		}
		else if (type == Texture_Type::ambient) {
			name = "texture_ambient";
		}

		// now set the sampler to the correct texture unit
		shader.set_uniform_int(name + std::to_string(type_cnt), i);
		//glUniform1i(glGetUniformLocation(shader.id(), (name + std::to_string(type_cnt)).c_str()), i);

		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, id);

		// always good practice to set everything back to defaults once configured.
		//glActiveTexture(GL_TEXTURE0);
	}


	auto Texture::color_of(glm::vec2 uv) const noexcept -> glm::vec3
	{
		auto clamp_uv = glm::fclamp(uv, glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f));
		int index = clamp_uv.y * height * width + clamp_uv.x * width - 1;
		if (index >= height * width) {
			index = height * width - 1;
		}

		return pixels[index];
	}


}