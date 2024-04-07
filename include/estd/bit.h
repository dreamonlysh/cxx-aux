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

// keywords: operation
// - count: count the number of bit 0 or 1
// - hl: short for highlight, will set the specific bits to 1 and others to 0
// - dim: opposite to highlight, will set the specific bits to 0 and others to 1
// - set: set bits to 1
// - reset: set bits to 0
// - is: validate bits match
//
// keywords: range of bits to take effect
// l: suffix for operation, start from the left(highest) with adjacent bits
// r: suffix for operation, start from the right(lowest) with adjacent bits
// inc: short for include, used with l/r will include the first opposite bit
// first: find from the right(lowest) bit, choose the first matched
// adjacent: used with first will include the adjacent same bits
//
// keywords: bit
// bit: bit 0/1
// bit0: bit 0, hidden when set
// bit1: bit 1, hidden when reset
// pow2: 2**n, only one bit 1

#include <estd/__bit/bit_count.h>
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

#include <estd/__bit/bit_hl_dim.h>
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

#include <estd/__bit/bit_pow2.h>
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

#include <estd/__bit/bit_set_reset.h>
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

#endif
