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

#include "elf.h"
#include <optional>

namespace {
using namespace elf;
using namespace binary;

template <typename BitNArch> class SectionLoader {
  using Ehdr = typename BitNArch::Ehdr;
  using Shdr = typename BitNArch::Shdr;

public:
  SectionLoader(std::string_view buf) : bb(buf), ehdr(bb.peek<Ehdr>()) {}

  std::vector<Section<Shdr>> doit() {
    // If the file has no section header table, this member(e_shoff) holds zero.
    if (ehdr->e_shoff == 0)
      return {};

    const Shdr& initial = getShdrInitializer();
    // If the number of entries in the section header table is
    // larger than or equal to SHN_LORESERVE (0xff00), e_shnum
    // holds the value zero and the real number of entries in the
    // section header table is held in the sh_size member of the
    // initial entry in section header table.  Otherwise, the
    // sh_size member of the initial entry in the section header
    // table holds the value zero.
    size_t shnum = ehdr->e_shnum == 0 ? initial.sh_size : ehdr->e_shnum;

    // If the file has no section name string table, this member holds the value
    // SHN_UNDEF.
    std::optional<BinaryBuffer> shstr;
    if (ehdr->e_shstrndx != SHN_UNDEF) {
      // If the index of section name string table section is
      // larger than or equal to SHN_LORESERVE (0xff00), this
      // member holds SHN_XINDEX (0xffff) and the real index of the
      // section name string table section is held in the sh_link
      // member of the initial entry in section header table.
      // Otherwise, the sh_link member of the initial entry in
      // section header table contains the value zero.
      size_t shstrndx =
          ehdr->e_shstrndx == SHN_XINDEX ? initial.sh_link : ehdr->e_shstrndx;
      bb.seekg(ehdr->e_shoff + ehdr->e_shentsize * shstrndx);
      auto* str = bb.peek<Shdr>();
      shstr = getSectionContent(str);
    }

    std::vector<Section<Shdr>> sections;
    bb.seekg(ehdr->e_shoff);
    for (size_t i = 0; i < shnum; ++i) {
      auto* shdr = bb.get<Shdr>();
      std::string_view shName;
      if (shstr.has_value()) {
        shName = getSectionName(shstr.value(), shdr);
      }
      sections.push_back({shName, getSectionContent(shdr).data(), shdr});
    }

    return sections;
  }

private:
  const Shdr& getShdrInitializer() {
    bb.seekg(ehdr->e_shoff);
    return *(bb.peek<Shdr>());
  }

  std::string_view getSectionName(BinaryBuffer shStrTbl,
                                  const Shdr* shdr) const {
    shStrTbl.seekg(shdr->sh_name);
    return shStrTbl.peekc();
  }

  BinaryBuffer getSectionContent(const Shdr* shdr) {
    return bb.slice(shdr->sh_offset, shdr->sh_size);
  }

private:
  BinaryBuffer bb;
  const Ehdr* ehdr;
};

} // namespace

namespace elf {

std::vector<Section<Bit32Arch::Shdr>> loadElf32Sections(std::string_view buf) {
  return SectionLoader<Bit32Arch>(buf).doit();
}

std::vector<Section<Bit64Arch::Shdr>> loadElf64Sections(std::string_view buf) {
  return SectionLoader<Bit64Arch>(buf).doit();
}

} // namespace elf
