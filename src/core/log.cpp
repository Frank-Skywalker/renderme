#include "log.hpp"

#include <iostream>
namespace renderme
{
	auto log(Status status, std::string const& msg)->void
	{
		std::cerr << msg << std::endl;
		if (status == Status::fatal) {
			exit(0);
		}
	}

}