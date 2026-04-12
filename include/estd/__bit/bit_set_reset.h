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

/**
 * @brief Sets a single bit in an unsigned integer.
 *
 * Sets the bit at the specified position to 1. If v is 0, this generates
 * a single-bit mask at position pos.
 *
 * @tparam T Unsigned integer type
 * @param v Value to modify (use 0 to generate a mask)
 * @param pos Bit position in range [0, bits_of(T))
 * @return Value with bit at pos set to 1
 *
 * @warning Behavior is undefined if pos >= bits_of(T)
 *
 * Example usage:
 * @code
 * unsigned value = 0u;
 * assert(set_bit(value, 0) == 0b00000001);
 * assert(set_bit(value, 31) == 0x80000000);
 *
 * // Set bit in existing value
 * unsigned flags = 0b00000010;
 * assert(set_bit(flags, 0) == 0b00000011);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T set_bit(T v, unsigned pos) noexcept {
  return v | static_cast<T>(1u) << pos;
}

/**
 * @brief Sets multiple bits in an unsigned integer.
 *
 * Sets n consecutive bits starting from position pos to 1.
 * If v is 0, this generates an n-bit mask starting at position pos.
 *
 * @tparam T Unsigned integer type
 * @param v Value to modify (use 0 to generate a mask)
 * @param pos Starting bit position in range [0, bits_of(T))
 * @param n Number of bits to set in range (0, bits_of(T)]
 * @return Value with n bits set starting at pos
 *
 * @warning Behavior is undefined if (pos + n) > bits_of(T)
 *
 * Example usage:
 * @code
 * unsigned value = 0u;
 * assert(set_bits(value, 0, 1) == 0b00000001);
 * assert(set_bits(value, 0, 32) == 0xFFFFFFFF);
 *
 * // Set bits in existing value
 * unsigned flags = 0b00000001;
 * assert(set_bits(flags, 1, 3) == 0b00001111);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T set_bits(T v, unsigned pos, unsigned n) noexcept {
  return v | ((std::numeric_limits<T>::max() >>
               (std::numeric_limits<T>::digits - n))
              << pos);
}

/**
 * @brief Turns off the rightmost 1 bit.
 *
 * Clears the least significant set bit to 0.
 *
 * Algorithm: `v & (v - 1)`
 * - v - 1 turns the rightmost 1 into 0 and trailing 0s into 1s
 * - AND operation clears the rightmost 1
 *
 * Example: 01011000 => 01010000
 *
 * @tparam T Unsigned integer type
 * @param v Value to modify
 * @return Value with rightmost 1 bit cleared
 *
 * @note Returns 0 if v is 0
 *
 * Example usage:
 * @code
 * unsigned value = 0b01011000;
 * assert(reset_first(value) == 0b01010000);
 *
 * // Useful for iterating through set bits
 * unsigned bits = 0b10101010;
 * while (bits != 0) {
 *     // Process rightmost set bit
 *     bits = reset_first(bits);
 * }
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T reset_first(T v) noexcept {
  return v & (v - 1);
}

/**
 * @brief Turns on the rightmost 0 bit.
 *
 * Sets the least significant clear bit to 1.
 *
 * Algorithm: `v | (v + 1)`
 * - v + 1 turns the rightmost 0 into 1 and trailing 1s into 0s
 * - OR operation sets the rightmost 0
 *
 * Example: 10100111 => 10101111
 *
 * @tparam T Unsigned integer type
 * @param v Value to modify
 * @return Value with rightmost 0 bit set
 *
 * @note Returns all 1s if v is all 1s
 *
 * Example usage:
 * @code
 * unsigned value = 0b10100111;
 * assert(set_first(value) == 0b10101111);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T set_first(T v) noexcept {
  return v | (v + 1);
}

/**
 * @brief Turns off trailing 1 bits.
 *
 * Clears all consecutive 1 bits starting from the least significant bit.
 *
 * Algorithm: `v & (v + 1)`
 * - v + 1 turns trailing 1s into 0s and the rightmost 0 into 1
 * - AND operation clears trailing 1s
 *
 * Example: 10100111 => 10100000
 *
 * @tparam T Unsigned integer type
 * @param v Value to modify
 * @return Value with trailing 1 bits cleared
 *
 * @note Returns v unchanged if LSB is 0
 *
 * Example usage:
 * @code
 * unsigned value = 0b10100111;
 * assert(resetr(value) == 0b10100000);
 *
 * unsigned value2 = 0b10101000;
 * assert(resetr(value2) == 0b10101000);  // No trailing 1s
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T resetr(T v) noexcept {
  return v & (v + 1);
}

/**
 * @brief Turns on trailing 0 bits.
 *
 * Sets all consecutive 0 bits starting from the least significant bit.
 *
 * Algorithm: `v | (v - 1)`
 * - v - 1 turns trailing 0s into 1s and the rightmost 1 into 0
 * - OR operation sets trailing 0s
 *
 * Example: 10101000 => 10101111
 *
 * @tparam T Unsigned integer type
 * @param v Value to modify
 * @return Value with trailing 0 bits set
 *
 * @note Returns v unchanged if LSB is 1
 *
 * Example usage:
 * @code
 * unsigned value = 0b10101000;
 * assert(setr(value) == 0b10101111);
 *
 * unsigned value2 = 0b10101011;
 * assert(setr(value2) == 0b10101011);  // No trailing 0s
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T setr(T v) noexcept {
  return v | (v - 1);
}

/**
 * @brief Turns off the rightmost adjacent 1 bits.
 *
 * Clears all consecutive 1 bits starting from the rightmost 1 bit.
 * This is useful for finding isolated bit fields.
 *
 * Algorithm: `((v | (v - 1)) + 1) & v`
 * - v | (v - 1) sets trailing 0s after the rightmost 1
 * - +1 carries through the set bits
 * - AND with v isolates the result
 *
 * Example: 01011100 => 01000000
 *
 * @tparam T Unsigned integer type
 * @param v Value to modify
 * @return Value with rightmost adjacent 1 bits cleared
 *
 * @note Returns 0 if v is 0
 *
 * Example usage:
 * @code
 * unsigned value = 0b01011100;
 * assert(reset_first_adjacent(value) == 0b01000000);
 *
 * unsigned value2 = 0b01011000;
 * assert(reset_first_adjacent(value2) == 0b01000000);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T reset_first_adjacent(T v) noexcept {
  return (setr(v) + 1) & v;
}

} // namespace es
#endif
