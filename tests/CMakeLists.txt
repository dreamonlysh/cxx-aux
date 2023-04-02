set(GMOCK_ROOT ${PROJECT_SOURCE_DIR}/tools/googletest)

include_directories(${GMOCK_ROOT}/googlemock/include)
include_directories(${GMOCK_ROOT}/googletest/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(estd)
add_subdirectory(binary)
