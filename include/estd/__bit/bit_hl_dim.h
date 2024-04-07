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

#ifndef ESTD___BIT_BIT_HIGHLIGHY_DIM_H
#define ESTD___BIT_BIT_HIGHLIGHY_DIM_H
#include <type_traits>

namespace es {

/// @brief highlight the first zero bit
///
/// algorithm: `~v & (v + 1)`
/// e.g. 10100111 => 00001000
///
/// @tparam T unsigned integer type
/// @param v value to highlight
/// @return highlighed
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T hl_first_bit0(T v) noexcept {
  return ~v & (v + 1);
}

/// @brief highlight the first non-zero bit
///
/// algorithm: `v & (-v)`
/// e.g. 10101000 => 11110111 => 00001000
///
/// @tparam T unsigned integer type
/// @param v value to highlight
/// @return highlighed
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T hl_first_bit1(T v) noexcept {
  return v & (-v);
}

/// @brief dim the first non-zero bit
///
/// algorithm: `~v | (v - 1)`
/// e.g. 10101000 => 11110111
///
/// @tparam T unsigned integer type
/// @param v value to dim
/// @return dimed
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T dim_first_bit1(T v) noexcept {
  return ~v | (v - 1);
}

/// @brief highlight the trailing zero bits
///
/// algorithm: `~v & (v - 1)`
/// e.g. 01011000 => 00000111
///
/// @tparam T unsigned integer type
/// @param v value to highlight
/// @return highlighted
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T hlr_bit0(T v) noexcept {
  return ~v & (v - 1);
}

/// @brief highlight the trailing zero bits with first bit 1 include
///
/// algorithm: `v ^ (v - 1)`
/// e.g. 01011000 => 00001111
///
/// @tparam T unsigned integer type
/// @param v value to highlight
/// @return highlighted
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T hlr_bit0_inc(T v) noexcept {
  return v ^ (v - 1);
}

/// @brief dim the trailing non-zero bits
///
/// algorithm: `~v | (v + 1)`
/// e.g. 10100111 => 11111000
///
/// @tparam T unsigned integer type
/// @param v value to dim
/// @return dimed
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T dimr_bit1(T v) noexcept {
  return ~v | (v + 1);
}

/// @brief highlight the trailing none-zero bits with first bit 0 include
///
/// algorithm: `v ^ (v + 1)`
/// e.g. 01010111 => 00001111
///
/// @tparam T unsigned integer type
/// @param v value to highlight
/// @return highlighted
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr T hlr_bit1_inc(T v) noexcept {
  return v ^ (v + 1);
}

} // namespace es
#endif
