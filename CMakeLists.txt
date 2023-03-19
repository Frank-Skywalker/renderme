cmake_minimum_required(VERSION 3.12)
PROJECT(renderme
        VERSION 0.0
        DESCRIPTION "a renderme"
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

# set all lib/dll output path of externals
# make sure to prevent reset of these variables in subdirectory's CMakeLists.txt
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

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

############# build lib part ##############
file(GLOB LIB_DIRECTORIES external/*)
foreach(LIB_DIRECTORY ${LIB_DIRECTORIES})
    if(IS_DIRECTORY ${LIB_DIRECTORY})
        # build library
        if(EXISTS ${LIB_DIRECTORY}/CMakeLists.txt)
            add_subdirectory(${LIB_DIRECTORY})
            # add library include files
            if(IS_DIRECTORY ${LIB_DIRECTORY}/include)
                target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_DIRECTORY}/include)
            endif()
            # Copy library's .dll files to executable output path
            # This step isn't needed any more if we set all dll output path directly as exe output path
            # file(RELATIVE_PATH LIB_DIRECTORY_RELATIVE ${PROJECT_SOURCE_DIR} ${LIB_DIRECTORY})
            #add_custom_command(
            #    TARGET ${PROJECT_NAME} POST_BUILD
            #    COMMAND ${CMAKE_COMMAND} -E copy_directory
            #        ${PROJECT_BINARY_DIR}/${LIB_DIRECTORY_RELATIVE}/bin
            #        ${EXECUTABLE_OUTPUT_PATH}
            #)
        endif()
    endif()
endforeach(LIB_DIRECTORY)

############ config part #################
# use double or not
option(USE_CUDA "use CUDA to pipeline" OFF)

# configure a header file to pass some of the CMake settings to the source code
configure_file(
    src/core/config.hpp.in
    ${PROJECT_SOURCE_DIR}/src/core/config.hpp
)

########## library link part ##############
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glew)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} hah)

# Find CUDA
if(USE_CUDA)
    find_package(CUDA REQUIRED)
    if(CUDA_FOUND)
        message(STATUS ${CUDA_INCLUDE_DIRS})
        include_directories(${CUDA_INCLUDE_DIRS})
    else(CUDA_FOUND)
        message(FATAL_ERROR "CUDA not found")
    endif(CUDA_FOUND)
endif(USE_CUDA)
