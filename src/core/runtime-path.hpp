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
		auto relative_path() const noexcept ->std::string;
		auto full_path() const noexcept ->std::string;
		auto upper_directory() const noexcept -> Runtime_Path;
		static auto renderme_root_path()->std::string;

	private:
		//Stores the relative path from RENDERME_ROOT_PATH
		//Begins with one single '/' and no tail '/'s
		std::string path;
	};
}