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

#ifndef ESTD___RANGES_TRANSFORM_VIEW_H
#define ESTD___RANGES_TRANSFORM_VIEW_H
#include <type_traits>
#if __cplusplus >= 202002L
#include <ranges>
#else
#include "range_adaptor_closure.h"
#include "view_interface.h"
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202002L
template <ranges::input_range V, std::copy_constructible F>
using transform_view = std::ranges::transform_view<V, F>;

template <typename Func>
constexpr auto transform(Func&& f) {
  return std::ranges::transform(std::forward<Func>(f));
}
#else
template <typename Range, typename Func>
class transform_view : public view_interface<transform_view<Range, Func>> {
public:
  template <typename R, typename F>
  constexpr transform_view(R&& r, F&& f)
      : range(std::forward<R>(r)), func(std::forward<F>(f)) {}

  template <typename Iter>
  struct iterator {
    Iter it;
    const Func* func;

    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(std::declval<Func>()(
        std::declval<typename std::iterator_traits<Iter>::reference>()));
    using difference_type =
        typename std::iterator_traits<Iter>::difference_type;
    using pointer = value_type*;
    using reference = value_type;

    constexpr iterator(Iter i, const Func* f) : it(i), func(f) {}
    constexpr iterator& operator++() {
      ++it;
      return *this;
    }
    constexpr auto operator*() const { return (*func)(*it); }
    constexpr bool operator!=(const iterator& other) const {
      return it != other.it;
    }
    constexpr bool operator==(const iterator& other) const {
      return it == other.it;
    }
  };

  constexpr auto begin() {
    return iterator<decltype(std::begin(range))>(std::begin(range), &func);
  }
  constexpr auto end() {
    return iterator<decltype(std::end(range))>(std::end(range), &func);
  }

  constexpr auto begin() const {
    return iterator<decltype(std::begin(range))>(std::begin(range), &func);
  }
  constexpr auto end() const {
    return iterator<decltype(std::end(range))>(std::end(range), &func);
  }

private:
  using stored_range = std::remove_reference_t<Range>;
  stored_range range;
  Func func;
};

template <typename Func>
struct transform_closure : range_adaptor_closure<transform_closure<Func>> {
  Func func;
  transform_closure(Func f) : func(std::move(f)) {}
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return transform_view<Range, Func>(std::forward<Range>(r), func);
  }
};
template <typename Func>
constexpr auto transform(Func&& f) {
  return transform_closure<Func>(std::forward<Func>(f));
}
#endif

}} // namespace es::ranges
#endif
