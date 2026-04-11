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

#ifndef ESTD___RANGES_REF_VIEW_H
#define ESTD___RANGES_REF_VIEW_H
#include <type_traits>
#if __cplusplus >= 202002L
#include <ranges>
#else
#include "view_interface.h"
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202002L
template <ranges::range R>
using ref_view = std::ranges::ref_view<R>;
#else
template <typename Range>
class ref_view : public view_interface<ref_view<Range>> {
public:
  constexpr ref_view(Range& range) : r(&range) {}

  constexpr ref_view(Range&& range) : r(&range) {}

  constexpr auto begin() { return std::begin(*r); }
  constexpr auto end() { return std::end(*r); }
  constexpr auto cbegin() const { return std::cbegin(*r); }
  constexpr auto cend() const { return std::cend(*r); }

private:
  Range* r;
};
#endif

}} // namespace es::ranges
#endif
