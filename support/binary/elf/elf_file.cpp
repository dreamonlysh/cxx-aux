// Copyright (c) 2023 guyuemeng
//
// Tony is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#include "binary/elf/elf_file.h"
#include "binary/buffer.h"
#include <llvm/BinaryFormat/ELF.h>

namespace elf {
using namespace binary;
using namespace llvm::ELF;

class ElfFileImpl : public ElfFile {
public:
  ElfFileImpl(std::string_view magic) : magic(magic) {}

  virtual ~ElfFileImpl() noexcept = default;

  virtual uint32_t bits() const {
    switch (magic[EI_CLASS]) {
    case ELFCLASS32:
      return 32;
    case ELFCLASS64:
      return 64;
    default:
      return 0;
    }
  }

  virtual uint32_t endian() const {
    switch (magic[EI_DATA]) {
    case ELFDATA2LSB:
      return LITTLE_ENDIAN;
    case ELFDATA2MSB:
      return BIG_ENDIAN;
    default:
      return 0;
    }
  }

  virtual uint8_t version() const { return magic[EI_VERSION]; }

  virtual uint8_t osABI() const { return magic[EI_OSABI]; }

  virtual uint8_t abiVersion() const { return magic[EI_ABIVERSION]; }

private:
  std::string_view magic;
};

std::unique_ptr<ElfFile> analyzeElfFile(const char* file, size_t size) {
  if (size < EI_NIDENT)
    return nullptr;

  BinaryBuffer bb(file, size);
  std::string_view magic = bb.peekn(EI_NIDENT);
  if (memcmp(magic.data(), ElfMagic, strlen(ElfMagic)) != 0)
    // This is not a elf format
    return nullptr;

  return std::make_unique<ElfFileImpl>(magic);
}

} // namespace elf