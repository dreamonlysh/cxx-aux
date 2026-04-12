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

#ifndef ESTD___RANGES_TRANSFORM_VIEW_H
#define ESTD___RANGES_TRANSFORM_VIEW_H
#include "range_adaptor_closure.h"
#include "view_interface.h"
#include <type_traits>

namespace es { namespace ranges {

/**
 * @brief A view that transforms elements using a function.
 *
 * This view adaptor applies a transformation function to each element
 * of an underlying range. It's a lazy view that applies the function
 * on-the-fly during iteration.
 *
 * @tparam Range The underlying range type
 * @tparam Func The transformation function type
 *
 * @note The function is stored by value in the view
 * @note The value_type of the iterator is the result type of the function
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 *
 * // Using the transform adaptor
 * auto doubled = vec | transform([](int x) { return x * 2; });
 * // doubled contains: 2, 4, 6, 8, 10
 *
 * // Direct construction
 * transform_view<std::vector<int>&, decltype(func)> tv(vec, func);
 * for (int x : tv) {
 *     std::cout << x << " ";  // Prints transformed values
 * }
 * @endcode
 */
template <typename Range, typename Func>
class transform_view : public view_interface<transform_view<Range, Func>> {
public:
  /**
   * @brief Constructs a transform_view from a range and function.
   *
   * @tparam R Range type (forwarding reference)
   * @tparam F Function type (forwarding reference)
   * @param r The range to transform
   * @param f The transformation function
   */
  template <typename R, typename F>
  constexpr transform_view(R&& r, F&& f)
      : range(std::forward<R>(r)), func(std::forward<F>(f)) {}

  /**
   * @brief Iterator type for transform_view.
   *
   * An input iterator that applies the transformation function on dereference.
   *
   * @tparam Iter The underlying iterator type
   */
  template <typename Iter>
  struct iterator {
    Iter it;
    const Func* func;

    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(std::declval<Func>()(
        std::declval<typename std::iterator_traits<Iter>::reference>()));
    using difference_type =
        typename std::iterator_traits<Iter>::difference_type;
    using pointer = value_type*;
    using reference = value_type;

    /**
     * @brief Constructs an iterator.
     * @param i Current position
     * @param f Pointer to the transformation function
     */
    constexpr iterator(Iter i, const Func* f) : it(i), func(f) {}

    /**
     * @brief Advances the iterator.
     * @return Reference to this iterator
     */
    constexpr iterator& operator++() {
      ++it;
      return *this;
    }

    /**
     * @brief Dereferences the iterator, applying the transformation.
     * @return The transformed value
     */
    constexpr auto operator*() const { return (*func)(*it); }

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
   * @brief Gets an iterator to the beginning.
   * @return Iterator to the first transformed element
   */
  constexpr auto begin() {
    return iterator<decltype(std::begin(range))>(std::begin(range), &func);
  }

  /**
   * @brief Gets an iterator to the end.
   * @return Iterator to the end
   */
  constexpr auto end() {
    return iterator<decltype(std::end(range))>(std::end(range), &func);
  }

  /**
   * @brief Gets a const iterator to the beginning.
   * @return Const iterator to the first transformed element
   */
  constexpr auto begin() const {
    return iterator<decltype(std::begin(range))>(std::begin(range), &func);
  }

  /**
   * @brief Gets a const iterator to the end.
   * @return Const iterator to the end
   */
  constexpr auto end() const {
    return iterator<decltype(std::end(range))>(std::end(range), &func);
  }

private:
  using stored_range = std::remove_reference_t<Range>;
  stored_range range;
  Func func;
};

/**
 * @brief Closure type for transform adaptor.
 *
 * This is a range adaptor closure that stores a function and can be
 * applied to a range using the pipe operator or function call syntax.
 *
 * @tparam Func The transformation function type
 *
 * Example usage:
 * @code
 * auto double_it = transform([](int x) { return x * 2; });
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * auto result = vec | double_it;  // result contains: 2, 4, 6, 8, 10
 * @endcode
 */
template <typename Func>
struct transform_closure : range_adaptor_closure<transform_closure<Func>> {
  Func func;

  /**
   * @brief Constructs a transform_closure with a function.
   * @param f The transformation function
   */
  transform_closure(Func f) : func(std::move(f)) {}

  /**
   * @brief Applies the transformation to a range.
   *
   * @tparam Range The range type
   * @param r The range to transform
   * @return A transform_view over the range
   */
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return transform_view<Range, Func>(std::forward<Range>(r), func);
  }
};

/**
 * @brief Creates a transform adaptor that applies a function to elements.
 *
 * This function returns a transform_closure that can be used with the pipe
 * operator to transform ranges.
 *
 * @tparam Func The function type
 * @param f The transformation function
 * @return A transform_closure that can be applied to ranges
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 *
 * // Use with pipe operator
 * auto doubled = vec | transform([](int x) { return x * 2; });
 *
 * // Compose with other adaptors
 * auto result = vec | filter(is_positive) | transform(double_it);
 * @endcode
 */
template <typename Func>
constexpr auto transform(Func&& f) {
  return transform_closure<Func>(std::forward<Func>(f));
}

}} // namespace es::ranges
#endif
