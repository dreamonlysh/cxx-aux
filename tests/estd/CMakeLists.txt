project(estd_test)

list(APPEND SRC_FILES
        fill_range_test.cpp
        any_test.cpp
        fold_expression_test.cpp
        invoke_test.cpp
        factory_test.cpp
        structured_binding_declaration_test.cpp)
list(APPEND SRC_FILES dl/dl_load_test.cpp)

add_subdirectory(dl)

add_executable(estd_test main.cpp ${SRC_FILES})
target_link_libraries(estd_test gmock ${CMAKE_DL_LIBS})