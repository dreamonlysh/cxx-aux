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

#ifndef BINARY_ELF_H
#define BINARY_ELF_H
#include "binary/buffer.h"
#include "elf_macro.h"
#include <memory>

namespace binary {

class Section {
public:
  Section(std::string_view s_name, SHType s_type, SHFlags s_flags)
      : s_name(s_name), s_type(s_type), s_flags(s_flags) {}

  virtual ~Section() noexcept = default;

  template <typename T>
  const T* dynCast() {
    if (T::instanceOf(*this))
      return static_cast<const T*>(this);
    return nullptr;
  }

  std::string_view name() const { return s_name; }

  SHType type() const { return s_type; }

  SHFlags flags() const { return s_flags; }

private:
  std::string_view s_name;
  SHType s_type;
  SHFlags s_flags;
};

class WithContent {
public:
  WithContent(BinaryBuffer s_content) : s_content(s_content) {}

  BinaryBuffer content() const { return s_content; }

private:
  BinaryBuffer s_content;
};

class ExecuteSection : public Section, public WithContent {
public:
  ExecuteSection(std::string_view s_name, SHType s_type, SHFlags s_flags,
                 BinaryBuffer s_content)
      : Section(s_name, s_type, s_flags), WithContent(s_content) {}

  virtual ~ExecuteSection() noexcept = default;

  static bool instanceOf(const Section& s) {
    return s.type() != SHType::SHT_NOBITS;
  }
};

class Elf {
public:
  virtual ~Elf() noexcept {}

  virtual EIClass archBits() const = 0;

  virtual EIData encoding() const = 0;

  virtual EType type() const = 0;

  virtual EMachine machine() const = 0;

  virtual size_t s_size() const = 0;

  virtual std::unique_ptr<Section> s_at(size_t ndx) const = 0;

  virtual void dump() const = 0;
};

std::unique_ptr<Elf> createBinaryElf(BinaryBuffer be);

} // namespace binary

#endif // BINARY_ELF_H
