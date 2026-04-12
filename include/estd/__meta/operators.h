// Copyright (c) 2023 guyuemeng
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

#ifndef ESTD___META_OPERATORS_H
#define ESTD___META_OPERATORS_H
#include <type_traits>

namespace es {

/**
 * @defgroup logic Logical Meta Operators
 * @brief Compile-time logical operators for type traits
 * @{
 */

template <typename... T>
struct __meta_and {};

template <typename T>
struct __meta_and<T>
    : std::bool_constant<std::is_base_of_v<std::true_type, T>> {};

template <typename T, typename... U>
struct __meta_and<T, U...>
    : std::conditional_t<std::is_base_of_v<std::true_type, T>, __meta_and<U...>,
                         std::false_type> {};

/**
 * @brief Compile-time logical AND for type traits.
 *
 * Evaluates to std::true_type if all template arguments inherit from
 * std::true_type, otherwise std::false_type.
 *
 * @tparam T Type traits to check (must be at least one)
 *
 * Example usage:
 * @code
 * static_assert(meta_and_v<std::true_type, std::true_type>);
 * static_assert(!meta_and_v<std::true_type, std::false_type>);
 * static_assert(meta_and_v<std::is_integral<int>, std::is_signed<int>>);
 * @endcode
 */
template <typename... T>
struct meta_and : __meta_and<T...> {};

/**
 * @brief Helper type for meta_and.
 * @tparam T Type traits to check
 */
template <typename... T>
using meta_and_t = typename meta_and<T...>::type;

/**
 * @brief Helper value for meta_and.
 * @tparam T Type traits to check
 */
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

/**
 * @brief Compile-time logical OR for type traits.
 *
 * Evaluates to std::true_type if any template argument inherits from
 * std::true_type, otherwise std::false_type.
 *
 * @tparam T Type traits to check (must be at least one)
 *
 * Example usage:
 * @code
 * static_assert(meta_or_v<std::false_type, std::true_type>);
 * static_assert(!meta_or_v<std::false_type, std::false_type>);
 * static_assert(meta_or_v<std::is_integral<int>, std::is_floating_point<int>>);
 * @endcode
 */
template <typename... T>
struct meta_or : __meta_or<T...> {};

/**
 * @brief Helper type for meta_or.
 * @tparam T Type traits to check
 */
template <typename... T>
using meta_or_t = typename meta_or<T...>::type;

/**
 * @brief Helper value for meta_or.
 * @tparam T Type traits to check
 */
template <typename... T>
constexpr bool meta_or_v = meta_or<T...>::value;

/**
 * @brief Compile-time logical NOT for type traits.
 *
 * Evaluates to std::true_type if T inherits from std::false_type,
 * otherwise std::false_type.
 *
 * @tparam T Type trait to negate
 *
 * Example usage:
 * @code
 * static_assert(meta_not_v<std::false_type>);
 * static_assert(!meta_not_v<std::true_type>);
 * static_assert(meta_not_v<std::is_const<int>>);
 * @endcode
 */
template <typename T>
struct meta_not : std::bool_constant<!static_cast<bool>(T::value)> {};

/**
 * @brief Helper type for meta_not.
 * @tparam T Type trait to negate
 */
template <typename T>
using meta_not_t = typename meta_not<T>::type;

/**
 * @brief Helper value for meta_not.
 * @tparam T Type trait to negate
 */
template <typename T>
constexpr bool meta_not_v = meta_not_t<T>::value;

/** @} */

} // namespace es
#endif // ESTD___META_OPERATORS_H
