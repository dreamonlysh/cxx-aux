include_directories(${LLVM_ROOT}/include)

set(SRC_FILES
        elf/elf.cpp
        elf/section.cpp
        elf/program.cpp
)

add_library(binary STATIC ${SRC_FILES})

set_property(TARGET binary PROPERTY POSITION_INDEPENDENT_CODE Os)
