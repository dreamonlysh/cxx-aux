// Copyright (c) 2023 guyuemeng
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

#ifndef ESTD_CONTAINER_RANGE_H
#define ESTD_CONTAINER_RANGE_H
#include <initializer_list>

namespace es {

template <typename T, typename Size = size_t>
class range_slice {
public:
  using value_type = T;
  using size_type = Size;

  constexpr range_slice(T first, T last) : first(first), last(last) {}

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

  constexpr enumerate_slice(std::initializer_list<T> init) : data(init) {}

  constexpr auto begin() const { return data.begin(); }
  constexpr auto end() const { return data.end(); }
  constexpr size_type size() const { return data.size(); }

private:
  std::initializer_list<T> data;
};

using char_enumerate_slice = enumerate_slice<char>;

} // namespace es
#endif // ESTD_META_RANGE_H
