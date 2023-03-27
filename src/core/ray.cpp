#include "ray.hpp"
#include "log.hpp"
namespace renderme
{
	Ray::Ray(Ray_Type _type, glm::vec3 const& origin, glm::vec3 const& direction, float tmin, float tmax)
		:type{_type}, origin {origin}, direction{ direction }, tmin{ tmin }, tmax{ tmax }, inv_direction{ 1.0f / direction }
	{}

	auto Ray::update_tmax(float t) const noexcept-> void
	{
		if (t > tmax) {
			log(Status::fatal, "Invalid tmax");
		}
		tmax = t;
	}


	auto Ray::is_valid(float t) const noexcept -> bool
	{
		return t >= tmin && t <= tmax;
	}

	auto Ray::point_at(float t) const noexcept -> glm::vec3
	{
		return origin + t * direction;
	}


}