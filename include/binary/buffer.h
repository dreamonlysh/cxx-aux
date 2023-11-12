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

#ifndef BINARY_BUFFER_H
#define BINARY_BUFFER_H
#include <cassert>
#include <string_view>

namespace binary {

// The BinaryBuffer is like the `std::ifstream` with more binary specific
// interface. Most of the api names in BinaryBuffer is inherited from
// `std::ifstream`.
class BinaryBuffer {
public:
  static constexpr size_t eob = std::string_view::npos;

  BinaryBuffer() = default;
  BinaryBuffer(const char* data, size_t size) : buf({data, size}) {}
  explicit BinaryBuffer(std::string_view buf) : buf(buf) {}

  BinaryBuffer(const BinaryBuffer&) = default;
  BinaryBuffer(BinaryBuffer&&) = default;
  BinaryBuffer& operator=(const BinaryBuffer&) = default;
  BinaryBuffer& operator=(BinaryBuffer&&) = default;

  ~BinaryBuffer() = default;

  size_t size() const {
    return buf.size();
  }

  std::string_view data() const {
    return buf;
  }

  // Set cursor position in the buffer.
  // If out of buffer, the end of the buffer will set `eob`
  void seekg(size_t idx) { cursor = idx < buf.size() ? idx : eob; }

  // Get current cursor in the buffer
  [[nodiscard]] size_t tellg() const { return cursor; }

  // Extracts and discards characters until the given string is found.
  // The given string will not be extracted
  size_t ignore(std::string_view s) {
    size_t offset = buf.find(s, cursor);
    if (offset == std::string_view::npos)
      return eob;
    seekg(offset);
    return tellg();
  }

  // Reads the next value without extracting it
  template <typename T>
  auto peek()
      -> std::conditional_t<std::is_aggregate_v<T>,
                            std::add_pointer_t<std::add_const_t<T>>, T> const {
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> ||
                    std::is_const_v<T> || std::is_volatile_v<T>));
    static_assert(std::is_arithmetic_v<T> || std::is_aggregate_v<T>);
    assert((cursor + sizeof(T)) <= buf.size());

    const char* data = std::next(buf.data(), cursor);
    if constexpr (std::is_arithmetic_v<T>) {
      return *reinterpret_cast<const T*>(data);
    } else {
      return reinterpret_cast<const T*>(data);
    }
  }

  // Reads the next char array without extracting it
  std::string_view peekn(size_t n) const {
    assert((cursor + n) <= buf.size());
    return {std::next(buf.data(), cursor), n};
  }

  // Reads the next char array that has a char end, mostly `\0`.
  // Won't extracting it
  std::string_view peekc(char c = '\0') const {
    size_t offset = buf.find_first_of(c, cursor);
    if (offset == eob)
      return buf.substr(cursor);
    return buf.substr(cursor, offset - cursor);
  }

  // Extracts characters
  template <typename T>
  auto get() -> std::conditional_t<std::is_aggregate_v<T>,
                                   std::add_pointer_t<std::add_const_t<T>>, T> {
    auto ret = peek<T>();
    seekg(cursor + sizeof(T));
    return ret;
  }

  // Extracts characters
  std::string_view getn(size_t n) {
    auto ret = peekn(n);
    seekg(cursor + n);
    return ret;
  }

  std::string_view getc(char c = '\0') {
    auto ret = peekc(c);
    seekg(cursor + ret.size());
    return ret;
  }

  // Slice to get a sub buffer
  BinaryBuffer slice(size_t idx, size_t size) {
    assert((idx + size) <= buf.size());
    return BinaryBuffer{buf.substr(idx, size)};
  }

private:
  std::string_view buf;
  size_t cursor = 0;
};

} // namespace binary

#endif // BINARY_BUFFER_H
