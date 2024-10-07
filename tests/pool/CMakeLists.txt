project(pool_test)

list(APPEND SRC_FILES
        object_pool_test.cpp
)

add_executable(pool_test ${SRC_FILES})

target_link_libraries(pool_test
        GTest::gtest
        GTest::gtest_main
        ${CMAKE_DL_LIBS}
)

gtest_discover_tests(pool_test)
