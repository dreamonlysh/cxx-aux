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

#ifndef ESTD___UTILITY_OVERLOADED_H
#define ESTD___UTILITY_OVERLOADED_H

namespace es {

/**
 * @brief Utility class for creating overloaded lambda visitors.
 *
 * This class template allows combining multiple lambda functions into a single
 * callable object that can handle different types. It's particularly useful
 * with std::visit for variant handling.
 *
 * The class inherits from all provided lambda types and brings their
 * operator() into scope using the "using" declaration, enabling overload
 * resolution to select the appropriate lambda based on argument types.
 *
 * @tparam Ts Lambda types to combine
 *
 * Example usage:
 * @code
 * std::variant<int, float, std::string> v = 42;
 *
 * auto visitor = overloaded{
 *     [](int i) { std::cout << "int: " << i << '\n'; },
 *     [](float f) { std::cout << "float: " << f << '\n'; },
 *     [](const std::string& s) { std::cout << "string: " << s << '\n'; }
 * };
 *
 * std::visit(visitor, v);  // Outputs: int: 42
 * @endcode
 */
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

/**
 * @brief Deduction guide for overloaded.
 *
 * Allows constructing overloaded from multiple lambdas without explicitly
 * specifying template parameters.
 *
 * @tparam Ts Lambda types
 */
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace es
#endif
