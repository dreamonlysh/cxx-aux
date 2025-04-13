// Copyright (c) 2024 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD_STRING_FLAT_STRING_H
#define ESTD_STRING_FLAT_STRING_H
#include "algorithm.h"
#include <estd/flat_vector.h>
#include <string>

namespace es { namespace string {

struct __flat_string_default_out_of_range_assert {
  constexpr void operator()(bool cond, const char* const msg) const {}
};

template <size_t N,
          typename OutOfRangeAssert = __flat_string_default_out_of_range_assert>
class flat_string {
  static constexpr inline OutOfRangeAssert out_of_range_assert{};

  struct __flat_vector_out_of_range_assert {
    constexpr void operator()(bool cond, const char* const msg) const {}
  };
  using __storage_type =
      flat_vector<N, char, __flat_vector_out_of_range_assert>;

public:
  using traits_type = std::char_traits<char>;

  using value_type = typename __storage_type::value_type;
  using size_type = typename __storage_type::size_type;
  using difference_type = typename __storage_type::difference_type;
  using reference = typename __storage_type::reference;
  using const_reference = typename __storage_type::const_reference;
  using pointer = typename __storage_type::pointer;
  using const_pointer = typename __storage_type::const_pointer;
  using iterator = typename __storage_type::iterator;
  using const_iterator = typename __storage_type::const_iterator;
  using reverse_iterator = typename __storage_type::reverse_iterator;
  using const_reverse_iterator =
      typename __storage_type::const_reverse_iterator;

  static constexpr size_type npos = size_type(-1);

  constexpr flat_string() noexcept = default;

  constexpr flat_string(size_type n, value_type c) : flat_string() {
    out_of_range_assert(n <= capacity(), "flat_string size is out of range");
    storage_.assign(n, c);
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr flat_string(InputIterator first, InputIterator last)
      : flat_string() {
    out_of_range_assert((std::distance(first, last)) <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(first, last);
  }

  constexpr flat_string(const value_type* s, size_type n) : flat_string() {
    out_of_range_assert(n <= capacity(), "flat_string size is out of range");
    storage_.assign(s, s + n);
  }

  constexpr explicit flat_string(const value_type* s)
      : flat_string(s, traits_type::length(s)) {}

  flat_string(std::nullptr_t) = delete;

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string(const StringViewLike& s)
      : flat_string(s.data(), s.size()) {}

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  flat_string(const StringViewLike& s, size_type pos, size_type n)
      : flat_string(s.data() + pos, std::min(n, s.size() - pos)) {}

  constexpr flat_string(const flat_string& other) = default;
  constexpr flat_string(flat_string&& other) noexcept = default;
  constexpr flat_string(const flat_string& other, size_type pos)
      : storage_(other.data() + pos, other.data() + other.size()) {}
  constexpr flat_string(flat_string&& other, size_type pos) noexcept
      : storage_(other.data() + pos, other.data() + other.size()) {}
  constexpr flat_string(const flat_string& other, size_type pos, size_type n)
      : storage_(other.data() + pos,
                 other.data() + pos + std::min(n, other.size() - pos)) {}
  constexpr flat_string(flat_string&& other, size_type pos, size_type n)
      : storage_(other.data() + pos,
                 other.data() + pos + std::min(n, other.size() - pos)) {}

  constexpr flat_string(std::initializer_list<value_type> il) : flat_string() {
    out_of_range_assert(il.size() <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(il);
  }

  ~flat_string() noexcept = default;

  constexpr flat_string& operator=(const flat_string& other) = default;

  constexpr flat_string& operator=(flat_string&& other) noexcept = default;

  constexpr flat_string& operator=(const value_type* s) {
    out_of_range_assert(traits_type::length(s) <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(s, s + traits_type::length(s));
    return *this;
  }

  constexpr flat_string& operator=(value_type c) {
    storage_.resize(1);
    storage_[0] = c;
    return *this;
  }

  constexpr flat_string& operator=(std::initializer_list<value_type> il) {
    out_of_range_assert(il.size() <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(il);
    return *this;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& operator=(const StringViewLike& s) {
    out_of_range_assert(s.size() <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(s.data(), s.data() + s.size());
    return *this;
  }

  constexpr flat_string& operator=(std::nullptr_t) = delete;

  constexpr flat_string& assign(const flat_string& other) {
    storage_.assign(other.data(), other.data() + other.size());
    return *this;
  }

  constexpr flat_string& assign(flat_string&& other) {
    storage_.assign(other.data(), other.data() + other.size());
    return *this;
  }

  constexpr flat_string& assign(size_type count, value_type c) {
    out_of_range_assert(count <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(count, c);
    return *this;
  }

  constexpr flat_string& assign(const value_type* s, size_type n) {
    out_of_range_assert(n <= capacity(), "flat_string size is out of range");
    storage_.assign(s, s + n);
    return *this;
  }

  constexpr flat_string& assign(const value_type* s) {
    out_of_range_assert(traits_type::length(s) <= capacity(),
                        "flat_string size is out of range");
    return assign(s, s + traits_type::length(s));
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& assign(const StringViewLike& s) {
    out_of_range_assert(s.size() <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(s.data(), s.data() + s.size());
    return *this;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& assign(const StringViewLike& s, size_type pos,
                                size_type n = npos) {
    storage_.assign(s.data() + pos,
                    s.data() + pos + std::min(n, s.size() - pos));
    return *this;
  }

  constexpr flat_string& assign(flat_string&& other, size_type pos,
                                size_type n = npos) {
    storage_.assign(other.data() + pos,
                    other.data() + pos + std::min(n, other.size() - pos));
    return *this;
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr flat_string& assign(InputIterator first, InputIterator last) {
    out_of_range_assert(std::distance(first, last) <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(first, last);
    return *this;
  }

  constexpr flat_string& assign(std::initializer_list<value_type> il) {
    out_of_range_assert(il.size() <= capacity(),
                        "flat_string size is out of range");
    storage_.assign(il);
    return *this;
  }

  template <typename RangeT>
  constexpr flat_string& assign_range(const RangeT& range) {
    out_of_range_assert(range.size() <= capacity(),
                        "flat_string size is out of range");
    storage_.assign_range(range);
    return *this;
  }

  constexpr reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("flat_string::at() out of range");
    }
    return storage_.at(pos);
  }

  constexpr const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("flat_string::at() out of range");
    }
    return storage_.at(pos);
  }

  constexpr reference operator[](size_type pos) { return storage_[pos]; }

  constexpr const_reference operator[](size_type pos) const {
    return storage_[pos];
  }

  constexpr reference front() { return storage_.front(); }

  constexpr const_reference front() const { return storage_.front(); }

  constexpr reference back() { return storage_.back(); }

  constexpr const_reference back() const { return storage_.back(); }

  /// @brief Returns a pointer to the underlying data of the string.
  /// The data is not guaranteed to be null-terminated.
  /// @return A pointer to the underlying data of the string.
  constexpr value_type* data() noexcept { return storage_.data(); }

  /// @brief Returns a pointer to the underlying data of the string.
  /// The data is not guaranteed to be null-terminated.
  /// @return A pointer to the underlying data of the string.
  constexpr const value_type* data() const noexcept { return storage_.data(); }

  /// @brief Returns a pointer to the underlying data of the string ending with
  /// a null character.
  /// @return A pointer to the underlying data of the string.
  /// @note the data is guaranteed to be null-terminated in std::string, however
  /// c_str is not thought to be used frequently, so the null-terminated is
  /// assigned when c_str called.
  constexpr const value_type* c_str() const noexcept {
    const_cast<pointer>(storage_.data())[size()] = '\0';
    return storage_.data();
  }

  constexpr
  operator std::basic_string_view<value_type, traits_type>() const noexcept {
    return std::basic_string_view<value_type, traits_type>(data(), size());
  }

  constexpr iterator begin() noexcept { return storage_.begin(); }

  constexpr const_iterator begin() const noexcept { return storage_.begin(); }

  constexpr const_iterator cbegin() const noexcept { return storage_.cbegin(); }

  constexpr iterator end() noexcept { return storage_.end(); }

  constexpr const_iterator end() const noexcept { return storage_.end(); }

  constexpr const_iterator cend() const noexcept { return storage_.cend(); }

  constexpr reverse_iterator rbegin() noexcept { return storage_.rbegin(); }

  constexpr const_reverse_iterator rbegin() const noexcept {
    return storage_.rbegin();
  }

  constexpr const_reverse_iterator crbegin() const noexcept {
    return storage_.crbegin();
  }

  constexpr reverse_iterator rend() noexcept { return storage_.rend(); }

  constexpr const_reverse_iterator rend() const noexcept {
    return storage_.rend();
  }

  constexpr const_reverse_iterator crend() const noexcept {
    return storage_.crend();
  }

  constexpr bool empty() const noexcept { return storage_.empty(); }

  constexpr size_type size() const noexcept { return storage_.size(); }

  constexpr size_type length() const noexcept { return storage_.size(); }

  constexpr size_type max_size() const noexcept { return N - 1; }

  constexpr void reserve(size_type new_cap) = delete;

  constexpr size_type capacity() const noexcept { return N - 1; }

  constexpr void shrink_to_fit() = delete;

  constexpr void clear() noexcept { storage_.clear(); }

  constexpr flat_string& insert(size_type pos, size_type count, value_type c) {
    out_of_range_assert(count <= (capacity() - size()),
                        "flat_string size is out of range");
    storage_.insert(storage_.begin() + pos, count, c);
    return *this;
  }

  constexpr flat_string& insert(size_type pos, const value_type* s) {
    out_of_range_assert(traits_type::length(s) <= (capacity() - size()),
                        "flat_string size is out of range");
    storage_.insert(storage_.cbegin() + pos, s, s + traits_type::length(s));
    return *this;
  }

  constexpr flat_string& insert(size_type pos, const value_type* s,
                                size_type n) {
    out_of_range_assert(n <= (capacity() - size()),
                        "flat_string size is out of range");
    storage_.insert(storage_.cbegin() + pos, s, s + n);
    return *this;
  }

  constexpr flat_string& insert(size_type pos, const flat_string& str) {
    out_of_range_assert(str.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    storage_.insert(storage_.cbegin() + pos, str.data(),
                    str.data() + str.size());
    return *this;
  }

  constexpr flat_string& insert(size_type pos, const flat_string& str,
                                size_type subpos, size_type sublen = npos) {
    out_of_range_assert(std::min(sublen, str.size() - subpos) <=
                            (capacity() - size()),
                        "flat_string size is out of range");
    storage_.insert(storage_.cbegin() + pos, str.data() + subpos,
                    str.data() + subpos +
                        std::min(sublen, str.size() - subpos));
    return *this;
  }

  constexpr iterator insert(const_iterator pos, value_type c) {
    out_of_range_assert(1 <= (capacity() - size()),
                        "flat_string size is out of range");
    return storage_.insert(pos, c);
  }

  constexpr iterator insert(const_iterator pos, size_type count, value_type c) {
    out_of_range_assert(count <= (capacity() - size()),
                        "flat_string size is out of range");
    return storage_.insert(pos, count, c);
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr iterator insert(const_iterator pos, InputIterator first,
                            InputIterator last) {
    out_of_range_assert(std::distance(first, last) <= (capacity() - size()),
                        "flat_string size is out of range");
    return storage_.insert(pos, first, last);
  }

  constexpr iterator insert(const_iterator pos,
                            std::initializer_list<value_type> il) {
    out_of_range_assert(il.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    return storage_.insert(pos, il.begin(), il.end());
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& insert(size_type pos, const StringViewLike& s) {
    out_of_range_assert(s.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    storage_.insert(storage_.cbegin() + pos, s.data(), s.data() + s.size());
    return *this;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& insert(size_type pos, const StringViewLike& s,
                                size_type subpos, size_type sublen = npos) {
    out_of_range_assert(std::min(sublen, s.size() - subpos) <=
                            (capacity() - size()),
                        "flat_string size is out of range");
    storage_.insert(storage_.cbegin() + pos, s.data() + subpos,
                    s.data() + subpos + std::min(sublen, s.size() - subpos));
    return *this;
  }

  template <typename RangeT>
  constexpr iterator insert_range(const_iterator pos, const RangeT& range) {
    out_of_range_assert(range.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    return storage_.insert_range(pos, range);
  }

  constexpr iterator erase(size_type pos = 0, size_type len = npos) {
    out_of_range_assert(pos < size(), "flat_string erase out of range");
    return storage_.erase(storage_.cbegin() + pos,
                          storage_.cbegin() + pos +
                              std::min(len, size() - pos));
  }

  constexpr iterator erase(const_iterator pos) {
    out_of_range_assert(pos >= begin() && pos < end(),
                        "flat_string erase out of range");
    return storage_.erase(pos);
  }

  constexpr iterator erase(const_iterator first, const_iterator last) {
    out_of_range_assert(first < last, "flat_string erase invalid range");
    out_of_range_assert(first >= begin(), "flat_string erase out of range");
    out_of_range_assert(last <= end(), "flat_string erase out of range");
    return storage_.erase(first, last);
  }

  constexpr void push_back(value_type c) {
    out_of_range_assert(1 <= (capacity() - size()),
                        "flat_string size is out of range");
    storage_.push_back(c);
  }

  constexpr void pop_back() {
    out_of_range_assert(!empty(), "flat_string pop_back out of range");
    storage_.pop_back();
  }

  constexpr flat_string& append(size_type count, value_type c) {
    out_of_range_assert(count <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, count, c);
    return *this;
  }

  constexpr flat_string& append(const value_type* s, size_type n) {
    out_of_range_assert(n <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, s, n);
    return *this;
  }

  constexpr flat_string& append(const value_type* s) {
    out_of_range_assert(traits_type::length(s) <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, s);
    return *this;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& append(const StringViewLike& s) {
    out_of_range_assert(s.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, s);
    return *this;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& append(const StringViewLike& s, size_type pos,
                                size_type n = npos) {
    out_of_range_assert(std::min(n, s.size() - pos) <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, s, pos, n);
    return *this;
  }

  constexpr flat_string& append(const flat_string& str) {
    out_of_range_assert(str.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, str);
    return *this;
  }

  constexpr flat_string& append(const flat_string& str, size_type subpos,
                                size_type sublen = npos) {
    out_of_range_assert(sublen <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, str, subpos, sublen);
    return *this;
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr flat_string& append(InputIterator first, InputIterator last) {
    out_of_range_assert(std::distance(first, last) <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, first, last);
    return *this;
  }

  constexpr flat_string& append(std::initializer_list<value_type> il) {
    out_of_range_assert(il.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, il);
    return *this;
  }

  template <typename RangeT>
  constexpr flat_string& append_range(const RangeT& range) {
    out_of_range_assert(range.size() <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::append(*this, range.begin(), range.end());
    return *this;
  }

  constexpr flat_string& operator+=(const flat_string& str) {
    return append(str);
  }

  constexpr flat_string& operator+=(value_type c) {
    push_back(c);
    return *this;
  }

  constexpr flat_string& operator+=(const value_type* s) { return append(s); }

  constexpr flat_string& operator+=(std::initializer_list<value_type> il) {
    return append(il);
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& operator+=(const StringViewLike& s) {
    return append(s);
  }

  constexpr flat_string& replace(size_type pos, size_type count,
                                 const flat_string& str) {
    es::string::replace(*this, pos, count, str);
    return *this;
  }

  constexpr flat_string& replace(const_iterator first, const_iterator last,
                                 const flat_string& str) {
    out_of_range_assert(first >= begin() && first <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(last >= begin() && last <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(first <= last, "flat_string replace invalid range");
    return replace(std::distance(cbegin(), first), std::distance(first, last),
                   str);
  }

  constexpr flat_string& replace(size_type pos, size_type count,
                                 const flat_string& str, size_type pos2,
                                 size_type count2 = npos) {
    if (pos2 > str.size()) {
      throw std::out_of_range("flat_string replace out of range");
    }

    count2 = std::min(count2, str.size() - pos2);
    es::string::replace(*this, pos, count, str.data() + pos2, count2);
    return *this;
  }

  constexpr flat_string& replace(size_type pos, size_type count,
                                 const value_type* cstr, size_type count2) {
    es::string::replace(*this, pos, count, cstr, count2);
    return *this;
  }

  constexpr flat_string& replace(const_iterator first, const_iterator last,
                                 const value_type* cstr, size_type count2) {
    out_of_range_assert(first >= begin() && first <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(last >= begin() && last <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(first <= last, "flat_string replace invalid range");
    return replace(std::distance(cbegin(), first), std::distance(first, last),
                   cstr, count2);
  }

  constexpr flat_string& replace(size_type pos, size_type count,
                                 const value_type* cstr) {
    es::string::replace(*this, pos, count, cstr);
    return *this;
  }

  constexpr flat_string& replace(const_iterator first, const_iterator last,
                                 const value_type* cstr) {
    out_of_range_assert(first >= begin() && first <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(last >= begin() && last <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(first <= last, "flat_string replace invalid range");
    return replace(std::distance(cbegin(), first), std::distance(first, last),
                   cstr);
  }

  constexpr flat_string& replace(size_type pos, size_type count, size_type n,
                                 value_type c) {
    es::string::replace(*this, pos, count, n, c);
    return *this;
  }

  constexpr flat_string& replace(const_iterator first, const_iterator last,
                                 size_type n, value_type c) {
    out_of_range_assert(first >= begin() && first <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(last >= begin() && last <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(first <= last, "flat_string replace invalid range");
    return replace(std::distance(cbegin(), first), std::distance(first, last),
                   n, c);
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr flat_string& replace(const_iterator first, const_iterator last,
                                 InputIterator first2, InputIterator last2) {
    out_of_range_assert(first >= begin() && first <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(last >= begin() && last <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(first <= last, "flat_string replace invalid range");
    out_of_range_assert(std::distance(first2, last2) <= (capacity() - size()),
                        "flat_string size is out of range");
    es::string::replace(*this, std::distance(cbegin(), first),
                        std::distance(first, last), first2, last2);
    return *this;
  }

  template <typename InputIterator,
            typename = std::void_t<
                typename std::iterator_traits<InputIterator>::value_type>>
  constexpr flat_string& replace(size_type pos, size_type count,
                                 std::initializer_list<value_type> il) {
    return replace(pos, count, il.begin(), il.end());
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& replace(size_type pos, size_type count,
                                 const StringViewLike& s) {
    return replace(pos, count, s.data(), s.size());
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& replace(const_iterator first, const_iterator last,
                                 const StringViewLike& s) {
    out_of_range_assert(first >= begin() && first <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(last >= begin() && last <= end(),
                        "flat_string replace out of range");
    out_of_range_assert(first <= last, "flat_string replace invalid range");
    return replace(std::distance(cbegin(), first), std::distance(first, last),
                   s.data(), s.size());
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr flat_string& replace(size_type pos, size_type count,
                                 const StringViewLike& s, size_type pos2,
                                 size_type count2 = npos) {
    return replace(pos, count, s.data() + pos2,
                   std::min(count2, s.size() - pos2));
  }

  template <typename RangeT>
  constexpr flat_string& replace_with_range(const_iterator first,
                                            const_iterator last,
                                            const RangeT& rg) {
    es::string::replace(*this, std::distance(cbegin(), first),
                        std::distance(first, last), std::begin(rg),
                        std::end(rg));
    return *this;
  }

  constexpr size_type copy(value_type* dest, size_type count,
                           size_type pos = 0) const {
    if (pos >= size()) {
      throw std::out_of_range("flat_string copy out of range");
    }
    size_type len = std::min(count, size() - pos);
    std::copy_n(data() + pos, len, dest);
    return len;
  }

  constexpr void resize(size_type count) { return resize(count, value_type{}); }

  constexpr void resize(size_type count, value_type c) {
    if (count > capacity()) {
      throw std::out_of_range("flat_string resize out of range");
    }
    storage_.resize(count, c);
  }

  template <typename Operation>
  constexpr void resize_and_overwrite(size_type count, Operation op) {
    storage_.resize_and_overwrite(count, op);
  }

  constexpr void swap(flat_string& other) noexcept {
    storage_.swap(other.storage_);
  }

  constexpr size_type find(const flat_string& str,
                           size_type pos = 0) const noexcept {
    return find(str.data(), pos, str.size());
  }

  constexpr size_type find(const value_type* s, size_type pos,
                           size_type n) const noexcept {
    if (n == 0) {
      return pos;
    }
    if (n > size()) {
      return npos;
    }
    const value_type* p = data() + pos;
    const value_type* last = data() + size() - n;
    while (p <= last) {
      if (traits_type::compare(p, s, n) == 0) {
        return std::distance(data(), p);
      }
      ++p;
    }
    return npos;
  }

  constexpr size_type find(const value_type* s,
                           size_type pos = 0) const noexcept {
    return find(s, pos, traits_type::length(s));
  }

  constexpr size_type find(value_type c, size_type pos = 0) const noexcept {
    const value_type* p = data() + pos;
    const value_type* last = data() + size();
    while (p < last) {
      if (traits_type::eq(*p, c)) {
        return std::distance(data(), p);
      }
      ++p;
    }
    return npos;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr size_type find(const StringViewLike& s,
                           size_type pos = 0) const noexcept {
    return find(s.data(), pos, s.size());
  }

  constexpr size_type rfind(const flat_string& str,
                            size_type pos = npos) const noexcept {
    return rfind(str.data(), pos, str.size());
  }

  constexpr size_type rfind(const value_type* s, size_type pos,
                            size_type n) const noexcept {
    if (n == 0) {
      return pos;
    }
    if (n > size()) {
      return npos;
    }
    const value_type* p = data() + std::min(pos, size() - n);
    while (p >= data()) {
      if (traits_type::compare(p, s, n) == 0) {
        return std::distance(data(), p);
      }
      --p;
    }
    return npos;
  }

  constexpr size_type rfind(const value_type* s,
                            size_type pos = npos) const noexcept {
    return rfind(s, pos, traits_type::length(s));
  }

  constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept {
    const value_type* p = data() + std::min(pos, size());
    while (p >= data()) {
      if (traits_type::eq(*p, c)) {
        return std::distance(data(), p);
      }
      --p;
    }
    return npos;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr size_type rfind(const StringViewLike& s,
                            size_type pos = npos) const noexcept {
    return rfind(s.data(), pos, s.size());
  }

  constexpr size_type find_first_of(const flat_string& str,
                                    size_type pos = 0) const noexcept {
    return find_first_of(str.data(), pos, str.size());
  }

  constexpr size_type find_first_of(const value_type* s, size_type pos,
                                    size_type n) const noexcept {
    if (n == 0) {
      return npos;
    }
    if (n > traits_type::length(s)) {
      n = traits_type::length(s);
    }
    const value_type* p = data() + pos;
    const value_type* last = data() + size();
    while (p < last) {
      if (traits_type::find(s, n, *p) != nullptr) {
        return std::distance(data(), p);
      }
      ++p;
    }
    return npos;
  }

  constexpr size_type find_first_of(const value_type* s,
                                    size_type pos = 0) const noexcept {
    return find_first_of(s, pos, traits_type::length(s));
  }

  constexpr size_type find_first_of(value_type c,
                                    size_type pos = 0) const noexcept {
    return find(c, pos);
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr size_type find_first_of(const StringViewLike& s,
                                    size_type pos = 0) const noexcept {
    return find_first_of(s.data(), pos, s.size());
  }

  constexpr size_type find_first_not_of(const flat_string& str,
                                        size_type pos = 0) const noexcept {
    return find_first_not_of(str.data(), pos, str.size());
  }

  constexpr size_type find_first_not_of(const value_type* s, size_type pos,
                                        size_type n) const noexcept {
    if (n == 0) {
      return pos;
    }
    if (n > traits_type::length(s)) {
      n = traits_type::length(s);
    }
    const value_type* p = data() + pos;
    const value_type* last = data() + size();
    while (p < last) {
      if (traits_type::find(s, n, *p) == nullptr) {
        return std::distance(data(), p);
      }
      ++p;
    }
    return npos;
  }

  constexpr size_type find_first_not_of(const value_type* s,
                                        size_type pos = 0) const noexcept {
    return find_first_not_of(s, pos, traits_type::length(s));
  }

  constexpr size_type find_first_not_of(value_type c,
                                        size_type pos = 0) const noexcept {
    const value_type* p = data() + pos;
    const value_type* last = data() + size();
    while (p < last) {
      if (!traits_type::eq(*p, c)) {
        return std::distance(data(), p);
      }
      ++p;
    }
    return npos;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr size_type find_first_not_of(const StringViewLike& s,
                                        size_type pos = 0) const noexcept {
    return find_first_not_of(s.data(), pos, s.size());
  }

  constexpr size_type find_last_of(const flat_string& str,
                                   size_type pos = npos) const noexcept {
    return find_last_of(str.data(), pos, str.size());
  }

  constexpr size_type find_last_of(const value_type* s, size_type pos,
                                   size_type n) const noexcept {
    if (n == 0) {
      return npos;
    }
    if (n > traits_type::length(s)) {
      n = traits_type::length(s);
    }
    const value_type* p = data() + std::min(pos, size());
    while (p >= data()) {
      if (traits_type::find(s, n, *p) != nullptr) {
        return std::distance(data(), p);
      }
      --p;
    }
    return npos;
  }

  constexpr size_type find_last_of(const value_type* s,
                                   size_type pos = npos) const noexcept {
    return find_last_of(s, pos, traits_type::length(s));
  }

  constexpr size_type find_last_of(value_type c,
                                   size_type pos = npos) const noexcept {
    return rfind(c, pos);
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr size_type find_last_of(const StringViewLike& s,
                                   size_type pos = npos) const noexcept {
    return find_last_of(s.data(), pos, s.size());
  }

  constexpr size_type find_last_not_of(const flat_string& str,
                                       size_type pos = npos) const noexcept {
    return find_last_not_of(str.data(), pos, str.size());
  }

  constexpr size_type find_last_not_of(const value_type* s, size_type pos,
                                       size_type n) const noexcept {
    if (n == 0) {
      return pos;
    }
    if (n > traits_type::length(s)) {
      n = traits_type::length(s);
    }
    const value_type* p = data() + std::min(pos, size());
    while (p >= data()) {
      if (traits_type::find(s, n, *p) == nullptr) {
        return std::distance(data(), p);
      }
      --p;
    }
    return npos;
  }

  constexpr size_type find_last_not_of(const value_type* s,
                                       size_type pos = npos) const noexcept {
    return find_last_not_of(s, pos, traits_type::length(s));
  }

  constexpr size_type find_last_not_of(value_type c,
                                       size_type pos = npos) const noexcept {
    return rfind(c, pos);
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr size_type find_last_not_of(const StringViewLike& s,
                                       size_type pos = npos) const noexcept {
    return find_last_not_of(s.data(), pos, s.size());
  }

  constexpr int comare(const flat_string& str) const {
    return compare(0, size(), str);
  }

  constexpr int compare(size_type pos, size_type count,
                        const flat_string& str) const {
    return compare(pos, count, str.data(), str.size());
  }

  constexpr int compare(size_type pos1, size_type count1,
                        const flat_string& str, size_type pos2,
                        size_type count2) const {
    if (pos2 > str.size()) {
      throw std::out_of_range("flat_string compare out of range");
    }
    return compare(pos1, count1, str.data() + pos2,
                   std::min(count2, str.size() - pos2));
  }

  constexpr int compare(const value_type* s) const {
    return compare(0, size(), s, traits_type::length(s));
  }

  constexpr int compare(size_type pos, size_type count,
                        const value_type* s) const {
    return compare(pos, count, s, traits_type::length(s));
  }

  constexpr int compare(size_type pos, size_type count1, const value_type* s,
                        size_type count2) const {
    if (pos >= size()) {
      throw std::out_of_range("flat_string compare out of range");
    }
    count1 = std::min(count1, size() - pos);
    size_type len = std::min(count1, size() - pos);
    int rst = traits_type::compare(data() + pos, s, std::min(len, count2));
    if (rst != 0) {
      return rst;
    }
    return len < count2 ? -1 : (len > count2 ? 1 : 0);
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr int compare(const StringViewLike& s) const noexcept {
    return compare(0, size(), s.data(), s.size());
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr int compare(size_type pos, size_type count,
                        const StringViewLike& s) const {
    return compare(pos, count, s.data(), s.size());
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr int compare(size_type pos1, size_type count1,
                        const StringViewLike& s, size_type pos2,
                        size_type count2) const {
    if (pos2 > s.size()) {
      throw std::out_of_range("flat_string compare out of range");
    }
    return compare(pos1, count1, s.data() + pos2,
                   std::min(count2, s.size() - pos2));
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr bool starts_with(const StringViewLike& str) const noexcept {
    if (str.size() > size()) {
      return false;
    }
    return traits_type::compare(data(), str.data(), str.size()) == 0;
  }

  constexpr bool starts_with(const value_type* s) const noexcept {
    if (traits_type::length(s) > size()) {
      return false;
    }
    return traits_type::compare(data(), s, traits_type::length(s)) == 0;
  }

  constexpr bool starts_with(value_type c) const noexcept {
    return !empty() && traits_type::eq(front(), c);
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr bool ends_with(const StringViewLike& str) const noexcept {
    if (str.size() > size()) {
      return false;
    }
    return traits_type::compare(data() + size() - str.size(), str.data(),
                                str.size()) == 0;
  }

  constexpr bool ends_with(const value_type* s) const noexcept {
    if (traits_type::length(s) > size()) {
      return false;
    }
    return traits_type::compare(data() + size() - traits_type::length(s), s,
                                traits_type::length(s)) == 0;
  }

  constexpr bool ends_with(value_type c) const noexcept {
    return !empty() && traits_type::eq(back(), c);
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  constexpr bool contains(const StringViewLike& str) const noexcept {
    return find(str) != npos;
  }

  constexpr bool contains(const value_type* s) const noexcept {
    return find(s) != npos;
  }

  constexpr bool contains(value_type c) const noexcept {
    return find(c) != npos;
  }

  constexpr flat_string substr(size_type pos = 0,
                               size_type count = npos) const& {
    return flat_string(*this, pos, count);
  }

  constexpr flat_string substr(size_type pos = 0, size_type count = npos) && {
    return flat_string(std::move(*this), pos, count);
  }

private:
  __storage_type storage_;
};

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(const flat_string<N, OutOfRangeAssert>& lhs,
          const flat_string<N, OutOfRangeAssert>& rhs) {
  flat_string<N, OutOfRangeAssert> result = lhs;
  result += rhs;
  return result;
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(const flat_string<N, OutOfRangeAssert>& lhs, const char* rhs) {
  flat_string<N, OutOfRangeAssert> result = lhs;
  result += rhs;
  return result;
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(const flat_string<N, OutOfRangeAssert>& lhs, char rhs) {
  flat_string<N, OutOfRangeAssert> result = lhs;
  result += rhs;
  return result;
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(const flat_string<N, OutOfRangeAssert>& lhs, std::string_view rhs) {
  flat_string<N, OutOfRangeAssert> result = lhs;
  result += rhs;
  return result;
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(const char* lhs, const flat_string<N, OutOfRangeAssert>& rhs) {
  flat_string<N, OutOfRangeAssert> result(lhs);
  result += rhs;
  return result;
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(char lhs, const flat_string<N, OutOfRangeAssert>& rhs) {
  flat_string<N, OutOfRangeAssert> result(1, lhs);
  result += rhs;
  return result;
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(std::string_view lhs, const flat_string<N, OutOfRangeAssert>& rhs) {
  flat_string<N, OutOfRangeAssert> result = lhs;
  result += rhs;
  return result;
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(flat_string<N, OutOfRangeAssert>&& lhs,
          flat_string<N, OutOfRangeAssert>&& rhs) {
  lhs += rhs;
  return std::move(lhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(flat_string<N, OutOfRangeAssert>&& lhs,
          const flat_string<N, OutOfRangeAssert>& rhs) {
  lhs += rhs;
  return std::move(lhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(flat_string<N, OutOfRangeAssert>&& lhs, const char* rhs) {
  lhs += rhs;
  return std::move(lhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(flat_string<N, OutOfRangeAssert>&& lhs, char rhs) {
  lhs += rhs;
  return std::move(lhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(flat_string<N, OutOfRangeAssert>&& lhs, std::string_view rhs) {
  lhs += rhs;
  return std::move(lhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(const flat_string<N, OutOfRangeAssert>& lhs,
          flat_string<N, OutOfRangeAssert>&& rhs) {
  rhs.insert(0, lhs);
  return std::move(rhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(const char* lhs, flat_string<N, OutOfRangeAssert>&& rhs) {
  rhs.insert(0, lhs);
  return std::move(rhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(char lhs, flat_string<N, OutOfRangeAssert>&& rhs) {
  rhs.insert(rhs.begin(), lhs);
  return std::move(rhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr flat_string<N, OutOfRangeAssert>
operator+(std::string_view lhs, flat_string<N, OutOfRangeAssert>&& rhs) {
  rhs.insert(0, lhs);
  return std::move(rhs);
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator==(const flat_string<N, OutOfRangeAssert>& lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return lhs.compare(rhs) == 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator==(const flat_string<N, OutOfRangeAssert>& lhs,
                          const char* rhs) {
  return lhs.compare(rhs) == 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator==(const char* lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return rhs.compare(lhs) == 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator!=(const flat_string<N, OutOfRangeAssert>& lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return lhs.compare(rhs) != 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator!=(const flat_string<N, OutOfRangeAssert>& lhs,
                          const char* rhs) {
  return lhs.compare(rhs) != 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator!=(const char* lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return rhs.compare(lhs) != 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator<(const flat_string<N, OutOfRangeAssert>& lhs,
                         const flat_string<N, OutOfRangeAssert>& rhs) {
  return lhs.compare(rhs) < 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator<(const flat_string<N, OutOfRangeAssert>& lhs,
                         const char* rhs) {
  return lhs.compare(rhs) < 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator<(const char* lhs,
                         const flat_string<N, OutOfRangeAssert>& rhs) {
  return rhs.compare(lhs) > 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator<=(const flat_string<N, OutOfRangeAssert>& lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return lhs.compare(rhs) <= 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator<=(const flat_string<N, OutOfRangeAssert>& lhs,
                          const char* rhs) {
  return lhs.compare(rhs) <= 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator<=(const char* lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return rhs.compare(lhs) >= 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator>(const flat_string<N, OutOfRangeAssert>& lhs,
                         const flat_string<N, OutOfRangeAssert>& rhs) {
  return lhs.compare(rhs) > 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator>(const flat_string<N, OutOfRangeAssert>& lhs,
                         const char* rhs) {
  return lhs.compare(rhs) > 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator>(const char* lhs,
                         const flat_string<N, OutOfRangeAssert>& rhs) {
  return rhs.compare(lhs) < 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator>=(const flat_string<N, OutOfRangeAssert>& lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return lhs.compare(rhs) >= 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator>=(const flat_string<N, OutOfRangeAssert>& lhs,
                          const char* rhs) {
  return lhs.compare(rhs) >= 0;
}

template <size_t N, typename OutOfRangeAssert>
constexpr bool operator>=(const char* lhs,
                          const flat_string<N, OutOfRangeAssert>& rhs) {
  return rhs.compare(lhs) <= 0;
}

}} // namespace es::string

namespace std {
template <size_t N, typename OutOfRangeAssert>
constexpr void
swap(es::string::flat_string<N, OutOfRangeAssert>& lhs,
     es::string::flat_string<N, OutOfRangeAssert>& rhs) noexcept {
  lhs.swap(rhs);
}

template <size_t N, typename OutOfRangeAssert, typename U>
constexpr typename es::string::flat_string<N, OutOfRangeAssert>::size_type
erase(es::string::flat_string<N, OutOfRangeAssert>& s, const U& value) {
  auto it = std::remove(s.begin(), s.end(), value);
  auto r = s.end() - it;
  s.erase(it, s.end());
  return r;
}

template <size_t N, typename OutOfRangeAssert, typename Pred>
constexpr typename es::string::flat_string<N, OutOfRangeAssert>::size_type
erase_if(es::string::flat_string<N, OutOfRangeAssert>& s, Pred pred) {
  auto it = std::remove_if(s.begin(), s.end(), pred);
  auto r = s.end() - it;
  s.erase(it, s.end());
  return r;
}

template <size_t N, typename OutOfRangeAssert>
std::basic_ostream<char>&
operator<<(std::basic_ostream<char>& os,
           const es::string::flat_string<N, OutOfRangeAssert>& str) {
  return os << str.data();
}

template <size_t N, typename OutOfRangeAssert>
std::basic_istream<char>&
operator>>(std::basic_istream<char>& is,
           es::string::flat_string<N, OutOfRangeAssert>& str) {
  std::string tmp;
  is >> tmp;
  str = tmp;
  return is;
}

template <size_t N, typename OutOfRangeAssert>
std::basic_istream<char>&
getline(std::basic_istream<char>& is,
        es::string::flat_string<N, OutOfRangeAssert>& str, char delim) {
  std::string tmp;
  std::getline(is, tmp, delim);
  str = tmp;
  return is;
}

template <size_t N, typename OutOfRangeAssert>
std::basic_istream<char>&
getline(std::basic_istream<char>&& is,
        es::string::flat_string<N, OutOfRangeAssert>& str, char delim) {
  std::string tmp;
  std::getline(is, tmp, delim);
  str = tmp;
  return is;
}

template <size_t N, typename OutOfRangeAssert>
std::basic_istream<char>&
getline(std::basic_istream<char>& is,
        es::string::flat_string<N, OutOfRangeAssert>& str) {
  std::string tmp;
  std::getline(is, tmp);
  str = tmp;
  return is;
}

template <size_t N, typename OutOfRangeAssert>
std::basic_istream<char>&
getline(std::basic_istream<char>&& is,
        es::string::flat_string<N, OutOfRangeAssert>& str) {
  std::string tmp;
  std::getline(is, tmp);
  str = tmp;
  return is;
}

template <size_t N, typename OutOfRangeAssert>
int stoi(const es::string::flat_string<N, OutOfRangeAssert>& str,
         size_t* pos = 0, int base = 10) {
  return std::stoi(str.data(), pos, base);
}

template <size_t N, typename OutOfRangeAssert>
long stol(const es::string::flat_string<N, OutOfRangeAssert>& str,
          size_t* pos = 0, int base = 10) {
  return std::stol(str.data(), pos, base);
}

template <size_t N, typename OutOfRangeAssert>
long long stoll(const es::string::flat_string<N, OutOfRangeAssert>& str,
                size_t* pos = 0, int base = 10) {
  return std::stoll(str.data(), pos, base);
}

template <size_t N, typename OutOfRangeAssert>
unsigned long stoul(const es::string::flat_string<N, OutOfRangeAssert>& str,
                    size_t* pos = 0, int base = 10) {
  return std::stoul(str.data(), pos, base);
}

template <size_t N, typename OutOfRangeAssert>
unsigned long long
stoull(const es::string::flat_string<N, OutOfRangeAssert>& str, size_t* pos = 0,
       int base = 10) {
  return std::stoull(str.data(), pos, base);
}

template <size_t N, typename OutOfRangeAssert>
float stof(const es::string::flat_string<N, OutOfRangeAssert>& str,
           size_t* pos = 0) {
  return std::stof(str.data(), pos);
}

template <size_t N, typename OutOfRangeAssert>
double stod(const es::string::flat_string<N, OutOfRangeAssert>& str,
            size_t* pos = 0) {
  return std::stod(str.data(), pos);
}

template <size_t N, typename OutOfRangeAssert>
long double stold(const es::string::flat_string<N, OutOfRangeAssert>& str,
                  size_t* pos = 0) {
  return std::stold(str.data(), pos);
}

template <size_t N, typename OutOfRangeAssert>
struct hash<es::string::flat_string<N, OutOfRangeAssert>> {
  size_t
  operator()(const es::string::flat_string<N, OutOfRangeAssert>& str) const {
    return std::hash<std::string_view>()(str);
  }
};
} // namespace std

#endif
