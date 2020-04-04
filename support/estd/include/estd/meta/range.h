#ifndef ESTD_META_RANGE_H
#define ESTD_META_RANGE_H

namespace estd { namespace meta {

template <typename T, typename Size = size_t>
class range_slice {
public:
  using value_type = T;
  using size_type = Size;

  constexpr range_slice(T first, T last)
      : first(first), last(last) {}
  ~range_slice() = default;

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

}}

#endif //ESTD_META_RANGE_H
