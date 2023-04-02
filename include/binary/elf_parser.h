#ifndef TONY_BINARY_ELF_PARSER_H
#define TONY_BINARY_ELF_PARSER_H
#include "buffer.h"
#include <memory>

namespace binary { namespace elf {

struct Section {
  std::string_view name;
  BinaryBuffer contents;
};

class BinaryElf {
public:
  virtual ~BinaryElf() noexcept {}

  virtual const std::vector<Section>& sections() = 0;
};

std::unique_ptr<BinaryElf> createBinaryElf(BinaryBuffer be);

}}// namespace binary::elf

#endif//TONY_BINARY_ELF_PARSER_H
