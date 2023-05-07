include_directories(${LLVM_ROOT}/include)

set(SRC_FILES
        elf.cpp
        elf/section.cpp
        elf/elf_file.cpp)

add_library(binary STATIC ${SRC_FILES})

