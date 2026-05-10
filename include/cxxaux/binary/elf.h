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

#ifndef CXXAUX_BINARY_ELF_H
#define CXXAUX_BINARY_ELF_H

#include "elf_format.h"
#include "binary_decoder.h"
#include <iosfwd>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

namespace cxxaux { namespace elf {

#define IMPORT_ELF_DETAIL
#include "elf.ini"

/// Check whether the given data starts with the ELF magic bytes.
///
/// @param data  Pointer to the raw file data.
/// @param size  Size of the data in bytes.
/// @return true if the first 4 bytes match \\x7fELF.
inline bool isElfFile(const char* data, size_t size) {
  if (size < EI_NIDENT)
    return false;
  return memcmp(data, ElfMagic, sizeof(ElfMagic)) == 0;
}

/// Represents a parsed ELF section with its name, type, flags, and
/// optional content. For SHT_NOBITS sections (e.g. .bss), content()
/// returns an empty pair.
class Section {
public:
  Section(std::string_view name, SHType type, SHFlags flags,
          std::pair<const char*, size_t> content = {})
      : name_(name), type_(type), flags_(flags), content_(content) {}
  ~Section() noexcept = default;

  /// Section name from the section header string table.
  std::string_view name() const { return name_; }

  /// Section type (SHT_PROGBITS, SHT_NOBITS, etc.).
  SHType type() const { return type_; }

  /// Section flags (SHF_WRITE, SHF_ALLOC, SHF_EXECINSTR, etc.).
  SHFlags flags() const { return flags_; }

  /// Raw section content as (pointer, size). Empty for SHT_NOBITS.
  std::pair<const char*, size_t> content() const { return content_; }

private:
  std::string_view name_;
  SHType type_;
  SHFlags flags_;
  std::pair<const char*, size_t> content_;
};

/// Represents a parsed ELF program header (segment) with its type,
/// flags, and layout information.
class Program {
public:
  explicit Program(PType type, PFlags flags, uint64_t offset, uint64_t vaddr,
                   uint64_t paddr, uint64_t filesz, uint64_t memsz,
                   uint64_t align)
      : type_(type), flags_(flags), offset_(offset), vaddr_(vaddr),
        paddr_(paddr), filesz_(filesz), memsz_(memsz), align_(align) {}
  ~Program() noexcept = default;

  /// Segment type (PT_LOAD, PT_DYNAMIC, etc.).
  PType type() const { return type_; }

  /// Segment flags (PF_R, PF_W, PF_X).
  PFlags flags() const { return flags_; }

  /// File offset of the segment start.
  uint64_t offset() const { return offset_; }

  /// Virtual address of the segment in memory.
  uint64_t vaddr() const { return vaddr_; }

  /// Physical address (reserved, usually 0 on modern systems).
  uint64_t paddr() const { return paddr_; }

  /// Size of the segment in the file image.
  uint64_t filesz() const { return filesz_; }

  /// Size of the segment in the memory image (>= filesz).
  uint64_t memsz() const { return memsz_; }

  /// Alignment constraint for the segment.
  uint64_t align() const { return align_; }

private:
  PType type_;
  PFlags flags_;
  uint64_t offset_;
  uint64_t vaddr_;
  uint64_t paddr_;
  uint64_t filesz_;
  uint64_t memsz_;
  uint64_t align_;
};

/// Abstract interface for a parsed ELF file. Provides access to
/// header metadata, sections, and program headers.
class Elf {
public:
  virtual ~Elf() noexcept = default;

  /// Architecture class (32-bit or 64-bit).
  virtual EIClass archBits() const = 0;

  /// Data encoding (little-endian or big-endian).
  virtual EIData encoding() const = 0;

  /// Object file type (executable, shared object, etc.).
  virtual EType type() const = 0;

  /// Required machine architecture.
  virtual EMachine machine() const = 0;

  /// Number of sections in the ELF file.
  virtual size_t s_size() const = 0;

  /// Access section at index @p ndx. Returns nullptr if out of range.
  virtual std::unique_ptr<Section> s_at(size_t ndx) const = 0;

  /// Number of program headers in the ELF file.
  virtual size_t p_size() const = 0;

  /// Access program header at index @p ndx. Returns nullptr if out of range.
  virtual std::unique_ptr<Program> p_at(size_t ndx) const = 0;

  /// Dump all function symbols to the given output stream (debug utility).
  template <typename OStream>
  void dump(OStream& os) const {
    dumpImpl(&os);
  }

private:
  virtual void dumpImpl(void* os) const = 0;
};

#define IMPORT_ELF_IMPL
#include "elf.ini"

/// Parse a raw ELF32 binary.
///
/// @param data  Pointer to the raw file data.
/// @param size  Size of the data in bytes.
/// @return Owning pointer to the parsed Elf, or nullptr on failure.
inline std::unique_ptr<Elf> parseElf32(const char* data, uint32_t size) {
  BinaryDecoder decoder(data, size);
  return std::make_unique<ElfImpl<Bit32Arch>>(decoder);
}

/// Parse a raw ELF64 binary.
///
/// @param data  Pointer to the raw file data.
/// @param size  Size of the data in bytes.
/// @return Owning pointer to the parsed Elf, or nullptr on failure.
inline std::unique_ptr<Elf> parseElf64(const char* data, uint32_t size) {
  BinaryDecoder decoder(data, size);
  return std::make_unique<ElfImpl<Bit64Arch>>(decoder);
}

/// Parse a raw ELF binary and return an Elf object.
///
/// Only little-endian ELF32 and ELF64 files are supported. If the data
/// does not contain a valid ELF magic, is big-endian, or has an unknown
/// class, returns nullptr.
///
/// @param data  Pointer to the raw file data.
/// @param size  Size of the data in bytes.
/// @return Owning pointer to the parsed Elf, or nullptr on failure.
inline std::unique_ptr<Elf> parseElf(const char* data, uint32_t size) {
  if (!isElfFile(data, size)) {
    return {};
  }

  if (data[EI_DATA] != static_cast<uint8_t>(EIData::ELFDATA2LSB)) {
    return {};
  }

  if (data[EI_CLASS] == static_cast<uint8_t>(EIClass::ELFCLASS64)) {
    return parseElf64(data, size);
  }
  if (data[EI_CLASS] == static_cast<uint8_t>(EIClass::ELFCLASS32)) {
    return parseElf32(data, size);
  }
  return {};
}

}} // namespace cxxaux::elf

#endif // CXXAUX_BINARY_ELF_H
