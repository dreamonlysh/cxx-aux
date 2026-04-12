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

/**
 * @brief Internal implementation for storing small strings (<= 16 bytes).
 *
 * This implementation uses a pool-based allocation strategy optimized for
 * small strings. It allocates memory in 4KB blocks and stores strings
 * contiguously within these blocks.
 *
 * Key features:
 * - Fixed 4KB block size
 * - Contiguous string storage within blocks
 * - Efficient for small strings (avoids malloc overhead)
 * - Strings are null-terminated for C compatibility
 *
 * @note This is an internal implementation detail
 */
class BasicStringTableSmallImpl : disabled_copy_move {
  // generally, a pointer size is used by the forward list
  static constexpr size_t string_bytes_capacity = 4 * 1024 - sizeof(void*);
  struct Block {
    char data[string_bytes_capacity];
  };

public:
  BasicStringTableSmallImpl() = default;
  ~BasicStringTableSmallImpl() noexcept = default;

  /**
   * @brief Stores a string and returns a stable string_view.
   *
   * If the current block has insufficient space, allocates a new block.
   * Copies the string into the block and null-terminates it.
   *
   * @param s String to store
   * @return string_view pointing to the stored string
   */
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

/**
 * @brief Internal implementation for storing large strings (> 16 bytes).
 *
 * This implementation uses individual heap allocations for each string,
 * which is more memory-efficient for large strings.
 *
 * Key features:
 * - Individual malloc for each string
 * - Stores pointers in 4KB blocks
 * - Efficient for large strings
 * - Strings are null-terminated for C compatibility
 *
 * @note This is an internal implementation detail
 */
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

  /**
   * @brief Stores a string and returns a stable string_view.
   *
   * Allocates memory via malloc and copies the string.
   * Manages pointers in blocks for efficient cleanup.
   *
   * @param s String to store
   * @return string_view pointing to the stored string
   */
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

/**
 * @brief Base class for string table with dual allocation strategy.
 *
 * Combines small and large string implementations to optimize memory
 * usage across all string sizes.
 *
 * Strategy:
 * - Strings < MaxBytesInPool: Use small string pool (contiguous allocation)
 * - Strings >= MaxBytesInPool: Use large string allocation (individual malloc)
 *
 * @tparam T Tag type for creating unique string table types
 * @tparam MaxBytesInPool Threshold for small vs large string allocation
 */
template <typename T, size_t MaxBytesInPool>
class BasicStringTable : disabled_copy_move {
protected:
  BasicStringTable() = default;
  ~BasicStringTable() noexcept = default;

  /**
   * @brief Adds a string using the appropriate allocation strategy.
   *
   * @param s String to add
   * @return string_view pointing to the stored string
   */
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

/**
 * @brief A string table for string interning and deduplication.
 *
 * This class provides a mechanism for storing strings with automatic
 * deduplication. It's useful for scenarios where many duplicate strings
 * need to be stored efficiently, such as:
 * - Symbol tables in compilers
 * - String constants in interpreters
 * - Configuration key-value stores
 * - JSON/XML parsing
 *
 * Key features:
 * - Automatic string deduplication via SetT
 * - Dual allocation strategy for memory efficiency
 * - Stable string_view references (never invalidated)
 * - Thread-unsafe (requires external synchronization)
 *
 * Memory strategy:
 * - Small strings (< 16 bytes): Pooled allocation (4KB blocks)
 * - Large strings (>= 16 bytes): Individual heap allocation
 *
 * @tparam SetT Set container type for deduplication (e.g., std::unordered_set,
 * std::set)
 * @tparam T Optional tag type for creating unique string table types
 *
 * Example usage:
 * @code
 * // Create a string table with unordered_set for O(1) lookup
 * StringTable<std::unordered_set> table;
 *
 * // Add strings - duplicates return the same string_view
 * std::string_view s1 = table.add("hello");
 * std::string_view s2 = table.add("world");
 * std::string_view s3 = table.add("hello");  // Returns same view as s1
 *
 * assert(s1 == s3);  // Same pointer
 * assert(s1 != s2);  // Different pointers
 *
 * // Use with std::set for ordered storage
 * StringTable<std::set> ordered_table;
 * @endcode
 */
template <template <typename...> typename SetT, typename T = void>
class StringTable : __impl_string_table::BasicStringTable<T, 16> {
  using base_type = __impl_string_table::BasicStringTable<T, 16>;

public:
  StringTable() = default;
  ~StringTable() noexcept = default;

  /**
   * @brief Adds a string to the table and returns a cached string_view.
   *
   * If the string already exists in the table, returns the existing
   * string_view. Otherwise, stores the string and returns a new
   * string_view.
   *
   * The returned string_view is stable and will remain valid for the
   * lifetime of the StringTable.
   *
   * @param s String to add or look up
   * @return string_view pointing to the stored/cached string
   *
   * @note Time complexity depends on SetT:
   *       - std::unordered_set: O(1) average
   *       - std::set: O(log n)
   *
   * Example usage:
   * @code
   * StringTable<std::unordered_set> table;
   *
   * // First insertion
   * auto view1 = table.add("config.key");
   *
   * // Duplicate - returns same view
   * auto view2 = table.add("config.key");
   * assert(view1.data() == view2.data());
   * @endcode
   */
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
