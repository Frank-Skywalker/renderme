project(hah)

# print project info
message(STATUS hererere)
message(STATUS ${PROJECT_NAME})
message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${PROJECT_BINARY_DIR})
message(STATUS ${EXECUTABLE_OUTPUT_PATH})
message(STATUS ${LIBRARY_OUTPUT_PATH})

add_library(hah SHARED test.cpp)