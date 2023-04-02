#include "binary/elf_parser.h"
#include "llvm/BinaryFormat/ELF.h"

namespace binary { namespace elf {
using namespace llvm::ELF;

class BinaryElfImpl : public BinaryElf {};

std::unique_ptr<BinaryElf> createBinaryElf(BinaryBuffer be) {
  auto *header = be.peek<Elf64_Ehdr>();

  return std::make_unique<BinaryElfImpl>();
}

}}// namespace binary::elf
