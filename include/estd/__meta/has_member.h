// Copyright (c) 2023 guyuemeng
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

#ifndef ESTD___META_HAS_MEMBER_H
#define ESTD___META_HAS_MEMBER_H
#include <type_traits>

namespace es {

#define __META_VALIDATE_CONTEXT(name, cond)                                    \
  template <typename T, typename = std::void_t<>>                              \
  struct name : std::false_type {};                                            \
  template <typename T>                                                        \
  struct name<T, std::void_t<cond>> : std::true_type {};                       \
  template <typename T>                                                        \
  constexpr bool name##_v = name<T>::value

/// @brief macro use to define the `has_member_{type}` meta function
#define META_HAS_MEMBER_TYPE(type)                                             \
  __META_VALIDATE_CONTEXT(has_member_##type, typename T::type)

/// @brief macro use to define the `has_member_{data}` meta function
#define META_HAS_MEMBER_DATA(data)                                             \
  __META_VALIDATE_CONTEXT(has_member_##data, decltype(std::declval<T>().data))

/// @brief macro use to define the `has_member_{func}` meta function
#define META_HAS_MEMBER_FUNCTION(func)                                         \
  __META_VALIDATE_CONTEXT(has_member_##func, decltype(std::declval<T>().func()))

/// @brief has_member_iterator, has_member_iterator_v
META_HAS_MEMBER_TYPE(iterator);

} // namespace es
#endif // ESTD___META_HAS_MEMBER_H
