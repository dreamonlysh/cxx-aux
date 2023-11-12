cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(cxx-aux LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

add_subdirectory(tools)
add_subdirectory(lib)
add_subdirectory(tests)
