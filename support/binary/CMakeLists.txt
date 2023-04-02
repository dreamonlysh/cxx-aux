include_directories(${LLVM_ROOT}/include)

set(SRC_FILES
        elf_parser.cpp)

add_library(binary STATIC ${SRC_FILES})

