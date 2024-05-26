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

/// @brief get type name of T
/// @tparam T a type
/// @return string view of the T
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

template <typename T>
constexpr std::string_view type_name(T&&) {
  return type_name<T>();
}

} // namespace es
#endif
