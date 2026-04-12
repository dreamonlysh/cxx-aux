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

#ifndef ESTD___BIT_BIT_HIGHLIGHT_DIM_H
#define ESTD___BIT_BIT_HIGHLIGHT_DIM_H
#include <type_traits>

namespace es {

/**
 * @brief Highlights the first zero bit (from the right).
 *
 * Sets all bits to 0 except the first zero bit, which becomes 1.
 *
 * Algorithm: `~v & (v + 1)`
 * - v + 1 turns the rightmost 0 into 1 and trailing 1s into 0s
 * - ~v inverts all bits
 * - AND operation isolates the position of the rightmost 0
 *
 * Example: 10100111 => 00001000
 *
 * @tparam T Unsigned integer type
 * @param v Value to highlight
 * @return Value with only the first zero bit set
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T hl_first_bit0(T v) noexcept {
  return ~v & (v + 1);
}

/**
 * @brief Highlights the first non-zero bit (from the right).
 *
 * Sets all bits to 0 except the first set bit, which remains 1.
 *
 * Algorithm: `v & (-v)`
 * - -v is the two's complement, which flips all bits left of the rightmost 1
 * - AND operation isolates the rightmost 1
 *
 * Example: 10101000 => 00001000
 *
 * @tparam T Unsigned integer type
 * @param v Value to highlight
 * @return Value with only the first non-zero bit set
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T hl_first_bit1(T v) noexcept {
  return v & (-v);
}

/**
 * @brief Dims the first non-zero bit (from the right).
 *
 * Turns off the rightmost 1 bit.
 *
 * Algorithm: `~v | (v - 1)`
 * - v - 1 turns the rightmost 1 into 0 and trailing 0s into 1s
 * - ~v inverts all bits
 * - OR operation creates a mask that dims the rightmost 1
 *
 * Example: 10101000 => 11110111
 *
 * @tparam T Unsigned integer type
 * @param v Value to dim
 * @return Value with the first non-zero bit dimmed
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T dim_first_bit1(T v) noexcept {
  return ~v | (v - 1);
}

/**
 * @brief Highlights the trailing zero bits.
 *
 * Sets all trailing zero bits to 1, all other bits to 0.
 *
 * Algorithm: `~v & (v - 1)`
 * - v - 1 turns trailing 0s into 1s and the rightmost 1 into 0
 * - ~v inverts all bits
 * - AND operation isolates the trailing zeros
 *
 * Example: 01011000 => 00000111
 *
 * @tparam T Unsigned integer type
 * @param v Value to highlight
 * @return Value with trailing zero bits highlighted
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T hlr_bit0(T v) noexcept {
  return ~v & (v - 1);
}

/**
 * @brief Highlights the trailing zero bits including the first bit 1.
 *
 * Sets all trailing zeros and the first 1 to 1, all other bits to 0.
 *
 * Algorithm: `v ^ (v - 1)`
 * - v - 1 changes the trailing pattern
 * - XOR operation highlights the changed bits
 *
 * Example: 01011000 => 00001111
 *
 * @tparam T Unsigned integer type
 * @param v Value to highlight
 * @return Value with trailing zeros and first 1 highlighted
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T hlr_bit0_inc(T v) noexcept {
  return v ^ (v - 1);
}

/**
 * @brief Dims the trailing non-zero bits.
 *
 * Turns off all trailing 1 bits.
 *
 * Algorithm: `~v | (v + 1)`
 * - v + 1 turns trailing 1s into 0s and the rightmost 0 into 1
 * - ~v inverts all bits
 * - OR operation creates a mask that dims trailing 1s
 *
 * Example: 10100111 => 11111000
 *
 * @tparam T Unsigned integer type
 * @param v Value to dim
 * @return Value with trailing non-zero bits dimmed
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T dimr_bit1(T v) noexcept {
  return ~v | (v + 1);
}

/**
 * @brief Highlights the trailing non-zero bits including the first bit 0.
 *
 * Sets all trailing 1s and the first 0 to 1, all other bits to 0.
 *
 * Algorithm: `v ^ (v + 1)`
 * - v + 1 changes the trailing pattern
 * - XOR operation highlights the changed bits
 *
 * Example: 01010111 => 00001111
 *
 * @tparam T Unsigned integer type
 * @param v Value to highlight
 * @return Value with trailing 1s and first 0 highlighted
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T hlr_bit1_inc(T v) noexcept {
  return v ^ (v + 1);
}

} // namespace es
#endif
