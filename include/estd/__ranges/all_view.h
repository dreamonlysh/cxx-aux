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
#include "filter_view.h"
#include "owning_view.h"
#include "ref_view.h"
#include "take_view.h"
#include "transform_view.h"
#include <type_traits>

namespace es { namespace ranges {

// Internal trait to check if a type is already a view
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

/**
 * @brief Function object for creating a view over a range.
 *
 * This is the main entry point for creating views. It automatically selects
 * the appropriate view type based on the input:
 * - If the input is already a view, returns it as-is
 * - If the input is an lvalue reference, creates a ref_view
 * - If the input is an rvalue, creates an owning_view
 *
 * This enables generic code that works with any range type.
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 *
 * // Creates a ref_view (lvalue)
 * auto view1 = ranges::all(vec);
 *
 * // Creates an owning_view (rvalue)
 * auto view2 = ranges::all(std::vector<int>{1, 2, 3});
 *
 * // Already a view, returns as-is
 * auto view3 = ranges::all(view1);
 * @endcode
 */
struct all_t {
  /**
   * @brief Creates a view over a range.
   *
   * @tparam Range The range type
   * @param r The range to create a view over
   * @return A view over the range
   */
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    using T = std::remove_reference_t<Range>;
    // If already a view, return as-is
    if constexpr (__is_view<T>::value) {
      return std::forward<Range>(r);
    }
    // If lvalue reference, create ref_view
    else if constexpr (std::is_lvalue_reference_v<Range>) {
      return ref_view<T>(r);
    }
    // If rvalue, create owning_view
    else {
      return owning_view<std::remove_cv_t<T>>(std::forward<Range>(r));
    }
  }
};

/**
 * @brief Global instance of all_t for creating views.
 */
inline constexpr all_t all;

}} // namespace es::ranges
#endif
