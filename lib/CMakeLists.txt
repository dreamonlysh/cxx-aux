include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(LLVM_ROOT ${PROJECT_SOURCE_DIR}/third_party/llvm)

add_subdirectory(binary)