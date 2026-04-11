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

#ifndef ESTD___RANGES_FILTER_VIEW_H
#define ESTD___RANGES_FILTER_VIEW_H
#if __cplusplus >= 202002L
#include <ranges>
#else
#include "range_adaptor_closure.h"
#include "view_interface.h"
#include <type_traits>
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202002L
template <ranges::input_range V,
          std::indirect_unary_predicate<ranges::iterator_t<V>> Pred>
using filter_view = std::ranges::filter_view<V, Pred>;
template <typename Pred>
constexpr auto filter(Pred&& p) {
  return std::ranges::filter(std::forward<Pred>(p));
}
#else
template <typename Range, typename Pred>
class filter_view : public view_interface<filter_view<Range, Pred>> {
public:
  template <typename R, typename P>
  constexpr filter_view(R&& r, P&& p)
      : range(std::forward<R>(r)), pred(std::forward<P>(p)) {}

  template <typename Iter>
  struct iterator {
    Iter it, end;
    const Pred* pred;

    using iterator_category = std::input_iterator_tag;
    using value_type = typename std::iterator_traits<Iter>::value_type;
    using difference_type =
        typename std::iterator_traits<Iter>::difference_type;
    using pointer = typename std::iterator_traits<Iter>::pointer;
    using reference = typename std::iterator_traits<Iter>::reference;

    constexpr iterator(Iter i, Iter e, const Pred* p) : it(i), end(e), pred(p) {
      satisfy();
    }
    constexpr void satisfy() {
      while (it != end && !(*pred)(*it))
        ++it;
    }
    constexpr iterator& operator++() {
      ++it;
      satisfy();
      return *this;
    }
    constexpr auto operator*() const { return *it; }
    constexpr bool operator!=(const iterator& other) const {
      return it != other.it;
    }
    constexpr bool operator==(const iterator& other) const {
      return it == other.it;
    }
  };

  constexpr auto begin() {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), &pred);
  }
  constexpr auto end() {
    return iterator<decltype(std::end(range))>(std::end(range), std::end(range),
                                               &pred);
  }

  constexpr auto begin() const {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), &pred);
  }
  constexpr auto end() const {
    return iterator<decltype(std::end(range))>(std::end(range), std::end(range),
                                               &pred);
  }

private:
  using stored_range = std::remove_reference_t<Range>;
  stored_range range;
  Pred pred;
};
template <typename Pred>
struct filter_closure : range_adaptor_closure<filter_closure<Pred>> {
  Pred pred;
  constexpr filter_closure(Pred p) : pred(std::move(p)) {}
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return filter_view<Range, Pred>(std::forward<Range>(r), pred);
  }
};
template <typename Pred>
constexpr auto filter(Pred&& p) {
  return filter_closure<Pred>(std::forward<Pred>(p));
}
#endif

}} // namespace es::ranges
#endif
