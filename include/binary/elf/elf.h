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
#include "elf_macro.h"
#include <memory>
#include <string_view>

namespace cxxaux { namespace elf {

class Section {
public:
  Section(std::string_view name, SHType type, SHFlags flags,
          std::pair<const char*, size_t> content = {})
      : name_(name), type_(type), flags_(flags), content_(content) {}
  ~Section() noexcept = default;

  std::string_view name() const { return name_; }

  SHType type() const { return type_; }

  SHFlags flags() const { return flags_; }

  std::pair<const char*, size_t> content() const { return content_; }

private:
  std::string_view name_;
  SHType type_;
  SHFlags flags_;
  std::pair<const char*, size_t> content_;
};

class Elf {
public:
  virtual ~Elf() noexcept = default;

  virtual EIClass archBits() const = 0;

  virtual EIData encoding() const = 0;

  virtual EType type() const = 0;

  virtual EMachine machine() const = 0;

  virtual size_t s_size() const = 0;

  virtual std::unique_ptr<Section> s_at(size_t ndx) const = 0;

  virtual void dump() const = 0;
};

std::unique_ptr<Elf> createBinaryElf(const char* data, uint32_t size);
}} // namespace cxxaux::elf

#endif // BINARY_ELF_H
