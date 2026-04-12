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

#ifndef ESTD___RANGES_OWNING_VIEW_H
#define ESTD___RANGES_OWNING_VIEW_H
#include "view_interface.h"

namespace es { namespace ranges {

/**
 * @brief A view that owns a range by moving it.
 *
 * This view takes ownership of a range by moving it into internal storage.
 * It's useful for creating views over temporary ranges or rvalues, ensuring
 * the range's lifetime is managed by the view.
 *
 * @tparam Range The type of the owned range
 *
 * @note The range is moved into the view, so the original range is invalidated
 * @note Suitable for use with temporary ranges
 *
 * Example usage:
 * @code
 * // Create a view that owns a temporary vector
 * auto view = owning_view<std::vector<int>>(std::vector<int>{1, 2, 3, 4, 5});
 *
 * for (int x : view) {
 *     std::cout << x << " ";
 * }
 * // Output: 1 2 3 4 5
 *
 * // Or with a moved range
 * std::vector<int> vec = {1, 2, 3};
 * auto owned_view = owning_view<std::vector<int>>(std::move(vec));
 * // vec is now in a valid but unspecified state
 * @endcode
 */
template <typename Range>
class owning_view : public view_interface<owning_view<Range>> {
public:
  /**
   * @brief Constructs an owning_view by moving a range.
   * @param range The range to move into the view
   */
  constexpr owning_view(Range&& range) : r(std::move(range)) {}

  /**
   * @brief Gets an iterator to the beginning.
   * @return Iterator to the first element
   */
  constexpr auto begin() { return std::begin(r); }

  /**
   * @brief Gets an iterator to the end.
   * @return Iterator past the last element
   */
  constexpr auto end() { return std::end(r); }

  /**
   * @brief Gets a const iterator to the beginning.
   * @return Const iterator to the first element
   */
  constexpr auto cbegin() const { return std::cbegin(r); }

  /**
   * @brief Gets a const iterator to the end.
   * @return Const iterator past the last element
   */
  constexpr auto cend() const { return std::cend(r); }

private:
  Range r;
};

}} // namespace es::ranges
#endif
