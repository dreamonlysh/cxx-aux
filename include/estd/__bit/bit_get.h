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

/**
 * @brief Extracts a single bit from an unsigned integer.
 *
 * Returns the value of the bit at the specified position, shifted to
 * the least significant bit position (either 0 or 1).
 *
 * @tparam T Unsigned integer type
 * @param v Value to extract bit from
 * @param pos Bit position in range [0, bits_of(T))
 * @return The bit value (0 or 1) at position pos
 *
 * @warning Behavior is undefined if pos >= bits_of(T)
 *
 * Example usage:
 * @code
 * unsigned value = 0b10101010;
 * assert(get_bit(value, 0) == 0);  // LSB
 * assert(get_bit(value, 1) == 1);
 * assert(get_bit(value, 7) == 1);  // MSB
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T get_bit(T v, unsigned pos) noexcept {
  return (v >> pos) & static_cast<T>(1u);
}

/**
 * @brief Extracts multiple bits from an unsigned integer.
 *
 * Extracts n bits starting from position pos, returning them as a
 * right-aligned value with leading zeros.
 *
 * @tparam T Unsigned integer type
 * @param v Value to extract bits from
 * @param pos Starting bit position in range [0, bits_of(T))
 * @param n Number of bits to extract in range (0, bits_of(T)]
 * @return The extracted bits, right-aligned
 *
 * @warning Behavior is undefined if (pos + n) > bits_of(T)
 *
 * Example usage:
 * @code
 * unsigned value = 0b11010110;
 * assert(get_bits(value, 2, 3) == 0b101);  // Extract bits 2-4
 * assert(get_bits(value, 0, 4) == 0b0110); // Extract bits 0-3
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T get_bits(T v, unsigned pos, unsigned n) noexcept {
  return (v >> pos) & (std::numeric_limits<T>::max() >>
                       (std::numeric_limits<T>::digits - n));
}

} // namespace es
#endif
