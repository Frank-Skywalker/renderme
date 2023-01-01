#include "runtime-path.hpp"

namespace renderme
{
	Runtime_Path::Runtime_Path()
	{
		path = "/";
	}

	Runtime_Path::Runtime_Path(char const* relative_path)
		:Runtime_Path(std::string(relative_path))
	{}


	Runtime_Path::Runtime_Path(std::string relative_path)
		:Runtime_Path()
	{
		//remove head '/'
		while ( 
			(!relative_path.empty()) && 
			(*relative_path.begin() == '/' || *relative_path.begin() == '\\')
			) {
			relative_path = relative_path.substr(1);
		}

		//remove tail '/'
		while (
			(!relative_path.empty()) &&
			( *(--relative_path.end()) == '/' || *(--relative_path.end()) == '\\')
			) {
			relative_path = relative_path.substr(0, relative_path.size() - 1);
		}
		path += relative_path;
	}

	auto Runtime_Path::full_path() const noexcept ->std::string
	{
		return RENDERME_ROOT_PATH + path;
	}

	auto Runtime_Path::relative_path() const noexcept ->std::string
	{
		return path;
	}

	auto Runtime_Path::upper_directory() const noexcept -> Runtime_Path
	{
		auto new_path = relative_path();
		//ensure no head '/' or tail '/'
		new_path = new_path.substr(1, new_path.rfind('/') - 1);
		return Runtime_Path(new_path);
	}


	auto Runtime_Path::renderme_root_path()->std::string
	{
		return std::string(RENDERME_ROOT_PATH);
	}
}