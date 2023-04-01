include_directories(${PROJECT_SOURCE_DIR}/include/driver)

set(SRC_FILES
        ../unittest/dynamic_library/dl_test.cpp)

add_library(estd ${SRC_FILES})
