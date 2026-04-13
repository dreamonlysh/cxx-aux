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
#include <cstdint>
#include <cstring>
#include <iterator>
#include <type_traits>

namespace es { namespace string {

namespace __impl_append {

/**
 * @brief Compile-time calculation of maximum decimal digits for integer types.
 *
 * This table provides the maximum number of characters needed to represent
 * any value of a given integer type in decimal, including the sign character.
 *
 * Calculation includes:
 * - Maximum digits for the absolute value
 * - Sign character for signed types ('-')
 * - No null terminator (strings are not null-terminated internally)
 *
 * Example calculations:
 * Type       Maximum value                Digits  Sign  Total
 * int8_t     127                          3       1     4
 * uint8_t    255                          3       0     3
 * int16_t    32,767                       5       1     6
 * uint16_t   65,535                       5       0     5
 * int32_t    2,147,483,647                10      1     11
 * uint32_t   4,294,967,295                10      0     10
 * int64_t    9,223,372,036,854,775,807    19      1     20
 * uint64_t   18,446,744,073,709,551,615   20      0     20
 *
 * @tparam IsSigned Whether the type is signed
 * @tparam Bytes Size of the type in bytes
 */
template <bool IsSigned, size_t Bytes>
struct digits10_impl;

template <>
struct digits10_impl<true, sizeof(int8_t)>
    : std::integral_constant<unsigned, 4> {};
template <>
struct digits10_impl<false, sizeof(uint8_t)>
    : std::integral_constant<unsigned, 3> {};
template <>
struct digits10_impl<true, sizeof(int16_t)>
    : std::integral_constant<unsigned, 6> {};
template <>
struct digits10_impl<false, sizeof(uint16_t)>
    : std::integral_constant<unsigned, 5> {};
template <>
struct digits10_impl<true, sizeof(int32_t)>
    : std::integral_constant<unsigned, 11> {};
template <>
struct digits10_impl<false, sizeof(uint32_t)>
    : std::integral_constant<unsigned, 10> {};
template <>
struct digits10_impl<true, sizeof(int64_t)>
    : std::integral_constant<unsigned, 20> {};
template <>
struct digits10_impl<false, sizeof(uint64_t)>
    : std::integral_constant<unsigned, 20> {};

/**
 * @brief Helper to get maximum decimal digits for an integral type.
 *
 * @tparam IntegralT The integral type
 */
template <typename IntegralT>
struct digits10
    : digits10_impl<std::is_signed_v<IntegralT>, sizeof(IntegralT)> {};

/**
 * @brief Adjusts string size to accommodate appended characters.
 *
 * This function expands the string by n characters, using the most
 * efficient method available:
 * - C++23: Uses resize_and_overwrite() to avoid default initialization
 * - Pre-C++23: Uses resize() which may zero-initialize new characters
 *
 * The resize_and_overwrite optimization is significant for performance:
 * - resize() typically zero-initializes new characters, then we overwrite them
 * - resize_and_overwrite() skips initialization, directly writing the data
 *
 * @tparam StringLike String-like type with resize() or resize_and_overwrite()
 * @param s String to expand
 * @param n Number of additional characters to accommodate
 */
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void adjust_range_to_be_append(StringLike& s,
                                         typename StringLike::size_type n) {

  auto size_after_replace = s.size() + n;
  if constexpr (__impl_type_traits::has_member_resize_and_overwrite_v<
                    StringLike>) {
    // C++23 optimization: skip default initialization
    // The lambda returns the final size, avoiding zero-initialization overhead
    s.resize_and_overwrite(size_after_replace,
                           [size_after_replace](auto*, std::size_t) {
                             return size_after_replace;
                           });
  } else {
    // Fallback: resize() may zero-initialize, but we'll overwrite anyway
    s.resize(size_after_replace);
  }
}

/**
 * @brief Assigns a single character to a position.
 *
 * @tparam CharT Character type
 * @param s Pointer to the position to assign
 * @param c Character to assign
 * @return Pointer to the next position
 */
template <typename CharT>
constexpr CharT* assign(CharT* s, CharT c) {
  *s = c;
  return std::next(s);
}

/**
 * @brief Assigns n copies of a character.
 *
 * @tparam CharT Character type
 * @param s Pointer to the start position
 * @param n Number of characters to assign
 * @param c Character to fill
 * @return Pointer past the last assigned character
 */
template <typename CharT>
constexpr CharT* assign(CharT* s, size_t n, CharT c) {
  return std::fill_n(s, n, c);
}

/**
 * @brief Assigns n characters from a source.
 *
 * @tparam CharT Character type
 * @param s Pointer to the destination
 * @param p Pointer to the source
 * @param n Number of characters to copy
 * @return Pointer past the last assigned character
 */
template <typename CharT>
constexpr CharT* assign(CharT* s, const CharT* p, size_t n) {
  return std::copy_n(p, n, s);
}

/**
 * @brief Converts an integer to characters and assigns them.
 *
 * Uses std::to_chars for efficient integer-to-string conversion.
 * The conversion is performed in a stack-allocated buffer sized
 * to hold the maximum possible digits for the type.
 *
 * @tparam CharT Character type
 * @tparam IntegralT Integral type to convert
 * @param s Pointer to the destination
 * @param v Integer value to convert and assign
 *
 * @note This overload is disabled when IntegralT == CharT to avoid
 *       ambiguity with character assignment
 */
template <typename CharT, typename IntegralT,
          typename = std::enable_if_t<std::is_integral_v<IntegralT> &&
                                      !std::is_same_v<IntegralT, CharT>>>
constexpr void assign(CharT* s, IntegralT v) {
  constexpr unsigned digits = __impl_append::digits10<IntegralT>::value;
  CharT data[digits];
  std::to_chars_result rst = std::to_chars(data, data + digits, v);
  assign(s, &data[0], static_cast<size_t>(std::distance(data, rst.ptr)));
}

} // namespace __impl_append

/**
 * @brief Appends a single character to a string.
 *
 * Expands the string by one character and places c at the end.
 * Uses the most efficient resize method available (resize_and_overwrite
 * for C++23 or later).
 *
 * @tparam StringLike String-like type with resize() and data()
 * @param s String to append to
 * @param c Character to append
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * append(str, '!');
 * // str is now "Hello!"
 * @endcode
 */
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, typename StringLike::value_type c) {
  __impl_append::adjust_range_to_be_append(s, 1);
  s.data()[s.size() - 1] = c;
}

/**
 * @brief Appends n copies of a character to a string.
 *
 * Expands the string by n characters and fills them with copies of c.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @param s String to append to
 * @param n Number of characters to append
 * @param c Character to fill
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * append(str, 3, '!');
 * // str is now "Hello!!!"
 * @endcode
 */
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, typename StringLike::size_type n,
                      typename StringLike::value_type c) {
  auto old_size = s.size();
  __impl_append::adjust_range_to_be_append(s, n);
  std::fill(s.data() + old_size, s.data() + old_size + n, c);
}

/**
 * @brief Appends n characters from a pointer to a string.
 *
 * Expands the string by n characters and copies them from p.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @param s String to append to
 * @param p Pointer to the source characters
 * @param n Number of characters to append
 *
 * @warning p must point to at least n valid characters
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * const char* world = " World";
 * append(str, world, 6);
 * // str is now "Hello World"
 * @endcode
 */
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, const typename StringLike::value_type* p,
                      typename StringLike::size_type n) {
  auto old_size = s.size();
  __impl_append::adjust_range_to_be_append(s, n);
  std::copy(p, p + n, s.data() + old_size);
}

/**
 * @brief Appends a null-terminated C string to a string.
 *
 * Calculates the length using strlen and appends all characters
 * from the null-terminated string.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @param s String to append to
 * @param p Null-terminated C string to append
 *
 * @warning p must be null-terminated
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * append(str, " World");
 * // str is now "Hello World"
 * @endcode
 */
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, const typename StringLike::value_type* p) {
  append(s, p, strlen(p));
}

/**
 * @brief Appends a string view to a string.
 *
 * Appends all characters from the string view.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @tparam StringViewLike String view-like type with data() and size()
 * @param s String to append to
 * @param sv String view to append
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * std::string_view sv = " World";
 * append(str, sv);
 * // str is now "Hello World"
 * @endcode
 */
template <typename StringLike, typename StringViewLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike> &&
              __impl_type_traits::is_string_view_like_v<StringViewLike>>>
constexpr void append(StringLike& s, const StringViewLike& sv) {
  append(s, sv.data(), sv.size());
}

/**
 * @brief Appends a substring from a string view to a string.
 *
 * Appends n characters starting from position pos in the string view.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @tparam StringViewLike String view-like type with data() and size()
 * @param s String to append to
 * @param sv String view to extract substring from
 * @param pos Starting position in sv
 * @param n Number of characters to append (default: npos = remainder)
 *
 * @throws std::out_of_range if pos >= sv.size()
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * std::string_view sv = " World!!!";
 * append(str, sv, 0, 6);  // Append " World"
 * // str is now "Hello World"
 * @endcode
 */
template <typename StringLike, typename StringViewLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike> &&
              __impl_type_traits::is_string_view_like_v<StringViewLike>>>
constexpr void append(StringLike& s, const StringViewLike& sv,
                      typename StringLike::size_type pos,
                      typename StringLike::size_type n = StringViewLike::npos) {
  append(s, sv.data() + pos, std::min(n, sv.size() - pos));
}

/**
 * @brief Appends an integer to a string.
 *
 * Converts the integer to its decimal representation and appends it.
 * Uses std::to_chars for efficient conversion.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @tparam IntegralT Integral type (int, long, etc.)
 * @param s String to append to
 * @param v Integer value to append
 *
 * @note This overload is disabled when IntegralT == CharT to avoid
 *       ambiguity with character append
 *
 * Example usage:
 * @code
 * std::string str = "Value: ";
 * append(str, 42);
 * // str is now "Value: 42"
 *
 * append(str, " - ");
 * append(str, -123);
 * // str is now "Value: 42 - -123"
 * @endcode
 */
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

/**
 * @brief Appends characters from an initializer list to a string.
 *
 * Appends all characters from the initializer list.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @param s String to append to
 * @param il Initializer list of characters
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * append(str, {' ', 'W', 'o', 'r', 'l', 'd'});
 * // str is now "Hello World"
 * @endcode
 */
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

/**
 * @brief Appends characters from an iterator range to a string.
 *
 * Appends all characters in the range [first, last).
 *
 * @tparam StringLike String-like type with resize() and data()
 * @tparam InputIterator Input iterator type
 * @param s String to append to
 * @param first Start of range
 * @param last End of range
 *
 * Example usage:
 * @code
 * std::string str = "Hello";
 * std::vector<char> vec = {' ', 'W', 'o', 'r', 'l', 'd'};
 * append(str, vec.begin(), vec.end());
 * // str is now "Hello World"
 * @endcode
 */
template <typename StringLike, typename InputIterator,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void append(StringLike& s, InputIterator first, InputIterator last) {
  auto old_size = s.size();
  __impl_append::adjust_range_to_be_append(s, std::distance(first, last));
  std::copy(first, last, s.data() + old_size);
}

/**
 * @brief Appends multiple items to a string in a single call.
 *
 * Uses fold expressions to append each argument sequentially.
 * Each argument must be a valid type for the append() function.
 *
 * @tparam StringLike String-like type with resize() and data()
 * @tparam Args Types of arguments to append
 * @param s String to append to
 * @param args Arguments to append (characters, strings, integers, etc.)
 *
 * Example usage:
 * @code
 * std::string str;
 * append_n(str, "Value: ", 42, ", Status: ", "OK");
 * // str is now "Value: 42, Status: OK"
 *
 * // Mix different types
 * append_n(str, ' ', 'C', 'o', 'u', 'n', 't', ':', ' ', 10);
 * // str is now "Value: 42, Status: OK Count: 10"
 * @endcode
 */
template <typename StringLike, typename... Args>
constexpr void append_n(StringLike& s, Args&&... args) {
  (append(s, std::forward<Args>(args)), ...);
}

}} // namespace es::string
#endif