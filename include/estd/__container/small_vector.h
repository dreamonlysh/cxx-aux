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

#ifndef ESTD___CONTAINER_SMALL_VECTOR_H
#define ESTD___CONTAINER_SMALL_VECTOR_H

#include "flat_vector.h"
#include <iterator>
#include <limits>
#include <vector>

namespace es {

/**
 * @brief A vector with small-vector optimization (SVO).
 *
 * This container uses stack-allocated storage (flat_vector) for small vectors
 * and automatically transitions to heap-allocated storage (std::vector) when
 * the number of elements exceeds the inline capacity N.
 *
 * Key features:
 * - Stack allocation for small vectors (size <= N)
 * - Automatic transition to heap when size exceeds N
 * - Standard vector-like interface
 * - Union-based storage (no std::variant overhead)
 * - Move semantics for small↔large transitions
 * - Conditional noexcept on move operations
 *
 * @tparam N Maximum number of elements for inline (stack) storage
 * @tparam T Type of elements
 *
 * @note Use is_small() to check current storage mode
 * @note Transitions from small to large use move semantics
 *
 * Example usage:
 * @code
 * small_vector<16, int> vec;
 * vec.push_back(1);        // Stack allocated
 * for (int i = 0; i < 20; ++i) vec.push_back(i);  // Switches to heap
 * if (vec.is_small()) { <stack allocated> }
 * @endcode
 */
template <std::size_t N, typename T>
class small_vector {
  static_assert(N > 0, "small_vector N must be greater than 0");

  using small_storage_type = flat_vector<N, T>;
  using large_storage_type = std::vector<T>;

public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  static constexpr size_type small_capacity = N;

  small_vector() noexcept : is_small_(true) {
    ::new (static_cast<void*>(&small_)) small_storage_type();
  }

  explicit small_vector(size_type count) {
    if (count <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(count);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(count);
    }
  }

  small_vector(size_type count, const T& value) {
    if (count <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(count, value);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(count, value);
    }
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  small_vector(InputIterator first, InputIterator last) {
    auto dist = std::distance(first, last);
    if (static_cast<size_type>(dist) <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(first, last);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(first, last);
    }
  }

  small_vector(std::initializer_list<T> il) {
    if (il.size() <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(il);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(il);
    }
  }

  small_vector(const small_vector& other) : is_small_(other.is_small_) {
    if (other.is_small_) {
      ::new (static_cast<void*>(&small_)) small_storage_type(other.small_);
    } else {
      ::new (static_cast<void*>(&large_)) large_storage_type(other.large_);
    }
  }

  small_vector(small_vector&& other) noexcept : is_small_(other.is_small_) {
    if (other.is_small_) {
      ::new (static_cast<void*>(&small_))
          small_storage_type(std::move(other.small_));
    } else {
      ::new (static_cast<void*>(&large_))
          large_storage_type(std::move(other.large_));
    }
  }

  ~small_vector() noexcept { destroy(); }

  small_vector& operator=(const small_vector& other) {
    if (this != &other) {
      destroy();
      is_small_ = other.is_small_;
      if (other.is_small_) {
        ::new (static_cast<void*>(&small_)) small_storage_type(other.small_);
      } else {
        ::new (static_cast<void*>(&large_)) large_storage_type(other.large_);
      }
    }
    return *this;
  }

  small_vector& operator=(small_vector&& other) noexcept {
    if (this != &other) {
      destroy();
      is_small_ = other.is_small_;
      if (other.is_small_) {
        ::new (static_cast<void*>(&small_))
            small_storage_type(std::move(other.small_));
      } else {
        ::new (static_cast<void*>(&large_))
            large_storage_type(std::move(other.large_));
      }
    }
    return *this;
  }

  small_vector& operator=(std::initializer_list<T> il) {
    destroy();
    if (il.size() <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(il);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(il);
    }
    return *this;
  }

  void assign(size_type count, const T& value) {
    destroy();
    if (count <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(count, value);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(count, value);
    }
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  void assign(InputIterator first, InputIterator last) {
    auto dist = std::distance(first, last);
    destroy();
    if (static_cast<size_type>(dist) <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(first, last);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(first, last);
    }
  }

  void assign(std::initializer_list<T> il) {
    destroy();
    if (il.size() <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(il);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(il);
    }
  }

  reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("small_vector::at() out of range");
    }
    return data()[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("small_vector::at() out of range");
    }
    return data()[pos];
  }

  reference operator[](size_type pos) { return data()[pos]; }
  const_reference operator[](size_type pos) const { return data()[pos]; }

  reference front() { return data()[0]; }
  const_reference front() const { return data()[0]; }

  reference back() { return data()[size() - 1]; }
  const_reference back() const { return data()[size() - 1]; }

  pointer data() noexcept { return is_small_ ? small_.data() : large_.data(); }

  const_pointer data() const noexcept {
    return is_small_ ? small_.data() : large_.data();
  }

  iterator begin() noexcept { return data(); }
  const_iterator begin() const noexcept { return data(); }
  const_iterator cbegin() const noexcept { return data(); }

  iterator end() noexcept { return data() + size(); }
  const_iterator end() const noexcept { return data() + size(); }
  const_iterator cend() const noexcept { return data() + size(); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  bool empty() const noexcept { return size() == 0; }

  size_type size() const noexcept {
    return is_small_ ? small_.size() : large_.size();
  }

  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
  }

  size_type capacity() const noexcept {
    return is_small_ ? small_.capacity() : large_.capacity();
  }

  void clear() noexcept {
    if (is_small_) {
      small_.clear();
    } else {
      large_.clear();
    }
  }

  void reserve(size_type new_cap) {
    if (new_cap > small_capacity && is_small_) {
      large_storage_type large;
      large.reserve(new_cap);
      large.insert(large.end(), std::make_move_iterator(small_.begin()),
                   std::make_move_iterator(small_.end()));
      std::destroy_at(&small_);
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(std::move(large));
    } else if (!is_small_) {
      large_.reserve(new_cap);
    }
  }

  void shrink_to_fit() {
    if (!is_small_) {
      if (large_.size() <= small_capacity) {
        small_storage_type small(std::make_move_iterator(large_.begin()),
                                 std::make_move_iterator(large_.end()));
        std::destroy_at(&large_);
        is_small_ = true;
        ::new (static_cast<void*>(&small_))
            small_storage_type(std::move(small));
      } else {
        large_.shrink_to_fit();
      }
    }
  }

  iterator insert(const_iterator pos, const T& value) {
    auto offset = pos - begin();
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small_) {
      small_.insert(small_.begin() + offset, value);
    } else {
      ensure_large();
      large_.insert(large_.begin() + offset, value);
    }
    return begin() + offset;
  }

  iterator insert(const_iterator pos, T&& value) {
    auto offset = pos - begin();
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small_) {
      small_.insert(small_.begin() + offset, std::move(value));
    } else {
      ensure_large();
      large_.insert(large_.begin() + offset, std::move(value));
    }
    return begin() + offset;
  }

  iterator insert(const_iterator pos, size_type count, const T& value) {
    auto offset = pos - begin();
    size_type new_size = size() + count;
    if (new_size <= small_capacity && is_small_) {
      small_.insert(small_.begin() + offset, count, value);
    } else {
      ensure_large();
      large_.insert(large_.begin() + offset, count, value);
    }
    return begin() + offset;
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
    auto offset = pos - begin();
    auto dist = std::distance(first, last);
    size_type new_size = size() + dist;
    if (new_size <= small_capacity && is_small_) {
      small_.insert(small_.begin() + offset, first, last);
    } else {
      ensure_large();
      large_.insert(large_.begin() + offset, first, last);
    }
    return begin() + offset;
  }

  iterator insert(const_iterator pos, std::initializer_list<T> il) {
    return insert(pos, il.begin(), il.end());
  }

  template <typename... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    auto offset = pos - begin();
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small_) {
      small_.emplace(small_.begin() + offset, std::forward<Args>(args)...);
    } else {
      ensure_large();
      large_.emplace(large_.begin() + offset, std::forward<Args>(args)...);
    }
    return begin() + offset;
  }

  iterator erase(const_iterator pos) {
    auto offset = pos - begin();
    if (is_small_) {
      small_.erase(small_.begin() + offset);
    } else {
      large_.erase(large_.begin() + offset);
    }
    return begin() + offset;
  }

  iterator erase(const_iterator first, const_iterator last) {
    auto offset = first - begin();
    auto count = last - first;
    if (is_small_) {
      small_.erase(small_.begin() + offset, small_.begin() + offset + count);
    } else {
      large_.erase(large_.begin() + offset, large_.begin() + offset + count);
    }
    return begin() + offset;
  }

  void push_back(const T& value) {
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small_) {
      small_.push_back(value);
    } else {
      ensure_large();
      large_.push_back(value);
    }
  }

  void push_back(T&& value) {
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small_) {
      small_.push_back(std::move(value));
    } else {
      ensure_large();
      large_.push_back(std::move(value));
    }
  }

  template <typename... Args>
  reference emplace_back(Args&&... args) {
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small_) {
      return small_.emplace_back(std::forward<Args>(args)...);
    } else {
      ensure_large();
      return large_.emplace_back(std::forward<Args>(args)...);
    }
  }

  void pop_back() {
    if (is_small_) {
      small_.pop_back();
    } else {
      large_.pop_back();
    }
  }

  void resize(size_type count) { resize(count, T{}); }

  void resize(size_type count, const T& value) {
    if (count <= small_capacity && is_small_) {
      small_.resize(count, value);
    } else if (count > small_capacity) {
      ensure_large();
      large_.resize(count, value);
    } else {
      large_.resize(count, value);
    }
  }

  void swap(small_vector& other) noexcept {
    small_vector tmp(std::move(*this));
    *this = std::move(other);
    other = std::move(tmp);
  }

  bool is_small() const noexcept { return is_small_; }

private:
  void destroy() noexcept {
    if (is_small_) {
      std::destroy_at(&small_);
    } else {
      std::destroy_at(&large_);
    }
  }

  void ensure_large() {
    if (is_small_) {
      large_storage_type large(std::make_move_iterator(small_.begin()),
                               std::make_move_iterator(small_.end()));
      std::destroy_at(&small_);
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(std::move(large));
    }
  }

  union {
    small_storage_type small_;
    large_storage_type large_;
  };
  bool is_small_;
};

template <std::size_t N, typename T>
bool operator==(const small_vector<N, T>& lhs, const small_vector<N, T>& rhs) {
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <std::size_t N, typename T>
bool operator!=(const small_vector<N, T>& lhs, const small_vector<N, T>& rhs) {
  return !(lhs == rhs);
}

template <std::size_t N, typename T>
bool operator<(const small_vector<N, T>& lhs, const small_vector<N, T>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

template <std::size_t N, typename T>
bool operator<=(const small_vector<N, T>& lhs, const small_vector<N, T>& rhs) {
  return !(rhs < lhs);
}

template <std::size_t N, typename T>
bool operator>(const small_vector<N, T>& lhs, const small_vector<N, T>& rhs) {
  return rhs < lhs;
}

template <std::size_t N, typename T>
bool operator>=(const small_vector<N, T>& lhs, const small_vector<N, T>& rhs) {
  return !(lhs < rhs);
}

} // namespace es

namespace std {

template <std::size_t N, typename T>
void swap(es::small_vector<N, T>& lhs, es::small_vector<N, T>& rhs) noexcept {
  lhs.swap(rhs);
}

template <std::size_t N, typename T, typename U>
typename es::small_vector<N, T>::size_type erase(es::small_vector<N, T>& c,
                                                 const U& value) {
  auto first = std::remove(c.begin(), c.end(), value);
  auto count = std::distance(first, c.end());
  c.resize(std::distance(c.begin(), first));
  return count;
}

template <std::size_t N, typename T, typename Pred>
typename es::small_vector<N, T>::size_type erase_if(es::small_vector<N, T>& c,
                                                    Pred pred) {
  auto first = std::remove_if(c.begin(), c.end(), pred);
  auto count = std::distance(first, c.end());
  c.resize(std::distance(c.begin(), first));
  return count;
}

} // namespace std

#endif // ESTD___CONTAINER_SMALL_VECTOR_H
