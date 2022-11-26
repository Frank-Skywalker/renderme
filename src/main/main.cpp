#include<iostream>
#include<string>
#include<glviewer/glviewer.hpp>

auto main(int argc, char* argv[]) -> int
{
    //Read obj
    std::cout << "hello, renderme" << std::endl;
    auto obj_path = std::string{};
    if (argc > 1) {
        obj_path = std::string{argv[1]};
    } else {
        std::cout << "Please specify obj path: " << std::endl;
        std::cin >> obj_path;
    }
    std::cout << "Reading obj from " << obj_path << std::endl;

    //Start GL_Viewer
    auto& gl_viewer=renderme::glviewer::GL_Viewer::instance();
    gl_viewer.main_loop();

    return 0;

}