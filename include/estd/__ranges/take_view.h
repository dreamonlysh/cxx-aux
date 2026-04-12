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
#include "range_adaptor_closure.h"
#include "view_interface.h"
#include <type_traits>

namespace es { namespace ranges {

/**
 * @brief A view that takes the first N elements from a range.
 *
 * This view adaptor limits a range to a specified number of elements.
 * It's a lazy view that stops iteration after N elements have been
 * produced, even if the underlying range has more elements.
 *
 * @tparam Range The underlying range type
 *
 * @note If the underlying range has fewer than N elements, all elements are
 * taken
 * @note The view stores a copy of the range
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *
 * // Using the take adaptor
 * auto first_three = vec | take(3);
 * // first_three contains: 1, 2, 3
 *
 * // Direct construction
 * take_view<std::vector<int>&> tv(vec, 5);
 * for (int x : tv) {
 *     std::cout << x << " ";  // Prints first 5 elements
 * }
 * @endcode
 */
template <typename Range>
class take_view : public view_interface<take_view<Range>> {
public:
  /**
   * @brief Constructs a take_view from a range and count.
   *
   * @param r The range to take elements from
   * @param n Maximum number of elements to take
   */
  constexpr take_view(Range r, size_t n)
      : range(std::forward<Range>(r)), count(n) {}

  /**
   * @brief Iterator type for take_view.
   *
   * An input iterator that tracks remaining elements and stops
   * when the count reaches zero or the underlying range ends.
   *
   * @tparam Iter The underlying iterator type
   */
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

    /**
     * @brief Constructs an iterator.
     * @param i Current position
     * @param e End position
     * @param n Number of elements remaining
     */
    constexpr iterator(Iter i, Iter e, size_t n)
        : it(i), end(e), remaining(n) {}

    /**
     * @brief Advances the iterator, decrementing remaining count.
     * @return Reference to this iterator
     */
    constexpr iterator& operator++() {
      ++it;
      if (remaining > 0)
        --remaining;
      return *this;
    }

    /**
     * @brief Dereferences the iterator.
     * @return Reference to the current element
     */
    constexpr auto operator*() const { return *it; }

    /**
     * @brief Compares iterators for inequality.
     *
     * Returns true if positions differ AND there are remaining elements.
     *
     * @param other Iterator to compare with
     * @return true if not equal and elements remain
     */
    constexpr bool operator!=(const iterator& other) const {
      return it != other.it && remaining > 0;
    }

    /**
     * @brief Compares iterators for equality.
     * @param other Iterator to compare with
     * @return true if equal
     */
    constexpr bool operator==(const iterator& other) const {
      return !(*this != other);
    }
  };

  /**
   * @brief Gets an iterator to the beginning.
   * @return Iterator to the first element
   */
  constexpr auto begin() {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), count);
  }

  /**
   * @brief Gets an iterator to the end.
   *
   * The end iterator is positioned after count elements or at the
   * underlying range's end, whichever comes first.
   *
   * @return Iterator to the end
   */
  constexpr auto end() {
    auto it = std::begin(range);
    size_t n = 0;
    while (it != std::end(range) && n < count) {
      ++it;
      ++n;
    }
    return iterator<decltype(std::end(range))>(it, std::end(range), 0);
  }

  /**
   * @brief Gets a const iterator to the beginning.
   * @return Const iterator to the first element
   */
  constexpr auto begin() const {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), count);
  }

  /**
   * @brief Gets a const iterator to the end.
   * @return Const iterator to the end
   */
  constexpr auto end() const {
    auto it = std::begin(range);
    size_t n = 0;
    while (it != std::end(range) && n < count) {
      ++it;
      ++n;
    }
    return iterator<decltype(std::end(range))>(it, std::end(range), 0);
  }

  /**
   * @brief Gets the actual size of the view.
   *
   * Returns the minimum of count and the underlying range size.
   *
   * @return Number of elements in the view
   */
  constexpr size_t size() {
    size_t n = 0;
    for (auto it = std::begin(range); it != std::end(range) && n < count; ++it)
      ++n;
    return n;
  }

  /**
   * @brief Gets the actual size of the view (const version).
   * @return Number of elements in the view
   */
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

/**
 * @brief Closure type for take adaptor.
 *
 * This is a range adaptor closure that stores a count and can be
 * applied to a range using the pipe operator or function call syntax.
 *
 * Example usage:
 * @code
 * auto first_five = take(5);
 * std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 * auto result = vec | first_five;  // result contains: 1, 2, 3, 4, 5
 * @endcode
 */
struct take_closure : range_adaptor_closure<take_closure> {
  size_t n;

  /**
   * @brief Constructs a take_closure with a count.
   * @param c Maximum number of elements to take
   */
  constexpr take_closure(size_t c) : n(c) {}

  /**
   * @brief Applies the take operation to a range.
   *
   * @tparam Range The range type
   * @param r The range to take elements from
   * @return A take_view over the range
   */
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return take_view<Range>(std::forward<Range>(r), n);
  }
};

/**
 * @brief Creates a take adaptor that limits a range to N elements.
 *
 * This function returns a take_closure that can be used with the pipe
 * operator to limit ranges.
 *
 * @tparam DifferenceType The count type (must be convertible to size_t)
 * @param count Maximum number of elements to take
 * @return A take_closure that can be applied to ranges
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *
 * // Use with pipe operator
 * auto first_three = vec | take(3);
 *
 * // Compose with other adaptors
 * auto result = vec | take(5) | transform([](int x) { return x * 2; });
 * @endcode
 */
template <class DifferenceType>
constexpr auto take(DifferenceType&& count) {
  return take_closure{
      count > 0 ? static_cast<size_t>(std::forward<DifferenceType>(count)) : 0};
}

}} // namespace es::ranges
#endif
