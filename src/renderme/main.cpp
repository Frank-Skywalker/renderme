#include "renderme.hpp"

auto main(int argc, char* argv[]) -> int
{
    std::string app_path="/config/veach-mis.json";

    if (argc > 1) {
        app_path = argv[1];
    }

    //Start Renderme
    auto& renderme=renderme::Renderme::instance();
    renderme.parse_from_file(app_path);
    renderme.main_loop();

    return EXIT_SUCCESS;
}