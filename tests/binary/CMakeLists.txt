project(binary_test)

list(APPEND SRC_FILES
        binary_decoder_test.cpp
        elf_test.cpp
        )

add_executable(binary_test ${SRC_FILES})

target_link_libraries(binary_test binary GTest::gtest GTest::gtest_main ${CMAKE_DL_LIBS})
