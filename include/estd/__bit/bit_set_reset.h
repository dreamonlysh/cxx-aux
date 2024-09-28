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

#ifndef ESTD___BIT_BIT_SET_RESET_H
#define ESTD___BIT_BIT_SET_RESET_H
#include <limits>
#include <type_traits>

namespace es {

/// @brief generate a mask of one bit in the position
///
/// v = 0u, T = unsigned, pos = 0, return 0x0000'0001
/// v = 0u, T = unsigned, pos = 31, return 0x8000'0000
/// pos out of bits_of(T) is undefined
///
/// @tparam T unsigned integer type
/// @param v value to set bit, if v == 0 it is like a mask
/// @param pos in range [0, bits_of(T))
/// @return masked value of T type
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T set_bit(T v, unsigned pos) noexcept {
  return v | static_cast<T>(1u) << pos;
}

/// @brief generate a mask of n bits in the position
///
/// v = 0u, T = unsigned, pos = 0, size = 1, return 0x0000'0001
/// v = 0u, T = unsigned, pos = 0, size = 32, return 0xffff'ffff
/// (pos + size) out of bits_of(T) is undefined
///
/// @tparam T unsigned integer type
/// @param v value to set bit, if v == 0 it is like a mask
/// @param pos in range [0, bits_of(T))
/// @param n in range (0, bits_of(T)]
/// @return masked value of T type
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T set_bits(T v, unsigned pos, unsigned n) noexcept {
  return v | ((std::numeric_limits<T>::max() >>
               (std::numeric_limits<T>::digits - n))
              << pos);
}

/// @brief turn off the right most non-zero bit
///
/// algorithm: `v & (v - 1)`
/// e.g. 01011000 => 01010000
///
/// @tparam T unsigned integer type
/// @param v value to reset the first bit 1
/// @return reseted value
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T reset_first(T v) noexcept {
  return v & (v - 1);
}

/// @brief to turn on the right most zero bit
///
/// algorithm: `v | (v + 1)`
/// e.g. 10100111 => 10101111
///
/// @tparam T unsigned integer type
/// @param v value to set the first bit 0
/// @return seted value
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T set_first(T v) noexcept {
  return v | (v + 1);
}

/// @brief turn off the trailing non-zero bits
///
/// algorithm: `v & (v + 1)`
/// e.g. 10100111 => 10100000
///
/// @tparam T unsigned integer type
/// @param v value to reset the trailing bit 1
/// @return reseted value
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T resetr(T v) noexcept {
  return v & (v + 1);
}

/// @brief turn on the trailing zero bits
///
/// algorithm: `v | (v - 1)`
/// e.g. 10101000 => 10101111
///
/// @tparam T unsigned integer type
/// @param v value to set the trailing bit 0
/// @return seted value
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T setr(T v) noexcept {
  return v | (v - 1);
}

/// @brief turn off the right most adjacent non-zero bit
///
/// algorithm: `((v | (v - 1)) + 1) & v)`
/// e.g. 01011100 => 01000000
///
/// @tparam T unsigned integer type
/// @param v value to reset the right most adjacent bit 1
/// @return reseted value
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T reset_first_adjacent(T v) noexcept {
  return (setr(v) + 1) & v;
}

} // namespace es
#endif
