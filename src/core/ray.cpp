#include "ray.hpp"

namespace renderme
{
	Ray::Ray(glm::vec3 const& origin, glm::vec3 const& direction, float tmax)
		:origin{origin},direction{direction}, tmax{tmax}, inv_direction{1.0f/direction}
	{}

}