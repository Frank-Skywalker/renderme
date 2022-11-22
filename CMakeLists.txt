cmake_minimum_required(VERSION 3.12)
PROJECT(mini-renderer 
        VERSION 2.3
        DESCRIPTION "a mini renderer"
        LANGUAGES CXX)

#set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_BUILD_TYPE Debug)

# print project info
message(STATUS ${PROJECT_NAME})
message(STATUS ${PROJECT_VERSION})
message(STATUS ${PROJECT_DESCRIPTION})
message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${PROJECT_BINARY_DIR})

# set exe and lib output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

############# src part ##############
# add source files
file(GLOB SRC_DIRECTORIES src/*)
foreach(SRC_DIRECTORY ${SRC_DIRECTORIES})
    if(IS_DIRECTORY ${SRC_DIRECTORY})
        aux_source_directory(${SRC_DIRECTORY} SRC_LIST)
    endif()
endforeach(SRC_DIRECTORY)
add_executable(${PROJECT_NAME} ${SRC_LIST})

# set include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

############# lib part ##############
file(GLOB LIB_DIRECTORIES lib/*)
foreach(LIB_DIRECTORY ${LIB_DIRECTORIES})
    if(IS_DIRECTORY ${LIB_DIRECTORY})
        if(IS_DIRECTORY ${LIB_DIRECTORY}/include)
            target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_DIRECTORY}/include)
        endif()
    endif()
endforeach(LIB_DIRECTORY)



############ config part ############
# use double or not
option(USE_DOUBLE "real uses double instead of float" ON)

# configure a header file to pass some of the CMake settings to the source code
configure_file(
    src/config/mini-renderer-config.h.in
    ${PROJECT_SOURCE_DIR}/src/config/mini-renderer-config.h
)