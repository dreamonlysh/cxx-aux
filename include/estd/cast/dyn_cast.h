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

namespace es {

namespace __dyn_cast_impl {
META_HAS_MEMBER_TYPE(dyn_cast_aux_type);
}

/// @brief Traits with static doit function to do type check
/// @tparam T the cast dest type
/// @tparam U the cast source type
template <typename T, typename U>
struct isa_traits {};

/// @brief Verify whether an object can be cast to T type
/// @tparam T the cast dest type
/// @tparam U the cast source type
/// @param v base object or an object holder
/// @return is a
template <typename T, typename U>
bool isa(U& v) {
  if constexpr (__dyn_cast_impl::has_member_dyn_cast_aux_type_v<U>)
    return isa_traits<std::remove_cv_t<T>, typename U::dyn_cast_aux_type>::doit(
        v);
  else
    return isa_traits<std::remove_cv_t<T>, std::remove_cv_t<U>>::doit(v);
}

/// @brief Verify whether an object can be cast to T type
/// @tparam T the cast dest type
/// @tparam U the cast source type
/// @param v base object or an object holder
/// @return is a
template <typename T, typename U>
bool isa(U* v) {
  return v != nullptr && isa<T, U>(*v);
}

/// @brief Traits with static doit function to do type cast
/// @tparam T the cast dest type
/// @tparam U the cast source type
template <typename T, typename U>
struct cast_traits {};

/// @brief Cast an object to another, always checked by isa(v)
/// @tparam T the cast dest type
/// @tparam U the cast source type
/// @param v base object or an object holder
/// @return reference to object with dest type
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>& cast(U& v) {
  if constexpr (__dyn_cast_impl::has_member_dyn_cast_aux_type_v<U>)
    return cast_traits<std::remove_cv_t<T>,
                       typename U::dyn_cast_aux_type>::doit(v);
  else
    return cast_traits<std::remove_cv_t<T>, std::remove_cv_t<U>>::doit(v);
}

/// @brief Cast an object to another, always checked by isa(v)
/// @tparam T the cast dest type
/// @tparam U the cast source type
/// @param v base object or an object holder
/// @return pointer to object with dest type
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>* cast(U* v) {
  return &cast<T, U>(*v);
}

/// @brief Cast an object to another
/// @tparam T the cast dest type
/// @tparam U the cast source type
/// @param v base object or an object holder
/// @return pointer to object with dest type or nullptr if not the type
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>* dyn_cast(U* v) {
  return isa<T>(v) ? &cast<T>(*v) : nullptr;
}

/// @brief Cast an object to another
/// @tparam T the cast dest type
/// @tparam U the cast source type
/// @param v base object or an object holder
/// @return reference to object with dest type or throw an runtime error if not
/// the type
template <typename T, typename U>
std::conditional_t<std::is_const_v<U>, std::add_const_t<T>, T>& dyn_cast(U& v) {
  if (isa<T>(v))
    return cast<T>(v);
  throw std::runtime_error("invalid dyn_cast");
}

} // namespace es
#endif // ESTD_CAST_DYN_CAST_H
