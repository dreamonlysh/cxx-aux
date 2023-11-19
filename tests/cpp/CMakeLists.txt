project(cpp_test)

list(APPEND SRC_FILES
        any_test.cpp
        fold_expression_test.cpp
        invoke_test.cpp
        structured_binding_declaration_test.cpp)

add_executable(cpp_test ${SRC_FILES})
target_link_libraries(cpp_test gtest gtest_main ${CMAKE_DL_LIBS})