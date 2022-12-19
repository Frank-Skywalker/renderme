#pragma once
#include "config.hpp"

#include <string>

namespace renderme
{

	struct Runtime_Path
	{
		Runtime_Path();
		Runtime_Path(char const* relative_path);
		Runtime_Path(std::string relative_path);
		auto path()->std::string const&;
		static auto renderme_root_path()->std::string;

	private:
		std::string _path;
	};
}