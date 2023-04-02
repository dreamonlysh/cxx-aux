project(binary_test)

list(APPEND SRC_FILES
        buffer_test.cpp
        elf_test.cpp
        )

add_executable(binary_test main.cpp ${SRC_FILES})
target_link_libraries(binary_test gmock binary ${CMAKE_DL_LIBS})