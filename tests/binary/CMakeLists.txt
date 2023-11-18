project(binary_test)

list(APPEND SRC_FILES
        binary_decoder_test.cpp
        elf_test.cpp
        )

add_executable(binary_test ${GMOCK_ROOT}/googletest/src/gtest_main.cc ${SRC_FILES})
target_link_libraries(binary_test gmock binary ${CMAKE_DL_LIBS})