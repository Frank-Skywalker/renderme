#include "transform.hpp"


namespace renderme
{

	Transform::Transform()
		:m(glm::identity<glm::mat4>()),minv(glm::identity<glm::mat4>())
	{
	}

	Transform::Transform(glm::mat4 const& _m)
		:m(_m), minv(glm::inverse(_m))
	{

	}

	Transform::Transform(glm::mat4 const& _m, glm::mat4 const& _minv)
		:m(_m), minv(_minv)
	{

	}


	auto Transform::is_identity() const noexcept -> bool
	{
		return m == glm::identity<glm::mat4>() && minv == glm::identity<glm::mat4>();
	}

	auto Transform::transpose() const noexcept->Transform
	{
		return Transform{glm::transpose(m), glm::transpose(minv)};
	}

	auto Transform::inverse() const noexcept->Transform
	{
		return Transform(minv, m);
	}

	auto Transform::operator*(Transform const& rhs) const noexcept -> Transform
	{
		return Transform(m * rhs.m, rhs.minv * minv);
	}

	auto Transform::operator==(Transform const& rhs) const noexcept->bool
	{
		return m == rhs.m && minv == rhs.minv;
	}

	auto Transform::operator!=(Transform const& rhs) const noexcept->bool
	{
		return m != rhs.m || minv != rhs.minv;
	}

	auto operator<<(std::ostream& os, Transform const& transform)->std::ostream&
	{
		return os << transform.m;
	}

	auto Transform::transform_ray(Ray const& ray) const noexcept ->Ray
	{
		glm::vec3 origin = transform_point(ray.origin);
		glm::vec3 direction = glm::normalize(transform_vector(ray.direction));
		return Ray(ray.type, origin, direction, ray.tmin, ray.tmax);
	}



}