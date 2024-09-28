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

#ifndef ESTD___BIT_BIT_GET_H
#define ESTD___BIT_BIT_GET_H
#include <limits>
#include <type_traits>

namespace es {

/// @brief get one bit in the position
///
/// pos out of bits_of(T) is undefined
///
/// @tparam T unsigned integer type
/// @param v value to get bit
/// @param pos in range [0, bits_of(T))
/// @return positional value of T type
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T get_bit(T v, unsigned pos) noexcept {
  return (v >> pos) & static_cast<T>(1u);
}

/// @brief get n bits in the position
///
/// (pos + size) out of bits_of(T) is undefined
///
/// @tparam T unsigned integer type
/// @param v value to get bit
/// @param pos in range [0, bits_of(T))
/// @param n in range (0, bits_of(T)]
/// @return positional value of T type
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T get_bits(T v, unsigned pos, unsigned n) noexcept {
  return (v >> pos) & (std::numeric_limits<T>::max() >>
                       (std::numeric_limits<T>::digits - n));
}

} // namespace es
#endif
