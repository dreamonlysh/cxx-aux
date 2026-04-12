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

#ifndef ESTD___UTILITY_BYTES_OF_H
#define ESTD___UTILITY_BYTES_OF_H

namespace es {

namespace __impl_bytes_of {
template <typename T>
struct bytes_of_impl {
  static constexpr auto value = sizeof(T);
};

template <typename T, unsigned N>
struct bytes_of_impl<T[N]> {
  static constexpr auto value = sizeof(T) * N;
};
} // namespace __impl_bytes_of

/**
 * @brief Compile-time constant representing the size of a type in bytes.
 *
 * This variable template provides the size of type T in bytes at compile time.
 * For array types, it returns the total size of all elements.
 *
 * @tparam T The type to get the size for
 *
 * Example usage:
 * @code
 * static_assert(bytes_of<int> == 4);
 * static_assert(bytes_of<int[10]> == 40);
 * static_assert(bytes_of<char> == 1);
 * @endcode
 */
template <typename T>
constexpr auto bytes_of = __impl_bytes_of::bytes_of_impl<T>::value;

} // namespace es
#endif
