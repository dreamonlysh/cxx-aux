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

#ifndef BINARY_FILE_H
#define BINARY_FILE_H
#include <cstdint>

namespace binary {

enum FileType {
  FT_ELF
};

class BinaryFile {
public:
  virtual ~BinaryFile() noexcept = default;

  virtual FileType type() const = 0;

  /// \brief Bit width of architecture.
  /// \return 0 if there is no arch bit width
  virtual uint32_t bits() const = 0;

  /// \brief Endian of the binary.
  /// \return LITTLE_ENDIAN/BIG_ENDIAN, 0 if there is no endian
  virtual uint32_t endian() const = 0;
};

}

#endif // BINARY_FILE_H
