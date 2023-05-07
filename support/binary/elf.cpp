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

#include "binary/elf/elf.h"
#include "elf/elf.h"
#include "llvm/BinaryFormat/ELF.h"
#include <iostream>
#include <optional>
#include <vector>

using namespace llvm::ELF;
using binary::BinaryBuffer;

namespace elf {
namespace {

template <typename BitNArch> struct Program {
  const typename BitNArch::Phdr* hdr;
};

template <typename BitNArch> struct ElfN {
  using Ehdr = typename BitNArch::Ehdr;
  using Shdr = typename BitNArch::Shdr;
  using Phdr = typename BitNArch::Phdr;

  const Ehdr* ehdr;
  std::vector<Program<BitNArch>> programs;
  std::vector<Section<Shdr>> sections;

  ElfN(BinaryBuffer bb) {
    ehdr = bb.peek<Ehdr>();
    loadPrograms(bb);
    loadSections(bb);
  }

private:
  void loadPrograms(BinaryBuffer bb) {
    // If the file has no program header table, this member(e_phoff) holds zero.
    if (ehdr->e_phoff == 0)
      return;

    const Shdr& entry = getInitialShdr(bb);
    // If the number of entries in the program header table is
    // larger than or equal to PN_XNUM (0xffff), this member
    // holds PN_XNUM (0xffff) and the real number of entries in
    // the program header table is held in the sh_info member of
    // the initial entry in section header table.  Otherwise, the
    // sh_info member of the initial entry contains the value
    // zero.
    size_t phSize = ehdr->e_phnum == 0xffff ? entry.sh_info : ehdr->e_phnum;

    bb.seekg(ehdr->e_phoff);
    for (size_t i = 0; i < phSize; ++i) {
      auto* phdr = bb.get<Phdr>();
      programs.push_back({phdr});
    }
  }

  void loadSections(BinaryBuffer bb) {
    if constexpr (std::is_same_v<BitNArch, Bit32Arch>) {
      sections = loadElf32Sections(bb.data());
    } else if constexpr (std::is_same_v<BitNArch, Bit64Arch>) {
      sections = loadElf64Sections(bb.data());
    }
  }

  const Shdr& getInitialShdr(BinaryBuffer& bb) const {
    bb.seekg(ehdr->e_shoff);
    return *(bb.peek<Shdr>());
  }
};
} // namespace
} // namespace elf

namespace binary {

template <typename BitNArch> class ElfImpl : public Elf {
public:
  ElfImpl(BinaryBuffer bb) : elfn(bb) {}

  virtual ~ElfImpl() noexcept = default;

  virtual EIClass archBits() const {
    return static_cast<EIClass>(elfn.ehdr->e_ident[EI_CLASS]);
  }

  virtual EIData encoding() const {
    return static_cast<EIData>(elfn.ehdr->e_ident[EI_DATA]);
  }

  virtual EType type() const { return static_cast<EType>(elfn.ehdr->e_type); }

  virtual EMachine machine() const {
    return static_cast<EMachine>(elfn.ehdr->e_machine);
  }

  virtual size_t s_size() const { return elfn.sections.size(); }

  virtual std::unique_ptr<Section> s_at(size_t ndx) const {
    const elf::Section<typename BitNArch::Shdr>& s = elfn.sections.at(ndx);
    if (s.hdr->sh_flags & SHF_EXECINSTR) {
      return std::make_unique<ExecuteSection>(
          s.name, static_cast<SHType>(s.hdr->sh_type),
          static_cast<SHFlags>(s.hdr->sh_flags), BinaryBuffer(s.content));
    }
    return std::make_unique<Section>(s.name,
                                     static_cast<SHType>(s.hdr->sh_type),
                                     static_cast<SHFlags>(s.hdr->sh_flags));
  }

  virtual void dump() const {
    for (const auto& sym : elfn.sections) {
      auto symTab = BinaryBuffer(sym.content);
      while (symTab.tellg() != symTab.eob) {
        auto* st = symTab.get<typename BitNArch::Sym>();
        if (st->st_info == STT_FUNC) {
          auto strTab = BinaryBuffer(elfn.sections[sym.hdr->sh_link].content);
          strTab.seekg(st->st_name);
          std::cout << strTab.getc() << std::endl;
        }
      }
    }
  }

private:
  const elf::ElfN<BitNArch> elfn;
};

std::unique_ptr<Elf> createBinaryElf(BinaryBuffer bb) {
  std::string_view magic = bb.peekn(EI_NIDENT);
  if (memcmp(magic.data(), ElfMagic, strlen(ElfMagic)) != 0) {
    // This is not a elf format
    return {};
  }
  if (magic[EI_DATA] != ELFDATA2LSB) {
    // Big edition is no supported till now
    return {};
  }

  if (magic[EI_CLASS] == ELFCLASS64) {
    return std::make_unique<ElfImpl<elf::Bit64Arch>>(bb);
  }
  if (magic[EI_CLASS] == ELFCLASS32) {
    return std::make_unique<ElfImpl<elf::Bit32Arch>>(bb);
  }
  return {};
}

} // namespace binary
