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
#if __cplusplus >= 202302L
#include <ranges>
#else
#include <type_traits>
#include <utility>
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202302L
template <class D>
  requires std::is_object_v<D> && std::same_as<D, std::remove_cv_t<D>>
using range_adaptor_closure = std::ranges::range_adaptor_closure<D>;
#else
template <typename Derived>
struct range_adaptor_closure {
  // Pipe a range into this closure: range | closure
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return static_cast<const Derived&>(*this)(std::forward<Range>(r));
  }
};

// Pipe operator for range | closure (exclude when Range is itself a closure)
template <typename Range, typename Closure,
          typename = std::enable_if_t<!std::is_base_of<
              range_adaptor_closure<std::remove_reference_t<Range>>,
              std::remove_reference_t<Range>>::value>>
constexpr auto operator|(Range&& r, const range_adaptor_closure<Closure>& c) {
  return static_cast<const Closure&>(c)(std::forward<Range>(r));
}

// Composed closure type
template <typename Lhs, typename Rhs>
struct __pipe_closure : range_adaptor_closure<__pipe_closure<Lhs, Rhs>> {
  Lhs lhs_fn;
  Rhs rhs_fn;
  constexpr __pipe_closure(Lhs l, Rhs r)
      : lhs_fn(std::move(l)), rhs_fn(std::move(r)) {}

  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    // Apply lhs first, then rhs: rhs(lhs(range))
    return rhs_fn(lhs_fn(std::forward<Range>(r)));
  }
};

// Pipe operator for closure | closure (composition)
template <typename Lhs, typename Rhs>
constexpr auto operator|(const range_adaptor_closure<Lhs>& lhs,
                         const range_adaptor_closure<Rhs>& rhs) {
  return __pipe_closure<Lhs, Rhs>(static_cast<const Lhs&>(lhs),
                                  static_cast<const Rhs&>(rhs));
}
#endif
}} // namespace es::ranges
#endif