project(estd_test)

list(APPEND SRC_FILES algorithm/fill_range_test.cpp)
list(APPEND SRC_FILES dl/dl_load_test.cpp)
list(APPEND SRC_FILES pattern/factory_test.cpp)

add_subdirectory(dl)

add_executable(estd_test ${SRC_FILES})
target_link_libraries(estd_test gtest gtest_main ${CMAKE_DL_LIBS})