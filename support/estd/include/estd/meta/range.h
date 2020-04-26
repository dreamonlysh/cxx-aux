#ifndef ESTD_META_RANGE_H
#define ESTD_META_RANGE_H
#include <initializer_list>

namespace estd { namespace meta {

template <typename T, typename Size = size_t>
class range_slice {
public:
  using value_type = T;
  using size_type = Size;

  constexpr range_slice(T first, T last)
      : first(first), last(last) {}

  constexpr value_type begin() const { return first; }
  constexpr value_type end() const { return last; }
  constexpr size_type size() const { return last - first; }

private:
  T first;
  T last;
};

constexpr auto char_slice_number = range_slice<char>('0', '9' + 1);
constexpr auto char_slice_lower_alphabet = range_slice<char>('a', 'z' + 1);
constexpr auto char_slice_upper_alphabet = range_slice<char>('A', 'Z' + 1);

template <typename T>
class enumerate_slice {
public:
  using value_type = T;
  using size_type = typename std::initializer_list<T>::size_type;

  constexpr enumerate_slice(std::initializer_list<T> init)
    : data(init) {}

  constexpr auto begin() const { return data.begin(); }
  constexpr auto end() const { return data.end(); }
  constexpr size_type size() const { return data.size(); }

private:
  std::initializer_list<T> data;
};

using char_enumerate_slice = enumerate_slice<char>;

/// Overload fill in `fill.h`, to enable `fill_n` with `enumerate_slice`
template <typename ArrayT, typename ValueT, typename PositionT>
constexpr void fill(ArrayT& cont, enumerate_slice<PositionT> pos, ValueT&& value) {
  for (auto it = pos.begin(); it != pos.end(); ++it) {
    cont[static_cast<size_t>(*it)] = value;
  }
}

}}

#endif //ESTD_META_RANGE_H
