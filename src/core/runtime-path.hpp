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
		auto path() const noexcept ->std::string const&;
		auto empty() const noexcept ->bool;
		static auto renderme_root_path()->std::string;

	private:
		std::string _path;
	};
}