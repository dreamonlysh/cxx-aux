project(estd_test)

include_directories(${GMOCK_ROOT}/googlemock/include)
include_directories(${GMOCK_ROOT}/googletest/include)

set(SRC_FILES
        main.cpp
        utils_test.cpp
        any_test.cpp
        exchange_test.cpp
        invoke_test.cpp
        factory_test.cpp
        dl_test.cpp)

add_subdirectory(dl)

add_executable(estd_test ${SRC_FILES})
target_link_libraries(estd_test gmock ${CMAKE_DL_LIBS})