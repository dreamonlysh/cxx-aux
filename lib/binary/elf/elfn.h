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

#ifndef BINARY_ELF_ELFN_H
#define BINARY_ELF_ELFN_H
#include <binary/binary_decoder.h>
#include <llvm/BinaryFormat/ELF.h>
#include <vector>

namespace cxxaux { namespace elf {
using namespace llvm::ELF;

struct Bit32Arch {
  using Ehdr = Elf32_Ehdr;
  using Phdr = Elf32_Phdr;
  using Shdr = Elf32_Shdr;

  using Sym = Elf32_Sym;
};

struct Bit64Arch {
  using Ehdr = Elf64_Ehdr;
  using Phdr = Elf64_Phdr;
  using Shdr = Elf64_Shdr;

  using Sym = Elf64_Sym;
};

template <typename BitNArch>
struct ElfNSection {
  const std::string_view name;
  const std::string_view content;
  const typename BitNArch::Shdr* hdr;
};

std::vector<ElfNSection<Bit32Arch>> loadElf32Sections(std::string_view buf);

std::vector<ElfNSection<Bit64Arch>> loadElf64Sections(std::string_view buf);

template <typename BitNArch>
struct ElfNProgram {
  const typename BitNArch::Phdr* hdr;
};

std::vector<ElfNProgram<Bit32Arch>> loadElf32Programs(std::string_view buf);

std::vector<ElfNProgram<Bit64Arch>> loadElf64Programs(std::string_view buf);

}}     // namespace cxxaux::elf
#endif // BINARY_ELF_ELFN_H
