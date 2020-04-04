#ifndef ESTD_META_FILL_H
#define ESTD_META_FILL_H

namespace estd { namespace meta {

// Fill the array with `value` on the range positions [first, last).
template <typename ArrayT, typename ValueT>
constexpr void fill(ArrayT& cont, size_t first, size_t last, ValueT&& value) {
  for (; first != last; ++first) {
    cont[first] = value;
  }
}

template <typename ArrayT, typename ValueT, typename RangeT>
constexpr void fill(ArrayT& cont, RangeT&& range, ValueT&& value) {
  fill(cont, range.begin(), range.end(), std::forward<ValueT>(value));
}

template <typename ArrayT, typename ValueT, typename PositionT>
constexpr void fill(ArrayT& cont, enumerate_slice<PositionT> pos, ValueT&& value) {
  for (auto it = pos.begin(); it != pos.end(); ++it) {
    cont[static_cast<size_t>(*it)] = value;
  }
}

template <typename ContainerT, typename ValueT, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Args&&... args) {
  (void)std::initializer_list<int>{
    (estd::meta::fill(cont, std::forward<Args>(args), value), 0)...
  };
}

template <typename ContainerT, typename ValueT, typename... Args>
constexpr ContainerT partial_initializer(ValueT&& value, Args&&... args) {
  ContainerT cont{};
  fill_n(cont, std::forward<ValueT>(value), std::forward<Args>(args)...);
  return cont;
}

}}

#endif //ESTD_META_FILL_H
