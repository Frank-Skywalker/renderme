#include<core/transform.hpp>
#include <core/vector.hpp>
#include "renderme.hpp"

#include<iostream>
#include<string>

auto main(int argc, char* argv[]) -> int
{
    renderme::Vector2f vec2;
    std::cerr<<vec2.x;

    //Start Renderme
    auto& renderme=renderme::Renderme::instance();
    renderme.main_loop();

    return 0;
}