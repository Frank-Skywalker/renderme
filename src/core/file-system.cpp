#include "file-system.hpp"

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

	auto Runtime_Path::path()->std::string const&
	{
		return _path;
	}

	auto Runtime_Path::renderme_root_path()->std::string
	{
		return std::string(RENDERME_ROOT_PATH);
	}
}