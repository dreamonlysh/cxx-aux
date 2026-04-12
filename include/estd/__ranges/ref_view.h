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
#include "view_interface.h"

namespace es { namespace ranges {

/**
 * @brief A view that references an existing range.
 *
 * This view holds a pointer to an existing range, providing a non-owning
 * view over the range's elements. It's useful for creating views over
 * lvalue ranges without copying.
 *
 * @tparam Range The type of the referenced range
 *
 * @note The referenced range must outlive the view
 * @note Provides both lvalue and rvalue constructors, but both store a pointer
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * ref_view<std::vector<int>> view(vec);
 *
 * for (int x : view) {
 *     std::cout << x << " ";
 * }
 * // Output: 1 2 3 4 5
 * @endcode
 */
template <typename Range>
class ref_view : public view_interface<ref_view<Range>> {
public:
  /**
   * @brief Constructs a ref_view from an lvalue reference.
   * @param range Reference to the range
   */
  constexpr ref_view(Range& range) : r(&range) {}

  /**
   * @brief Constructs a ref_view from an rvalue reference.
   *
   * Note: This still stores a pointer, so the rvalue must be a named temporary.
   *
   * @param range Rvalue reference to the range
   */
  constexpr ref_view(Range&& range) : r(&range) {}

  /**
   * @brief Gets an iterator to the beginning.
   * @return Iterator to the first element
   */
  constexpr auto begin() { return std::begin(*r); }

  /**
   * @brief Gets an iterator to the end.
   * @return Iterator past the last element
   */
  constexpr auto end() { return std::end(*r); }

  /**
   * @brief Gets a const iterator to the beginning.
   * @return Const iterator to the first element
   */
  constexpr auto cbegin() const { return std::cbegin(*r); }

  /**
   * @brief Gets a const iterator to the end.
   * @return Const iterator past the last element
   */
  constexpr auto cend() const { return std::cend(*r); }

private:
  Range* r;
};

}} // namespace es::ranges
#endif
