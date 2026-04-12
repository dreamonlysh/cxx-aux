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

#ifndef ESTD___UTILITY_BITS_OF_H
#define ESTD___UTILITY_BITS_OF_H

namespace es {

namespace __impl_bits_of {
template <typename T>
struct bits_of_impl {
  static constexpr auto value = sizeof(T) * 8;
};

template <typename T, unsigned N>
struct bits_of_impl<T[N]> {
  static constexpr auto value = sizeof(T) * 8 * N;
};
} // namespace __impl_bits_of

/**
 * @brief Compile-time constant representing the size of a type in bits.
 *
 * This variable template provides the size of type T in bits at compile time.
 * For array types, it returns the total number of bits across all elements.
 *
 * @tparam T The type to get the bit size for
 *
 * Example usage:
 * @code
 * static_assert(bits_of<int> == 32);
 * static_assert(bits_of<int[10]> == 320);
 * static_assert(bits_of<char> == 8);
 * @endcode
 */
template <typename T>
constexpr auto bits_of = __impl_bits_of::bits_of_impl<T>::value;

} // namespace es
#endif
