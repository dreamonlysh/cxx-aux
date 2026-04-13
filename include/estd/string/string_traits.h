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

#ifndef ESTD_STRING_TRAITS_H
#define ESTD_STRING_TRAITS_H
#include <estd/meta.h>
#include <type_traits>

namespace es { namespace string {

/**
 * @brief Type trait to check if a type is convertible to a C-style string.
 *
 * Checks if type T can be implicitly converted to `const CharT*`.
 * This includes:
 * - `const CharT*` pointers
 * - `CharT*` pointers
 * - `CharT` arrays
 * - Any class with implicit conversion to `const CharT*`
 *
 * @tparam T Type to check
 * @tparam CharT Character type (default: char)
 *
 * Example usage:
 * @code
 * static_assert(is_c_string_v<char*>);
 * static_assert(is_c_string_v<const char*>);
 * static_assert(is_c_string_v<char[10]>);
 * static_assert(!is_c_string_v<std::string>);  // No implicit conversion
 * static_assert(!is_c_string_v<int>);
 * @endcode
 */
template <typename T, typename CharT = char>
struct is_c_string
    : std::bool_constant<std::is_convertible_v<T, const CharT*>> {};

/**
 * @brief Helper variable template for is_c_string.
 * @tparam T Type to check
 * @tparam CharT Character type (default: char)
 */
template <typename T, typename CharT = char>
constexpr bool is_c_string_v = is_c_string<T, CharT>::value;

/**
 * @brief Type trait to check if a type is string-like.
 *
 * A type is considered string-like if it provides:
 * - data() member function
 * - size() member function
 * - value_type member type
 * - size_type member type
 * - Is iterable (has begin() and end())
 *
 * Examples of string-like types:
 * - std::string
 * - std::string_view
 * - flat_string
 * - Any custom string class with the required interface
 *
 * @tparam T Type to check
 *
 * Example usage:
 * @code
 * static_assert(is_string_like_v<std::string>);
 * static_assert(is_string_like_v<std::string_view>);
 * static_assert(!is_string_like_v<int>);
 * static_assert(!is_string_like_v<std::vector<int>>);
 * @endcode
 */
template <typename T>
struct is_string_like : std::bool_constant<false> {};

template <typename CharT, typename Traits, typename Allocator>
struct is_string_like<std::basic_string<CharT, Traits, Allocator>>
    : std::bool_constant<true> {};

template <typename CharT, typename Traits>
struct is_string_like<std::basic_string_view<CharT, Traits>>
    : std::bool_constant<true> {};

/**
 * @brief Helper variable template for is_string_like.
 * @tparam T Type to check
 */
template <typename T>
constexpr bool is_string_like_v = is_string_like<T>::value;

}} // namespace es::string

#endif
