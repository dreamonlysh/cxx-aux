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
template <typename T, typename CharT = char>
struct is_c_string
    : std::bool_constant<std::is_convertible_v<T, const CharT*>> {};

template <typename T, typename CharT = char>
constexpr bool is_c_string_v = is_c_string<T, CharT>::value;

}} // namespace es::string

#endif
