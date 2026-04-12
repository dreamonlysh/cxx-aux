// Copyright (c) 2026 guyuemeng
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

#ifndef ESTD___RANGES_RANGE_ADAPTOR_CLOSURE_H
#define ESTD___RANGES_RANGE_ADAPTOR_CLOSURE_H
#include <type_traits>
#include <utility>

namespace es { namespace ranges {

/**
 * @brief CRTP base class for range adaptor closures.
 *
 * This class template provides the foundation for range adaptor closures,
 * enabling pipe syntax (range | adaptor) and closure composition (adaptor1 |
 * adaptor2). Range adaptor closures are callable objects that can transform
 * ranges.
 *
 * @tparam Derived The derived closure type (CRTP pattern)
 *
 * Example usage:
 * @code
 * // Define a custom adaptor
 * struct take_closure : range_adaptor_closure<take_closure> {
 *     size_t n;
 *     take_closure(size_t n) : n(n) {}
 *
 *     template<typename Range>
 *     auto operator()(Range&& r) const {
 *         return take_view(std::forward<Range>(r), n);
 *     }
 * };
 *
 * // Use with pipe syntax
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * auto result = vec | take_closure(3);  // Takes first 3 elements
 *
 * // Compose adaptors
 * auto composed = take_closure(3) | transform_closure([](int x) { return x * 2;
 * }); auto result2 = vec | composed;  // Takes first 3, then doubles them
 * @endcode
 */
template <typename Derived>
struct range_adaptor_closure {
  /**
   * @brief Applies this closure to a range.
   *
   * This enables the syntax: closure(range)
   *
   * @tparam Range The range type
   * @param r The range to transform
   * @return The transformed range
   */
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return static_cast<const Derived&>(*this)(std::forward<Range>(r));
  }
};

/**
 * @brief Pipe operator for range | closure.
 *
 * Enables the syntax: range | closure
 * This is disabled when Range is itself a closure to avoid ambiguity.
 *
 * @tparam Range The range type
 * @tparam Closure The closure type
 * @param r The range to transform
 * @param c The closure to apply
 * @return The transformed range
 */
template <typename Range, typename Closure,
          typename = std::enable_if_t<!std::is_base_of<
              range_adaptor_closure<std::remove_reference_t<Range>>,
              std::remove_reference_t<Range>>::value>>
constexpr auto operator|(Range&& r, const range_adaptor_closure<Closure>& c) {
  return static_cast<const Closure&>(c)(std::forward<Range>(r));
}

/**
 * @brief Composed closure that chains two closures.
 *
 * When applied to a range, applies lhs first, then rhs.
 * This enables closure composition: closure1 | closure2
 *
 * @tparam Lhs The first closure type
 * @tparam Rhs The second closure type
 */
template <typename Lhs, typename Rhs>
struct __pipe_closure : range_adaptor_closure<__pipe_closure<Lhs, Rhs>> {
  Lhs lhs_fn;
  Rhs rhs_fn;

  /**
   * @brief Constructs a composed closure from two closures.
   * @param l The first closure (applied first)
   * @param r The second closure (applied second)
   */
  constexpr __pipe_closure(Lhs l, Rhs r)
      : lhs_fn(std::move(l)), rhs_fn(std::move(r)) {}

  /**
   * @brief Applies the composed closure to a range.
   *
   * Applies lhs_fn first, then rhs_fn: rhs_fn(lhs_fn(range))
   *
   * @tparam Range The range type
   * @param r The range to transform
   * @return The transformed range
   */
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    // Apply lhs first, then rhs: rhs(lhs(range))
    return rhs_fn(lhs_fn(std::forward<Range>(r)));
  }
};

/**
 * @brief Pipe operator for closure | closure (composition).
 *
 * Enables the syntax: closure1 | closure2
 * Creates a composed closure that applies closure1 first, then closure2.
 *
 * @tparam Lhs The first closure type
 * @tparam Rhs The second closure type
 * @param lhs The first closure (applied first)
 * @param rhs The second closure (applied second)
 * @return A composed closure
 */
template <typename Lhs, typename Rhs>
constexpr auto operator|(const range_adaptor_closure<Lhs>& lhs,
                         const range_adaptor_closure<Rhs>& rhs) {
  return __pipe_closure<Lhs, Rhs>(static_cast<const Lhs&>(lhs),
                                  static_cast<const Rhs&>(rhs));
}

}} // namespace es::ranges
#endif