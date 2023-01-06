#pragma once
#include <glm/glm.hpp>

#include <iostream>

namespace renderme
{
	template<typename T>
	auto operator<<(std::ostream& os, glm::tvec2<T> const& v)->std::ostream&
	{
		return os << "["
			<< " " << v[0]
			<< " " << v[1]
			<< " " << "]" << std::endl;
	}

	template<typename T>
	auto operator<<(std::ostream& os, glm::tvec3<T> const& v)->std::ostream&
	{
		return os << "["
			<< " " << v[0]
			<< " " << v[1]
			<< " " << v[2]
			<< " " << "]" << std::endl;
	}

	template<typename T>
	auto operator<<(std::ostream& os, glm::tvec4<T> const& v)->std::ostream&
	{
		return os << "["
			<< " " << v[0]
			<< " " << v[1]
			<< " " << v[2]
			<< " " << v[3]
			<< " " << "]" << std::endl;
	}

	template<typename T>
	auto operator<<(std::ostream& os, glm::tmat4x4<T> const& m)->std::ostream&
	{
		return os << "[" << std::endl
			<< m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << std::endl
			<< m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << std::endl
			<< m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << std::endl
			<< m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << std::endl
			<< "]" << std::endl;
	}
}