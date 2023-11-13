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

#ifndef BINARY_ELF_FILE_H
#define BINARY_ELF_FILE_H
#include "binary/binary_file.h"
#include <memory>

namespace elf {
using binary::BinaryFile;
using binary::FileType;

class ElfFile : public BinaryFile {
public:
  virtual ~ElfFile() noexcept = default;

  virtual FileType type() const override { return binary::FT_ELF; }

  virtual uint32_t bits() const = 0;

  virtual uint32_t endian() const = 0;

  virtual uint8_t version() const = 0;

  virtual uint8_t osABI() const = 0;

  virtual uint8_t abiVersion() const = 0;
};

std::unique_ptr<ElfFile> analyzeElfFile(const char* file, size_t size);

} // namespace elf

#endif // BINARY_ELF_FILE_H
