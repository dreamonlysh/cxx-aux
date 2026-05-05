project(cxxaux_utility_test)

list(APPEND SRC_FILES
        version_test.cpp
)

add_executable(cxxaux_utility_test ${SRC_FILES})

target_link_libraries(cxxaux_utility_test
        GTest::gtest
        GTest::gtest_main
        ${CMAKE_DL_LIBS}
)

gtest_discover_tests(cxxaux_utility_test)
