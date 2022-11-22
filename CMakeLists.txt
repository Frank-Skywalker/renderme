cmake_minimum_required(VERSION 3.12)
PROJECT(mini-renderer)


# version number
set(mini-renderer_VERSION_MAJOR 0)
set(mini-renderer_VERSION_MINOR 0)
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    src/config/mini-renderer-config.h.in
    ${PROJECT_SOURCE_DIR}/src/config/mini-renderer-config.h
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(EXE_NAME mini-renderer)
include_directories(src)

add_executable(${EXE_NAME} src/main/mini-renderer.cpp)
