project(estd_test)

include_directories(${GMOCK_ROOT}/googlemock/include)
include_directories(${GMOCK_ROOT}/googletest/include)
include_directories(${ESTD_INCLUDE})

add_executable(estd_test main.cpp utils_test.cpp any_test.cpp exchange_test.cpp invoke_test.cpp factory_test.cpp)
target_link_libraries(estd_test gmock)