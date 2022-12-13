#pragma once
#include<core/scene.hpp>

#include<string>
namespace renderme
{
	auto parse_from_file(std::string const& path)->Scene;

}