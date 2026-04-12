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

#ifndef ESTD_CAST_DYN_CAST_H
#define ESTD_CAST_DYN_CAST_H
#include <algorithm>
#include <estd/__meta/has_member.h>
#include <stdexcept>

namespace es {

namespace __impl_dyn_cast {
META_HAS_MEMBER_TYPE(dyn_cast_aux_type);
}

/**
 * @brief Type traits for runtime type checking.
 *
 * Specialize this template to provide custom type checking logic for
 * dynamic casting. The specialization must provide a static `doit` method
 * that returns true if the object can be cast to type T.
 *
 * @tparam T The target type to cast to
 * @tparam U The source type to cast from
 *
 * Example specialization:
 * @code
 * template <>
 * struct isa_traits<Derived, Base> {
 *     static bool doit(const Base& v) {
 *         return v.type() == BaseType::Derived;
 *     }
 * };
 * @endcode
 */
template <typename T, typename U>
struct isa_traits {};

/**
 * @brief Checks if an object can be cast to type T (reference version).
 *
 * This function performs a runtime type check to determine if the given
 * object can be safely cast to type T. It uses isa_traits for the actual
 * type checking logic.
 *
 * @tparam T The target type to cast to
 * @tparam U The source type (deduced from the argument)
 * @param v Reference to the object to check
 * @return true if the object can be cast to T, false otherwise
 *
 * @note If U has a nested `dyn_cast_aux_type` type, it will be used
 *       as the intermediate type for the type check
 *
 * Example usage:
 * @code
 * Base& obj = get_object();
 * if (isa<Derived>(obj)) {
 *     Derived& derived = cast<Derived>(obj);
 *     // use derived
 * }
 * @endcode
 */
template <typename T, typename U>
bool isa(U& v) {
  if constexpr (__impl_dyn_cast::has_member_dyn_cast_aux_type_v<U>)
    return isa_traits<std::remove_cv_t<T>, typename U::dyn_cast_aux_type>::doit(
        v);
  else
    return isa_traits<std::remove_cv_t<T>, std::remove_cv_t<U>>::doit(v);
}

/**
 * @brief Checks if an object can be cast to type T (pointer version).
 *
 * This overload handles null pointers gracefully by returning false.
 *
 * @tparam T The target type to cast to
 * @tparam U The source type (deduced from the argument)
 * @param v Pointer to the object to check
 * @return true if the object can be cast to T, false otherwise (including if v
 * is null)
 */
template <typename T, typename U>
bool isa(U* v) {
  return v != nullptr && isa<T, U>(*v);
}

/**
 * @brief Type traits for performing the actual type cast.
 *
 * Specialize this template to provide custom casting logic for
 * dynamic casting. The specialization must provide a static `doit` method
 * that returns a reference to the object cast to type T.
 *
 * @tparam T The target type to cast to
 * @tparam U The source type to cast from
 *
 * Example specialization:
 * @code
 * template <>
 * struct cast_traits<Derived, Base> {
 *     static Derived& doit(Base& v) {
 *         return static_cast<Derived&>(v);
 *     }
 *     static const Derived& doit(const Base& v) {
 *         return static_cast<const Derived&>(v);
 *     }
 * };
 * @endcode
 */
template <typename T, typename U>
struct cast_traits {};

/**
 * @brief Performs an unchecked cast to type T (reference version).
 *
 * This function casts the object to type T without runtime checking.
 * The caller must ensure that isa<T>(v) returns true before calling
 * this function, otherwise the behavior is undefined.
 *
 * @tparam T The target type to cast to
 * @tparam U The source type (deduced from the argument)
 * @param v Reference to the object to cast
 * @return Reference to the object cast to type T
 *
 * @note Preserves const-ness: if U is const, returns const T&
 * @note If U has a nested `dyn_cast_aux_type` type, it will be used
 *       as the intermediate type for the cast
 *
 * @warning This function does not perform runtime type checking.
 *          Use dyn_cast for safe casting.
 */
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>& cast(U& v) {
  if constexpr (__impl_dyn_cast::has_member_dyn_cast_aux_type_v<U>)
    return cast_traits<std::remove_cv_t<T>,
                       typename U::dyn_cast_aux_type>::doit(v);
  else
    return cast_traits<std::remove_cv_t<T>, std::remove_cv_t<U>>::doit(v);
}

/**
 * @brief Performs an unchecked cast to type T (pointer version).
 *
 * @tparam T The target type to cast to
 * @tparam U The source type (deduced from the argument)
 * @param v Pointer to the object to cast
 * @return Pointer to the object cast to type T
 *
 * @warning This function does not perform runtime type checking.
 *          Use dyn_cast for safe casting.
 */
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>* cast(U* v) {
  return &cast<T, U>(*v);
}

/**
 * @brief Performs a checked dynamic cast to type T (pointer version).
 *
 * This function combines isa() and cast() to provide safe dynamic casting.
 * If the object cannot be cast to type T, returns nullptr.
 *
 * @tparam T The target type to cast to
 * @tparam U The source type (deduced from the argument)
 * @param v Pointer to the object to cast
 * @return Pointer to the object cast to type T, or nullptr if the cast fails
 *
 * @note Preserves const-ness: if U is const, returns const T*
 *
 * Example usage:
 * @code
 * Base* obj = get_object();
 * if (Derived* derived = dyn_cast<Derived>(obj)) {
 *     // use derived safely
 * }
 * @endcode
 */
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>* dyn_cast(U* v) {
  return isa<T>(v) ? &cast<T>(*v) : nullptr;
}

/**
 * @brief Performs a checked dynamic cast to type T (reference version).
 *
 * This function combines isa() and cast() to provide safe dynamic casting.
 * If the object cannot be cast to type T, throws std::runtime_error.
 *
 * @tparam T The target type to cast to
 * @tparam U The source type (deduced from the argument)
 * @param v Reference to the object to cast
 * @return Reference to the object cast to type T
 * @throws std::runtime_error if the cast fails
 *
 * @note Preserves const-ness: if U is const, returns const T&
 *
 * Example usage:
 * @code
 * Base& obj = get_object();
 * try {
 *     Derived& derived = dyn_cast<Derived>(obj);
 *     // use derived
 * } catch (const std::runtime_error& e) {
 *     // handle cast failure
 * }
 * @endcode
 */
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>& dyn_cast(U& v) {
  if (isa<T>(v))
    return cast<T>(v);
  throw std::runtime_error("invalid dyn_cast");
}

} // namespace es
#endif // ESTD_CAST_DYN_CAST_H
