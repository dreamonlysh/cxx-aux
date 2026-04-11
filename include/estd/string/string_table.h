// Copyright (c) 2024 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD_STRING_STRING_TABLE_H
#define ESTD_STRING_STRING_TABLE_H
#include <cstdint>
#include <cstdlib>
#include <estd/__utility/disabled_copy_move.h>
#include <forward_list>
#include <string_view>

namespace es { namespace string {
namespace __impl_string_table {

class BasicStringTableSmallImpl : disabled_copy_move {
  // generally, a pointer size is used by the forward list
  static constexpr size_t string_bytes_capacity = 4 * 1024 - sizeof(void*);
  struct Block {
    char data[string_bytes_capacity];
  };

public:
  BasicStringTableSmallImpl() = default;
  ~BasicStringTableSmallImpl() noexcept = default;

  std::string_view store(std::string_view s) {
    auto size = s.size() + 1;
    if (free_bytes_ < size) {
      storage_.emplace_front();
      cursor_ = storage_.front().data;
      free_bytes_ = string_bytes_capacity;
    }
    char* begin = cursor_;
    cursor_ += size;
    free_bytes_ -= size;

    std::copy(s.begin(), s.end(), begin);
    begin[s.size()] = '\0';
    return begin;
  }

private:
  std::forward_list<Block> storage_;
  char* cursor_ = nullptr;
  uint16_t free_bytes_ = 0;
};

class BasicStringTableLargeImpl : disabled_copy_move {
  // a pointer size is taken by the forward list
  static constexpr size_t string_pointer_capacity =
      (4 * 1024) / sizeof(void*) - 1;
  struct Block {
    char* data[string_pointer_capacity] = {};
  };

public:
  BasicStringTableLargeImpl() = default;
  ~BasicStringTableLargeImpl() noexcept {
    for (auto& block : storage_) {
      for (auto* p : block.data) {
        if (p == nullptr)
          break;
        std::free(p);
      }
    }
  }

  std::string_view store(std::string_view s) {
    if (cursor_ == string_pointer_capacity) {
      storage_.emplace_front();
      cursor_ = 1;
    }
    auto current = cursor_++;

    auto* p = static_cast<char*>(std::malloc(s.size() + 1));
    std::copy(s.begin(), s.end(), p);
    p[s.size()] = '\0';
    storage_.front().data[current] = p;
    return p;
  }

private:
  std::forward_list<Block> storage_;
  uint16_t cursor_ = string_pointer_capacity;
};

template <typename T, size_t MaxBytesInPool>
class BasicStringTable : disabled_copy_move {
protected:
  BasicStringTable() = default;
  ~BasicStringTable() noexcept = default;

  std::string_view add(std::string_view s) {
    // \0 is needed for a string, so the actual string memory size is `size + 1`
    // size + 1 <= MaxBytesInPool, is also size < MaxBytesInPool
    return s.size() < MaxBytesInPool ? small_.store(s) : large_.store(s);
  }

private:
  __impl_string_table::BasicStringTableSmallImpl small_;
  __impl_string_table::BasicStringTableLargeImpl large_;
};

} // namespace __impl_string_table

/// @brief A simple string table to help pass string_view instread of string
/// @tparam SetT is a set like container with find and insert to make the
/// storage of string unique
/// @tparam T is a type to make the StringTable unique
template <template <typename...> typename SetT, typename T = void>
class StringTable : __impl_string_table::BasicStringTable<T, 16> {
  using base_type = __impl_string_table::BasicStringTable<T, 16>;

public:
  StringTable() = default;
  ~StringTable() noexcept = default;

  /// @brief Add a string to table and get he cached string view
  /// @param s string to cache or may be cached
  /// @return view of a pool controlled string
  std::string_view add(std::string_view s) {
    auto it = strtbl_.find(s);
    if (it != strtbl_.end())
      return *it;

    std::string_view ret = base_type::add(s);

    strtbl_.insert(ret);
    return ret;
  }

private:
  SetT<std::string_view> strtbl_;
};

}} // namespace es::string
#endif
