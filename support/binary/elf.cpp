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

#include "binary/elf.h"
#include "llvm/BinaryFormat/ELF.h"
#include <optional>
#include <vector>

using namespace llvm::ELF;
using binary::BinaryBuffer;

namespace elf {
namespace {

struct Bit32Arch {
  using Ehdr = Elf32_Ehdr;
  using Phdr = Elf32_Phdr;
  using Shdr = Elf32_Shdr;
};

struct Bit64Arch {
  using Ehdr = Elf64_Ehdr;
  using Phdr = Elf64_Phdr;
  using Shdr = Elf64_Shdr;
};

template <typename BitNArch> struct Program {
  const typename BitNArch::Phdr* hdr;
};

template <typename BitNArch> struct Section {
  std::string_view name;
  const typename BitNArch::Shdr* hdr;
  BinaryBuffer content;
};

template <typename BitNArch> struct ElfN {
  using Ehdr = typename BitNArch::Ehdr;
  using Shdr = typename BitNArch::Shdr;
  using Phdr = typename BitNArch::Phdr;

  const Ehdr* ehdr;
  std::vector<Program<BitNArch>> programs;
  std::vector<Section<BitNArch>> sections;

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

    const Shdr& entry = getSectionEntry(bb);
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
    // If the file has no section header table, this member(e_shoff) holds zero.
    if (ehdr->e_shoff == 0)
      return;

    const Shdr& entry = getSectionEntry(bb);
    // If the number of entries in the section header table is
    // larger than or equal to SHN_LORESERVE (0xff00), e_shnum
    // holds the value zero and the real number of entries in the
    // section header table is held in the sh_size member of the
    // initial entry in section header table.  Otherwise, the
    // sh_size member of the initial entry in the section header
    // table holds the value zero.
    size_t shSize = ehdr->e_shnum == 0 ? entry.sh_size : ehdr->e_shnum;

    // If the file has no section name string table, this member holds the value
    // SHN_UNDEF.
    std::optional<BinaryBuffer> shStrTbl;
    if (ehdr->e_shstrndx != SHN_UNDEF) {
      // If the index of section name string table section is
      // larger than or equal to SHN_LORESERVE (0xff00), this
      // member holds SHN_XINDEX (0xffff) and the real index of the
      // section name string table section is held in the sh_link
      // member of the initial entry in section header table.
      // Otherwise, the sh_link member of the initial entry in
      // section header table contains the value zero.
      size_t shStrNdx =
          ehdr->e_shstrndx == SHN_XINDEX ? entry.sh_link : ehdr->e_shstrndx;
      bb.seekg(ehdr->e_shoff + ehdr->e_shentsize * shStrNdx);
      auto* str = bb.peek<Shdr>();
      shStrTbl = getSectionContent(bb, str);
    }

    bb.seekg(ehdr->e_shoff);
    for (size_t i = 0; i < shSize; ++i) {
      auto* shdr = bb.get<Shdr>();
      std::string_view shName;
      if (shStrTbl.has_value()) {
        shName = getSectionName(shStrTbl.value(), shdr);
      }
      sections.push_back({shName, shdr, getSectionContent(bb, shdr)});
    }
  }

  const Shdr& getSectionEntry(BinaryBuffer& bb) const {
    bb.seekg(ehdr->e_shoff);
    return *(bb.peek<Shdr>());
  }

  std::string_view getSectionName(BinaryBuffer shStrTbl,
                                  const Shdr* shdr) const {
    shStrTbl.seekg(shdr->sh_name);
    return shStrTbl.peekc();
  }

  BinaryBuffer getSectionContent(BinaryBuffer bb, const Shdr* shdr) const {
    return bb.slice(shdr->sh_offset, shdr->sh_size);
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
    const elf::Section<BitNArch>& s = elfn.sections.at(ndx);
    if (s.hdr->sh_flags & SHF_EXECINSTR) {
      return std::make_unique<ExecuteSection>(
          s.name, static_cast<SHType>(s.hdr->sh_type),
          static_cast<SHFlags>(s.hdr->sh_flags), s.content);
    }
    return std::make_unique<Section>(s.name,
                                     static_cast<SHType>(s.hdr->sh_type),
                                     static_cast<SHFlags>(s.hdr->sh_flags));
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
