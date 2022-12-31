#include "runtime-path.hpp"

namespace renderme
{
	Runtime_Path::Runtime_Path()
		:_path(RENDERME_ROOT_PATH)
	{
	}

	Runtime_Path::Runtime_Path(char const* relative_path)
		:Runtime_Path(std::string(relative_path))
	{}


	Runtime_Path::Runtime_Path(std::string relative_path)
		:Runtime_Path()
	{
		if (relative_path.size() > 0) {
			if (relative_path[0] == '/' || relative_path[0] == '\\') {
				_path += std::move(relative_path);
			}
			else {
				_path += '/' + std::move(relative_path);
			}
		}
	}

	auto Runtime_Path::path() const noexcept ->std::string const&
	{
		return _path;
	}

	auto Runtime_Path::empty() const noexcept ->bool
	{
		return _path == RENDERME_ROOT_PATH;
	}

	auto Runtime_Path::renderme_root_path()->std::string
	{
		return std::string(RENDERME_ROOT_PATH);
	}
}