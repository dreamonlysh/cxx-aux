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
#include <estd/meta.h>
#include <limits>

namespace es {

/// @brief count the non-zero bits
/// @tparam T unsigned integer type
/// @param v value to count
/// @return number of bit 1
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

/// @brief count the zero bits
/// @tparam T unsigned integer type
/// @param v value to count
/// @return number of bit 0
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned count_bit0(T v) noexcept {
  return count_bit1<T>(~v);
}

/// @brief count the adjacent zero bits from left
/// @tparam T unsigned integer type
/// @param v value to count
/// @return number of bit 0
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
    return static_cast<unsigned>(__builtin_clz(v) -
                                 (std::numeric_limits<unsigned>::digits -
                                  std::numeric_limits<T>::digits));
  }
}

/// @brief count the adjacent non-zero bits from left
/// @tparam T unsigned integer type
/// @param v value to count
/// @return number of bit 1
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned countl_bit1(T v) noexcept {
  return countl_bit0<T>(~v);
}

/// @brief count the adjacent zero bits from right
/// @tparam T unsigned integer type
/// @param v value to count
/// @return number of bit 0
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned countr_bit0(T v) noexcept {
  // __builtin_clz is undefined with 0
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

/// @brief count the adjacent zero bits from right
/// @tparam T unsigned integer type
/// @param v value to count
/// @return number of bit 1
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned countr_bit1(T v) noexcept {
  return countr_bit0<T>(~v);
}

} // namespace es
#endif
