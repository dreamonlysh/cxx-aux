include_directories(${LLVM_ROOT}/include)

add_library(memory_pool STATIC memory_pool.cpp)

set_property(TARGET memory_pool PROPERTY POSITION_INDEPENDENT_CODE Os)
