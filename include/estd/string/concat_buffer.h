// Copyright (c) 2026 guyuemeng
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

#ifndef ESTD_STRING_CONCAT_BUFFER_H
#define ESTD_STRING_CONCAT_BUFFER_H

#include <algorithm>
#include <charconv>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "__algorithm/append.h"
#include "__algorithm/lazy_views.h"
#include "__algorithm/type_traits_string.h"
#include <estd/__bit/bit_pow2.h>
#include <estd/__container/maybe_owned.h>
#include <estd/__meta/array.h>

namespace es { namespace string {

namespace __impl_concat_buffer {
template <typename T = void>
struct conditional_result {
  constexpr conditional_result(bool v) : value(v) {}
  constexpr operator bool() const { return value; }
  bool value;
};

template <>
struct conditional_result<void> {
  constexpr conditional_result(bool) {}
  constexpr operator bool() const { return true; }
};
} // namespace __impl_concat_buffer

/**
 * @brief A flexible string concatenation buffer with fixed or dynamic storage.
 *
 * Supports appending characters, C-strings, string views, integral types,
 * iterables, and lazy views. Fixed buffers return false on overflow;
 * dynamic buffers automatically expand.
 *
 * @note The buffer is NOT null-terminated by default. Call c_str() to obtain
 *       a null-terminated C-string (writes '\0' lazily on demand).
 *
 * @tparam N Buffer size for fixed storage, or initial capacity for dynamic
 * @tparam Container Storage type (defaults to char[N] → std::array<char, N>)
 */
template <size_t N, typename Container = char[N]>
class concat_buffer {
  using container_type = to_std_array_t<Container>;

  static_assert(__impl_type_traits::has_member_data_v<container_type> &&
                    __impl_type_traits::has_member_size_v<container_type>,
                "Container must have data() and size() methods");

  static constexpr bool can_reserve =
      __impl_type_traits::has_member_reserve_v<container_type>;
  static constexpr bool can_resize =
      __impl_type_traits::has_member_resize_v<container_type>;
  static constexpr bool can_resize_and_overwrite =
      __impl_type_traits::has_member_resize_and_overwrite_v<container_type>;

  static constexpr bool can_expand =
      can_reserve && (can_resize || can_resize_and_overwrite);

  using append_result_t = __impl_concat_buffer::conditional_result<
      std::conditional_t<can_expand, void, bool>>;

public:
  constexpr concat_buffer() : storage_(container_type{}) {
    if constexpr (can_reserve) {
      storage_->reserve(N);
    }
    cursor_ = begin();
  }

  /**
   * @brief Constructor with initial capacity (dynamic buffers only).
   * @param n Initial capacity to reserve
   */
  template <
      typename C = container_type,
      typename = std::enable_if_t<!std::is_same_v<C, std::array<char, N>>>>
  constexpr concat_buffer(size_t n) : storage_(container_type{}) {
    static_assert(can_reserve,
                  "Container must support reserve() to use this constructor");
    storage_->reserve(n);
    cursor_ = begin();
  }

  /**
   * @brief Construct from lvalue reference (reference semantics).
   * @param c External container to reference
   */
  template <
      typename C = container_type,
      typename = std::enable_if_t<!std::is_same_v<C, std::array<char, N>> &&
                                  std::is_same_v<C, container_type>>>
  constexpr concat_buffer(C& c) : storage_(c) {
    if constexpr (can_reserve) {
      if (storage_->capacity() < N) {
        storage_->reserve(N);
      }
    }
    cursor_ = begin() + storage_->size();
  }

  /**
   * @brief Construct from rvalue reference (ownership transfer).
   * @param c Container to move from
   */
  template <
      typename C = container_type,
      typename = std::enable_if_t<!std::is_same_v<C, std::array<char, N>> &&
                                  std::is_same_v<C, container_type>>>
  constexpr concat_buffer(C&& c) : storage_(std::move(c)) {
    if constexpr (can_reserve) {
      if (storage_->capacity() < N) {
        storage_->reserve(N);
      }
    }
    cursor_ = begin() + storage_->size();
  }

  constexpr concat_buffer(const concat_buffer& other)
      : storage_(other.storage_), cursor_(begin() + other.size()) {}

  constexpr concat_buffer(concat_buffer&& other) noexcept
      : storage_(std::move(other.storage_)), cursor_(begin() + other.size()) {
    other.cursor_ = other.begin();
  }

  constexpr concat_buffer& operator=(const concat_buffer& other) {
    if (this != &other) {
      storage_ = other.storage_;
      cursor_ = begin() + other.size();
    }
    return *this;
  }

  constexpr concat_buffer& operator=(concat_buffer&& other) noexcept {
    if (this != &other) {
      storage_ = std::move(other.storage_);
      cursor_ = begin() + other.size();
      other.cursor_ = other.begin();
    }
    return *this;
  }

  constexpr char* begin() { return storage_->data(); }
  constexpr const char* begin() const { return storage_->data(); }

  constexpr char* end() { return cursor_; }
  constexpr const char* end() const { return cursor_; }

  constexpr size_t size() const {
    return static_cast<size_t>(cursor_ - begin());
  }

  constexpr size_t capacity() const {
    if constexpr (es::is_std_array_v<container_type>) {
      return storage_->size();
    } else {
      return storage_->capacity();
    }
  }

  /**
   * @brief Append multiple items to the buffer.
   *
   * @tparam Args Types of items to append
   * @param args Items to append (char, string, integral, etc.)
   * @return For fixed buffers: true if all appends succeeded, false if
   * overflow. For dynamic buffers: void (always succeeds).
   */
  template <typename... Args>
  constexpr auto append(Args&&... args) {
    if constexpr (can_expand) {
      (append_each(std::forward<Args>(args)), ...);
    } else {
      return (append_each(std::forward<Args>(args)) && ...);
    }
  }

  constexpr void clear() { cursor_ = begin(); }

  constexpr std::string_view view() const { return {begin(), size()}; }

  std::string to_string() const { return {begin(), size()}; }

  /**
   * @brief Get a null-terminated C-string.
   *
   * Writes '\0' at the current cursor position if space is available.
   * The null terminator is NOT maintained between operations — only written
   * on demand by this call.
   */
  constexpr char* c_str() {
    if (size() < capacity()) {
      *cursor_ = '\0';
    }
    return begin();
  }

  constexpr const char* c_str() const noexcept {
    if (size() < capacity()) {
      *const_cast<char*>(cursor_) = '\0';
    }
    return begin();
  }

  constexpr explicit operator char*() { return begin(); }
  constexpr explicit operator const char*() const { return begin(); }

private:
  constexpr append_result_t ensure_capacity(size_t additional) {
    size_t current_size = size();
    size_t required = current_size + additional;
    if (required <= capacity()) {
      return true;
    }

    if constexpr (can_reserve) {
      size_t new_capacity = std::max(capacity() * 2, ceil_to_pow2(required));
      char* old_begin = begin();
      storage_->reserve(new_capacity);
      char* new_begin = begin();
      if (old_begin != new_begin) {
        cursor_ = new_begin + current_size;
      }
      return true;
    } else {
      return false;
    }
  }

  constexpr void resize_for_raw(size_t additional) {
    size_t new_size = size() + additional;
    if constexpr (can_resize_and_overwrite) {
      storage_->resize_and_overwrite(
          new_size, [new_size](char* p, size_t s) { return new_size; });
    } else if constexpr (can_resize) {
      storage_->resize(new_size);
    }
  }

  constexpr append_result_t append_each(char c) {
    if (!ensure_capacity(1))
      return false;

    resize_for_raw(1);
    cursor_ = __impl_append::assign(cursor_, c);
    return true;
  }

  constexpr append_result_t append_each(const char* p, size_t n) {
    if (!ensure_capacity(n))
      return false;
    resize_for_raw(n);
    cursor_ = __impl_append::assign(cursor_, p, n);
    return true;
  }

  constexpr append_result_t append_each(const char* p) {
    size_t n = std::strlen(p);
    return append_each(p, n);
  }

  template <typename T, std::enable_if_t<
                            es::is_iterable_v<T> &&
                                !std::is_integral_v<std::remove_reference_t<T>>,
                            int> = 0>
  constexpr append_result_t append_each(T&& v) {
    bool success = true;
    for (auto m : std::forward<T>(v)) {
      success &= append_each(m);
    }
    return success;
  }

  template <
      typename T, typename = std::enable_if_t<!es::is_iterable_v<T>>,
      typename = std::enable_if_t<__impl_type_traits::has_member_data_v<T>>,
      typename = std::enable_if_t<__impl_type_traits::has_member_size_v<T>>>
  constexpr append_result_t append_each(const T& v) {
    return append_each(v.data(), v.size());
  }

  template <typename T, typename = std::enable_if_t<
                            std::is_integral_v<std::remove_reference_t<T>>>>
  constexpr append_result_t append_each(T&& v) {
    using ValueType = remove_cvref_t<T>;
    constexpr size_t max_digits = __impl_append::digits10<ValueType>::value;
    size_t remaining = capacity() - size();

    if (remaining >= max_digits) {
      resize_for_raw(max_digits);
      auto [ptr, ec] = std::to_chars(cursor_, cursor_ + remaining, v);
      resize_for_raw(ptr - cursor_);
      cursor_ = ptr;
      return true;
    }

    char buffer[32] = {};
    auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), v);
    return append_each(buffer, ptr - buffer);
  }

  constexpr append_result_t append_each(std::string_view sv) {
    return append_each(sv.data(), sv.size());
  }

private:
  maybe_owned<container_type> storage_;
  char* cursor_ = nullptr;
};

template <typename Container>
concat_buffer(Container& c) -> concat_buffer<0, Container>;

template <typename Container>
concat_buffer(Container&& c) -> concat_buffer<0, Container>;

}} // namespace es::string

#endif // ESTD_STRING_CONCAT_BUFFER_H
