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

/// @brief check if is 2**n
///
/// v = 0, return true
///
/// @tparam T unsigned integer type
/// @param v value to check
/// @return is pow2
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr bool is_pow2(T v) noexcept {
  return reset_first(v) == 0;
}

/// @brief check if is 2**n-1
///
/// v = 0, return true
///
/// @tparam T unsigned integer type
/// @param v value to check
/// @return is pow2 sub 1
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr bool is_pow2_sub1(T v) noexcept {
  return resetr(v) == 0;
}

/// @brief check if is 2**N-2**M
///
/// v = 0, return true
///
/// @tparam T unsigned integer type
/// @param v value to check
/// @return is pow2 sub pow2
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr bool is_pow2_sub_pow2(T v) noexcept {
  return reset_first_adjacent(v) == 0;
}

/// @brief floor to pow2
///
/// flp2(v) => 1<<(31-clz(v)) => 1<<(clz(v)^31) => 0x8000'0000>>clz(v)
/// v = 0, return 0
///
/// @tparam T unsigned integer type
/// @param v value to check
/// @return value of pow2
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T floor_to_pow2(T v) noexcept {
  // 0 is not suitable for the following algorithm
  if (v == 0)
    return 0;
  return set_bit<T>(0, std::numeric_limits<T>::digits - 1) >> countl_bit0(v);
}

/// @brief ceil to pow2
///
/// clp2(v) => 1<<(32-clz(v-1)) => 0x8000'0000>>(clz(v-1)-1)
/// v = 0, return 0
/// v > 0x80(00){sizeof(T)-1}, return 0
///
/// @tparam T unsigned integer type
/// @param v value to check
/// @return value of pow2
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T ceil_to_pow2(T v) noexcept {
  // 0 and 1 is not suitable for the following algorithm
  if ((v >> 1) == 0)
    return v;

  if constexpr (std::is_same_v<T, unsigned>) {
    // use clzll instead of clz will avoid one jmp instruction,
    // also saved total 4-6 instrutions compared each with clang and gcc
    // return v > LAST_BIT_ON ? 0u : (LAST_BIT_ON >> (__builtin_clz(v - 1) -
    // 1));
    return static_cast<T>(
        set_bit(0ull, std::numeric_limits<unsigned long long>::digits - 1) >>
        (countl_bit0<unsigned long long>(v - 1) - 1));
  } else if constexpr (std::is_same_v<T, unsigned long> ||
                       std::is_same_v<T, unsigned long long>) {
    constexpr auto LAST_BIT_ON =
        set_bit<T>(0, std::numeric_limits<T>::digits - 1);
    return v > LAST_BIT_ON ? static_cast<T>(0u)
                           : (LAST_BIT_ON >> (countl_bit0(v - 1) - 1));
  } else {
    return static_cast<T>(
        set_bit(0u, std::numeric_limits<unsigned>::digits - 1) >>
        (countl_bit0<unsigned>(v - 1) - 1));
  }
}

} // namespace es
#endif