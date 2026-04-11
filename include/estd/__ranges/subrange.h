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
#if __cplusplus >= 202002L
#include <ranges>
#else
#include "view_interface.h"
#include <iterator>
#include <type_traits>
#include <utility>
#endif

namespace es { namespace ranges {

#if __cplusplus >= 202002L
template <std::input_or_output_iterator I, std::sentinel_for<I> S = I,
          ranges::subrange_kind K = std::sized_sentinel_for<S, I>
                                        ? ranges::subrange_kind::sized
                                        : ranges::subrange_kind::unsized>
using subrange = std::ranges::subrange<I, S, K>;
#else
enum class subrange_kind { unsized, sized };

namespace __impl_subrange {
template <typename T>
struct __make_unsigned_like {
  using type = std::make_unsigned_t<T>;
};

template <typename T>
using make_unsigned_like_t = typename __make_unsigned_like<T>::type;

// Helper to check if sentinel is sized
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

template <typename I, typename S, subrange_kind K>
class subrange : public view_interface<subrange<I, S, K>> {
public:
  // Data members (exposition only)
  static constexpr bool StoreSize =
      K == subrange_kind::sized && !__impl_subrange::is_sized_sentinel_v<S, I>;

  // Constructors
  constexpr subrange() = default;

  constexpr subrange(I begin, S end)
      : begin_(std::move(begin)), end_(std::move(end)) {
    if constexpr (StoreSize) {
      size_ = static_cast<__impl_subrange::make_unsigned_like_t<
          typename std::iterator_traits<I>::difference_type>>(
          std::distance(begin_, end_));
    }
  }

  constexpr subrange(I begin, S end,
                     typename std::iterator_traits<I>::difference_type size)
      : begin_(std::move(begin)), end_(std::move(end)) {
    if constexpr (StoreSize) {
      size_ = static_cast<__impl_subrange::make_unsigned_like_t<
          typename std::iterator_traits<I>::difference_type>>(size);
    }
  }

  // Observers
  constexpr I begin() const { return begin_; }
  constexpr S end() const { return end_; }

  constexpr bool empty() const { return begin_ == end_; }

  constexpr typename std::iterator_traits<I>::difference_type size() const {
    static_assert(K == subrange_kind::sized, "size() requires sized subrange");
    if constexpr (__impl_subrange::is_sized_sentinel_v<S, I>) {
      return end_ - begin_;
    } else {
      return static_cast<typename std::iterator_traits<I>::difference_type>(
          size_);
    }
  }

  // Iterator operations
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

  constexpr subrange
  next(typename std::iterator_traits<I>::difference_type n = 1) const {
    auto tmp = *this;
    tmp.advance(n);
    return tmp;
  }

  // Conversion to pair-like
  template <typename PairLike>
  constexpr operator PairLike() const {
    return PairLike(begin_, end_);
  }

  // For structured binding
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

#endif

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