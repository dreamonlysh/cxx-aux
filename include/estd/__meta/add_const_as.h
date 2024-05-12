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

#ifndef ESTD___META_ADD_CONST_AS_H
#define ESTD___META_ADD_CONST_AS_H
#include <type_traits>

namespace es {

/// @brief add const to T if U is const
/// @tparam T type to add const
/// @tparam U type to traits const
template <typename T, typename U>
struct add_const_as {
  using type = std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>;
};

/// @brief add const to T if U is const
/// @tparam T type to add const
/// @tparam U type to traits const
template <typename T, typename U>
using add_const_as_t = typename add_const_as<T, U>::type;

} // namespace es
#endif
