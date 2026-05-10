cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(cxx-aux LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

option(CXXAUX_ENABLE_TESTS "Enable all of the cxxaux own tests." OFF)
mark_as_advanced(CXXAUX_ENABLE_TESTS)

add_subdirectory(src)

if (CXXAUX_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
else()
  message(STATUS "cxxaux tests are disabled.")
endif()

install(DIRECTORY include/estd include/cxxaux include/experiment
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS memory_pool
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
