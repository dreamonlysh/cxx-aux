#include "cxxaux/binary/elf.h"
#include "cxxaux/binary/elf_format.h"
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>
#include <type_traits>

using namespace cxxaux::elf;
using cxxaux::BinaryDecoder;

// ---- Enum value tests (based on elf(5) man page) ----

TEST(ElfEnum, EIClass) {
  EXPECT_EQ(static_cast<int>(EIClass::ELFCLASSNONE), 0);
  EXPECT_EQ(static_cast<int>(EIClass::ELFCLASS32), 1);
  EXPECT_EQ(static_cast<int>(EIClass::ELFCLASS64), 2);
}

TEST(ElfEnum, EIData) {
  EXPECT_EQ(static_cast<int>(EIData::ELFDATANONE), 0);
  EXPECT_EQ(static_cast<int>(EIData::ELFDATA2LSB), 1);
  EXPECT_EQ(static_cast<int>(EIData::ELFDATA2MSB), 2);
}

TEST(ElfEnum, EIVersion) {
  EXPECT_EQ(static_cast<int>(EIVersion::EV_NONE), 0);
  EXPECT_EQ(static_cast<int>(EIVersion::EV_CURRENT), 1);
}

TEST(ElfEnum, EIOSABI) {
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_NONE), 0);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_SYSV), 0);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_HPUX), 1);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_NETBSD), 2);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_LINUX), 3);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_SOLARIS), 6);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_IRIX), 8);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_FREEBSD), 9);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_TRU64), 10);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_ARM), 97);
  EXPECT_EQ(static_cast<int>(EIOSABI::ELFOSABI_STANDALONE), 255);
}

TEST(ElfEnum, EType) {
  EXPECT_EQ(static_cast<int>(EType::ET_NONE), 0);
  EXPECT_EQ(static_cast<int>(EType::ET_REL), 1);
  EXPECT_EQ(static_cast<int>(EType::ET_EXEC), 2);
  EXPECT_EQ(static_cast<int>(EType::ET_DYN), 3);
  EXPECT_EQ(static_cast<int>(EType::ET_CORE), 4);
}

TEST(ElfEnum, EMachine) {
  EXPECT_EQ(static_cast<int>(EMachine::EM_NONE), 0);
  EXPECT_EQ(static_cast<int>(EMachine::EM_386), 3);
  EXPECT_EQ(static_cast<int>(EMachine::EM_ARM), 40);
  EXPECT_EQ(static_cast<int>(EMachine::EM_X86_64), 62);
}

TEST(ElfEnum, PType) {
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_NULL), 0);
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_LOAD), 1);
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_DYNAMIC), 2);
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_INTERP), 3);
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_NOTE), 4);
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_SHLIB), 5);
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_PHDR), 6);
  EXPECT_EQ(static_cast<uint32_t>(PType::PT_GNU_STACK), 0x6474e551);
}

TEST(ElfEnum, PFlags) {
  EXPECT_EQ(static_cast<uint32_t>(PFlags::PF_X), 0x1);
  EXPECT_EQ(static_cast<uint32_t>(PFlags::PF_W), 0x2);
  EXPECT_EQ(static_cast<uint32_t>(PFlags::PF_R), 0x4);
  EXPECT_TRUE(PFlags::PF_R & PFlags::PF_R);
  EXPECT_TRUE((PFlags::PF_R | PFlags::PF_X) & PFlags::PF_X);
}

TEST(ElfEnum, SHType) {
  EXPECT_EQ(static_cast<uint32_t>(SHType::SHT_NULL), 0);
  EXPECT_EQ(static_cast<uint32_t>(SHType::SHT_PROGBITS), 1);
  EXPECT_EQ(static_cast<uint32_t>(SHType::SHT_SYMTAB), 2);
  EXPECT_EQ(static_cast<uint32_t>(SHType::SHT_STRTAB), 3);
  EXPECT_EQ(static_cast<uint32_t>(SHType::SHT_NOBITS), 8);
}

TEST(ElfEnum, SHFlags) {
  EXPECT_EQ(static_cast<uint32_t>(SHFlags::SHF_WRITE), 0x1);
  EXPECT_EQ(static_cast<uint32_t>(SHFlags::SHF_ALLOC), 0x2);
  EXPECT_EQ(static_cast<uint32_t>(SHFlags::SHF_EXECINSTR), 0x4);
  EXPECT_EQ(static_cast<uint32_t>(SHFlags::SHF_MERGE), 0x10);
  EXPECT_EQ(static_cast<uint32_t>(SHFlags::SHF_TLS), 0x400);
  EXPECT_TRUE(SHFlags::SHF_WRITE & SHFlags::SHF_WRITE);
  EXPECT_TRUE((SHFlags::SHF_WRITE | SHFlags::SHF_ALLOC) & SHFlags::SHF_ALLOC);
}

// ---- ELF format constant tests ----

TEST(ElfFormat, Constants) {
  EXPECT_EQ(EI_NIDENT, 16u);
  EXPECT_EQ(ElfMagic[0], '\x7f');
  EXPECT_EQ(ElfMagic[1], 'E');
  EXPECT_EQ(ElfMagic[2], 'L');
  EXPECT_EQ(ElfMagic[3], 'F');
  EXPECT_EQ(SHN_UNDEF, 0u);
  EXPECT_EQ(SHN_LORESERVE, 0xff00u);
  EXPECT_EQ(SHN_XINDEX, 0xffffu);
  EXPECT_EQ(PN_XNUM, 0xffffu);
  EXPECT_EQ(static_cast<uint8_t>(STType::STT_FUNC), 2u);
}

TEST(ElfFormat, StructSizes) {
  EXPECT_EQ(sizeof(Elf32_Ehdr), 52u);
  EXPECT_EQ(sizeof(Elf64_Ehdr), 64u);
  EXPECT_EQ(sizeof(Elf32_Phdr), 32u);
  EXPECT_EQ(sizeof(Elf64_Phdr), 56u);
  EXPECT_EQ(sizeof(Elf32_Shdr), 40u);
  EXPECT_EQ(sizeof(Elf64_Shdr), 64u);
  EXPECT_EQ(sizeof(Elf32_Sym), 16u);
  EXPECT_EQ(sizeof(Elf64_Sym), 24u);
}

TEST(ElfFormat, BasicTypes) {
  EXPECT_EQ(sizeof(Elf_Byte), 1u);

  EXPECT_EQ(sizeof(Elf32_Half), 2u);
  EXPECT_EQ(sizeof(Elf32_Sword), 4u);
  EXPECT_EQ(sizeof(Elf32_Word), 4u);
  EXPECT_EQ(sizeof(Elf32_Addr), 4u);
  EXPECT_EQ(sizeof(Elf32_Off), 4u);
  EXPECT_EQ(sizeof(Elf32_Section), 2u);
  EXPECT_EQ(sizeof(Elf32_Versym), 2u);

  EXPECT_EQ(sizeof(Elf64_Half), 2u);
  EXPECT_EQ(sizeof(Elf64_Sword), 4u);
  EXPECT_EQ(sizeof(Elf64_Word), 4u);
  EXPECT_EQ(sizeof(Elf64_Sxword), 8u);
  EXPECT_EQ(sizeof(Elf64_Xword), 8u);
  EXPECT_EQ(sizeof(Elf64_Addr), 8u);
  EXPECT_EQ(sizeof(Elf64_Off), 8u);
  EXPECT_EQ(sizeof(Elf64_Section), 2u);
  EXPECT_EQ(sizeof(Elf64_Versym), 2u);
}

TEST(ElfFormat, BitArchAliases) {
  EXPECT_TRUE((std::is_same_v<Bit32Arch::Addr, Elf32_Addr>));
  EXPECT_TRUE((std::is_same_v<Bit32Arch::Off, Elf32_Off>));
  EXPECT_TRUE((std::is_same_v<Bit32Arch::Half, Elf32_Half>));
  EXPECT_TRUE((std::is_same_v<Bit32Arch::Word, Elf32_Word>));
  EXPECT_TRUE((std::is_same_v<Bit32Arch::Sword, Elf32_Sword>));
  EXPECT_TRUE((std::is_same_v<Bit32Arch::Section, Elf32_Section>));
  EXPECT_TRUE((std::is_same_v<Bit32Arch::Versym, Elf32_Versym>));

  EXPECT_TRUE((std::is_same_v<Bit64Arch::Addr, Elf64_Addr>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Off, Elf64_Off>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Half, Elf64_Half>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Word, Elf64_Word>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Sword, Elf64_Sword>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Sxword, Elf64_Sxword>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Xword, Elf64_Xword>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Section, Elf64_Section>));
  EXPECT_TRUE((std::is_same_v<Bit64Arch::Versym, Elf64_Versym>));
}

// ---- isElfFile tests ----

TEST(ElfIsElfFile, ValidMagic) {
  char buf[16] = {};
  buf[0] = 0x7f;
  buf[1] = 'E';
  buf[2] = 'L';
  buf[3] = 'F';
  EXPECT_TRUE(isElfFile(buf, sizeof(buf)));
}

TEST(ElfIsElfFile, InvalidMagic) {
  char buf[16] = {};
  EXPECT_FALSE(isElfFile(buf, sizeof(buf)));
}

TEST(ElfIsElfFile, TooSmall) {
  char buf[4] = {0x7f, 'E', 'L', 'F'};
  EXPECT_FALSE(isElfFile(buf, sizeof(buf)));
  EXPECT_FALSE(isElfFile(nullptr, 0));
  EXPECT_FALSE(isElfFile(nullptr, 15));
}

TEST(ElfIsElfFile, PartialMagic) {
  char buf[16] = {};
  buf[0] = 0x7f;
  buf[1] = 'E';
  buf[2] = 'L';
  EXPECT_FALSE(isElfFile(buf, sizeof(buf)));
}

// ---- Minimal ELF binary construction helpers ----

static const char kShstrtab64[] = "\0.shstrtab\0.text\0.data\0.bss\0";
static constexpr size_t kShstrtab64Size = sizeof(kShstrtab64) - 1;

static const char kShstrtab32[] = "\0.shstrtab\0.text\0";
static constexpr size_t kShstrtab32Size = sizeof(kShstrtab32) - 1;

static std::vector<char> makeMinimalElf64() {
  size_t ehdrOff = 0;
  size_t phdrOff = sizeof(Elf64_Ehdr);
  size_t shdrOff = phdrOff + sizeof(Elf64_Phdr);
  size_t shstrtabOff = shdrOff + 4 * sizeof(Elf64_Shdr);
  size_t totalSize = shstrtabOff + kShstrtab64Size;

  std::vector<char> buf(totalSize, 0);

  Elf64_Ehdr ehdr = {};
  ehdr.e_ident[EI_MAG0] = 0x7f;
  ehdr.e_ident[EI_MAG1] = 'E';
  ehdr.e_ident[EI_MAG2] = 'L';
  ehdr.e_ident[EI_MAG3] = 'F';
  ehdr.e_ident[EI_CLASS] = static_cast<uint8_t>(EIClass::ELFCLASS64);
  ehdr.e_ident[EI_DATA] = static_cast<uint8_t>(EIData::ELFDATA2LSB);
  ehdr.e_ident[EI_VERSION] = static_cast<uint8_t>(EIVersion::EV_CURRENT);
  ehdr.e_type = static_cast<uint16_t>(EType::ET_EXEC);
  ehdr.e_machine = static_cast<uint16_t>(EMachine::EM_X86_64);
  ehdr.e_version = static_cast<uint32_t>(EIVersion::EV_CURRENT);
  ehdr.e_entry = 0x400000;
  ehdr.e_phoff = phdrOff;
  ehdr.e_shoff = shdrOff;
  ehdr.e_ehsize = sizeof(Elf64_Ehdr);
  ehdr.e_phentsize = sizeof(Elf64_Phdr);
  ehdr.e_phnum = 1;
  ehdr.e_shentsize = sizeof(Elf64_Shdr);
  ehdr.e_shnum = 4;
  ehdr.e_shstrndx = 1;
  memcpy(buf.data() + ehdrOff, &ehdr, sizeof(Elf64_Ehdr));

  Elf64_Phdr phdr = {};
  phdr.p_type = static_cast<uint32_t>(PType::PT_LOAD);
  phdr.p_flags =
      static_cast<uint32_t>(PFlags::PF_R) | static_cast<uint32_t>(PFlags::PF_X);
  phdr.p_offset = 0;
  phdr.p_vaddr = 0x400000;
  phdr.p_paddr = 0x400000;
  phdr.p_filesz = totalSize;
  phdr.p_memsz = totalSize;
  phdr.p_align = 0x1000;
  memcpy(buf.data() + phdrOff, &phdr, sizeof(Elf64_Phdr));

  Elf64_Shdr shdrs[4] = {};

  shdrs[0].sh_name = 0;
  shdrs[0].sh_type = static_cast<uint32_t>(SHType::SHT_NULL);

  shdrs[1].sh_name = 1;
  shdrs[1].sh_type = static_cast<uint32_t>(SHType::SHT_STRTAB);
  shdrs[1].sh_offset = shstrtabOff;
  shdrs[1].sh_size = kShstrtab64Size;

  shdrs[2].sh_name = 11;
  shdrs[2].sh_type = static_cast<uint32_t>(SHType::SHT_PROGBITS);
  shdrs[2].sh_flags = static_cast<uint64_t>(SHFlags::SHF_ALLOC) |
                      static_cast<uint64_t>(SHFlags::SHF_EXECINSTR);
  shdrs[2].sh_offset = 0;
  shdrs[2].sh_size = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);

  shdrs[3].sh_name = 17;
  shdrs[3].sh_type = static_cast<uint32_t>(SHType::SHT_NOBITS);
  shdrs[3].sh_flags = static_cast<uint64_t>(SHFlags::SHF_ALLOC) |
                      static_cast<uint64_t>(SHFlags::SHF_WRITE);
  shdrs[3].sh_size = 0x100;

  memcpy(buf.data() + shdrOff, shdrs, sizeof(shdrs));
  memcpy(buf.data() + shstrtabOff, kShstrtab64, kShstrtab64Size);

  return buf;
}

static std::vector<char> makeMinimalElf32() {
  size_t ehdrOff = 0;
  size_t phdrOff = sizeof(Elf32_Ehdr);
  size_t shdrOff = phdrOff + sizeof(Elf32_Phdr);
  size_t shstrtabOff = shdrOff + 3 * sizeof(Elf32_Shdr);
  size_t totalSize = shstrtabOff + kShstrtab32Size;

  std::vector<char> buf(totalSize, 0);

  Elf32_Ehdr ehdr = {};
  ehdr.e_ident[EI_MAG0] = 0x7f;
  ehdr.e_ident[EI_MAG1] = 'E';
  ehdr.e_ident[EI_MAG2] = 'L';
  ehdr.e_ident[EI_MAG3] = 'F';
  ehdr.e_ident[EI_CLASS] = static_cast<uint8_t>(EIClass::ELFCLASS32);
  ehdr.e_ident[EI_DATA] = static_cast<uint8_t>(EIData::ELFDATA2LSB);
  ehdr.e_ident[EI_VERSION] = static_cast<uint8_t>(EIVersion::EV_CURRENT);
  ehdr.e_type = static_cast<uint16_t>(EType::ET_DYN);
  ehdr.e_machine = static_cast<uint16_t>(EMachine::EM_386);
  ehdr.e_version = static_cast<uint32_t>(EIVersion::EV_CURRENT);
  ehdr.e_entry = 0x08048000;
  ehdr.e_phoff = phdrOff;
  ehdr.e_shoff = shdrOff;
  ehdr.e_ehsize = sizeof(Elf32_Ehdr);
  ehdr.e_phentsize = sizeof(Elf32_Phdr);
  ehdr.e_phnum = 1;
  ehdr.e_shentsize = sizeof(Elf32_Shdr);
  ehdr.e_shnum = 3;
  ehdr.e_shstrndx = 1;
  memcpy(buf.data() + ehdrOff, &ehdr, sizeof(Elf32_Ehdr));

  Elf32_Phdr phdr = {};
  phdr.p_type = static_cast<uint32_t>(PType::PT_LOAD);
  phdr.p_flags = static_cast<uint32_t>(PFlags::PF_R);
  phdr.p_offset = 0;
  phdr.p_vaddr = 0x08048000;
  phdr.p_paddr = 0x08048000;
  phdr.p_filesz = totalSize;
  phdr.p_memsz = totalSize;
  phdr.p_align = 0x1000;
  memcpy(buf.data() + phdrOff, &phdr, sizeof(Elf32_Phdr));

  Elf32_Shdr shdrs[3] = {};

  shdrs[0].sh_name = 0;
  shdrs[0].sh_type = static_cast<uint32_t>(SHType::SHT_NULL);

  shdrs[1].sh_name = 1;
  shdrs[1].sh_type = static_cast<uint32_t>(SHType::SHT_STRTAB);
  shdrs[1].sh_offset = shstrtabOff;
  shdrs[1].sh_size = kShstrtab32Size;

  shdrs[2].sh_name = 11;
  shdrs[2].sh_type = static_cast<uint32_t>(SHType::SHT_PROGBITS);
  shdrs[2].sh_flags = static_cast<uint32_t>(SHFlags::SHF_ALLOC) |
                      static_cast<uint32_t>(SHFlags::SHF_EXECINSTR);
  shdrs[2].sh_offset = 0;
  shdrs[2].sh_size = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr);

  memcpy(buf.data() + shdrOff, shdrs, sizeof(shdrs));
  memcpy(buf.data() + shstrtabOff, kShstrtab32, kShstrtab32Size);

  return buf;
}

// ---- parseElf tests ----

TEST(ElfCreate, Elf64) {
  auto buf = makeMinimalElf64();
  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  EXPECT_EQ(elf->archBits(), EIClass::ELFCLASS64);
  EXPECT_EQ(elf->encoding(), EIData::ELFDATA2LSB);
  EXPECT_EQ(elf->type(), EType::ET_EXEC);
  EXPECT_EQ(elf->machine(), EMachine::EM_X86_64);
}

TEST(ElfCreate, Elf32) {
  auto buf = makeMinimalElf32();
  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  EXPECT_EQ(elf->archBits(), EIClass::ELFCLASS32);
  EXPECT_EQ(elf->encoding(), EIData::ELFDATA2LSB);
  EXPECT_EQ(elf->type(), EType::ET_DYN);
  EXPECT_EQ(elf->machine(), EMachine::EM_386);
}

TEST(ElfCreate, InvalidMagic) {
  std::vector<char> buf(64, 0);
  auto elf = parseElf(buf.data(), buf.size());
  EXPECT_EQ(elf, nullptr);
}

TEST(ElfCreate, TooSmall) {
  char buf[4] = {0x7f, 'E', 'L', 'F'};
  auto elf = parseElf(buf, sizeof(buf));
  EXPECT_EQ(elf, nullptr);
}

TEST(ElfCreate, BigEndianRejected) {
  auto buf = makeMinimalElf64();
  buf[EI_DATA] = static_cast<uint8_t>(EIData::ELFDATA2MSB);
  auto elf = parseElf(buf.data(), buf.size());
  EXPECT_EQ(elf, nullptr);
}

TEST(ElfCreate, InvalidClass) {
  auto buf = makeMinimalElf64();
  buf[EI_CLASS] = 0;
  auto elf = parseElf(buf.data(), buf.size());
  EXPECT_EQ(elf, nullptr);
}

// ---- Section tests ----

TEST(ElfSection, Elf64Sections) {
  auto buf = makeMinimalElf64();
  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  ASSERT_EQ(elf->s_size(), 4u);

  auto s0 = elf->s_at(0);
  EXPECT_EQ(s0->type(), SHType::SHT_NULL);
  EXPECT_EQ(s0->name(), "");

  auto s1 = elf->s_at(1);
  EXPECT_EQ(s1->type(), SHType::SHT_STRTAB);
  EXPECT_EQ(s1->name(), ".shstrtab");

  auto s2 = elf->s_at(2);
  EXPECT_EQ(s2->type(), SHType::SHT_PROGBITS);
  EXPECT_EQ(s2->name(), ".text");
  EXPECT_TRUE(s2->flags() & SHFlags::SHF_EXECINSTR);
  EXPECT_TRUE(s2->flags() & SHFlags::SHF_ALLOC);
  EXPECT_GT(s2->content().second, 0u);

  auto s3 = elf->s_at(3);
  EXPECT_EQ(s3->type(), SHType::SHT_NOBITS);
  EXPECT_EQ(s3->name(), ".data");
  EXPECT_TRUE(s3->flags() & SHFlags::SHF_WRITE);
  EXPECT_EQ(s3->content().second, 0u);
}

TEST(ElfSection, Elf32Sections) {
  auto buf = makeMinimalElf32();
  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  ASSERT_EQ(elf->s_size(), 3u);

  auto s0 = elf->s_at(0);
  EXPECT_EQ(s0->type(), SHType::SHT_NULL);

  auto s1 = elf->s_at(1);
  EXPECT_EQ(s1->type(), SHType::SHT_STRTAB);
  EXPECT_EQ(s1->name(), ".shstrtab");

  auto s2 = elf->s_at(2);
  EXPECT_EQ(s2->type(), SHType::SHT_PROGBITS);
  EXPECT_EQ(s2->name(), ".text");
}

TEST(ElfSection, NoBitsHasNoContent) {
  auto buf = makeMinimalElf64();
  auto elf = parseElf(buf.data(), buf.size());
  auto s = elf->s_at(3);
  EXPECT_EQ(s->type(), SHType::SHT_NOBITS);
  EXPECT_EQ(s->content().first, nullptr);
  EXPECT_EQ(s->content().second, 0u);
}

TEST(ElfSection, OutOfRange) {
  auto buf = makeMinimalElf64();
  auto elf = parseElf(buf.data(), buf.size());
  EXPECT_THROW(elf->s_at(999), std::out_of_range);
}

// ---- Program tests ----

TEST(ElfProgram, Elf64Programs) {
  auto buf = makeMinimalElf64();
  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  ASSERT_EQ(elf->p_size(), 1u);

  auto p = elf->p_at(0);
  EXPECT_EQ(p->type(), PType::PT_LOAD);
  EXPECT_TRUE(p->flags() & PFlags::PF_R);
  EXPECT_TRUE(p->flags() & PFlags::PF_X);
  EXPECT_EQ(p->offset(), 0u);
  EXPECT_EQ(p->vaddr(), 0x400000u);
  EXPECT_EQ(p->paddr(), 0x400000u);
  EXPECT_GT(p->filesz(), 0u);
  EXPECT_EQ(p->filesz(), p->memsz());
  EXPECT_EQ(p->align(), 0x1000u);
}

TEST(ElfProgram, Elf32Programs) {
  auto buf = makeMinimalElf32();
  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  ASSERT_EQ(elf->p_size(), 1u);

  auto p = elf->p_at(0);
  EXPECT_EQ(p->type(), PType::PT_LOAD);
  EXPECT_TRUE(p->flags() & PFlags::PF_R);
  EXPECT_EQ(p->vaddr(), 0x08048000u);
}

TEST(ElfProgram, OutOfRange) {
  auto buf = makeMinimalElf64();
  auto elf = parseElf(buf.data(), buf.size());
  EXPECT_THROW(elf->p_at(999), std::out_of_range);
}

// ---- No program headers ----

TEST(ElfNoProgram, NoPhdr) {
  size_t ehdrOff = 0;
  size_t shdrOff = sizeof(Elf64_Ehdr);
  size_t totalSize = shdrOff + sizeof(Elf64_Shdr);

  std::vector<char> buf(totalSize, 0);

  Elf64_Ehdr ehdr = {};
  ehdr.e_ident[EI_MAG0] = 0x7f;
  ehdr.e_ident[EI_MAG1] = 'E';
  ehdr.e_ident[EI_MAG2] = 'L';
  ehdr.e_ident[EI_MAG3] = 'F';
  ehdr.e_ident[EI_CLASS] = static_cast<uint8_t>(EIClass::ELFCLASS64);
  ehdr.e_ident[EI_DATA] = static_cast<uint8_t>(EIData::ELFDATA2LSB);
  ehdr.e_type = static_cast<uint16_t>(EType::ET_REL);
  ehdr.e_machine = static_cast<uint16_t>(EMachine::EM_X86_64);
  ehdr.e_version = static_cast<uint32_t>(EIVersion::EV_CURRENT);
  ehdr.e_ehsize = sizeof(Elf64_Ehdr);
  ehdr.e_phoff = 0;
  ehdr.e_phentsize = sizeof(Elf64_Phdr);
  ehdr.e_phnum = 0;
  ehdr.e_shoff = shdrOff;
  ehdr.e_shentsize = sizeof(Elf64_Shdr);
  ehdr.e_shnum = 1;
  ehdr.e_shstrndx = 0;
  memcpy(buf.data() + ehdrOff, &ehdr, sizeof(Elf64_Ehdr));

  Elf64_Shdr shdr = {};
  shdr.sh_type = static_cast<uint32_t>(SHType::SHT_NULL);
  memcpy(buf.data() + shdrOff, &shdr, sizeof(Elf64_Shdr));

  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  EXPECT_EQ(elf->p_size(), 0u);
  EXPECT_EQ(elf->s_size(), 1u);
}

// ---- No section headers ----

TEST(ElfNoSection, NoShdr) {
  size_t totalSize = sizeof(Elf64_Ehdr);
  std::vector<char> buf(totalSize, 0);

  Elf64_Ehdr ehdr = {};
  ehdr.e_ident[EI_MAG0] = 0x7f;
  ehdr.e_ident[EI_MAG1] = 'E';
  ehdr.e_ident[EI_MAG2] = 'L';
  ehdr.e_ident[EI_MAG3] = 'F';
  ehdr.e_ident[EI_CLASS] = static_cast<uint8_t>(EIClass::ELFCLASS64);
  ehdr.e_ident[EI_DATA] = static_cast<uint8_t>(EIData::ELFDATA2LSB);
  ehdr.e_type = static_cast<uint16_t>(EType::ET_CORE);
  ehdr.e_machine = static_cast<uint16_t>(EMachine::EM_X86_64);
  ehdr.e_version = static_cast<uint32_t>(EIVersion::EV_CURRENT);
  ehdr.e_ehsize = sizeof(Elf64_Ehdr);
  ehdr.e_phoff = 0;
  ehdr.e_shoff = 0;
  ehdr.e_shentsize = sizeof(Elf64_Shdr);
  ehdr.e_shnum = 0;
  ehdr.e_shstrndx = 0;
  memcpy(buf.data(), &ehdr, sizeof(Elf64_Ehdr));

  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  EXPECT_EQ(elf->type(), EType::ET_CORE);
  EXPECT_EQ(elf->s_size(), 0u);
  EXPECT_EQ(elf->p_size(), 0u);
}

// ---- Section class API ----

TEST(SectionClass, Basic) {
  const char data[] = "hello";
  Section s("test", SHType::SHT_PROGBITS, SHFlags::SHF_ALLOC,
            {data, sizeof(data)});
  EXPECT_EQ(s.name(), "test");
  EXPECT_EQ(s.type(), SHType::SHT_PROGBITS);
  EXPECT_TRUE(s.flags() & SHFlags::SHF_ALLOC);
  EXPECT_EQ(s.content().first, data);
  EXPECT_EQ(s.content().second, sizeof(data));
}

TEST(SectionClass, NoBitsDefault) {
  Section s(".bss", SHType::SHT_NOBITS, SHFlags::SHF_WRITE);
  EXPECT_EQ(s.content().first, nullptr);
  EXPECT_EQ(s.content().second, 0u);
}

// ---- Program class API ----

TEST(ProgramClass, Basic) {
  Program p(PType::PT_LOAD, PFlags::PF_R | PFlags::PF_W, 0x1000, 0x400000,
            0x400000, 0x200, 0x300, 0x1000);
  EXPECT_EQ(p.type(), PType::PT_LOAD);
  EXPECT_TRUE(p.flags() & PFlags::PF_R);
  EXPECT_TRUE(p.flags() & PFlags::PF_W);
  EXPECT_EQ(p.offset(), 0x1000u);
  EXPECT_EQ(p.vaddr(), 0x400000u);
  EXPECT_EQ(p.paddr(), 0x400000u);
  EXPECT_EQ(p.filesz(), 0x200u);
  EXPECT_EQ(p.memsz(), 0x300u);
  EXPECT_EQ(p.align(), 0x1000u);
}

// ---- Multiple program headers ----

TEST(ElfProgram, MultiplePhdrs) {
  size_t ehdrOff = 0;
  size_t phdrOff = sizeof(Elf64_Ehdr);
  size_t shdrOff = phdrOff + 3 * sizeof(Elf64_Phdr);
  size_t totalSize = shdrOff + sizeof(Elf64_Shdr);

  std::vector<char> buf(totalSize, 0);

  Elf64_Ehdr ehdr = {};
  ehdr.e_ident[EI_MAG0] = 0x7f;
  ehdr.e_ident[EI_MAG1] = 'E';
  ehdr.e_ident[EI_MAG2] = 'L';
  ehdr.e_ident[EI_MAG3] = 'F';
  ehdr.e_ident[EI_CLASS] = static_cast<uint8_t>(EIClass::ELFCLASS64);
  ehdr.e_ident[EI_DATA] = static_cast<uint8_t>(EIData::ELFDATA2LSB);
  ehdr.e_type = static_cast<uint16_t>(EType::ET_EXEC);
  ehdr.e_machine = static_cast<uint16_t>(EMachine::EM_X86_64);
  ehdr.e_version = static_cast<uint32_t>(EIVersion::EV_CURRENT);
  ehdr.e_ehsize = sizeof(Elf64_Ehdr);
  ehdr.e_phoff = phdrOff;
  ehdr.e_phentsize = sizeof(Elf64_Phdr);
  ehdr.e_phnum = 3;
  ehdr.e_shoff = shdrOff;
  ehdr.e_shentsize = sizeof(Elf64_Shdr);
  ehdr.e_shnum = 1;
  ehdr.e_shstrndx = 0;
  memcpy(buf.data() + ehdrOff, &ehdr, sizeof(Elf64_Ehdr));

  Elf64_Phdr phdrs[3] = {};
  phdrs[0].p_type = static_cast<uint32_t>(PType::PT_LOAD);
  phdrs[0].p_flags =
      static_cast<uint32_t>(PFlags::PF_R) | static_cast<uint32_t>(PFlags::PF_X);
  phdrs[0].p_vaddr = 0x400000;
  phdrs[0].p_memsz = 0x1000;
  phdrs[0].p_align = 0x1000;

  phdrs[1].p_type = static_cast<uint32_t>(PType::PT_LOAD);
  phdrs[1].p_flags =
      static_cast<uint32_t>(PFlags::PF_R) | static_cast<uint32_t>(PFlags::PF_W);
  phdrs[1].p_vaddr = 0x600000;
  phdrs[1].p_memsz = 0x100;
  phdrs[1].p_align = 0x1000;

  phdrs[2].p_type = static_cast<uint32_t>(PType::PT_GNU_STACK);
  phdrs[2].p_flags =
      static_cast<uint32_t>(PFlags::PF_R) | static_cast<uint32_t>(PFlags::PF_W);
  phdrs[2].p_align = 0x10;
  memcpy(buf.data() + phdrOff, phdrs, sizeof(phdrs));

  Elf64_Shdr shdr = {};
  shdr.sh_type = static_cast<uint32_t>(SHType::SHT_NULL);
  memcpy(buf.data() + shdrOff, &shdr, sizeof(Elf64_Shdr));

  auto elf = parseElf(buf.data(), buf.size());
  ASSERT_TRUE(elf != nullptr);
  ASSERT_EQ(elf->p_size(), 3u);

  auto p0 = elf->p_at(0);
  EXPECT_EQ(p0->type(), PType::PT_LOAD);
  EXPECT_TRUE(p0->flags() & PFlags::PF_X);

  auto p1 = elf->p_at(1);
  EXPECT_EQ(p1->type(), PType::PT_LOAD);
  EXPECT_TRUE(p1->flags() & PFlags::PF_W);

  auto p2 = elf->p_at(2);
  EXPECT_EQ(p2->type(), PType::PT_GNU_STACK);
}

// ---- Load self binary ----

TEST(ElfSelf, LoadSelfBinary) {
#if defined(__APPLE__)
  GTEST_SKIP() << "macOS uses Mach-O format, not ELF";
#else
  std::ifstream fs("/proc/self/exe", std::ios::binary);
  if (!fs.is_open())
    GTEST_SKIP() << "/proc/self/exe not available on this platform";

  std::string content{std::istreambuf_iterator<char>(fs),
                      std::istreambuf_iterator<char>()};
  if (content.size() < 16)
    GTEST_SKIP() << "Self binary too small";

  EXPECT_TRUE(isElfFile(content.data(), content.size()));

  auto elf = parseElf(content.data(), content.size());
  ASSERT_TRUE(elf != nullptr);
  EXPECT_NE(elf->archBits(), EIClass::ELFCLASSNONE);
  EXPECT_EQ(elf->encoding(), EIData::ELFDATA2LSB);
  EXPECT_GT(elf->s_size(), 0u);
#endif
}
