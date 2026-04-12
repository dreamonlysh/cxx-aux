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

#ifndef ESTD___RANGES_SUBRANGE_H
#define ESTD___RANGES_SUBRANGE_H
#include "view_interface.h"
#include <iterator>
#include <type_traits>
#include <utility>

namespace es { namespace ranges {

/**
 * @brief Enumeration indicating whether a subrange stores its size.
 *
 * - unsized: The subrange does not store size, must compute on demand
 * - sized: The subrange stores size or can compute it in O(1)
 */
enum class subrange_kind { unsized, sized };

namespace __impl_subrange {
// Internal implementation details
template <typename T>
struct __make_unsigned_like {
  using type = std::make_unsigned_t<T>;
};

template <typename T>
using make_unsigned_like_t = typename __make_unsigned_like<T>::type;

// Helper to check if sentinel is sized (can compute distance in O(1))
template <typename S, typename I>
struct is_sized_sentinel {
  static constexpr bool value = false;
};

template <typename I>
struct is_sized_sentinel<I, I> {
  static constexpr bool value = true;
};

template <typename S, typename I>
inline constexpr bool is_sized_sentinel_v = is_sized_sentinel<S, I>::value;

// Helper for iter_difference_t
template <typename Iterator>
using iter_difference_t =
    typename std::iterator_traits<Iterator>::difference_type;
} // namespace __impl_subrange

/**
 * @brief A view over a subrange (iterator-sentinel pair).
 *
 * This class template represents a view over a subrange defined by an
 * iterator-sentinel pair. It can be sized or unsized depending on the
 * subrange_kind parameter.
 *
 * A sized subrange either:
 * - Stores the size explicitly (when StoreSize is true)
 * - Can compute size in O(1) (when iterator and sentinel are the same type)
 *
 * An unsized subrange does not support size() operations.
 *
 * @tparam I Iterator type
 * @tparam S Sentinel type
 * @tparam K Whether the subrange is sized or unsized
 *
 * @note Supports structured bindings: auto [begin, end] = subrange;
 * @note Can be used with range-based for loops
 *
 * Example usage:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 *
 * // Create a subrange from iterators
 * auto sub = subrange(vec.begin() + 2, vec.begin() + 7);
 * // sub contains: 3, 4, 5, 6, 7
 *
 * // Use with structured bindings
 * auto [begin, end] = sub;
 *
 * // Iterate over subrange
 * for (int x : sub) {
 *     std::cout << x << " ";
 * }
 * @endcode
 */
template <typename I, typename S, subrange_kind K>
class subrange : public view_interface<subrange<I, S, K>> {
public:
  /**
   * @brief Whether the subrange stores size explicitly.
   *
   * True when K == sized and the sentinel is not the same type as iterator.
   */
  static constexpr bool StoreSize =
      K == subrange_kind::sized && !__impl_subrange::is_sized_sentinel_v<S, I>;

  /**
   * @brief Default constructor. Creates an empty subrange.
   */
  constexpr subrange() = default;

  /**
   * @brief Constructs a subrange from iterator and sentinel.
   *
   * If StoreSize is true, computes and stores the size.
   *
   * @param begin Iterator to the start
   * @param end Sentinel to the end
   */
  constexpr subrange(I begin, S end)
      : begin_(std::move(begin)), end_(std::move(end)) {
    if constexpr (StoreSize) {
      size_ = static_cast<__impl_subrange::make_unsigned_like_t<
          typename std::iterator_traits<I>::difference_type>>(
          std::distance(begin_, end_));
    }
  }

  /**
   * @brief Constructs a sized subrange from iterator, sentinel, and size.
   *
   * @param begin Iterator to the start
   * @param end Sentinel to the end
   * @param size Size of the subrange
   */
  constexpr subrange(I begin, S end,
                     typename std::iterator_traits<I>::difference_type size)
      : begin_(std::move(begin)), end_(std::move(end)) {
    if constexpr (StoreSize) {
      size_ = static_cast<__impl_subrange::make_unsigned_like_t<
          typename std::iterator_traits<I>::difference_type>>(size);
    }
  }

  /**
   * @brief Gets the begin iterator.
   * @return Iterator to the start
   */
  constexpr I begin() const { return begin_; }

  /**
   * @brief Gets the end sentinel.
   * @return Sentinel to the end
   */
  constexpr S end() const { return end_; }

  /**
   * @brief Checks if the subrange is empty.
   * @return true if begin == end
   */
  constexpr bool empty() const { return begin_ == end_; }

  /**
   * @brief Gets the size of the subrange.
   *
   * Only available for sized subranges.
   *
   * @return Number of elements in the subrange
   * @throws Static assertion if K != sized
   */
  constexpr typename std::iterator_traits<I>::difference_type size() const {
    static_assert(K == subrange_kind::sized, "size() requires sized subrange");
    if constexpr (__impl_subrange::is_sized_sentinel_v<S, I>) {
      return end_ - begin_;
    } else {
      return static_cast<typename std::iterator_traits<I>::difference_type>(
          size_);
    }
  }

  /**
   * @brief Advances the begin iterator by n positions.
   *
   * Updates the stored size if applicable.
   *
   * @param n Number of positions to advance
   */
  constexpr void advance(typename std::iterator_traits<I>::difference_type n) {
    std::advance(begin_, n);
    if constexpr (StoreSize) {
      size_ = static_cast<__impl_subrange::make_unsigned_like_t<
          typename std::iterator_traits<I>::difference_type>>(
          static_cast<typename std::iterator_traits<I>::difference_type>(
              size_) -
          n);
    }
  }

  /**
   * @brief Gets a subrange starting n positions before the current begin.
   *
   * @param n Number of positions to move back (default 1)
   * @return A new subrange with adjusted begin
   */
  constexpr subrange
  prev(typename std::iterator_traits<I>::difference_type n = 1) const {
    auto tmp = *this;
    std::advance(tmp.begin_, -n);
    if constexpr (StoreSize) {
      tmp.size_ = static_cast<__impl_subrange::make_unsigned_like_t<
          typename std::iterator_traits<I>::difference_type>>(
          static_cast<typename std::iterator_traits<I>::difference_type>(
              tmp.size_) +
          n);
    }
    return tmp;
  }

  /**
   * @brief Gets a subrange starting n positions after the current begin.
   *
   * @param n Number of positions to move forward (default 1)
   * @return A new subrange with adjusted begin
   */
  constexpr subrange
  next(typename std::iterator_traits<I>::difference_type n = 1) const {
    auto tmp = *this;
    tmp.advance(n);
    return tmp;
  }

  /**
   * @brief Converts to a pair-like type.
   *
   * Enables conversion to std::pair, std::tuple, etc.
   *
   * @tparam PairLike The target pair-like type
   * @return A PairLike constructed from (begin, end)
   */
  template <typename PairLike>
  constexpr operator PairLike() const {
    return PairLike(begin_, end_);
  }

  /**
   * @brief Gets the begin or end for structured bindings.
   *
   * @tparam N Index (0 for begin, 1 for end)
   * @return begin if N == 0, end if N == 1
   * @throws Static assertion if N >= 2
   */
  template <std::size_t N>
  constexpr auto get() const {
    static_assert(N < 2, "subrange has only 2 elements");
    if constexpr (N == 0) {
      return begin_;
    } else {
      return end_;
    }
  }

private:
  I begin_;
  S end_;
  __impl_subrange::make_unsigned_like_t<
      typename std::iterator_traits<I>::difference_type>
      size_ = 0;
};

// Deduction guides
template <typename I, typename S>
subrange(I, S) -> subrange<I, S,
                           __impl_subrange::is_sized_sentinel_v<S, I>
                               ? subrange_kind::sized
                               : subrange_kind::unsized>;

template <typename I, typename S>
subrange(I, S, typename std::iterator_traits<I>::difference_type)
    -> subrange<I, S, subrange_kind::sized>;

// Non-member get() functions for structured binding
template <std::size_t N, typename I, typename S, subrange_kind K>
constexpr auto get(const subrange<I, S, K>& sr) {
  static_assert(N < 2, "subrange has only 2 elements");
  if constexpr (N == 0) {
    return sr.begin();
  } else {
    return sr.end();
  }
}

}} // namespace es::ranges

// For structured binding
namespace std {

template <typename I, typename S, es::ranges::subrange_kind K>
struct tuple_size<es::ranges::subrange<I, S, K>>
    : integral_constant<size_t, 2> {};

template <typename I, typename S, es::ranges::subrange_kind K>
struct tuple_element<0, es::ranges::subrange<I, S, K>> {
  using type = I;
};

template <typename I, typename S, es::ranges::subrange_kind K>
struct tuple_element<1, es::ranges::subrange<I, S, K>> {
  using type = S;
};

} // namespace std

#endif