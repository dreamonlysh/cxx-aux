#ifndef TONY_BINARY_ELF_PARSER_H
#define TONY_BINARY_ELF_PARSER_H
#include "buffer.h"
#include <memory>

namespace binary { namespace elf {

class BinaryElf {
};

std::unique_ptr<BinaryElf> createBinaryElf(BinaryBuffer be);

}}// namespace binary::elf

#endif//TONY_BINARY_ELF_PARSER_H
