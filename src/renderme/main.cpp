#include "renderme.hpp"

auto main(int argc, char* argv[]) -> int
{
    //Start Renderme
    auto& renderme=renderme::Renderme::instance();
    renderme.main_loop();

    return EXIT_SUCCESS;
}