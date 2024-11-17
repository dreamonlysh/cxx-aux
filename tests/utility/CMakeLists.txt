project(utility_test)

list(APPEND SRC_FILES
        memory_pool_test.cpp
        guard_test.cpp
)

add_executable(utility_test ${SRC_FILES})

target_link_libraries(utility_test
        GTest::gtest
        GTest::gtest_main
        memory_pool
        ${CMAKE_DL_LIBS}
)

gtest_discover_tests(utility_test)
