#include "texture.hpp"
#include"log.hpp"

#include<stb/stb_image.h>
#include<GL/glew.h>

namespace renderme
{
	Texture::Texture(Runtime_Path path, Texture_Type type)
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
		}
		else if (num_components == 3) {
			format = GL_RGB;
		}
		else if (num_components == 4) {
			format = GL_RGBA;
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

		stbi_image_free(data);
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

}