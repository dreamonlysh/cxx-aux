project(estd_test)

include_directories(${GMOCK_ROOT}/googlemock/include)
include_directories(${GMOCK_ROOT}/googletest/include)

list(APPEND SRC_FILES
        fill_range_test.cpp
        any_test.cpp
        fold_expression_test.cpp
        invoke_test.cpp
        factory_test.cpp
        structured_binding_declaration_test.cpp)
list(APPEND SRC_FILES dl/test.cpp)

add_subdirectory(dl)

add_executable(estd_test main.cpp ${SRC_FILES})
target_link_libraries(estd_test gmock al_sum ${CMAKE_DL_LIBS})