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

#ifndef ESTD___RANGES_ALL_VIEW_H
#define ESTD___RANGES_ALL_VIEW_H
#if __cplusplus >= 202002L
#include <ranges>
#else
#include "filter_view.h"
#include "owning_view.h"
#include "ref_view.h"
#include "take_view.h"
#include "transform_view.h"
#include <type_traits>
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202002L
template <ranges::viewable_range R>
using all_t = std::ranges::all_t<R>;
inline constexpr all_t all;
#else
template <typename T>
struct __is_view : std::false_type {};
template <typename T>
struct __is_view<ref_view<T>> : std::true_type {};
template <typename R, typename P>
struct __is_view<filter_view<R, P>> : __is_view<R> {};
template <typename R, typename F>
struct __is_view<transform_view<R, F>> : __is_view<R> {};
template <typename R>
struct __is_view<take_view<R>> : std::false_type {};

struct all_t {
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    using T = std::remove_reference_t<Range>;
    if constexpr (__is_view<T>::value) {
      return std::forward<Range>(r);
    } else if constexpr (std::is_lvalue_reference_v<Range>) {
      return ref_view<T>(r);
    } else {
      return owning_view<std::remove_cv_t<T>>(std::forward<Range>(r));
    }
  }
};
inline constexpr all_t all;
#endif

}} // namespace es::ranges
#endif
