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

#ifndef ESTD___BIT_BIT_POW2_H
#define ESTD___BIT_BIT_POW2_H
#include "bit_count.h"
#include "bit_set_reset.h"
#include <limits>

namespace es {

/**
 * @brief Checks if a value is a power of 2.
 *
 * A value is a power of 2 if it has exactly one bit set.
 * This function returns true for v = 0 (0 = 2^(-∞)).
 *
 * Algorithm: Uses reset_first() to clear the rightmost 1 bit,
 * then checks if the result is 0.
 *
 * @tparam T Unsigned integer type
 * @param v Value to check
 * @return true if v is a power of 2 (including 0)
 *
 * Example usage:
 * @code
 * assert(is_pow2(0u));
 * assert(is_pow2(1u));
 * assert(is_pow2(2u));
 * assert(is_pow2(4u));
 * assert(is_pow2(1024u));
 * assert(!is_pow2(3u));
 * assert(!is_pow2(5u));
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr bool is_pow2(T v) noexcept {
  return reset_first(v) == 0;
}

/**
 * @brief Checks if a value is one less than a power of 2.
 *
 * A value is 2^n - 1 if all bits from 0 to n-1 are set.
 * This function returns true for v = 0 (0 = 2^0 - 1).
 *
 * Algorithm: Uses resetr() to clear trailing 1 bits,
 * then checks if the result is 0.
 *
 * @tparam T Unsigned integer type
 * @param v Value to check
 * @return true if v is one less than a power of 2 (including 0)
 *
 * Example usage:
 * @code
 * assert(is_pow2_sub1(0u));   // 2^0 - 1
 * assert(is_pow2_sub1(1u));   // 2^1 - 1
 * assert(is_pow2_sub1(3u));   // 2^2 - 1
 * assert(is_pow2_sub1(7u));   // 2^3 - 1
 * assert(is_pow2_sub1(15u));  // 2^4 - 1
 * assert(!is_pow2_sub1(2u));
 * assert(!is_pow2_sub1(5u));
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr bool is_pow2_sub1(T v) noexcept {
  return resetr(v) == 0;
}

/**
 * @brief Checks if a value is a difference of two powers of 2.
 *
 * A value is 2^n - 2^m (where n > m) if it has a contiguous range
 * of set bits. This function returns true for v = 0 (0 = 2^0 - 2^0).
 *
 * Algorithm: Uses reset_first_adjacent() to clear the rightmost
 * adjacent 1 bits, then checks if the result is 0.
 *
 * @tparam T Unsigned integer type
 * @param v Value to check
 * @return true if v is a difference of two powers of 2 (including 0)
 *
 * Example usage:
 * @code
 * assert(is_pow2_sub_pow2(0u));    // 2^1 - 2^1
 * assert(is_pow2_sub_pow2(2u));    // 2^2 - 2^1
 * assert(is_pow2_sub_pow2(12u));   // 2^4 - 2^2
 * assert(is_pow2_sub_pow2(28u));   // 2^5 - 2^2
 * assert(!is_pow2_sub_pow2(5u));   // Not contiguous
 * assert(!is_pow2_sub_pow2(10u));  // Not contiguous
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr bool is_pow2_sub_pow2(T v) noexcept {
  return reset_first_adjacent(v) == 0;
}

/**
 * @brief Rounds down to the nearest power of 2.
 *
 * Returns the largest power of 2 that is less than or equal to v.
 *
 * Algorithm: `1 << (bits - 1 - clz(v))`
 * - Finds the position of the most significant 1 bit
 * - Returns 2^(position)
 *
 * @tparam T Unsigned integer type
 * @param v Value to round down
 * @return Largest power of 2 <= v, or 0 if v is 0
 *
 * Example usage:
 * @code
 * assert(floor_to_pow2(0u) == 0u);
 * assert(floor_to_pow2(1u) == 1u);
 * assert(floor_to_pow2(2u) == 2u);
 * assert(floor_to_pow2(3u) == 2u);
 * assert(floor_to_pow2(5u) == 4u);
 * assert(floor_to_pow2(100u) == 64u);
 * assert(floor_to_pow2(1023u) == 512u);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T floor_to_pow2(T v) noexcept {
  // 0 is not suitable for the following algorithm
  if (v == 0)
    return 0;
  return set_bit<T>(0, std::numeric_limits<T>::digits - 1) >> countl_bit0(v);
}

/**
 * @brief Rounds up to the nearest power of 2.
 *
 * Returns the smallest power of 2 that is greater than or equal to v.
 *
 * Algorithm: `1 << (bits - clz(v - 1))`
 * - Subtracts 1 to handle values that are already powers of 2
 * - Finds the position where the next power of 2 would be
 * - Returns 2^(position)
 *
 * @tparam T Unsigned integer type
 * @param v Value to round up
 * @return Smallest power of 2 >= v, or 0 if v is 0 or overflow occurs
 *
 * @note Returns 0 if the result would overflow (v > 2^(bits-1))
 * @note Returns v unchanged if v is 0 or 1
 *
 * Example usage:
 * @code
 * assert(ceil_to_pow2(0u) == 0u);
 * assert(ceil_to_pow2(1u) == 1u);
 * assert(ceil_to_pow2(2u) == 2u);
 * assert(ceil_to_pow2(3u) == 4u);
 * assert(ceil_to_pow2(5u) == 8u);
 * assert(ceil_to_pow2(100u) == 128u);
 * assert(ceil_to_pow2(1023u) == 1024u);
 * // Overflow case:
 * assert(ceil_to_pow2(0x80000001u) == 0u);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
[[nodiscard]] constexpr T ceil_to_pow2(T v) noexcept {
  // 0 and 1 is not suitable for the following algorithm
  if ((v >> 1) == 0)
    return v;

  if constexpr (std::is_same_v<T, unsigned>) {
    // For unsigned int, use unsigned long long to avoid overflow check
    // This saves 4-6 instructions compared to checking for overflow
    return static_cast<T>(
        set_bit(0ull, std::numeric_limits<unsigned long long>::digits - 1) >>
        (countl_bit0<unsigned long long>(v - 1) - 1));
  } else if constexpr (std::is_same_v<T, unsigned long> ||
                       std::is_same_v<T, unsigned long long>) {
    // For larger types, check for overflow explicitly
    constexpr auto LAST_BIT_ON =
        set_bit<T>(0, std::numeric_limits<T>::digits - 1);
    return v > LAST_BIT_ON ? static_cast<T>(0u)
                           : (LAST_BIT_ON >> (countl_bit0(v - 1) - 1));
  } else {
    // For smaller types, use unsigned int for calculation
    return static_cast<T>(
        set_bit(0u, std::numeric_limits<unsigned>::digits - 1) >>
        (countl_bit0<unsigned>(v - 1) - 1));
  }
}

} // namespace es
#endif