#include "phong.hpp"


namespace renderme
{

	Phong_Material::Phong_Material(std::string name)
		:Material{std::move(name)}
	{
		textures.fill(nullptr);
	}

	auto Phong_Material::gl_draw(Shader const& shader) const noexcept ->void
	{
		unsigned int diffuse_cnt = 0;
		unsigned int specular_cnt = 0;
		unsigned int normal_cnt = 0;
		unsigned int ambient_cnt = 0;

		unsigned int i = 0;
		for (auto const& texture : textures) {
			if (texture != nullptr) {
				if (texture->type == Texture_Type::diffuse) {
					texture->gl_draw(shader, i++, diffuse_cnt++);
				}
				else if (texture->type == Texture_Type::specular) {
					texture->gl_draw(shader, i++, specular_cnt++);
				}
				else if (texture->type == Texture_Type::normal) {
					texture->gl_draw(shader, i++, normal_cnt++);
				}
				else if (texture->type == Texture_Type::ambient) {
					texture->gl_draw(shader, i++, ambient_cnt++);
				}
			}
		}
	}

	auto Phong_Material::ambient(glm::vec2 uv) const noexcept ->glm::vec3
	{
		return diffuse(uv) * glm::vec3(0.1f, 0.1f, 0.1f);
	}

	auto Phong_Material::diffuse(glm::vec2 uv) const noexcept ->glm::vec3
	{
		if (textures[size_t(Texture_Type::diffuse)] != nullptr) {
			return textures[size_t(Texture_Type::diffuse)]->color_of(uv);
		}
		return _diffuse;
	}

	auto Phong_Material::specular(glm::vec2 uv) const noexcept ->glm::vec3
	{
		if (textures[size_t(Texture_Type::specular)] != nullptr) {
			return textures[size_t(Texture_Type::diffuse)]->color_of(uv);
		}
		return _specular;
	}

	auto Phong_Material::specular_exponent(glm::vec2 uv) const noexcept ->float
	{
		return _specular_exponent;
	}

	auto Phong_Material::transparent(glm::vec2 uv) const noexcept ->glm::vec3
	{
		return _transparent;
	}

	auto Phong_Material::refraction_index(glm::vec2 uv) const noexcept ->float
	{
		return _refraction_index;
	}

	auto Phong_Material::emition(glm::vec2 uv) const noexcept ->glm::vec3
	{
		return _emition;
	}

}
