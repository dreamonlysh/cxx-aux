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

#ifndef ESTD___META_IS_ITERABLE_H
#define ESTD___META_IS_ITERABLE_H
#include "has_member.h"

namespace es {

META_HAS_MEMBER_FUNCTION(begin);
META_HAS_MEMBER_FUNCTION(end);

/**
 * @brief Type trait to check if a type is iterable.
 *
 * A type is considered iterable if it has both begin() and end() member
 * functions. This trait is useful for SFINAE constraints and concept-like
 * type checking.
 *
 * @tparam T Type to check
 *
 * Example usage:
 * @code
 * static_assert(is_iterable_v<std::vector<int>>);
 * static_assert(is_iterable_v<std::string>);
 * static_assert(!is_iterable_v<int>);
 *
 * // Use in SFINAE
 * template<typename T>
 * std::enable_if_t<is_iterable_v<T>> print_all(const T& container) {
 *     for (const auto& item : container) {
 *         std::cout << item << " ";
 *     }
 * }
 * @endcode
 */
template <typename T>
struct is_iterable
    : std::bool_constant<has_member_begin_v<T> && has_member_end_v<T>> {};

/**
 * @brief Helper variable for is_iterable.
 * @tparam T Type to check
 */
template <typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

} // namespace es
#endif
