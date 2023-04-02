cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(tony LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

add_subdirectory(tools)
add_subdirectory(support)
add_subdirectory(tests)
