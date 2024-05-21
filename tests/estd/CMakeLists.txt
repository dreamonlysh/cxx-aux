project(estd_test)

list(APPEND SRC_FILES algorithm/fill_range_test.cpp)
list(APPEND SRC_FILES container/integer_test.cpp)
list(APPEND SRC_FILES bit/bit_count_test.cpp)
list(APPEND SRC_FILES bit/bit_hl_dim_test.cpp)
list(APPEND SRC_FILES bit/bit_set_reset_test.cpp)
list(APPEND SRC_FILES bit/bit_pow2_test.cpp)
list(APPEND SRC_FILES idiom/pimpl_test.cpp)
list(APPEND SRC_FILES idiom/guard_test.cpp)
list(APPEND SRC_FILES meta/operators_test.cpp)
list(APPEND SRC_FILES dl/dl_load_test.cpp)
list(APPEND SRC_FILES pattern/factory_test.cpp)

add_subdirectory(dl)

add_executable(estd_test ${SRC_FILES})

target_link_libraries(estd_test GTest::gtest GTest::gtest_main ${CMAKE_DL_LIBS})

gtest_discover_tests(estd_test)
