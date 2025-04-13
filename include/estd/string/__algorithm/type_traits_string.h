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

META_HAS_MEMBER_FUNCTION(resize, 1u);
META_HAS_MEMBER_FUNCTION(resize_and_overwrite, 1u,
                         std::declval<void (*)(void*, std::size_t)>());

template <typename T>
struct is_string_view_like
    : std::bool_constant<is_iterable_v<T> && has_member_data_v<T> &&
                         has_member_size_v<T> && has_member_value_type_v<T> &&
                         has_member_size_type_v<T>> {};

template <typename T>
inline constexpr bool is_string_view_like_v = is_string_view_like<T>::value;

template <typename T>
struct is_string_like
    : std::bool_constant<is_string_view_like_v<T> && has_member_resize_v<T>> {};

template <typename T>
inline constexpr bool is_string_like_v = is_string_like<T>::value;

}}} // namespace es::string::__impl_type_traits
#endif
