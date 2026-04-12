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

#ifndef ESTD_STRING___ALGORITHM_TYPE_TRAITS_STRING_H
#define ESTD_STRING___ALGORITHM_TYPE_TRAITS_STRING_H
#include <estd/__meta/is_iterable.h>

namespace es { namespace string { namespace __impl_type_traits {

META_HAS_MEMBER_TYPE(value_type);
META_HAS_MEMBER_TYPE(size_type);

META_HAS_MEMBER_FUNCTION(data);
META_HAS_MEMBER_FUNCTION(size);
META_HAS_MEMBER_FUNCTION(capacity);

META_HAS_MEMBER_FUNCTION(resize, 1u);
META_HAS_MEMBER_FUNCTION(reserve, 1u);
META_HAS_MEMBER_FUNCTION(resize_and_overwrite, 1u,
                         std::declval<void (*)(void*, std::size_t)>());

/**
 * @brief Type trait to check if a type is string-view-like.
 *
 * A type is considered string-view-like if it provides:
 * - Iterable interface (begin() and end())
 * - data() member function
 * - size() member function
 * - value_type member type
 * - size_type member type
 *
 * String-view-like types are read-only string representations that
 * can be viewed but not necessarily modified.
 *
 * Examples:
 * - std::string_view
 * - std::string (also mutable)
 * - flat_string
 * - Any custom string view class
 *
 * @tparam T Type to check
 */
template <typename T>
struct is_string_view_like
    : std::bool_constant<is_iterable_v<T> && has_member_data_v<T> &&
                         has_member_size_v<T> && has_member_value_type_v<T> &&
                         has_member_size_type_v<T>> {};

/**
 * @brief Helper variable template for is_string_view_like.
 * @tparam T Type to check
 */
template <typename T>
inline constexpr bool is_string_view_like_v = is_string_view_like<T>::value;

/**
 * @brief Type trait to check if a type is string-like (mutable).
 *
 * A type is considered string-like if it is string-view-like AND
 * provides a resize() member function. This indicates the string
 * can be modified in size.
 *
 * Examples:
 * - std::string
 * - flat_string
 * - std::vector<char>
 * - Any custom mutable string class
 *
 * @tparam T Type to check
 */
template <typename T>
struct is_string_like
    : std::bool_constant<is_string_view_like_v<T> && has_member_resize_v<T>> {};

/**
 * @brief Helper variable template for is_string_like.
 * @tparam T Type to check
 */
template <typename T>
inline constexpr bool is_string_like_v = is_string_like<T>::value;

}}} // namespace es::string::__impl_type_traits
#endif
