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

#ifndef TONY_BINARY_ELF_PARSER_H
#define TONY_BINARY_ELF_PARSER_H
#include "buffer.h"
#include <memory>

namespace binary { namespace elf {

struct Section {
  std::string_view name;
  BinaryBuffer contents;
};

class BinaryElf {
public:
  virtual ~BinaryElf() noexcept {}

  virtual const std::vector<Section>& sections() = 0;
};

std::unique_ptr<BinaryElf> createBinaryElf(BinaryBuffer be);

}}// namespace binary::elf

#endif//TONY_BINARY_ELF_PARSER_H
