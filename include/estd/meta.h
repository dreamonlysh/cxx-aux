
// Copyright (c) 2024 guyuemeng
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

#ifndef ESTD_META_H
#define ESTD_META_H

#include "__meta/has_member.h"
// #define META_HAS_MEMBER_TYPE(type)
// #define META_HAS_MEMBER_DATA(data)
// #define META_HAS_MEMBER_FUNCTION(func, ...)
//
// Default member element validation provided
// - META_HAS_MEMBER_TYPE(iterator);
//   Defined: has_member_iterator, has_member_iterator_v

#include "__meta/operators.h"
// template <typename... T>
// struct meta_and;
// template <typename... T>
// using meta_and_t = typename meta_and<T...>::type;
// template <typename... T>
// constexpr bool meta_and_v = meta_and_t<T...>::value;
// template <typename... T>
// struct meta_or;
// template <typename... T>
// using meta_or_t = typename meta_or<T...>::type;
// template <typename... T>
// constexpr bool meta_or_v = meta_or<T...>::value;
// template <typename T>
// struct meta_not;
// template <typename T>
// using meta_not_t = typename meta_not<T>::type;
// template <typename T>
// constexpr bool meta_not_v = meta_not_t<T>::value;

#include "__meta/system.h"
// struct is_win;
// constexpr bool is_win_v = is_win::value;
// struct is_win64;
// constexpr bool is_win64_v = is_win64::value;
// struct is_win32;
// constexpr bool is_win32_v = is_win32::value;
// struct is_cygwin;
// constexpr bool is_cygwin_v = is_cygwin::value;
// struct is_unix;
// constexpr bool is_unix_v = is_unix::value;
// struct is_linux;
// constexpr bool is_linux_v = is_linux::value;
// struct is_free_bsd;
// constexpr bool is_free_bsd_v = is_free_bsd::value;
// struct is_macos;
// constexpr bool is_macos_v = is_macos::value;
// struct is_android;
// constexpr bool is_android_v = is_android::value;

#include "__meta/add_const_as.h"
// template <typename T, typename U>
// using add_const_as;
// template <typename T, typename U>
// using add_const_as_t = typename add_const_as<T, U>::type;

#include "__meta/is_iterable.h"
// template <typename T>
// struct has_member_begin;
// template <typename T>
// constexpr bool has_member_begin_v;
// template <typename T>
// struct has_member_end;
// template <typename T>
// constexpr has_member_end_v
//
// template <typename T>
// struct is_iterable;
// template <typename T>
// constexpr bool is_iterable_v = is_iterable<T>::value;

#include "__meta/remove_cvref.h"
// template <typename T>
// struct remove_cvref;
// template <typename T>
// using remove_cvref_t = typename remove_cvref<T>::type;

#include "__meta/array.h"
// template <typename T>
// struct is_c_array;
// template <typename T>
// inline constexpr bool is_c_array_v;
// template <typename T>
// struct is_std_array;
// template <typename T>
// inline constexpr bool is_std_array_v;
// template <typename T>
// struct array_traits;
// template <typename T>
// struct to_std_array;
// template <typename T>
// using to_std_array_t = typename to_std_array<T>::type;

#endif
