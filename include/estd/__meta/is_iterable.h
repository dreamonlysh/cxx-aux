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

#ifndef ESTD___META_IS_ITERABLE_H
#define ESTD___META_IS_ITERABLE_H
#include "has_member.h"

namespace es {

/// @brief has_member_begin, has_member_begin_v
META_HAS_MEMBER_FUNCTION(begin);
/// @brief has_member_end, has_member_end_v
META_HAS_MEMBER_FUNCTION(end);

/// @brief whether a container can be visited by begin and end
/// @tparam T a container
template <typename T>
struct is_iterable
    : std::bool_constant<has_member_begin_v<T> && has_member_end_v<T>> {};

/// @brief whether a container can be visited by begin and end
/// @tparam T a container
template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

} // namespace es
#endif
