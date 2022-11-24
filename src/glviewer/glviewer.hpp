#include <core/util.hpp>

namespace renderme::glviewer
{
	struct GL_Viewer final: util::Singleton<GL_Viewer>
	{
		auto main_loop()->void;
		auto init(int* argc, char** argv)->void;
	};
}