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

#ifndef ESTD___RANGES_VIEW_INTERFACE_H
#define ESTD___RANGES_VIEW_INTERFACE_H
#if __cplusplus >= 202002L
#include <ranges>
#else
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202002L
template <class D>
  requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
using view_interface = std::ranges::view_interface<D>;
#else

// The Derived class must implement begin() and end() methods for this to work
template <typename Derived>
class view_interface {
private:
  constexpr Derived& derived() { return static_cast<Derived&>(*this); }
  constexpr const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }

public:
  constexpr bool empty() { return derived().begin() == derived().end(); }

  constexpr bool empty() const { return derived().begin() == derived().end(); }

  constexpr auto cbegin() { return std::cbegin(derived()); }

  constexpr auto cbegin() const { return std::cbegin(derived()); }

  constexpr auto cend() { return std::cend(derived()); }

  constexpr auto cend() const { return std::cend(derived()); }

  constexpr explicit operator bool() { return !empty(); }

  constexpr explicit operator bool() const { return !empty(); }

  constexpr auto data() {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "data() requires contiguous_iterator");
    return std::addressof(*derived().begin());
  }

  constexpr auto data() const {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "data() requires contiguous_iterator");
    return std::addressof(*derived().begin());
  }

  constexpr size_t size() { return std::as_const(derived()).size(); }

  constexpr size_t size() const {
    return static_cast<size_t>(
        std::distance(derived().begin(), derived().end()));
  }

  decltype(auto) front() { return *derived().begin(); }

  decltype(auto) front() const { return *derived().begin(); }

  decltype(auto) back() {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category> ||
            std::is_base_of_v<std::bidirectional_iterator_tag,
                              iterator_category>,
        "back() not supported for non-bidirectional view");
    auto it = derived().end();
    --it;
    return *it;
  }

  decltype(auto) back() const {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category> ||
            std::is_base_of_v<std::bidirectional_iterator_tag,
                              iterator_category>,
        "back() not supported for non-bidirectional view");
    auto it = derived().end();
    --it;
    return *it;
  }

  template <typename Self = Derived>
  decltype(auto) operator[](size_t n) {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "operator[] requires random_access_iterator");
    auto it = derived().begin();
    it += n;
    return *it;
  }

  template <typename Self = Derived>
  decltype(auto) operator[](size_t n) const {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "operator[] requires random_access_iterator");
    auto it = derived().begin();
    it += n;
    return *it;
  }
};
#endif
}} // namespace es::ranges
#endif
