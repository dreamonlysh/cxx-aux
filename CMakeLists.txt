cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(tony LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(GMOCK_ROOT ${PROJECT_SOURCE_DIR}/tools/googletest)

add_subdirectory(tools)
add_subdirectory(support)
