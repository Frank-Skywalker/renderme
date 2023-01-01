#include "phong.hpp"


namespace renderme
{

	Phong_Material::Phong_Material(std::vector<std::shared_ptr<Texture>> texs)
		:textures{std::move(texs)}
	{

	}

	auto Phong_Material::gl_draw(Shader const& shader) const noexcept ->void
	{
		unsigned int diffuse_cnt = 0;
		unsigned int specular_cnt = 0;
		unsigned int normal_cnt = 0;
		unsigned int ambient_cnt = 0;

		unsigned int i = 0;
		for (auto const& texture : textures) {
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