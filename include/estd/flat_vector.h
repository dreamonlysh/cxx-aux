// Copyright (c) 2024 guyuemeng
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

#ifndef ESTD_INTEGER_H
#define ESTD_INTEGER_H

#include "__container/flat_vector.h"
// template <std::size_t N, typename T>
// class flat_vector {
// public:
//   using value_type = T;
//   using size_type = std::size_t;
//   using difference_type = std::ptrdiff_t;
//   using reference = value_type&;
//   using const_reference = const value_type&;
//   using pointer = value_type*;
//   using const_pointer = const value_type*;
//   using iterator = pointer;
//   using const_iterator = const_pointer;
//   using reverse_iterator = std::reverse_iterator<iterator>;
//   using const_reverse_iterator = std::reverse_iterator<const_iterator>;
//
//   constexpr flat_vector() noexcept;
//   constexpr explicit flat_vector(size_type n);
//   constexpr flat_vector(size_type n, const T& value);
//   template <typename InputIterator,
//             typename = std::void_t<
//                 typename std::iterator_traits<InputIterator>::value_type>>
//   constexpr flat_vector(InputIterator first, InputIterator last);
//   constexpr flat_vector(std::initializer_list<T> il);
//   constexpr flat_vector(const flat_vector& other);
//   constexpr flat_vector(flat_vector&& other) noexcept;
//   constexpr flat_vector& operator=(const flat_vector& other);
//   constexpr flat_vector& operator=(flat_vector&& other) noexcept;
//   ~flat_vector() noexcept = default;
//
//   constexpr void assign(size_type n, const T& value);
//   template <typename InputIterator,
//             typename = std::void_t<
//                 typename std::iterator_traits<InputIterator>::value_type>>
//   constexpr void assign(InputIterator first, InputIterator last);
//   constexpr void assign(std::initializer_list<T> il);
//
//   template <typename RangeT>
//   constexpr void assign_range(const RangeT& range);
//
//   constexpr reference at(size_type pos);
//   constexpr const_reference at(size_type pos) const;
//   constexpr reference operator[](size_type pos);
//   constexpr const_reference operator[](size_type pos) const;
//
//   constexpr reference front();
//   constexpr const_reference front() const;
//   constexpr reference back();
//   constexpr const_reference back() const;
//
//   constexpr T* data() noexcept;
//   constexpr const T* data() const noexcept;
//
//   constexpr iterator begin() noexcept;
//   constexpr const_iterator begin() const noexcept;
//   constexpr const_iterator cbegin() const noexcept;
//   constexpr iterator end() noexcept;
//   constexpr const_iterator end() const noexcept;
//   constexpr const_iterator cend() const noexcept;
//   constexpr reverse_iterator rbegin() noexcept;
//   constexpr const_reverse_iterator rbegin() const noexcept;
//   constexpr const_reverse_iterator crbegin() const noexcept;
//   constexpr reverse_iterator rend() noexcept;
//   constexpr const_reverse_iterator rend() const noexcept;
//   constexpr const_reverse_iterator crend() const noexcept;
//
//   constexpr bool empty() const noexcept;
//
//   constexpr size_type size() const noexcept;
//
//   constexpr size_type max_size() const noexcept;
//
//   constexpr void reserve(size_type new_cap) = delete;
//
//   constexpr size_type capacity() const noexcept;
//
//   constexpr void shrink_to_fit() = delete;
//
//   constexpr void clear() noexcept;
//
//   constexpr iterator insert(const_iterator pos, const T& value);
//   constexpr iterator insert(const_iterator pos, T&& value);
//   constexpr iterator insert(const_iterator pos, size_type count,
//                             const T& value);
//   template <typename InputIterator>
//   constexpr iterator insert(const_iterator pos, InputIterator first,
//                             InputIterator last);
//   constexpr iterator insert(const_iterator pos, std::initializer_list<T> il);
//
//   template <typename RangeT>
//   constexpr iterator insert_range(const_iterator pos, const RangeT& range);
//
//   template <typename... Args>
//   constexpr iterator emplace(const_iterator pos, Args&&... args);
//
//   constexpr iterator erase(const_iterator pos);
//
//   constexpr iterator erase(const_iterator first, const_iterator last);
//
//   constexpr void push_back(const T& value);
//
//   constexpr void push_back(T&& value);
//
//   template <typename... Args>
//   constexpr reference emplace_back(Args&&... args);
//
//   template <typename RangeT>
//   constexpr void append_range(const RangeT& range);
//
//   constexpr void pop_back();
//
//   constexpr void resize(size_type count);
//
//   constexpr void resize(size_type count, const T& value);
//
//   constexpr void swap(flat_vector& other) noexcept;
// };
//
// template <std::size_t N, typename T>
// constexpr bool operator==(const flat_vector<N, T>& lhs,
//                           const flat_vector<N, T>& rhs);
// template <std::size_t N, typename T>
// constexpr bool operator!=(const flat_vector<N, T>& lhs,
//                           const flat_vector<N, T>& rhs);
// template <std::size_t N, typename T>
// constexpr bool operator<(const flat_vector<N, T>& lhs,
//                          const flat_vector<N, T>& rhs);
// template <std::size_t N, typename T>
// constexpr bool operator<=(const flat_vector<N, T>& lhs,
//                           const flat_vector<N, T>& rhs);
// template <std::size_t N, typename T>
// constexpr bool operator>(const flat_vector<N, T>& lhs,
//                          const flat_vector<N, T>& rhs);
// template <std::size_t N, typename T>
// constexpr bool operator>=(const flat_vector<N, T>& lhs,
//                           const flat_vector<N, T>& rhs);

// namespace std {
// template <std::size_t N, typename T>
// constexpr void swap(es::flat_vector<N, T>& lhs,
//                     es::flat_vector<N, T>& rhs) noexcept;

// template <std::size_t N, typename T, typename U>
// constexpr typename es::flat_vector<N, T>::size_type
// erase(es::flat_vector<N, T>& c, const U& value);

// template <std::size_t N, typename T, typename Pred>
// constexpr typename es::flat_vector<N, T>::size_type
// erase_if(es::flat_vector<N, T>& c, Pred pred);
// } // namespace std

#endif
