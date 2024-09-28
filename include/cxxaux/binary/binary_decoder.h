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

#ifndef BINARY_BINARY_DECODER_H
#define BINARY_BINARY_DECODER_H
#include <cassert>
#include <cstdint>
#include <string_view>

namespace cxxaux {

/// The BinaryDecoder is like the `std::ifstream` with more binary specific
/// interface. Most of the api names in BinaryDecoder is inherited from
/// `std::ifstream`.
/// BinaryDecoder does not take owner ship of the memory.
class BinaryDecoder {
public:
  static constexpr uint32_t eob = -1;

  BinaryDecoder() = default;
  BinaryDecoder(const char* data, uint32_t size) : data_(data), size_(size) {}

  BinaryDecoder(const BinaryDecoder&) = default;
  BinaryDecoder(BinaryDecoder&&) = default;
  BinaryDecoder& operator=(const BinaryDecoder&) = default;
  BinaryDecoder& operator=(BinaryDecoder&&) = default;

  ~BinaryDecoder() = default;

  const char* data() const { return data_; }

  uint32_t size() const { return size_; }

  // Set cursor position in the buffer.
  // If out of buffer, the end of the buffer will set `eob`
  void seekg(uint32_t idx) { cursor_ = idx < size_ ? idx : eob; }

  // Get current cursor in the buffer
  uint32_t tellg() const { return cursor_; }

  // Extracts and discards characters until the given string is found.
  // The given string will not be extracted
  uint32_t ignore(std::string_view s) {
    size_t offset = std::string_view(data_, size_).find(s, cursor_);
    if (offset == std::string_view::npos)
      return eob;
    seekg(offset);
    return tellg();
  }

  // Reads the next value without extracting it
  template <typename T>
  auto peek() const
      -> std::conditional_t<std::is_aggregate_v<T>,
                            std::add_pointer_t<std::add_const_t<T>>, T> {
    static_assert(!(std::is_reference_v<T> || std::is_pointer_v<T> ||
                    std::is_const_v<T> || std::is_volatile_v<T>));
    static_assert(std::is_arithmetic_v<T> || std::is_aggregate_v<T>);
    assert((cursor_ + sizeof(T)) <= size_);

    const char* data = std::next(data_, cursor_);
    if constexpr (std::is_arithmetic_v<T>) {
      return *reinterpret_cast<const T*>(data);
    } else {
      return reinterpret_cast<const T*>(data);
    }
  }

  // Reads the next char array without extracting it
  std::string_view peekn(uint32_t n) const {
    assert((cursor_ + n) <= size_);
    return {std::next(data_, cursor_), n};
  }

  // Reads the next char array that has a char end, mostly `\0`.
  // Won't extracting it
  std::string_view peekc(char c = '\0') const {
    std::string_view buf(data_, size_);
    size_t offset = buf.find_first_of(c, cursor_);
    if (offset == std::string_view::npos)
      return buf.substr(cursor_);
    return buf.substr(cursor_, offset - cursor_);
  }

  // Extracts characters
  template <typename T>
  auto get() -> std::conditional_t<std::is_aggregate_v<T>,
                                   std::add_pointer_t<std::add_const_t<T>>, T> {
    auto ret = peek<T>();
    seekg(cursor_ + sizeof(T));
    return ret;
  }

  // Extracts characters
  std::string_view getn(uint32_t n) {
    std::string_view ret = peekn(n);
    seekg(cursor_ + n);
    return ret;
  }

  std::string_view getc(char c = '\0') {
    std::string_view ret = peekc(c);
    seekg(cursor_ + ret.size());
    return ret;
  }

  // Slice to get a sub buffer
  BinaryDecoder slice(uint32_t idx, uint32_t size) const {
    assert((idx + size) <= size_);
    return BinaryDecoder{std::next(data_, idx), size};
  }

private:
  const char* data_ = nullptr;
  uint32_t size_ = 0;
  uint32_t cursor_ = 0;
};

} // namespace cxxaux

#endif // BINARY_BINARY_DECODER_H
