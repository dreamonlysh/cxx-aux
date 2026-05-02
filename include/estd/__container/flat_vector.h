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
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace es {

struct __flat_vector_default_out_of_range_assert {
  constexpr void operator()(bool cond, const char* const msg) const {}
};

/**
 * @brief A fixed-capacity vector with stack-allocated storage.
 *
 * This container provides a vector-like interface with a fixed maximum capacity
 * determined at compile time. All storage is allocated on the stack, making it
 * suitable for embedded systems, real-time applications, or scenarios where
 * dynamic memory allocation must be avoided.
 *
 * Key features:
 * - Fixed capacity N, allocated on the stack
 * - Standard vector-like interface
 * - No dynamic memory allocation
 * - Bounds checking with customizable assertion behavior
 * - Supports all standard container operations
 *
 * @tparam N Maximum number of elements the container can hold
 * @tparam T Type of elements
 * @tparam OutOfRangeAssert Custom assertion handler for out-of-range conditions
 *
 * @note The container throws std::out_of_range for operations that would exceed
 * capacity
 * @note OutOfRangeAssert can be customized to change behavior (e.g., assert,
 * log, ignore)
 *
 * Example usage:
 * @code
 * flat_vector<10, int> vec;
 * vec.push_back(1);
 * vec.push_back(2);
 * vec.push_back(3);
 * // vec.size() == 3, vec.capacity() == 10
 * @endcode
 */
template <std::size_t N, typename T,
          typename OutOfRangeAssert = __flat_vector_default_out_of_range_assert>
class flat_vector {
  static constexpr OutOfRangeAssert out_of_range_assert{};

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

  /**
   * @brief Default constructor. Creates an empty vector.
   */
  constexpr flat_vector() noexcept : cursor_(data()) {}

  /**
   * @brief Constructs a vector with n default-initialized elements.
   * @param n Number of elements to create
   * @throws std::out_of_range if n > N
   */
  constexpr explicit flat_vector(size_type n) : flat_vector(n, T{}) {}

  /**
   * @brief Constructs a vector with n copies of value.
   * @param n Number of elements to create
   * @param value Value to fill the vector with
   * @throws std::out_of_range if n > N
   */
  constexpr flat_vector(size_type n, const T& value) {
    out_of_range_assert(n <= N, "flat_vector size is out of range");
    cursor_ = data() + n;
    std::fill(data(), cursor_, value);
  }

  /**
   * @brief Constructs a vector from an iterator range.
   * @tparam InputIterator Iterator type
   * @param first Start of range
   * @param last End of range
   * @throws std::out_of_range if distance(first, last) > N
   */
  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr flat_vector(InputIterator first, InputIterator last) {
    out_of_range_assert(std::distance(first, last) <= N,
                        "flat_vector size is out of range");
    cursor_ = std::copy(first, last, data());
  }

  /**
   * @brief Constructs a vector from an initializer list.
   * @param il Initializer list
   * @throws std::out_of_range if il.size() > N
   */
  constexpr flat_vector(std::initializer_list<T> il) {
    out_of_range_assert(il.size() <= N, "flat_vector size is out of range");
    cursor_ = std::copy(il.begin(), il.end(), data());
  }

  constexpr flat_vector(const flat_vector& other) {
    cursor_ = std::copy(other.begin(), other.end(), data());
  }

  constexpr flat_vector(flat_vector&& other) noexcept {
    cursor_ = std::move(other.begin(), other.end(), data());
  }

  constexpr flat_vector& operator=(const flat_vector& other) {
    if (this != &other) {
      cursor_ = std::copy(other.begin(), other.end(), data());
    }
    return *this;
  }

  constexpr flat_vector& operator=(flat_vector&& other) noexcept {
    cursor_ = std::move(other.begin(), other.end(), data());
    return *this;
  }

  ~flat_vector() noexcept = default;

  /**
   * @brief Assigns new contents to the vector, replacing its current contents.
   * @param n New size
   * @param value Value to fill with
   * @throws std::out_of_range if n > N
   */
  constexpr void assign(size_type n, const T& value) {
    out_of_range_assert(n <= N, "flat_vector size is out of range");
    cursor_ = data() + n;
    std::fill(data(), cursor_, value);
  }

  /**
   * @brief Assigns new contents from an iterator range.
   * @tparam InputIterator Iterator type
   * @param first Start of range
   * @param last End of range
   * @throws std::out_of_range if distance(first, last) > N
   */
  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr void assign(InputIterator first, InputIterator last) {
    out_of_range_assert(std::distance(first, last) <= N,
                        "flat_vector size is out of range");
    cursor_ = std::copy(first, last, data());
  }

  /**
   * @brief Assigns new contents from an initializer list.
   * @param il Initializer list
   * @throws std::out_of_range if il.size() > N
   */
  constexpr void assign(std::initializer_list<T> il) {
    out_of_range_assert(il.size() <= N, "flat_vector size is out of range");
    cursor_ = std::copy(il.begin(), il.end(), data());
  }

  /**
   * @brief Assigns new contents from a range.
   * @tparam RangeT Range type
   * @param range Range to assign from
   * @throws std::out_of_range if range size > N
   */
  template <typename RangeT>
  constexpr void assign_range(const RangeT& range) {
    auto first = std::begin(range);
    auto last = std::end(range);
    out_of_range_assert(std::distance(first, last) <= N,
                        "flat_vector size is out of range");
    cursor_ = std::copy(first, last, data());
  }

  /**
   * @brief Access element with bounds checking.
   * @param pos Position of element
   * @return Reference to element
   * @throws std::out_of_range if pos >= size()
   */
  constexpr reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("flat_vector::at() out of range");
    }
    return data()[pos];
  }

  /**
   * @brief Access element with bounds checking (const version).
   * @param pos Position of element
   * @return Const reference to element
   * @throws std::out_of_range if pos >= size()
   */
  constexpr const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("flat_vector::at() out of range");
    }
    return data()[pos];
  }

  constexpr reference operator[](size_type pos) { return data()[pos]; }

  constexpr const_reference operator[](size_type pos) const {
    return data()[pos];
  }

  constexpr reference front() { return *data(); }

  constexpr const_reference front() const { return *data(); }

  constexpr reference back() { return *(cursor_ - 1); }

  constexpr const_reference back() const { return *(cursor_ - 1); }

  constexpr T* data() noexcept { return reinterpret_cast<T*>(storage_); }

  constexpr const T* data() const noexcept {
    return reinterpret_cast<const T*>(storage_);
  }

  constexpr iterator begin() noexcept { return data(); }

  constexpr const_iterator begin() const noexcept { return data(); }

  constexpr const_iterator cbegin() const noexcept { return data(); }

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

  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(data());
  }

  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(data());
  }

  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(data());
  }

  constexpr bool empty() const noexcept { return data() == cursor_; }

  constexpr size_type size() const noexcept { return cursor_ - data(); }

  constexpr size_type max_size() const noexcept { return N; }

  constexpr void reserve(size_type new_cap) = delete;

  constexpr size_type capacity() const noexcept { return N; }

  constexpr void shrink_to_fit() = delete;

  constexpr void clear() noexcept { cursor_ = data(); }

  constexpr iterator insert(const_iterator pos, const T& value) {
    out_of_range_assert(pos >= begin() && pos <= end(),
                        "flat_vector insert out of range");
    if (cursor_ == capacity_end()) {
      throw std::out_of_range("flat_vector insert out of range");
    }
    auto new_pos = begin() + std::distance(cbegin(), pos);
    std::copy_backward(new_pos, end(), cursor_ + 1);
    *new_pos = value;
    ++cursor_;
    return new_pos;
  }

  constexpr iterator insert(const_iterator pos, T&& value) {
    out_of_range_assert(pos >= begin() && pos <= end(),
                        "flat_vector insert out of range");
    if (cursor_ == capacity_end()) {
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
    out_of_range_assert(pos >= begin() && pos <= end(),
                        "flat_vector insert out of range");
    if ((cursor_ + count) > capacity_end()) {
      throw std::out_of_range("flat_vector insert out of range");
    }
    auto new_pos = begin() + std::distance(cbegin(), pos);
    std::copy_backward(new_pos, end(), cursor_ + count);
    std::fill(new_pos, new_pos + count, value);
    cursor_ += count;
    return new_pos;
  }

  template <typename InputIterator>
  constexpr iterator insert(const_iterator pos, InputIterator first,
                            InputIterator last) {
    out_of_range_assert(pos >= begin() && pos <= end(),
                        "flat_vector insert out of range");
    size_type range_size = std::distance(first, last);
    if ((size() + range_size) > capacity()) {
      throw std::out_of_range("flat_vector insert out of range");
    }

    auto new_pos = begin() + std::distance(cbegin(), pos);
    std::copy_backward(new_pos, end(), end() + range_size);
    std::copy(first, last, new_pos);
    cursor_ += range_size;
    return new_pos;
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
    out_of_range_assert(pos >= begin() && pos <= end(),
                        "flat_vector emplace out of range");
    if (cursor_ == capacity_end()) {
      throw std::out_of_range("flat_vector emplace out of range");
    }
    auto tmp = const_cast<iterator>(pos);
    std::copy_backward(tmp, cursor_, cursor_ + 1);
    *tmp = T(std::forward<Args>(args)...);
    ++cursor_;
    return tmp;
  }

  constexpr iterator erase(const_iterator pos) {
    out_of_range_assert(pos >= begin() && pos < end(),
                        "flat_vector erase out of range");
    auto tmp = const_cast<iterator>(pos);
    std::copy(tmp + 1, cursor_, tmp);
    --cursor_;
    return tmp;
  }

  constexpr iterator erase(const_iterator first, const_iterator last) {
    out_of_range_assert(first <= last, "flat_vector erase invalid range");
    out_of_range_assert(first >= begin(), "flat_vector erase out of range");
    out_of_range_assert(last <= end(), "flat_vector erase out of range");
    iterator range_first = begin() + std::distance(cbegin(), first);
    iterator range_last = begin() + std::distance(cbegin(), last);
    std::copy(range_last, end(), range_first);
    cursor_ -= std::distance(first, last);
    return range_first;
  }

  constexpr void push_back(const T& value) {
    if (cursor_ == capacity_end()) {
      throw std::out_of_range("flat_vector push_back out of range");
    }
    *cursor_ = value;
    ++cursor_;
  }

  constexpr void push_back(T&& value) {
    if (cursor_ == capacity_end()) {
      throw std::out_of_range("flat_vector push_back out of range");
    }
    *cursor_ = std::move(value);
    ++cursor_;
  }

  /**
   * @brief Constructs an element in-place at the end.
   * @tparam Args Constructor argument types
   * @param args Arguments to forward to the constructor
   * @return Reference to the constructed element
   * @throws std::out_of_range if vector is full
   */
  template <typename... Args>
  constexpr reference emplace_back(Args&&... args) {
    if (cursor_ == capacity_end()) {
      throw std::out_of_range("flat_vector emplace_back out of range");
    }
    *cursor_ = T(std::forward<Args>(args)...);
    ++cursor_;
    return *(cursor_ - 1);
  }

  /**
   * @brief Appends elements from a range.
   * @tparam RangeT Range type
   * @param range Range to append
   * @throws std::out_of_range if not enough capacity
   */
  template <typename RangeT>
  constexpr void append_range(const RangeT& range) {
    auto first = std::begin(range);
    auto last = std::end(range);
    out_of_range_assert(std::distance(first, last) <=
                            std::distance(cursor_, capacity_end()),
                        "flat_vector append_range out of range");
    cursor_ = std::copy(first, last, cursor_);
  }

  /**
   * @brief Removes the last element.
   * @throws std::out_of_range if vector is empty
   */
  constexpr void pop_back() {
    if (cursor_ == data()) {
      throw std::out_of_range("flat_vector pop_back out of range");
    }
    --cursor_;
  }

  /**
   * @brief Resizes the vector.
   * @param count New size
   * @throws std::out_of_range if count > N
   */
  constexpr void resize(size_type count) { resize(count, T{}); }

  /**
   * @brief Resizes the vector with a fill value.
   * @param count New size
   * @param value Value to fill new elements with
   * @throws std::out_of_range if count > N
   */
  constexpr void resize(size_type count, const T& value) {
    out_of_range_assert(count <= N, "flat_vector resize out of range");
    if (count > size()) {
      std::fill(cursor_, data() + count, value);
    }
    cursor_ = data() + count;
  }

  /**
   * @brief Resizes and allows overwriting the content.
   * @tparam Operation Operation type
   * @param count Maximum new size
   * @param op Operation to perform on the buffer
   * @throws std::out_of_range if operation returns size > N
   */
  template <typename Operation>
  constexpr void resize_and_overwrite(size_type count, Operation op) {
    auto size = op(data(), capacity());
    out_of_range_assert(size <= N, "flat_vector resize out of range");
    cursor_ = data() + size;
  }

  /**
   * @brief Swaps contents with another flat_vector.
   * @param other Vector to swap with
   */
  constexpr void swap(flat_vector& other) noexcept {
    auto max = std::max(size(), other.size());
    std::swap_ranges(data(), data() + max, other.data());
    auto this_size = size();
    auto other_size = other.size();
    cursor_ = data() + other_size;
    other.cursor_ = other.data() + this_size;
  }

private:
  T* capacity_end() noexcept { return data() + N; }

  const T* capacity_end() const noexcept { return data() + N; }

private:
  alignas(T) std::byte storage_[N * sizeof(T)];
  T* cursor_;
};

template <std::size_t N, typename T>
constexpr bool operator==(const flat_vector<N, T>& lhs,
                          const flat_vector<N, T>& rhs) {
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin());
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
