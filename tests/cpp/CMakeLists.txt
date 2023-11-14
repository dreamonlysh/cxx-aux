project(cpp_test)

list(APPEND SRC_FILES
        any_test.cpp
        fold_expression_test.cpp
        invoke_test.cpp
        structured_binding_declaration_test.cpp)

add_executable(cpp_test ${GMOCK_ROOT}/googletest/src/gtest_main.cc ${SRC_FILES})
target_link_libraries(cpp_test gmock ${CMAKE_DL_LIBS})