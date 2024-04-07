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
//
// The macros are from https://man7.org/linux/man-pages/man5/elf.5.html

#ifndef BINARY_ELF_MACRO_H
#define BINARY_ELF_MACRO_H
#include <cstdint>

namespace cxxaux { namespace elf {

// Architecture for this binary:
enum class EIClass {
  ELFCLASSNONE = 0, // This class is invalid.
  ELFCLASS32 = 1,   // This defines the 32-bit architecture.  It
                    //  supports machines with files and virtual
                    //  address spaces up to 4 Gigabytes.
  ELFCLASS64 = 2    // This defines the 64-bit architecture.
};

// The data encoding of the processor-specific data in the file.
// Currently, these encodings are supported:
enum class EIData {
  ELFDATANONE = 0, // Unknown data format.
  ELFDATA2LSB = 1, // Two's complement, little-endian.
  ELFDATA2MSB = 2  // Two's complement, big-endian.
};

// This member of the structure identifies the object file type:
enum class EType {
  ET_NONE = 0, // An unknown type.
  ET_REL = 1,  // A relocatable file.
  ET_EXEC = 2, // An executable file.
  ET_DYN = 3,  // A shared object.
  ET_CORE = 4, // A core file.
};

//               This member specifies the required architecture for an
//              individual file.  For example:
enum class EMachine {
  EM_NONE = 0,         // An unknown machine
  EM_M32 = 1,          // AT&T WE 32100
  EM_SPARC = 2,        // Sun Microsystems SPARC
  EM_386 = 3,          // Intel 80386
  EM_68K = 4,          // Motorola 68000
  EM_88K = 5,          // Motorola 88000
  EM_860 = 7,          // Intel 80860
  EM_MIPS = 8,         // MIPS RS3000 (big-endian only)
  EM_PARISC = 15,      // HP/PA
  EM_SPARC32PLUS = 18, // SPARC with enhanced instruction set
  EM_PPC = 20,         // PowerPC
  EM_PPC64 = 21,       // PowerPC 64-bit
  EM_S390 = 22,        // IBM S/390
  EM_ARM = 40,         // Advanced RISC Machines
  EM_SH = 42,          // Renesas SuperH
  EM_SPARCV9 = 43,     // SPARC v9 64-bit
  EM_IA_64 = 50,       // Intel Itanium
  EM_X86_64 = 62,      // AMD x86-64
  EM_VAX = 75          // DEC Vax
};

// This member categorizes the section's contents and semantics.
enum class SHType : uint32_t {
  SHT_NULL = 0,     // This value marks the section header as inactive.
                    // It does not have an associated section.  Other
                    // members of the section header have undefined
                    // values.
  SHT_PROGBITS = 1, // This section holds information defined by the
                    // program, whose format and meaning are determined
                    // solely by the program.
  SHT_SYMTAB = 2,   // This section holds a symbol table.  Typically,
                    // SHT_SYMTAB provides symbols for link editing,
                    // though it may also be used for dynamic linking.  As
                    // a complete symbol table, it may contain many
                    // symbols unnecessary for dynamic linking.  An object
                    // file can also contain a SHT_DYNSYM section.
  SHT_STRTAB = 3,   // This section holds a string table.  An object file
                    // may have multiple string table sections.
  SHT_RELA = 4,     // This section holds relocation entries with explicit
                    // addends, such as type Elf32_Rela for the 32-bit
                    // class of object files.  An object may have multiple
                    // relocation sections.
  SHT_HASH = 5,     // This section holds a symbol hash table.  An object
                    // participating in dynamic linking must contain a
                    // symbol hash table.  An object file may have only
                    // one hash table.
  SHT_DYNAMIC = 6,  // This section holds information for dynamic linking.
                    // An object file may have only one dynamic section.
  SHT_NOTE = 7,     // This section holds notes (ElfN_Nhdr).
  SHT_NOBITS = 8,   // A section of this type occupies no space in the
                    // file but otherwise resembles SHT_PROGBITS.
                    // Although this section contains no bytes, the
                    // sh_offset member contains the conceptual file
                    // offset.
  SHT_REL = 9,      // This section holds relocation offsets without
                    // explicit addends, such as type Elf32_Rel for the
                    // 32-bit class of object files.  An object file may
                    // have multiple relocation sections.
  SHT_SHLIB = 10,   // This section is reserved but has unspecified
                    // semantics.
  SHT_DYNSYM = 11,  // This section holds a minimal set of dynamic linking
                    // symbols.  An object file can also contain a
                    // SHT_SYMTAB section.
  SHT_LOPROC = 0x70000000,
  SHT_HIPROC = 0x7fffffff, // Values in the inclusive range [SHT_LOPROC,
                           // SHT_HIPROC] are reserved for processor-specific
                           // semantics.
  SHT_LOUSER = 0x80000000, // This value specifies the lower bound of the range
                           // of indices reserved for application programs.
  SHT_HIUSER = 0xffffffff, // This value specifies the upper bound of the range
                           // of indices reserved for application programs.
                           // Section types between SHT_LOUSER and SHT_HIUSER
                           // may be used by the application, without
                           // conflicting with current or future system-defined
                           // section types.
};

// Sections support one-bit flags that describe miscellaneous attributes.  If a
// flag bit is set in sh_flags, the attribute is "on" for the section.
// Otherwise, the attribute is "off" or does not apply.  Undefined attributes
// are set to zero.
enum class SHFlags : uint32_t {
  SHF_WRITE = 0x1,     // This section contains data that should be writable
                       // during process execution.
  SHF_ALLOC = 0x2,     // This section occupies memory during process
                       // execution.  Some control sections do not reside in
                       // the memory image of an object file.  This attribute
                       // is off for those sections.
  SHF_EXECINSTR = 0x4, // This section contains executable machine
                       // instructions.
  SHF_MASKPROC = 0xf0000000 // All bits included in this mask are reserved for
                            // processor-specific semantics.
};

inline bool operator&(const SHFlags& lhs, const SHFlags& rhs) {
  return static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs);
}

}}     // namespace cxxaux::elf
#endif // BINARY_ELF_MACRO_H
