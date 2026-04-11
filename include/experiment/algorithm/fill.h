// Copyright (c) 2023 guyuemeng
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

#ifndef EXPERIMENT_ALGORITHM_FILL_H
#define EXPERIMENT_ALGORITHM_FILL_H
#include <experiment/container/range.h>

namespace es {

// Fill the array with `value` on the range positions [first, last).
template <typename ArrayT, typename ValueT>
constexpr void fill(ArrayT& cont, size_t first, size_t last, ValueT&& value) {
  for (; first != last; ++first) {
    cont[first] = value;
  }
}

/// Overload fill in `fill.h`, to enable `fill_n` with `enumerate_slice`
template <typename ArrayT, typename ValueT, typename PositionT>
constexpr void fill(ArrayT& cont, enumerate_slice<PositionT> pos,
                    ValueT&& value) {
  for (auto it = pos.begin(); it != pos.end(); ++it) {
    cont[static_cast<size_t>(*it)] = value;
  }
}

template <typename ArrayT, typename ValueT, typename RangeT>
constexpr void fill(ArrayT& cont, RangeT&& range, ValueT&& value) {
  fill(cont, range.begin(), range.end(), std::forward<ValueT>(value));
}

template <typename ContainerT, typename ValueT, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Args&&... args) {
  (..., fill(cont, std::forward<Args>(args), value));
}

template <typename ContainerT, typename ValueT, typename... Args>
constexpr ContainerT partial_initializer(ValueT&& value, Args&&... args) {
  ContainerT cont{};
  fill_n(cont, std::forward<ValueT>(value), std::forward<Args>(args)...);
  return cont;
}

} // namespace es
#endif // ESTD_META_FILL_H
