// Copyright (c) 2025 guyuemeng
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

#ifndef ESTD_STRING___ALGORITHM_APPEND_H
#define ESTD_STRING___ALGORITHM_APPEND_H
#include "type_traits_string.h"
#include <algorithm>
#include <charconv>
#include <iterator>

namespace es { namespace string {

namespace __impl_append {

// Type       Maximum value                Digits  Sign  Total
// int8_t     127                          3       1     4
// uint8_t    255                          3       0     3
// int16_t    32,767                       5       1     6
// uint16_t   65,535                       5       0     5
// int32_t    2,147,483,647                10      1     11
// uint32_t   4,294,967,295                10      0     10
// int64_t    9,223,372,036,854,775,807    19      1     20
// uint64_t   18,446,744,073,709,551,615   20      0     20
template <typename IntegralT>
struct digits10;

template <>
struct digits10<bool> : std::integral_constant<unsigned, 1> {};
template <>
struct digits10<int8_t> : std::integral_constant<unsigned, 4> {};
template <>
struct digits10<uint8_t> : std::integral_constant<unsigned, 3> {};
template <>
struct digits10<int16_t> : std::integral_constant<unsigned, 6> {};
template <>
struct digits10<uint16_t> : std::integral_constant<unsigned, 5> {};
template <>
struct digits10<int32_t> : std::integral_constant<unsigned, 11> {};
template <>
struct digits10<uint32_t> : std::integral_constant<unsigned, 10> {};
template <>
struct digits10<int64_t> : std::integral_constant<unsigned, 20> {};
template <>
struct digits10<uint64_t> : std::integral_constant<unsigned, 20> {};

template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void adjust_range_to_be_append(StringLike& s,
                                         typename StringLike::size_type n) {

  auto size_after_replace = s.size() + n;
  if constexpr (__impl_type_traits::has_member_resize_and_overwrite_v<
                    StringLike>) {
    s.resize_and_overwrite(size_after_replace,
                           [size_after_replace](auto*, std::size_t) {
                             return size_after_replace;
                           });
  } else {
    s.resize(size_after_replace);
  }
}
} // namespace __impl_append

/// @brief Append a character to the end of string
/// @tparam StringLike std::string like type
/// @param s string to be appended
/// @param c character to be appended
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, typename StringLike::value_type c) {
  __impl_append::adjust_range_to_be_append(s, 1);
  s.data()[s.size() - 1] = c;
}

/// @brief Append a string to the end of string
/// @tparam StringLike std::string like type
/// @param s string to be appended
/// @param n number of characters to be appended
/// @param c character to be appended
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, typename StringLike::size_type n,
                      typename StringLike::value_type c) {
  auto old_size = s.size();
  __impl_append::adjust_range_to_be_append(s, n);
  std::fill(s.data() + old_size, s.data() + old_size + n, c);
}

/// @brief Append a string to the end of string
/// @tparam StringLike std::string like type
/// @param s string to be appended
/// @param p begin of the string used to be appended
/// @param n number of characters to be appended
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, const typename StringLike::value_type* p,
                      typename StringLike::size_type n) {
  auto old_size = s.size();
  __impl_append::adjust_range_to_be_append(s, n);
  std::copy(p, p + n, s.data() + old_size);
}

/// @brief Append a c string to the end of string
/// @tparam StringLike std::string like type
/// @param s string to be appended
/// @param p a c string used to be appended
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, const typename StringLike::value_type* p) {
  append(s, p, strlen(p));
}

/// @brief Append a string view to the end of string
/// @tparam StringLike std::string like type
/// @tparam StringViewLike std::string_view like type
/// @param s string to be appended
/// @param sv string view used to be appended
template <typename StringLike, typename StringViewLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike> &&
              __impl_type_traits::is_string_view_like_v<StringViewLike>>>
constexpr void append(StringLike& s, const StringViewLike& sv) {
  append(s, sv.data(), sv.size());
}

/// @brief Append partial of a string view to the end of string
/// @tparam StringLike std::string like type
/// @tparam StringViewLike std::string_view like type
/// @param s string to be appended
/// @param sv string view used to be appended
/// @param pos start position to be appended
/// @param n number of characters to be appended
template <typename StringLike, typename StringViewLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike> &&
              __impl_type_traits::is_string_view_like_v<StringViewLike>>>
constexpr void append(StringLike& s, const StringViewLike& sv,
                      typename StringLike::size_type pos,
                      typename StringLike::size_type n = StringViewLike::npos) {
  append(s, sv.data() + pos, std::min(n, sv.size() - pos));
}

/// @brief Append an integer to the end of string
/// @tparam StringLike std::string like type
/// @tparam IntegralT integral type
/// @param s string to be appended
/// @param v integer to be appended
template <typename StringLike, typename IntegralT,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike> &&
              std::is_integral_v<IntegralT> &&
              !std::is_same_v<IntegralT, typename StringLike::value_type>>>
constexpr void append(StringLike& s, IntegralT v) {
  constexpr unsigned digits = __impl_append::digits10<IntegralT>::value;
  typename StringLike::value_type data[digits];
  std::to_chars_result rst = std::to_chars(data, data + digits, v);
  append(s, &data[0],
         static_cast<typename StringLike::size_type>(
             std::distance(data, rst.ptr)));
}

/// @brief Append a list of char to the end of string
/// @tparam StringLike std::string like type
/// @param s string to be appended
/// @param il initializer list used to be appended
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void
append(StringLike& s,
       std::initializer_list<typename StringLike::value_type> il) {
  auto old_size = s.size();
  __impl_append::adjust_range_to_be_append(s, il.size());
  std::copy(il.begin(), il.end(), s.data() + old_size);
}

/// @brief Append a range of char to the end of string
/// @tparam StringLike std::string like type
/// @tparam InputIterator input iterator type
/// @param s string to be appended
/// @param first range begin to be used to append
/// @param last range end to be used to append
template <typename StringLike, typename InputIterator,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, InputIterator first, InputIterator last) {
  auto old_size = s.size();
  __impl_append::adjust_range_to_be_append(s, std::distance(first, last));
  std::copy(first, last, s.data() + old_size);
}

/// @brief Append numbers of data to the end of string
/// @tparam StringLike std::string like type
/// @tparam Args data types
/// @param s string to be appended
/// @param args datas to be appended
template <typename StringLike, typename... Args>
constexpr void append_n(StringLike& s, Args&&... args) {
  (append(s, std::forward<Args>(args)), ...);
}

}} // namespace es::string
#endif