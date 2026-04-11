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

#ifndef ESTD___RANGES_TAKE_VIEW_H
#define ESTD___RANGES_TAKE_VIEW_H
#include <type_traits>
#if __cplusplus >= 202002L
#include <ranges>
#else
#include "range_adaptor_closure.h"
#include "view_interface.h"
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202002L
template <ranges::view V>
using take_view = std::ranges::take_view<V>;
template <class DifferenceType>
constexpr auto take(DifferenceType&& count) {
  return std::ranges::take(std::forward<DifferenceType>(count));
}
#else
template <typename Range>
class take_view : public view_interface<take_view<Range>> {
public:
  constexpr take_view(Range r, size_t n)
      : range(std::forward<Range>(r)), count(n) {}

  template <typename Iter>
  struct iterator {
    Iter it;
    Iter end;
    size_t remaining;

    using iterator_category = std::input_iterator_tag;
    using value_type = typename std::iterator_traits<Iter>::value_type;
    using difference_type =
        typename std::iterator_traits<Iter>::difference_type;
    using pointer = typename std::iterator_traits<Iter>::pointer;
    using reference = typename std::iterator_traits<Iter>::reference;

    constexpr iterator(Iter i, Iter e, size_t n)
        : it(i), end(e), remaining(n) {}
    constexpr iterator& operator++() {
      ++it;
      if (remaining > 0)
        --remaining;
      return *this;
    }
    constexpr auto operator*() const { return *it; }
    constexpr bool operator!=(const iterator& other) const {
      return it != other.it && remaining > 0;
    }
    constexpr bool operator==(const iterator& other) const {
      return !(*this != other);
    }
  };

  constexpr auto begin() {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), count);
  }
  constexpr auto end() {
    auto it = std::begin(range);
    size_t n = 0;
    while (it != std::end(range) && n < count) {
      ++it;
      ++n;
    }
    return iterator<decltype(std::end(range))>(it, std::end(range), 0);
  }

  constexpr auto begin() const {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), count);
  }
  constexpr auto end() const {
    auto it = std::begin(range);
    size_t n = 0;
    while (it != std::end(range) && n < count) {
      ++it;
      ++n;
    }
    return iterator<decltype(std::end(range))>(it, std::end(range), 0);
  }

  constexpr size_t size() {
    size_t n = 0;
    for (auto it = std::begin(range); it != std::end(range) && n < count; ++it)
      ++n;
    return n;
  }

  constexpr size_t size() const {
    size_t n = 0;
    for (auto it = std::begin(range); it != std::end(range) && n < count; ++it)
      ++n;
    return n;
  }

private:
  using stored_range = std::remove_reference_t<Range>;
  stored_range range;
  size_t count;
};

struct take_closure : range_adaptor_closure<take_closure> {
  size_t n;
  constexpr take_closure(size_t c) : n(c) {}
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return take_view<Range>(std::forward<Range>(r), n);
  }
};
template <class DifferenceType>
constexpr auto take(DifferenceType&& count) {
  return take_closure{
      count > 0 ? static_cast<size_t>(std::forward<DifferenceType>(count)) : 0};
}
#endif

}} // namespace es::ranges
#endif
