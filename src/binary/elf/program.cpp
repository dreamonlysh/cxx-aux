// Copyright (c) 2023 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#include "elfn.h"
#include <cxxaux/binary/elf/elf.h>

namespace {
using namespace cxxaux::elf;
using cxxaux::BinaryDecoder;

template <typename BitNArch>
class ProgramLoader {
  using Ehdr = typename BitNArch::Ehdr;
  using Shdr = typename BitNArch::Shdr;
  using Phdr = typename BitNArch::Phdr;

public:
  ProgramLoader(std::string_view buf)
      : decoder(buf.data(), buf.size()), ehdr(decoder.peek<Ehdr>()) {}

  std::vector<ElfNProgram<BitNArch>> doit() {
    // If the file has no program header table, this member(e_phoff) holds zero.
    if (ehdr->e_phoff == 0)
      return {};

    const Shdr& initial = getShdrInitializer();
    // If the number of entries in the program header table is
    // larger than or equal to PN_XNUM (0xffff), this member
    // holds PN_XNUM (0xffff) and the real number of entries in
    // the program header table is held in the sh_info member of
    // the initial entry in section header table.  Otherwise, the
    // sh_info member of the initial entry contains the value
    // zero.
    size_t phSize = ehdr->e_phnum == 0xffff ? initial.sh_info : ehdr->e_phnum;

    std::vector<ElfNProgram<BitNArch>> programs;
    programs.reserve(phSize);
    decoder.seekg(ehdr->e_phoff);
    for (size_t i = 0; i < phSize; ++i) {
      auto* phdr = decoder.get<Phdr>();
      programs.push_back({phdr});
    }
    return programs;
  }

private:
  const Shdr& getShdrInitializer() {
    decoder.seekg(ehdr->e_shoff);
    return *(decoder.peek<Shdr>());
  }

private:
  BinaryDecoder decoder;
  const Ehdr* ehdr;
};

} // namespace

namespace cxxaux { namespace elf {

std::vector<ElfNProgram<Bit32Arch>> loadElf32Programs(std::string_view buf) {
  return ProgramLoader<Bit32Arch>(buf).doit();
}

std::vector<ElfNProgram<Bit64Arch>> loadElf64Programs(std::string_view buf) {
  return ProgramLoader<Bit64Arch>(buf).doit();
}

}} // namespace cxxaux::elf
