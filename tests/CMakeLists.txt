include_directories(${PROJECT_SOURCE_DIR}/include)

# find system or package controller first
find_package(GTest)
if (NOT GTest_FOUND)
    message(STATUS "Try to import googletest by FetchContent")
    include(${CMAKE_SOURCE_DIR}/cmake/fetch_content_googletest.cmake)
    find_package(GTest)
endif()

add_subdirectory(cpp)
add_subdirectory(estd)
add_subdirectory(binary)
