// Copyright (c) 2025 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD_STRING_ALGORITHM_H
#define ESTD_STRING_ALGORITHM_H

#include "__algorithm/append.h"
// template <typename StringLike, typename = std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike>>>
// constexpr void append(StringLike& s, typename StringLike::value_type c);
// template <typename StringLike, typename = std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike>>>
// constexpr void append(StringLike& s, typename StringLike::size_type n,
//                       typename StringLike::value_type c);
// template <typename StringLike, typename = std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike>>>
// constexpr void append(StringLike& s, const typename StringLike::value_type*
// p,
//                       typename StringLike::size_type n);
// template <typename StringLike, typename = std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike>>>
// constexpr void append(StringLike& s, const typename StringLike::value_type*
// p); template <typename StringLike, typename StringViewLike, typename =
// std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike> &&
//     __impl_type_traits::is_string_view_like_v<StringViewLike>>>
// constexpr void append(StringLike& s, const StringViewLike& sv);
// template <typename StringLike, typename StringViewLike, typename =
// std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike> &&
//     __impl_type_traits::is_string_view_like_v<StringViewLike>>>
// constexpr void append(StringLike& s, const StringViewLike& sv,
//                       typename StringLike::size_type pos,
//                       typename StringLike::size_type n =
//                       StringViewLike::npos);
// template <typename StringLike, typename IntegralT, typename =
// std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike> &&
//     std::is_integral_v<IntegralT> && !std::is_same_v<IntegralT, typename
//     StringLike::value_type>>>
// constexpr void append(StringLike& s, IntegralT v);
// template <typename StringLike, typename = std::enable_if_t<
//     __impl_type_traits::is_string_like_v<StringLike>>>
// constexpr void append(StringLike& s,
//                       std::initializer_list<typename StringLike::value_type>
//                       il);
// template <typename StringLike, typename InputIterator,
//           typename = std::enable_if_t<
//               __impl_type_traits::is_string_like_v<StringLike>>>
// constexpr void append(StringLike& s, InputIterator first, InputIterator
// last);
//
// template <typename StringLike, typename... Args>
// constexpr void append_n(StringLike& s, Args&&... args);

#include "__algorithm/replace.h"
// template <typename StringLike,
//           typename = std::enable_if_t<
//               __impl_string_algorithm::is_string_like_v<StringLike>>>
// void replace(StringLike& s, typename StringLike::size_type pos,
//              typename StringLike::size_type count,
//              const typename StringLike::value_type* p,
//              typename StringLike::size_type n);
// template <typename StringLike,
//           typename = std::enable_if_t<
//               __impl_string_algorithm::is_string_like_v<StringLike>>>
// void replace(StringLike& s, typename StringLike::size_type pos,
//              typename StringLike::size_type count,
//              const typename StringLike::value_type* p);
// template <typename StringLike, typename StringViewLike,
//           typename = std::enable_if_t<
//               __impl_string_algorithm::is_string_like_v<StringLike> &&
//               __impl_string_algorithm::is_string_view_like_v<StringViewLike>>>
// void replace(StringLike& s, typename StringLike::size_type pos,
//              typename StringLike::size_type count, const StringViewLike& sv);
// template <typename StringLike,
//           typename = std::enable_if_t<
//               __impl_string_algorithm::is_string_like_v<StringLike>>>
// void replace(StringLike& s, typename StringLike::size_type pos,
//         typename StringLike::size_type count, typename StringLike::size_type
//         n, typename StringLike::value_type c);
// template <typename StringLike,
//           typename = std::enable_if_t<
//               __impl_string_algorithm::is_string_like_v<StringLike>>>
// void replace(StringLike& s, typename StringLike::size_type pos,
//              typename StringLike::size_type count,
//              std::initializer_list<typename StringLike::value_type> il);

#endif
