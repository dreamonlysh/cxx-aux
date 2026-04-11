// Copyright (c) 2026 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD_STRING___ALGORITHM_LAZY_VIEWS_H
#define ESTD_STRING___ALGORITHM_LAZY_VIEWS_H

#include <cctype>
#include <cstring>
#include <estd/__ranges/range_adaptor_closure.h>
#include <estd/__ranges/subrange.h>
#include <estd/__ranges/transform_view.h>

namespace es { namespace string { namespace views {

// Base template for string view adapters
template <typename Derived, typename Func>
struct string_adapter_closure : es::ranges::range_adaptor_closure<Derived> {
  // Primary overload for ranges
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return Func{}(std::forward<Range>(r));
  }

  // Overload for const char*
  constexpr auto operator()(const char* str) const {
    return operator()(es::ranges::subrange(str, str + std::strlen(str)));
  }

  // Overload for char arrays
  template <size_t N>
  constexpr auto operator()(const char (&arr)[N]) const {
    return operator()(es::ranges::subrange(arr, arr + N));
  }

  // Overload for char*
  constexpr auto operator()(char* str) const {
    return operator()(es::ranges::subrange(str, str + std::strlen(str)));
  }

  // Overload for non-const char arrays
  template <size_t N>
  constexpr auto operator()(char (&arr)[N]) const {
    return operator()(es::ranges::subrange(arr, arr + N));
  }
};

// to_upper view adapter
struct to_upper_range_closure {
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return es::ranges::transform([](char c) {
      return static_cast<char>(std::toupper(c));
    })(std::forward<Range>(r));
  }
};

struct to_upper_closure
    : string_adapter_closure<to_upper_closure, to_upper_range_closure> {};
constexpr inline to_upper_closure to_upper;

// to_lower view adapter
struct to_lower_range_closure {
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return es::ranges::transform([](char c) {
      return static_cast<char>(std::tolower(c));
    })(std::forward<Range>(r));
  }
};

struct to_lower_closure
    : string_adapter_closure<to_lower_closure, to_lower_range_closure> {};
constexpr inline to_lower_closure to_lower;

// dot_to_underline view adapter
struct dot_to_underline_range_closure {
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return es::ranges::transform([](char c) { return c == '.' ? '_' : c; })(
        std::forward<Range>(r));
  }
};

struct dot_to_underline_closure
    : string_adapter_closure<dot_to_underline_closure,
                             dot_to_underline_range_closure> {};
constexpr inline dot_to_underline_closure dot_to_underline;

}}} // namespace es::string::views

#endif // ESTD_STRING___ALGORITHM_LAZY_VIEWS_H
