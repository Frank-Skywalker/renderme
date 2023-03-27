#pragma once
#include<glm/glm-all.hpp>

namespace renderme
{

	enum struct Ray_Type
	{
		camera,
		diffuse,
		specular,
		refract,
	};

	struct Ray final
	{
		Ray(Ray_Type _type, glm::vec3 const& origin, glm::vec3 const& direction, float tmin = 0, float tmax = std::numeric_limits<float>::max());
		auto is_valid(float t) const noexcept -> bool;
		auto update_tmax(float t) const noexcept -> void;
		auto point_at(float t) const noexcept -> glm::vec3;

		Ray_Type type;
		glm::vec3 origin;
		glm::vec3 direction;
		float tmin{0};
		mutable float tmax{ std::numeric_limits<float>::max() };
		glm::vec3 inv_direction;
	};
}