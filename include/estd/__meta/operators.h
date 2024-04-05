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

#ifndef ESTD___META_OPERATORS_H
#define ESTD___META_OPERATORS_H
#include <type_traits>

namespace es {

/// @defgroup logic
/// logic operators &&, || and !
/// @{

template <typename... T>
struct __meta_and {};

template <typename T>
struct __meta_and<T>
    : std::bool_constant<std::is_base_of_v<std::true_type, T>> {};

template <typename T, typename... U>
struct __meta_and<T, U...>
    : std::conditional_t<std::is_base_of_v<std::true_type, T>, __meta_and<U...>,
                         std::false_type> {};

/// @brief logic operator &&(and) for meta
/// @tparam ...T bool_constant meta data, at least one
template <typename... T>
struct meta_and : __meta_and<T...> {};

/// @brief bool_constant type for meta_and
/// @tparam ...T bool_constant meta data, at least one
template <typename... T>
using meta_and_t = typename meta_and<T...>::type;

/// @brief bool_constant value for meta_and
/// @tparam ...T bool_constant meta data, at least one
template <typename... T>
constexpr bool meta_and_v = meta_and_t<T...>::value;

template <typename... T>
struct __meta_or {};

template <typename T>
struct __meta_or<T> : std::bool_constant<std::is_base_of_v<std::true_type, T>> {
};

template <typename T, typename... U>
struct __meta_or<T, U...>
    : std::conditional_t<std::is_base_of_v<std::true_type, T>, std::true_type,
                         __meta_or<U...>> {};

/// @brief logic operator ||(or) for meta
/// @tparam ...T bool_constant meta data, at least one
template <typename... T>
struct meta_or : __meta_or<T...> {};

/// @brief bool_constant type for meta_or
/// @tparam ...T bool_constant meta data, at least one
template <typename... T>
using meta_or_t = typename meta_or<T...>::type;

/// @brief bool_constant value for meta_or
/// @tparam ...T bool_constant meta data, at least one
template <typename... T>
constexpr bool meta_or_v = meta_or<T...>::value;

/// @brief logic operator !(not) for meta
/// @tparam T bool_constant meta data
template <typename T>
struct meta_not : std::bool_constant<!static_cast<bool>(T::value)> {};

/// @brief bool_constant type for meta_not
/// @tparam ...T bool_constant meta data
template <typename T>
using meta_not_t = typename meta_not<T>::type;

/// @brief bool_constant value for meta_not
/// @tparam ...T bool_constant meta data
template <typename T>
constexpr bool meta_not_v = meta_not_t<T>::value;

/// @} logic operator end

} // namespace es
#endif // ESTD___META_OPERATORS_H
