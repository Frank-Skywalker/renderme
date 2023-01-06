#pragma once

#include"vector.hpp"
#include"transform.hpp"

#include<glm/glm.hpp>
#include<glm/ext.hpp>

namespace deprecated
{
	using namespace renderme;

	template<Category category, typename T>
	inline auto to_glm(Vec<3,category, T> v)->glm::vec3
	{
		return glm::vec3(v.x, v.y, v.z);
	}


	template<Category category, typename T>
	inline auto to_glm(Vec<2, category, T> v)->glm::vec2
	{
		return glm::vec2(v.x, v.y);
	}

	template<typename T>
	inline auto to_glm(Mat<4, 4, T> m)->glm::mat4
	{
		auto m_trans = m.transpose();
		return glm::mat4(
			m_trans.m[0][0], m_trans.m[0][1], m_trans.m[0][2], m_trans.m[0][3],
			m_trans.m[1][0], m_trans.m[1][1], m_trans.m[1][2], m_trans.m[1][3],
			m_trans.m[2][0], m_trans.m[2][1], m_trans.m[2][2], m_trans.m[2][3],
			m_trans.m[3][0], m_trans.m[3][1], m_trans.m[3][2], m_trans.m[3][3]
		);
	}


	inline auto from_glm(glm::vec3 v)->Vector3f
	{
		return Vector3f(v.x, v.y, v.z);
	}

	inline auto from_glm(glm::vec2 v)->Vector2f
	{
		return Vector2f(v.x, v.y);
	}

	inline auto from_glm(glm::mat4 m)->Matrix4f
	{
		auto m_trans = glm::transpose(m);
		return Matrix4f(
			m_trans[0][0], m_trans[0][1], m_trans[0][2], m_trans[0][3],
			m_trans[1][0], m_trans[1][1], m_trans[1][2], m_trans[1][3],
			m_trans[2][0], m_trans[2][1], m_trans[2][2], m_trans[2][3],
			m_trans[3][0], m_trans[3][1], m_trans[3][2], m_trans[3][3]
		);
	}


	inline auto print(glm::mat4 m)->void
	{
		std::cout << "[" << std::endl
			<< m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << std::endl
			<< m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << std::endl
			<< m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << std::endl
			<< m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << std::endl
			<< std::endl << "]" << std::endl;
	}

	inline auto print(float* m)->void
	{
		for (auto i = 0; i < 16; ++i) {
			std::cout << m[i] << " ";
		}
		std::cout << std::endl;
	}

}