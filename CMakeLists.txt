cmake_minimum_required(VERSION 3.1)
project(tony)

set(CMAKE_CXX_STANDARD 17)
set(GMOCK_ROOT ${PROJECT_SOURCE_DIR}/tools/googletest)

add_subdirectory(tools)
add_subdirectory(support)
