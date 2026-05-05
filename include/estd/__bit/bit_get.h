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
#include "bit_count.h"
#include "bit_hl_dim.h"
#include "bit_set_reset.h"
#include <immintrin.h>
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
[[nodiscard]] constexpr T get_bit(T v, unsigned pos) noexcept {
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
[[nodiscard]] constexpr T get_bits(T v, unsigned pos, unsigned n) noexcept {
  return (v >> pos) & (std::numeric_limits<T>::max() >>
                       (std::numeric_limits<T>::digits - n));
}

/**
 * @brief Extracts the position of the nth set bit as a bitmask.
 *
 * Returns a value with a single bit set at the position of the nth set bit
 * (1-bit) in the input value, counting from the right (LSB).
 *
 * @tparam T Unsigned integer type
 * @param v Value to extract bit position from
 * @param n Index of the set bit to find (0-based, counting set bits from right)
 * @return Bitmask with a single bit set at the nth set bit position,
 *         or 0 if n >= count_bit1(v)
 *
 * Example usage:
 * @code
 * // 0b10101010 has set bits at positions 1, 3, 5, 7
 * assert(get_nth_bit(0b10101010u, 0) == 0b00000010);  // 1st set bit at pos 1
 * assert(get_nth_bit(0b10101010u, 1) == 0b00001000);  // 2nd set bit at pos 3
 * assert(get_nth_bit(0b10101010u, 2) == 0b00100000);  // 3rd set bit at pos 5
 * assert(get_nth_bit(0b10101010u, 3) == 0b10000000);  // 4th set bit at pos 7
 * assert(get_nth_bit(0b10101010u, 4) == 0);           // No 5th set bit
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] inline T get_nth_bit(T v, unsigned n) noexcept {
  if (n >= count_bit1(v)) {
    return 0;
  }

#if defined(__BMI2__) && (defined(__x86_64__) || defined(_M_X64))
  if constexpr (sizeof(T) <= 4) {
    uint32_t x = static_cast<uint32_t>(v);
    return static_cast<T>(_pdep_u32(1u << n, x));
  } else {
    uint64_t x = static_cast<uint64_t>(v);
    return static_cast<T>(_pdep_u64(1ull << n, x));
  }
#else
  T tmp = v;
  for (unsigned i = 0; i < n; ++i) {
    tmp = reset_first(tmp);
  }
  return hl_first_bit1(tmp);
#endif
}

} // namespace es
#endif
