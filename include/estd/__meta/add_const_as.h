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

#ifndef ESTD___META_ADD_CONST_AS_H
#define ESTD___META_ADD_CONST_AS_H
#include <type_traits>

namespace es {

/**
 * @brief Adds const qualifier to T if U is const.
 *
 * This type trait conditionally adds the const qualifier to type T based on
 * the const-ness of type U. This is useful for propagating const-correctness
 * in template code.
 *
 * @tparam T Type to potentially add const to
 * @tparam U Type to check for const-ness
 *
 * Example usage:
 * @code
 * using T1 = add_const_as_t<int, const double>;  // T1 is const int
 * using T2 = add_const_as_t<int, double>;        // T2 is int
 * using T3 = add_const_as_t<int, const int&>;    // T3 is const int
 *
 * // Useful in template code
 * template<typename U>
 * void process(add_const_as_t<int, U>& value) {
 *     // value is const int if U is const, otherwise int
 * }
 * @endcode
 */
template <typename T, typename U>
struct add_const_as {
  using type = std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>;
};

/**
 * @brief Helper type for add_const_as.
 * @tparam T Type to potentially add const to
 * @tparam U Type to check for const-ness
 */
template <typename T, typename U>
using add_const_as_t = typename add_const_as<T, U>::type;

} // namespace es
#endif
