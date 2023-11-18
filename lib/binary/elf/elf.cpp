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
#include "elfn.h"
#include <iostream>
#include <llvm/BinaryFormat/ELF.h>
#include <optional>
#include <vector>

namespace cxxaux { namespace elf {
using namespace llvm::ELF;
using cxxaux::BinaryDecoder;

bool isElfFile(const char* data, size_t size) {
  if (size < EI_NIDENT)
    return false;
  BinaryDecoder decoder(data, size);
  std::string_view magic = decoder.peekn(EI_NIDENT);
  return memcmp(magic.data(), ElfMagic, strlen(ElfMagic)) == 0;
}

template <typename BitNArch>
class ElfImpl : public Elf {
  using Ehdr = typename BitNArch::Ehdr;

public:
  ElfImpl(BinaryDecoder decoder) {
    ehdr_ = decoder.peek<Ehdr>();
    loadPrograms(decoder);
    loadSections(decoder);
  }
  virtual ~ElfImpl() noexcept = default;

  EIClass archBits() const override {
    return static_cast<EIClass>(ehdr_->e_ident[EI_CLASS]);
  }

  EIData encoding() const override {
    return static_cast<EIData>(ehdr_->e_ident[EI_DATA]);
  }

  EType type() const override { return static_cast<EType>(ehdr_->e_type); }

  EMachine machine() const override {
    return static_cast<EMachine>(ehdr_->e_machine);
  }

  size_t s_size() const override { return sections_.size(); }

  std::unique_ptr<Section> s_at(size_t ndx) const override {
    const ElfNSection<BitNArch>& s = sections_.at(ndx);
    if (s.hdr->sh_type != SHT_NOBITS) {
      return std::make_unique<Section>(
          s.name, static_cast<SHType>(s.hdr->sh_type),
          static_cast<SHFlags>(s.hdr->sh_flags),
          std::make_pair(s.content.data(), s.content.size()));
    }
    return std::make_unique<Section>(s.name,
                                     static_cast<SHType>(s.hdr->sh_type),
                                     static_cast<SHFlags>(s.hdr->sh_flags));
  }

  void dump() const override {
    for (const auto& sym : sections_) {
      BinaryDecoder symTab(sym.content.data(), sym.content.size());
      while (symTab.tellg() != BinaryDecoder::eob) {
        auto* st = symTab.get<typename BitNArch::Sym>();
        if (st->st_info == STT_FUNC) {
          auto strTab =
              BinaryDecoder(sections_[sym.hdr->sh_link].content.data(),
                            sections_[sym.hdr->sh_link].content.size());
          strTab.seekg(st->st_name);
          std::cout << strTab.getc() << std::endl;
        }
      }
    }
  }

private:
  void loadPrograms(BinaryDecoder decoder) {
    if constexpr (std::is_same_v<BitNArch, Bit32Arch>) {
      programs_ = loadElf32Programs(decoder.data());
    } else if constexpr (std::is_same_v<BitNArch, Bit64Arch>) {
      programs_ = loadElf64Programs(decoder.data());
    }
  }

  void loadSections(BinaryDecoder decoder) {
    if constexpr (std::is_same_v<BitNArch, Bit32Arch>) {
      sections_ = loadElf32Sections(decoder.data());
    } else if constexpr (std::is_same_v<BitNArch, Bit64Arch>) {
      sections_ = loadElf64Sections(decoder.data());
    }
  }

private:
  const Ehdr* ehdr_;
  std::vector<ElfNProgram<BitNArch>> programs_;
  std::vector<ElfNSection<BitNArch>> sections_;
};

std::unique_ptr<Elf> createBinaryElf(const char* data, uint32_t size) {
  if (!isElfFile(data, size)) {
    return {};
  }

  BinaryDecoder decoder(data, size);
  std::string_view magic = decoder.peekn(EI_NIDENT);
  if (magic[EI_DATA] != ELFDATA2LSB) {
    // Big edition is no supported till now
    return {};
  }

  if (magic[EI_CLASS] == ELFCLASS64) {
    return std::make_unique<ElfImpl<Bit64Arch>>(decoder);
  }
  if (magic[EI_CLASS] == ELFCLASS32) {
    return std::make_unique<ElfImpl<Bit32Arch>>(decoder);
  }
  return {};
}

}} // namespace cxxaux::elf
