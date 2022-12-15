#include<core/transform.hpp>
#include "renderme.hpp"

#include<iostream>
#include<string>

auto main(int argc, char* argv[]) -> int
{
    //Start Renderme
    auto& renderme=renderme::Renderme::instance();
    renderme.main_loop();

    return 0;
}