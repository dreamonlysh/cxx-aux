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

#ifndef ESTD___BIT_BIT_COUNT_H
#define ESTD___BIT_BIT_COUNT_H
#include "bit_hl_dim.h"
#include <limits>

namespace es {

/**
 * @brief Counts the number of set bits (bit 1) in an unsigned integer.
 *
 * Uses compiler intrinsics for optimal performance.
 *
 * @tparam T Unsigned integer type
 * @param v Value to count
 * @return Number of bits set to 1
 *
 * Example usage:
 * @code
 * assert(count_bit1(0b10101010u) == 4);
 * assert(count_bit1(0xFFFFFFFFu) == 32);
 * assert(count_bit1(0u) == 0);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned count_bit1(T v) noexcept {
  if constexpr (std::is_same_v<T, unsigned long>) {
    return static_cast<unsigned>(__builtin_popcountl(v));
  } else if constexpr (std::is_same_v<T, unsigned long long>) {
    return static_cast<unsigned>(__builtin_popcountll(v));
  } else {
    return static_cast<unsigned>(__builtin_popcount(v));
  }
}

/**
 * @brief Counts the number of unset bits (bit 0) in an unsigned integer.
 *
 * @tparam T Unsigned integer type
 * @param v Value to count
 * @return Number of bits set to 0
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned count_bit0(T v) noexcept {
  return count_bit1<T>(~v);
}

/**
 * @brief Counts the number of leading zero bits from the left.
 *
 * Counts consecutive zero bits starting from the most significant bit.
 *
 * @tparam T Unsigned integer type
 * @param v Value to count
 * @return Number of leading zero bits
 *
 * @note Returns std::numeric_limits<T>::digits if v is 0
 *
 * Example usage:
 * @code
 * assert(countl_bit0(0b00010100u) == 27);  // 32-bit unsigned
 * assert(countl_bit0(0xFFFFFFFFu) == 0);
 * assert(countl_bit0(0u) == 32);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned countl_bit0(T v) noexcept {
  // __builtin_clz is undefined with 0
  if (v == 0)
    return std::numeric_limits<T>::digits;

  if constexpr (std::is_same_v<T, unsigned>) {
    return static_cast<unsigned>(__builtin_clz(v));
  } else if constexpr (std::is_same_v<T, unsigned long>) {
    return static_cast<unsigned>(__builtin_clzl(v));
  } else if constexpr (std::is_same_v<T, unsigned long long>) {
    return static_cast<unsigned>(__builtin_clzll(v));
  } else {
    // For types smaller than unsigned, adjust the count
    return static_cast<unsigned>(__builtin_clz(v) -
                                 (std::numeric_limits<unsigned>::digits -
                                  std::numeric_limits<T>::digits));
  }
}

/**
 * @brief Counts the number of leading one bits from the left.
 *
 * @tparam T Unsigned integer type
 * @param v Value to count
 * @return Number of leading one bits
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned countl_bit1(T v) noexcept {
  return countl_bit0<T>(~v);
}

/**
 * @brief Counts the number of trailing zero bits from the right.
 *
 * Counts consecutive zero bits starting from the least significant bit.
 *
 * @tparam T Unsigned integer type
 * @param v Value to count
 * @return Number of trailing zero bits
 *
 * @note Returns std::numeric_limits<T>::digits if v is 0
 *
 * Example usage:
 * @code
 * assert(countr_bit0(0b10100000u) == 5);
 * assert(countr_bit0(0xFFFFFFFFu) == 0);
 * assert(countr_bit0(0u) == 32);
 * @endcode
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned countr_bit0(T v) noexcept {
  // __builtin_ctz is undefined with 0
  if (v == 0)
    return std::numeric_limits<T>::digits;

  if constexpr (std::is_same_v<T, unsigned>) {
    return static_cast<unsigned>(__builtin_ctz(v));
  } else if constexpr (std::is_same_v<T, unsigned long>) {
    return static_cast<unsigned>(__builtin_ctzl(v));
  } else if constexpr (std::is_same_v<T, unsigned long long>) {
    return static_cast<unsigned>(__builtin_ctzll(v));
  } else {
    return static_cast<unsigned>(__builtin_ctz(v));
  }
}

/**
 * @brief Counts the number of trailing one bits from the right.
 *
 * @tparam T Unsigned integer type
 * @param v Value to count
 * @return Number of trailing one bits
 */
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned countr_bit1(T v) noexcept {
  return countr_bit0<T>(~v);
}

} // namespace es
#endif
