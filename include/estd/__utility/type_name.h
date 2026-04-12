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

#ifndef ESTD___UTILITY_TYPE_NAME_H
#define ESTD___UTILITY_TYPE_NAME_H
#include <string_view>

namespace es {

/**
 * @brief Gets the human-readable name of a type at compile time.
 *
 * This function uses compiler-specific intrinsics to obtain a string
 * representation of a type name. The result is constexpr and can be used
 * at compile time.
 *
 * @tparam T The type to get the name for
 * @return A string_view containing the type name
 *
 * @note The exact format of the returned string depends on the compiler
 * @note Works with Clang, GCC, and MSVC
 *
 * Example usage:
 * @code
 * constexpr auto name = type_name<int>(); // "int" on most compilers
 * auto name2 = type_name(42); // Deduces T as int
 * @endcode
 */
template <typename T>
constexpr std::string_view type_name() {
#if defined(__clang__) || defined(__GNUC__)
  std::string_view funcName = __PRETTY_FUNCTION__;
  auto start = funcName.find_last_of('=') + 2;
  auto end = funcName.find_last_of(']');
  return funcName.substr(start, end - start);
#elif defined(_MSC_VER)
  std::string_view funcName = __FUNCSIG__;
  auto start = funcName.find_last_of('<') + 1;
  auto end = funcName.find_last_of('>');
  return funcName.substr(start, end - start);
#else
  return "";
#endif
}

/**
 * @brief Gets the human-readable name of a type from a value.
 *
 * This overload deduces the type T from the argument and calls
 * type_name<T>().
 *
 * @tparam T The type to deduce and get the name for
 * @param Unused parameter for type deduction
 * @return A string_view containing the type name
 */
template <typename T>
constexpr std::string_view type_name(T&&) {
  return type_name<T>();
}

} // namespace es
#endif
