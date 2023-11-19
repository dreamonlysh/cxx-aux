# the archive library in third parity is inner used
# the googletest is special to change in googletest/cmake/internal_utils.cmake
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/third_party/lib)
# the dynamic library in third parity is outer depend
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(googletest)
add_subdirectory(libdwarf)