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

#ifndef ESTD___RANGES_FILTER_VIEW_H
#define ESTD___RANGES_FILTER_VIEW_H
#include "range_adaptor_closure.h"
#include "view_interface.h"
#include <type_traits>

namespace es { namespace ranges {

/**
 * @brief A view that filters elements based on a predicate.
 *
 * This view adaptor filters elements from an underlying range, only yielding
 * elements that satisfy the provided predicate. It's a lazy view that applies
 * the predicate on-the-fly during iteration.
 *
 * @tparam Range The underlying range type
 * @tparam Pred The predicate type (must be callable with range elements)
 *
 * @note The predicate is stored by value in the view
 * @note Iterators are input iterators that skip non-matching elements
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5, 6};
 *
 * // Using the filter adaptor
 * auto even_view = vec | filter([](int x) { return x % 2 == 0; });
 * // even_view contains: 2, 4, 6
 *
 * // Direct construction
 * filter_view<std::vector<int>&, decltype(pred)> fv(vec, pred);
 * for (int x : fv) {
 *     std::cout << x << " ";  // Only prints even numbers
 * }
 * @endcode
 */
template <typename Range, typename Pred>
class filter_view : public view_interface<filter_view<Range, Pred>> {
public:
  /**
   * @brief Constructs a filter_view from a range and predicate.
   *
   * @tparam R Range type (forwarding reference)
   * @tparam P Predicate type (forwarding reference)
   * @param r The range to filter
   * @param p The predicate to apply
   */
  template <typename R, typename P>
  constexpr filter_view(R&& r, P&& p)
      : range(std::forward<R>(r)), pred(std::forward<P>(p)) {}

  /**
   * @brief Iterator type for filter_view.
   *
   * An input iterator that automatically skips elements that don't satisfy
   * the predicate. The satisfy() method advances the iterator to the next
   * matching element.
   *
   * @tparam Iter The underlying iterator type
   */
  template <typename Iter>
  struct iterator {
    Iter it, end;
    const Pred* pred;

    using iterator_category = std::input_iterator_tag;
    using value_type = typename std::iterator_traits<Iter>::value_type;
    using difference_type =
        typename std::iterator_traits<Iter>::difference_type;
    using pointer = typename std::iterator_traits<Iter>::pointer;
    using reference = typename std::iterator_traits<Iter>::reference;

    /**
     * @brief Constructs an iterator and advances to first matching element.
     * @param i Current position
     * @param e End position
     * @param p Pointer to the predicate
     */
    constexpr iterator(Iter i, Iter e, const Pred* p) : it(i), end(e), pred(p) {
      satisfy();
    }

    /**
     * @brief Advances iterator to next element satisfying the predicate.
     *
     * Skips all elements for which pred(*it) returns false.
     */
    constexpr void satisfy() {
      while (it != end && !(*pred)(*it))
        ++it;
    }

    /**
     * @brief Advances to next matching element.
     * @return Reference to this iterator
     */
    constexpr iterator& operator++() {
      ++it;
      satisfy();
      return *this;
    }

    /**
     * @brief Dereferences the iterator.
     * @return Reference to the current element
     */
    constexpr auto operator*() const { return *it; }

    /**
     * @brief Compares iterators for inequality.
     * @param other Iterator to compare with
     * @return true if iterators are at different positions
     */
    constexpr bool operator!=(const iterator& other) const {
      return it != other.it;
    }

    /**
     * @brief Compares iterators for equality.
     * @param other Iterator to compare with
     * @return true if iterators are at the same position
     */
    constexpr bool operator==(const iterator& other) const {
      return it == other.it;
    }
  };

  /**
   * @brief Gets an iterator to the first matching element.
   * @return Iterator to the beginning
   */
  constexpr auto begin() {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), &pred);
  }

  /**
   * @brief Gets an iterator to the end.
   * @return Iterator to the end
   */
  constexpr auto end() {
    return iterator<decltype(std::end(range))>(std::end(range), std::end(range),
                                               &pred);
  }

  /**
   * @brief Gets a const iterator to the first matching element.
   * @return Const iterator to the beginning
   */
  constexpr auto begin() const {
    return iterator<decltype(std::begin(range))>(std::begin(range),
                                                 std::end(range), &pred);
  }

  /**
   * @brief Gets a const iterator to the end.
   * @return Const iterator to the end
   */
  constexpr auto end() const {
    return iterator<decltype(std::end(range))>(std::end(range), std::end(range),
                                               &pred);
  }

private:
  using stored_range = std::remove_reference_t<Range>;
  stored_range range;
  Pred pred;
};

/**
 * @brief Closure type for filter adaptor.
 *
 * This is a range adaptor closure that stores a predicate and can be
 * applied to a range using the pipe operator or function call syntax.
 *
 * @tparam Pred The predicate type
 *
 * Example usage:
 * @code
 * auto is_even = filter([](int x) { return x % 2 == 0; });
 * std::vector<int> vec = {1, 2, 3, 4, 5, 6};
 * auto result = vec | is_even;  // result contains: 2, 4, 6
 * @endcode
 */
template <typename Pred>
struct filter_closure : range_adaptor_closure<filter_closure<Pred>> {
  Pred pred;

  /**
   * @brief Constructs a filter_closure with a predicate.
   * @param p The predicate to use for filtering
   */
  constexpr filter_closure(Pred p) : pred(std::move(p)) {}

  /**
   * @brief Applies the filter to a range.
   *
   * @tparam Range The range type
   * @param r The range to filter
   * @return A filter_view over the range
   */
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return filter_view<Range, Pred>(std::forward<Range>(r), pred);
  }
};

/**
 * @brief Creates a filter adaptor that filters elements by a predicate.
 *
 * This function returns a filter_closure that can be used with the pipe
 * operator to filter ranges.
 *
 * @tparam Pred The predicate type
 * @param p The predicate to use for filtering
 * @return A filter_closure that can be applied to ranges
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5, 6};
 *
 * // Use with pipe operator
 * auto even = vec | filter([](int x) { return x % 2 == 0; });
 *
 * // Compose with other adaptors
 * auto result = vec | filter(is_positive) | transform(double_it);
 * @endcode
 */
template <typename Pred>
constexpr auto filter(Pred&& p) {
  return filter_closure<Pred>(std::forward<Pred>(p));
}

}} // namespace es::ranges
#endif
