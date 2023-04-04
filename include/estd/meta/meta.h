// Copyright (c) 2023 guyuemeng
//
// Tony is licensed under Mulan PSL v2.
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
#include <type_traits>

namespace es {

template <typename T, typename U>
struct meta_and
    : std::bool_constant<static_cast<bool>(T::value)
                         && static_cast<bool>(U::value)> {};

template <typename T, typename U>
using meta_and_t = typename meta_and<T, U>::type;

template <typename T, typename U>
struct meta_or
    : std::bool_constant<static_cast<bool>(T::value)
                         || static_cast<bool>(U::value)> {};

template <typename T, typename U>
using meta_or_t = typename meta_or<T, U>::type;

template <typename T>
struct meta_not : std::bool_constant<!static_cast<bool>(T::value)> {};

template <typename T> using meta_not_t = typename meta_not<T>::type;

}// namespace es
#endif//ESTD_META_H
