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

#ifndef ESTD_CAST_TYPE_ID_H
#define ESTD_CAST_TYPE_ID_H
#include <cstddef>
#include <type_traits>

namespace es {

namespace __impl_type_id {
template <typename T>
size_t __type_id() noexcept {
  static const char id = '\0';
  return reinterpret_cast<size_t>(&id);
}
} // namespace __impl_type_id

/**
 * @brief Gets a unique identifier for a type at runtime.
 *
 * This function returns a unique identifier for each distinct type T.
 * The identifier is generated using the address of a static variable,
 * ensuring uniqueness across different types.
 *
 * @tparam T The type to get the identifier for (must be a class type)
 * @return A unique size_t identifier for the type
 *
 * @note The identifier is stable within a single program execution
 * @note Only works for class types (enforced by static_assert)
 *
 * Example usage:
 * @code
 * struct A {};
 * struct B {};
 *
 * size_t id_a = type_id<A>();
 * size_t id_b = type_id<B>();
 * assert(id_a != id_b);
 * @endcode
 */
template <typename T>
size_t type_id() noexcept {
  static_assert(std::is_class_v<T>);
  return __impl_type_id::__type_id<std::remove_cv_t<T>>();
}

/**
 * @brief Gets a unique identifier for a type from a value.
 *
 * This overload deduces the type T from the argument and calls
 * type_id<T>().
 *
 * @tparam T The type to deduce and get the identifier for
 * @param Unused parameter for type deduction
 * @return A unique size_t identifier for the type
 */
template <typename T>
size_t type_id(T&&) noexcept {
  return type_id<std::remove_reference_t<T>>();
}

} // namespace es

#endif // ESTD_CAST_TYPE_ID_H
