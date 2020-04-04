#ifndef ESTD_META_FILL_H
#define ESTD_META_FILL_H

namespace estd { namespace meta {

// Fill the array with `value` on the range positions [first, last).
template <typename T, size_t N>
constexpr void fill(std::array<T, N>& cont, size_t first, size_t last, T&& value) {
  for (; first != last; ++first) {
    cont[first] = value;
  }
}

template <typename T, size_t N>
constexpr void fill(T (&cont)[N], size_t first, size_t last, T&& value) {
  for (; first != last; ++first) {
    cont[first] = value;
  }
}

template <typename T, size_t N, typename RangeT>
constexpr void fill(std::array<T, N>& cont, RangeT&& range, T&& value) {
  fill(cont, range.begin(), range.end(), std::forward<T>(value));
}

template <typename T, size_t N, typename RangeT>
constexpr void fill(T (&cont)[N], RangeT&& range, T&& value) {
  fill(cont, range.begin(), range.end(), std::forward<T>(value));
}

template <typename T, size_t N, typename PositionT>
constexpr void fill(std::array<T, N>& cont, std::initializer_list<PositionT> pos, T&& value) {
  for (auto it = pos.begin(); it != pos.end(); ++it) {
    cont[*it] = value;
  }
}
template <typename T, size_t N>
constexpr void fill(T (&cont)[N], std::initializer_list<T> pos, T&& value) {
  for (auto it = pos.begin(); it != pos.end(); ++it) {
    cont[*it] = value;
  }
}

}}

#endif //ESTD_META_FILL_H
