// Copyright (c) 2025 guyuemeng
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

#ifndef ESTD___CONTAINER_FLAT_VECTOR_H
#define ESTD___CONTAINER_FLAT_VECTOR_H
#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>

namespace es {

template <std::size_t N, typename T>
class flat_vector {
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr flat_vector() noexcept : cursor_(data_) {}

  constexpr explicit flat_vector(size_type n) {
    assert(n <= N && "flat_vector size is out of range");
    cursor_ = data_ + n;
  }

  constexpr flat_vector(size_type n, const T& value) {
    assert(n <= N && "flat_vector size is out of range");
    cursor_ = data_ + n;
    std::fill(data_, cursor_, value);
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr flat_vector(InputIterator first, InputIterator last) {
    assert(std::distance(first, last) <= N &&
           "flat_vector size is out of range");
    cursor_ = std::copy(first, last, data_);
  }

  constexpr flat_vector(std::initializer_list<T> il) {
    assert(il.size() <= N && "flat_vector size is out of range");
    cursor_ = std::copy(il.begin(), il.end(), data_);
  }

  constexpr flat_vector(const flat_vector& other) {
    cursor_ = std::copy(other.begin(), other.end(), data_);
  }

  constexpr flat_vector(flat_vector&& other) noexcept {
    cursor_ = std::move(other.begin(), other.end(), data_);
  }

  constexpr flat_vector& operator=(const flat_vector& other) {
    if (this != &other) {
      cursor_ = std::copy(other.begin(), other.end(), data_);
    }
    return *this;
  }

  constexpr flat_vector& operator=(flat_vector&& other) noexcept {
    cursor_ = std::move(other.begin(), other.end(), data_);
    return *this;
  }

  ~flat_vector() noexcept = default;

  constexpr void assign(size_type n, const T& value) {
    assert(n <= N && "flat_vector size is out of range");
    cursor_ = data_ + n;
    std::fill(data_, cursor_, value);
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr void assign(InputIterator first, InputIterator last) {
    assert(std::distance(first, last) <= N &&
           "flat_vector size is out of range");
    cursor_ = std::copy(first, last, data_);
  }

  constexpr void assign(std::initializer_list<T> il) {
    assert(il.size() <= N && "flat_vector size is out of range");
    cursor_ = std::copy(il.begin(), il.end(), data_);
  }

  template <typename RangeT>
  constexpr void assign_range(const RangeT& range) {
    auto first = std::begin(range);
    auto last = std::end(range);
    assert(std::distance(first, last) <= N &&
           "flat_vector size is out of range");
    cursor_ = std::copy(first, last, data_);
  }

  constexpr reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("flat_vector::at() out of range");
    }
    return data_[pos];
  }

  constexpr const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("flat_vector::at() out of range");
    }
    return data_[pos];
  }

  constexpr reference operator[](size_type pos) { return data_[pos]; }

  constexpr const_reference operator[](size_type pos) const {
    return data_[pos];
  }

  constexpr reference front() { return *data_; }

  constexpr const_reference front() const { return *data_; }

  constexpr reference back() { return *(cursor_ - 1); }

  constexpr const_reference back() const { return *(cursor_ - 1); }

  constexpr T* data() noexcept { return data_; }

  constexpr const T* data() const noexcept { return data_; }

  constexpr iterator begin() noexcept { return data_; }

  constexpr const_iterator begin() const noexcept { return data_; }

  constexpr const_iterator cbegin() const noexcept { return data_; }

  constexpr iterator end() noexcept { return cursor_; }

  constexpr const_iterator end() const noexcept { return cursor_; }

  constexpr const_iterator cend() const noexcept { return cursor_; }

  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(cursor_);
  }

  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(cursor_);
  }

  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cursor_);
  }

  constexpr reverse_iterator rend() noexcept { return reverse_iterator(data_); }

  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(data_);
  }

  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(data_);
  }

  constexpr bool empty() const noexcept { return data_ == cursor_; }

  constexpr size_type size() const noexcept { return cursor_ - data_; }

  constexpr size_type max_size() const noexcept { return N; }

  constexpr void reserve(size_type new_cap) = delete;

  constexpr size_type capacity() const noexcept { return N; }

  constexpr void shrink_to_fit() = delete;

  constexpr void clear() noexcept { cursor_ = data_; }

  constexpr iterator insert(const_iterator pos, const T& value) {
    assert(pos >= begin() && pos <= end() && "flat_vector insert out of range");
    if (cursor_ == std::end(data_)) {
      throw std::out_of_range("flat_vector insert out of range");
    }
    std::copy_backward(pos, cursor_, cursor_ + 1);
    *pos = value;
    ++cursor_;
    return pos;
  }

  constexpr iterator insert(const_iterator pos, T&& value) {
    assert(pos >= begin() && pos <= end() && "flat_vector insert out of range");
    if (cursor_ == std::end(data_)) {
      throw std::out_of_range("flat_vector insert out of range");
    }
    auto tmp = const_cast<iterator>(pos);
    std::copy_backward(tmp, cursor_, cursor_ + 1);
    *tmp = std::move(value);
    ++cursor_;
    return tmp;
  }

  constexpr iterator insert(const_iterator pos, size_type count,
                            const T& value) {
    assert(pos >= begin() && pos <= end() && "flat_vector insert out of range");
    if ((cursor_ + count) > std::end(data_)) {
      throw std::out_of_range("flat_vector insert out of range");
    }
    std::copy_backward(pos, cursor_, cursor_ + count);
    std::fill(pos, pos + count, value);
    cursor_ += count;
    return pos;
  }

  template <typename InputIterator>
  constexpr iterator insert(const_iterator pos, InputIterator first,
                            InputIterator last) {
    assert(pos >= begin() && pos <= end() && "flat_vector insert out of range");
    if ((cursor_ + std::distance(first, last)) > std::end(data_)) {
      throw std::out_of_range("flat_vector insert out of range");
    }
    std::copy_backward(pos, cursor_, cursor_ + std::distance(first, last));
    std::copy(first, last, pos);
    cursor_ += std::distance(first, last);
    return pos;
  }

  constexpr iterator insert(const_iterator pos, std::initializer_list<T> il) {
    return insert(pos, il.begin(), il.end());
  }

  template <typename RangeT>
  constexpr iterator insert_range(const_iterator pos, const RangeT& range) {
    return insert(pos, std::begin(range), std::end(range));
  }

  template <typename... Args>
  constexpr iterator emplace(const_iterator pos, Args&&... args) {
    assert(pos >= begin() && pos <= end() &&
           "flat_vector emplace out of range");
    if (cursor_ == std::end(data_)) {
      throw std::out_of_range("flat_vector emplace out of range");
    }
    auto tmp = const_cast<iterator>(pos);
    std::copy_backward(tmp, cursor_, cursor_ + 1);
    *tmp = T(std::forward<Args>(args)...);
    ++cursor_;
    return tmp;
  }

  constexpr iterator erase(const_iterator pos) {
    assert(pos >= begin() && pos < end() && "flat_vector erase out of range");
    auto tmp = const_cast<iterator>(pos);
    std::copy(tmp + 1, cursor_, tmp);
    --cursor_;
    return tmp;
  }

  constexpr iterator erase(const_iterator first, const_iterator last) {
    assert(first <= last && "flat_vector erase invalid range");
    assert(first >= begin() && "flat_vector erase out of range");
    assert(last <= end() && "flat_vector erase out of range");
    std::copy(last, cursor_, first);
    cursor_ -= std::distance(first, last);
    return first;
  }

  constexpr void push_back(const T& value) {
    if (cursor_ == std::end(data_)) {
      throw std::out_of_range("flat_vector push_back out of range");
    }
    *cursor_ = value;
    ++cursor_;
  }

  constexpr void push_back(T&& value) {
    if (cursor_ == std::end(data_)) {
      throw std::out_of_range("flat_vector push_back out of range");
    }
    *cursor_ = std::move(value);
    ++cursor_;
  }

  template <typename... Args>
  constexpr reference emplace_back(Args&&... args) {
    if (cursor_ == std::end(data_)) {
      throw std::out_of_range("flat_vector emplace_back out of range");
    }
    *cursor_ = T(std::forward<Args>(args)...);
    ++cursor_;
    return *(cursor_ - 1);
  }

  template <typename RangeT>
  constexpr void append_range(const RangeT& range) {
    auto first = std::begin(range);
    auto last = std::end(range);
    assert(std::distance(first, last) <=
               std::distance(cursor_, std::end(data_)) &&
           "flat_vector append_range out of range");
    cursor_ = std::copy(first, last, cursor_);
  }

  constexpr void pop_back() {
    if (cursor_ == std::begin(data_)) {
      throw std::out_of_range("flat_vector pop_back out of range");
    }
    --cursor_;
  }

  constexpr void resize(size_type count) {
    assert(count <= N && "flat_vector resize out of range");
    cursor_ = data_ + count;
  }

  constexpr void resize(size_type count, const T& value) {
    assert(count <= N && "flat_vector resize out of range");
    if (count > size()) {
      std::fill(cursor_, data_ + count, value);
    }
    cursor_ = data_ + count;
  }

  constexpr void swap(flat_vector& other) noexcept {
    auto max = std::max(size(), other.size());
    std::swap_ranges(data_, data_ + max, other.data_);
    std::swap(cursor_, other.cursor_);
  }

private:
  T data_[N];
  T* cursor_;
};

template <std::size_t N, typename T>
constexpr bool operator==(const flat_vector<N, T>& lhs,
                          const flat_vector<N, T>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <std::size_t N, typename T>
constexpr bool operator!=(const flat_vector<N, T>& lhs,
                          const flat_vector<N, T>& rhs) {
  return !(lhs == rhs);
}

template <std::size_t N, typename T>
constexpr bool operator<(const flat_vector<N, T>& lhs,
                         const flat_vector<N, T>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

template <std::size_t N, typename T>
constexpr bool operator<=(const flat_vector<N, T>& lhs,
                          const flat_vector<N, T>& rhs) {
  return !(rhs < lhs);
}

template <std::size_t N, typename T>
constexpr bool operator>(const flat_vector<N, T>& lhs,
                         const flat_vector<N, T>& rhs) {
  return rhs < lhs;
}

template <std::size_t N, typename T>
constexpr bool operator>=(const flat_vector<N, T>& lhs,
                          const flat_vector<N, T>& rhs) {
  return !(lhs < rhs);
}

} // namespace es

namespace std {
template <std::size_t N, typename T>
constexpr void swap(es::flat_vector<N, T>& lhs,
                    es::flat_vector<N, T>& rhs) noexcept {
  lhs.swap(rhs);
}

template <std::size_t N, typename T, typename U>
constexpr typename es::flat_vector<N, T>::size_type
erase(es::flat_vector<N, T>& c, const U& value) {
  auto first = std::remove(c.begin(), c.end(), value);
  auto count = std::distance(first, c.end());
  c.resize(std::distance(c.begin(), first));
  return count;
}

template <std::size_t N, typename T, typename Pred>
constexpr typename es::flat_vector<N, T>::size_type
erase_if(es::flat_vector<N, T>& c, Pred pred) {
  auto first = std::remove_if(c.begin(), c.end(), pred);
  auto count = std::distance(first, c.end());
  c.resize(std::distance(c.begin(), first));
  return count;
}
} // namespace std

#endif // ESTD___CONTAINER_FLAT_VECTOR_H
