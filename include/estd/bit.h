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

#ifndef ESTD_BIT_H
#define ESTD_BIT_H

/**
 * @defgroup bit Bit manipulation utilities
 *
 * Bit operations following a consistent naming convention:
 * - **count**: Count the number of bit 0 or 1
 * - **hl** (highlight): Set specific bits to 1, others to 0
 * - **dim**: Set specific bits to 0, others to 1
 * - **set/reset**: Set bits to 1/0
 * - **is**: Validate bits match
 *
 * Range suffixes:
 * - **l/r**: Start from left (highest) / right (lowest) with adjacent bits
 * - **inc**: Include the first opposite bit
 * - **first**: Find from the right, choose the first matched
 * - **adjacent**: Include adjacent same bits
 *
 * Bit keywords:
 * - **bit0**: Bit 0 (hidden when set)
 * - **bit1**: Bit 1 (hidden when reset)
 * - **pow2**: 2^n (only one bit 1)
 *
 * @note All functions require unsigned integer types
 * @{
 */

#include "__bit/bit_count.h"
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr unsigned count_bit1(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr unsigned count_bit0(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr unsigned countl_bit0(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr unsigned countl_bit1(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr unsigned countr_bit0(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr unsigned countr_bit1(T v) noexcept;

#include "__bit/bit_hl_dim.h"
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T hl_first_bit0(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T hl_first_bit1(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T dim_first_bit1(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T hlr_bit0(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T hlr_bit0_inc(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T dimr_bit1(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T hlr_bit1_inc(T v) noexcept;

#include "__bit/bit_pow2.h"
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr bool is_pow2(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr bool is_pow2_sub1(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr bool is_pow2_sub_pow2(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T floor_to_pow2(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T ceil_to_pow2(T v) noexcept;

#include "__bit/bit_get.h"
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T get_bit(T v, unsigned pos) noexcept
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T get_bits(T v, unsigned pos, unsigned n) noexcept
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T get_nth_bit(T v, unsigned n) noexcept

#include "__bit/bit_set_reset.h"
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T set_bit(T v, unsigned pos) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T set_bits(T v, unsigned pos, unsigned n) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T reset_first(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T set_first(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T resetr(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T setr(T v) noexcept;
// template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
// constexpr T reset_first_adjacent(T v) noexcept;

/** @} */

#endif
