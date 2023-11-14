project(estd_test)

list(APPEND SRC_FILES algorithm/fill_range_test.cpp)
list(APPEND SRC_FILES dl/dl_load_test.cpp)
list(APPEND SRC_FILES pattern/factory_test.cpp)

add_subdirectory(dl)

add_executable(estd_test ${GMOCK_ROOT}/googletest/src/gtest_main.cc ${SRC_FILES})
target_link_libraries(estd_test gmock ${CMAKE_DL_LIBS})