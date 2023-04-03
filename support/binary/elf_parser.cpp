#include "binary/elf_parser.h"
#include "llvm/BinaryFormat/ELF.h"
#include <vector>

namespace binary { namespace elf {
using namespace llvm::ELF;

struct Elf32 {
  using Ehdr = Elf32_Ehdr;
  using Shdr = Elf32_Shdr;
};

struct Elf64 {
  using Ehdr = Elf64_Ehdr;
  using Shdr = Elf64_Shdr;
};

template <typename ElfN> class BinaryElfImpl : public BinaryElf {
  using Ehdr = typename ElfN::Ehdr;
  using Shdr = typename ElfN::Shdr;

public:
  virtual ~BinaryElfImpl() noexcept = default;

  virtual const std::vector<Section>& sections() { return secs; }

  static std::unique_ptr<BinaryElfImpl> create(BinaryBuffer bb) {
    auto be = std::make_unique<BinaryElfImpl>();

    auto* ehdr = bb.peek<Ehdr>();
    // If the file has no section header table, this member(e_shoff) holds zero.
    if (ehdr->e_shoff != 0)
      be->loadSections(bb, ehdr);

    return be;
  }

private:
  void loadSections(BinaryBuffer bb, const Ehdr* ehdr) {
    bb.seekg(ehdr->e_shoff);

    auto* entry = bb.peek<Shdr>();
    // If a file has no section header table, e_shnum holds the value of zero.
    //
    // If the number of entries in the section header table is
    // larger than or equal to SHN_LORESERVE (0xff00), e_shnum
    // holds the value zero and the real number of entries in the
    // section header table is held in the sh_size member of the
    // initial entry in section header table.  Otherwise, the
    // sh_size member of the initial entry in the section header
    // table holds the value zero.
    size_t shCount = ehdr->e_shnum == 0 ? entry->sh_size : ehdr->e_shnum;
    // If the file has no section name string table, this member holds the value
    // SHN_UNDEF.
    //
    // If the index of section name string table section is
    // larger than or equal to SHN_LORESERVE (0xff00), this
    // member holds SHN_XINDEX (0xffff) and the real index of the
    // section name string table section is held in the sh_link
    // member of the initial entry in section header table.
    // Otherwise, the sh_link member of the initial entry in
    // section header table contains the value zero.
    size_t strTblIdx =
      ehdr->e_shstrndx == SHN_UNDEF ? entry->sh_link : ehdr->e_shstrndx;

    bb.seekg(ehdr->e_shoff + ehdr->e_shentsize * strTblIdx);
    auto* str = bb.get<Shdr>();
    BinaryBuffer strTbl = getSectionContent(bb, str);

    bb.seekg(ehdr->e_shoff + ehdr->e_shentsize);
    for (size_t i = 1; i < shCount; ++i) {
      auto* shdr = bb.get<Shdr>();
      secs.push_back(
        {getSectionName(strTbl, shdr), getSectionContent(bb, shdr)});
    }
  }

  std::string_view getSectionName(BinaryBuffer strTbl, const Shdr* shdr) const {
    strTbl.seekg(shdr->sh_name);
    return strTbl.peekc();
  }

  BinaryBuffer getSectionContent(BinaryBuffer bb, const Shdr* shdr) const {
    return bb.slice(shdr->sh_offset, shdr->sh_size);
  }

private:
  std::vector<Section> secs;
};

std::unique_ptr<BinaryElf> createBinaryElf(BinaryBuffer bb) {
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
    return BinaryElfImpl<Elf64>::create(bb);
  }
  if (magic[EI_CLASS] == ELFCLASS32) {
    return BinaryElfImpl<Elf32>::create(bb);
  }
  return {};
}

}}// namespace binary::elf
