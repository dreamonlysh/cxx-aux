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
  constexpr inline bool name##_v = name<T>::value

/**
 * @brief Macro to define a type trait that checks for a member type.
 *
 * This macro generates a type trait `has_member_{type}` that checks whether
 * a class has a specific member type (typedef, using, etc.).
 *
 * @param type The name of the member type to check for
 *
 * Generated traits:
 * - `has_member_{type}`: Type trait struct inheriting from std::true_type or
 * std::false_type
 * - `has_member_{type}_v`: Inline variable for the trait value
 *
 * Example usage:
 * @code
 * META_HAS_MEMBER_TYPE(iterator);
 * // Generates: has_member_iterator and has_member_iterator_v
 *
 * static_assert(has_member_iterator_v<std::vector<int>>);
 * static_assert(!has_member_iterator_v<int>);
 * @endcode
 */
#define META_HAS_MEMBER_TYPE(type)                                             \
  __META_VALIDATE_CONTEXT(has_member_##type, typename T::type)

/**
 * @brief Macro to define a type trait that checks for a member data.
 *
 * This macro generates a type trait `has_member_{data}` that checks whether
 * a class has a specific member data (variable).
 *
 * @param data The name of the member data to check for
 *
 * Generated traits:
 * - `has_member_{data}`: Type trait struct inheriting from std::true_type or
 * std::false_type
 * - `has_member_{data}_v`: Inline variable for the trait value
 *
 * Example usage:
 * @code
 * META_HAS_MEMBER_DATA(size);
 * // Generates: has_member_size and has_member_size_v
 *
 * struct MyStruct { int size; };
 * static_assert(has_member_size_v<MyStruct>);
 * static_assert(!has_member_size_v<int>);
 * @endcode
 */
#define META_HAS_MEMBER_DATA(data)                                             \
  __META_VALIDATE_CONTEXT(has_member_##data, decltype(std::declval<T>().data))

/**
 * @brief Macro to define a type trait that checks for a member function.
 *
 * This macro generates a type trait `has_member_{func}` that checks whether
 * a class has a specific member function with the given signature.
 *
 * @param func The name of the member function to check for
 * @param ... The argument types for the function signature (can be empty)
 *
 * Generated traits:
 * - `has_member_{func}`: Type trait struct inheriting from std::true_type or
 * std::false_type
 * - `has_member_{func}_v`: Inline variable for the trait value
 *
 * Example usage:
 * @code
 * META_HAS_MEMBER_FUNCTION(begin);  // No arguments
 * META_HAS_MEMBER_FUNCTION(at, size_t{});  // With argument
 * // Generates: has_member_begin, has_member_begin_v, has_member_at,
 * has_member_at_v
 *
 * static_assert(has_member_begin_v<std::vector<int>>);
 * static_assert(has_member_at_v<std::vector<int>>);
 * @endcode
 */
#define META_HAS_MEMBER_FUNCTION(func, ...)                                    \
  __META_VALIDATE_CONTEXT(has_member_##func,                                   \
                          decltype(std::declval<T>().func(__VA_ARGS__)))

META_HAS_MEMBER_TYPE(iterator);

} // namespace es
#endif // ESTD___META_HAS_MEMBER_H
