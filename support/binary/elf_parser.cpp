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
    if (ehdr->e_shoff != 0)
      be->loadSections(bb, ehdr);

    return be;
  }

private:
  void loadSections(BinaryBuffer bb, const Ehdr* ehdr) {
    bb.seekg(ehdr->e_shoff);

    auto* entry = bb.get<Shdr>();
    size_t shCount = ehdr->e_shnum == 0 ? entry->sh_size : ehdr->e_shnum;
    std::vector<const Shdr*> ss;
    ss.reserve(shCount);
    ss.push_back(entry);
    for (size_t i = 1; i < shCount; ++i) {
      auto* shdr = bb.get<Shdr>();
      ss.push_back(shdr);
    }

    size_t shStrIdx = ehdr->e_shstrndx == 0 ? entry->sh_link : ehdr->e_shstrndx;
    auto* shstrtabidx = ss[shStrIdx];
    BinaryBuffer shstrtab(
      bb.slice(shstrtabidx->sh_offset, shstrtabidx->sh_size));
    for (size_t i = 1; i < ss.size(); ++i) {
      shstrtab.seekg(ss[i]->sh_name);
      secs.push_back(
        {shstrtab.peekc(), bb.slice(ss[i]->sh_offset, ss[i]->sh_size)});
    }
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
