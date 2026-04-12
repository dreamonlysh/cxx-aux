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

#ifndef ESTD_CAST_DYN_CAST_VARIANT_H
#define ESTD_CAST_DYN_CAST_VARIANT_H
#include "dyn_cast.h"
#include <variant>

namespace es {

/**
 * @brief Specialization of isa_traits for std::variant.
 *
 * Enables runtime type checking for std::variant using std::holds_alternative.
 *
 * @tparam T The target type to check for
 * @tparam U The variant types
 */
template <typename T, typename... U>
struct isa_traits<T, std::variant<U...>> {
  static bool doit(const std::variant<U...>& v) {
    return std::holds_alternative<T>(v);
  }
};

/**
 * @brief Specialization of cast_traits for std::variant.
 *
 * Enables casting for std::variant using std::get.
 *
 * @tparam T The target type to cast to
 * @tparam U The variant types
 */
template <typename T, typename... U>
struct cast_traits<T, std::variant<U...>> {
  static T& doit(std::variant<U...>& v) { return std::get<T>(v); }

  static const T& doit(const std::variant<U...>& v) { return std::get<T>(v); }
};

} // namespace es
#endif
