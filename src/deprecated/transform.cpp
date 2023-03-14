#include "transform.hpp"

#include "math.hpp"

namespace deprecated
{
	Transform::Transform(float const value[4][4])
		:m{value}
	{
		m_inv = m.inverse();
	}

	Transform::Transform(
		float t00, float t01, float t02, float t03,
		float t10, float t11, float t12, float t13,
		float t20, float t21, float t22, float t23,
		float t30, float t31, float t32, float t33)
		:m{ t00,  t01,  t02,  t03,
		 t10,  t11,  t12,  t13,
		 t20,  t21,  t22,  t23,
		 t30,  t31,  t32,  t33}
	{
		m_inv = m.inverse();
	}

	Transform::Transform(Matrix4f const& _m)
		:m{_m}, m_inv{_m.inverse()}
	{

	}

	Transform::Transform(Matrix4f const& _m, Matrix4f const& _m_inv)
		:m{_m},m_inv{_m_inv}
	{

	}

	auto Transform::matrix() const noexcept ->Matrix4f const&
	{
		return m;
	}

	auto Transform::inverse_matrix() const noexcept ->Matrix4f const&
	{
		return m_inv;
	}

	auto Transform::print() const noexcept ->void
	{
		m.print();
	}

	auto Transform::is_identity() const noexcept -> bool
	{
		return (m.m[0][0] == 1.f && m.m[0][1] == 0.f && m.m[0][2] == 0.f &&
			m.m[0][3] == 0.f && m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
			m.m[1][2] == 0.f && m.m[1][3] == 0.f && m.m[2][0] == 0.f &&
			m.m[2][1] == 0.f && m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
			m.m[3][0] == 0.f && m.m[3][1] == 0.f && m.m[3][2] == 0.f &&
			m.m[3][3] == 1.f);
	}

	auto Transform::transpose() const noexcept->Transform
	{
		return Transform{m.transpose(), m_inv.transpose()};
	}

	auto Transform::inverse() const noexcept->Transform
	{
		return Transform{m_inv, m};
	}




	auto Transform::operator*(Transform const& rhs) const noexcept -> Transform
	{
		return Transform{m * rhs.m, rhs.m_inv * m_inv};
	}

	auto Transform::operator==(Transform const& rhs) const noexcept->bool
	{
		return m == rhs.m && m_inv == rhs.m_inv;
	}

	auto Transform::operator!=(Transform const& rhs) const noexcept->bool
	{
		return m != rhs.m || m_inv != rhs.m_inv;
	}


	auto operator<<(std::ostream& os, Transform const& m)->std::ostream&
	{
		return os << m.m;
	}


	auto translate(Vector3f const& delta)->Transform
	{
		return translate(delta.x, delta.y, delta.z);
	}

	auto translate(float x, float y, float z)->Transform
	{
		Matrix4f m(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		);
		Matrix4f minv(
			1, 0, 0, -x,
			0, 1, 0, -y,
			0, 0, 1, -z,
			0, 0, 0, 1
		);
		return Transform(m, minv);
	}

	auto scale(Vector3f const& _scale)->Transform
	{
		return scale(_scale.x, _scale.y, _scale.z);
	}

	auto scale(float x, float y, float z)->Transform
	{
		Matrix4f m(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
		Matrix4f minv(
			1 / x, 0, 0, 0,
			0, 1 / y, 0, 0,
			0, 0, 1 / z, 0,
			0, 0, 0, 1);
		return Transform(m, minv);
	}

	auto rotate_x(float theta)->Transform
	{
		float sintheta = std::sin(radians(theta));
		float costheta = std::cos(radians(theta));
		Matrix4f m(
			1, 0, 0, 0,
			0, costheta, -sintheta, 0,
			0, sintheta, costheta, 0,
			0, 0, 0, 1
		);
		return Transform(m, m.transpose());
	}

	auto rotate_y(float theta)->Transform
	{
		float sintheta = std::sin(radians(theta));
		float costheta = std::cos(radians(theta));
		Matrix4f m(
			costheta, 0, sintheta, 0,
			0, 1, 0, 0,
			-sintheta, 0, costheta, 0,
			0, 0, 0, 1
		);
		return Transform(m, m.transpose());
	}

	auto rotate_z(float theta)->Transform
	{
		float sintheta = std::sin(radians(theta));
		float costheta = std::cos(radians(theta));
		Matrix4f m(
			costheta, -sintheta, 0, 0,
			sintheta, costheta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
		return Transform(m, m.transpose());
	}

	auto rotate(Vector3f const& axis, float theta)->Transform
	{
		Vector3f a = normalize(axis);
		float sintheta = std::sin(radians(theta));
		float costheta = std::cos(radians(theta));
		Matrix4f m;
		// Compute rotation of first basis vector
		m.m[0][0] = a.x * a.x + (1 - a.x * a.x) * costheta;
		m.m[0][1] = a.x * a.y * (1 - costheta) - a.z * sintheta;
		m.m[0][2] = a.x * a.z * (1 - costheta) + a.y * sintheta;
		m.m[0][3] = 0;

		// Compute rotations of second and third basis vectors
		m.m[1][0] = a.x * a.y * (1 - costheta) + a.z * sintheta;
		m.m[1][1] = a.y * a.y + (1 - a.y * a.y) * costheta;
		m.m[1][2] = a.y * a.z * (1 - costheta) - a.x * sintheta;
		m.m[1][3] = 0;

		m.m[2][0] = a.x * a.z * (1 - costheta) - a.y * sintheta;
		m.m[2][1] = a.y * a.z * (1 - costheta) + a.x * sintheta;
		m.m[2][2] = a.z * a.z + (1 - a.z * a.z) * costheta;
		m.m[2][3] = 0;
		return Transform(m, m.transpose());
	}


	auto lookat(Point3f const& position, Point3f const& lookat, Vector3f const& up) -> Transform
	{
		Matrix4f cameratoworld;
		// Initialize fourth column of viewing matrix
		cameratoworld.m[0][3] = position.x;
		cameratoworld.m[1][3] = position.y;
		cameratoworld.m[2][3] = position.z;
		cameratoworld.m[3][3] = 1;

		// Initialize first three columns of viewing matrix
		Vector3f dir = normalize(lookat - position);
		if (length(cross(normalize(up), dir)) == 0){
			log(Status::error, "up and viewing direction");
			return Transform();
		}

		Vector3f right = normalize(cross(normalize(up), dir));
		Vector3f newup = cross(dir, right);
		cameratoworld.m[0][0] = right.x;
		cameratoworld.m[1][0] = right.y;
		cameratoworld.m[2][0] = right.z;
		cameratoworld.m[3][0] = 0.;
		cameratoworld.m[0][1] = newup.x;
		cameratoworld.m[1][1] = newup.y;
		cameratoworld.m[2][1] = newup.z;
		cameratoworld.m[3][1] = 0.;
		cameratoworld.m[0][2] = dir.x;
		cameratoworld.m[1][2] = dir.y;
		cameratoworld.m[2][2] = dir.z;
		cameratoworld.m[3][2] = 0.;

		return Transform(cameratoworld.inverse(), cameratoworld);
	}

	auto orthographic(float znear, float zfar)->Transform
	{
		return scale(1, 1, 1 / (zfar - znear)) * translate(Vector3f(0, 0, -znear));
	}

	auto perspective(float fov, float znear, float zfar)->Transform
	{
		// Perform projective divide for perspective projection
		Matrix4f persp(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, zfar / (zfar - znear), -zfar * znear / (zfar - znear),
			0, 0, 1, 0);

		// Scale canonical perspective view to specified field of view
		float inv_tan_ang = 1 / std::tan(radians(fov) / 2);
		return scale(inv_tan_ang, inv_tan_ang, 1) * Transform(persp);
	}
}
