#pragma once
#include <string>
namespace renderme
{
	enum struct Status
	{
		fatal,
		error,
		log
	};
	auto log(Status status, std::string const& msg)->void;
}