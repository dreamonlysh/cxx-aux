// Copyright (c) 2026 guyuemeng
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

#ifndef ESTD_STRING_SMALL_STRING_H
#define ESTD_STRING_SMALL_STRING_H

#include "flat_string.h"
#include <limits>
#include <string>
#include <variant>

namespace es { namespace string {

/**
 * @brief A string with small string optimization using flat_string for small
 * strings.
 *
 * This class provides a string that uses stack-allocated storage for small
 * strings (up to N characters) and automatically switches to heap-allocated
 * std::string when the size exceeds N.
 *
 * Key features:
 * - Uses flat_string<N> for strings with size <= N (stack allocation)
 * - Switches to std::string when size exceeds N (heap allocation)
 * - Standard string-like interface
 * - Automatic transition between small and large storage
 *
 * @tparam N Maximum number of characters for small string storage
 *
 * Example usage:
 * @code
 * small_string<32> str;
 * str = "Hello";           // Uses flat_string<32> (stack)
 * str += " World";         // Still uses flat_string<32>
 * str += " very long...";  // Switches to std::string (heap)
 * @endcode
 */
template <size_t N>
class small_string {
  static_assert(N > 0, "small_string N must be greater than 0");

  using small_storage_type = flat_string<N>;
  using large_storage_type = std::string;

public:
  using traits_type = std::char_traits<char>;
  using value_type = char;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = char&;
  using const_reference = const char&;
  using pointer = char*;
  using const_pointer = const char*;
  using iterator = char*;
  using const_iterator = const char*;

  static constexpr size_type npos = size_type(-1);
  static constexpr size_type small_capacity = N - 1;

  small_string() noexcept : storage_(small_storage_type{}) {}

  small_string(size_type count, char c) {
    if (count <= small_capacity) {
      storage_ = small_storage_type(count, c);
    } else {
      storage_ = large_storage_type(count, c);
    }
  }

  small_string(const char* s, size_type count) {
    if (count <= small_capacity) {
      storage_ = small_storage_type(s, count);
    } else {
      storage_ = large_storage_type(s, count);
    }
  }

  small_string(const char* s) : small_string(s, traits_type::length(s)) {}

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  small_string(const StringViewLike& s) : small_string(s.data(), s.size()) {}

  small_string(const small_string& other) = default;
  small_string(small_string&& other) noexcept = default;

  small_string(std::initializer_list<char> il)
      : small_string(il.begin(), il.size()) {}

  ~small_string() = default;

  small_string& operator=(const small_string& other) = default;
  small_string& operator=(small_string&& other) noexcept = default;

  small_string& operator=(const char* s) {
    assign(s, traits_type::length(s));
    return *this;
  }

  small_string& operator=(char c) {
    assign(&c, 1);
    return *this;
  }

  small_string& operator=(std::initializer_list<char> il) {
    assign(il.begin(), il.size());
    return *this;
  }

  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  small_string& operator=(const StringViewLike& s) {
    assign(s.data(), s.size());
    return *this;
  }

  small_string& assign(const char* s, size_type count) {
    if (count <= small_capacity) {
      storage_ = small_storage_type(s, count);
    } else {
      storage_ = large_storage_type(s, count);
    }
    return *this;
  }

  small_string& assign(const char* s) {
    return assign(s, traits_type::length(s));
  }

  small_string& assign(size_type count, char c) {
    if (count <= small_capacity) {
      storage_ = small_storage_type(count, c);
    } else {
      storage_ = large_storage_type(count, c);
    }
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("small_string::at() out of range");
    }
    return data()[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("small_string::at() out of range");
    }
    return data()[pos];
  }

  reference operator[](size_type pos) { return data()[pos]; }
  const_reference operator[](size_type pos) const { return data()[pos]; }

  reference front() { return data()[0]; }
  const_reference front() const { return data()[0]; }

  reference back() { return data()[size() - 1]; }
  const_reference back() const { return data()[size() - 1]; }

  pointer data() noexcept {
    return std::visit([](auto& s) -> pointer { return s.data(); }, storage_);
  }

  const_pointer data() const noexcept {
    return std::visit([](const auto& s) -> const_pointer { return s.data(); },
                      storage_);
  }

  const_pointer c_str() const noexcept {
    if (is_small()) {
      return std::get<small_storage_type>(storage_).c_str();
    }
    return std::get<large_storage_type>(storage_).c_str();
  }

  operator std::string_view() const noexcept {
    return std::string_view(data(), size());
  }

  iterator begin() noexcept { return data(); }
  const_iterator begin() const noexcept { return data(); }
  const_iterator cbegin() const noexcept { return data(); }

  iterator end() noexcept { return data() + size(); }
  const_iterator end() const noexcept { return data() + size(); }
  const_iterator cend() const noexcept { return data() + size(); }

  bool empty() const noexcept { return size() == 0; }

  size_type size() const noexcept {
    return std::visit([](const auto& s) -> size_type { return s.size(); },
                      storage_);
  }

  size_type length() const noexcept { return size(); }

  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
  }

  size_type capacity() const noexcept {
    return std::visit([](const auto& s) -> size_type { return s.capacity(); },
                      storage_);
  }

  void clear() noexcept { storage_ = small_storage_type{}; }

  void reserve(size_type new_cap) {
    if (new_cap > small_capacity && is_small()) {
      large_storage_type large;
      large.reserve(new_cap);
      large.assign(data(), size());
      storage_ = std::move(large);
    } else if (!is_small()) {
      std::get<large_storage_type>(storage_).reserve(new_cap);
    }
  }

  void shrink_to_fit() {
    if (!is_small()) {
      auto& large = std::get<large_storage_type>(storage_);
      if (large.size() <= small_capacity) {
        small_storage_type small(large.data(), large.size());
        storage_ = std::move(small);
      } else {
        large.shrink_to_fit();
      }
    }
  }

  small_string& append(const char* s, size_type n) {
    size_type new_size = size() + n;
    if (new_size <= small_capacity && is_small()) {
      std::get<small_storage_type>(storage_).append(s, n);
    } else {
      ensure_large();
      std::get<large_storage_type>(storage_).append(s, n);
    }
    return *this;
  }

  small_string& append(const char* s) {
    return append(s, traits_type::length(s));
  }

  small_string& append(size_type count, char c) {
    size_type new_size = size() + count;
    if (new_size <= small_capacity && is_small()) {
      std::get<small_storage_type>(storage_).append(count, c);
    } else {
      ensure_large();
      std::get<large_storage_type>(storage_).append(count, c);
    }
    return *this;
  }

  small_string& append(std::string_view sv) {
    return append(sv.data(), sv.size());
  }

  small_string& operator+=(const small_string& str) {
    return append(str.data(), str.size());
  }

  small_string& operator+=(const char* s) { return append(s); }

  small_string& operator+=(char c) {
    push_back(c);
    return *this;
  }

  small_string& operator+=(std::string_view sv) { return append(sv); }

  void push_back(char c) {
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small()) {
      std::get<small_storage_type>(storage_).push_back(c);
    } else {
      ensure_large();
      std::get<large_storage_type>(storage_).push_back(c);
    }
  }

  void pop_back() {
    if (is_small()) {
      std::get<small_storage_type>(storage_).pop_back();
    } else {
      auto& large = std::get<large_storage_type>(storage_);
      large.pop_back();
      if (large.size() <= small_capacity) {
        small_storage_type small(large.data(), large.size());
        storage_ = std::move(small);
      }
    }
  }

  small_string& insert(size_type pos, const char* s, size_type n) {
    size_type new_size = size() + n;
    if (new_size <= small_capacity && is_small()) {
      std::get<small_storage_type>(storage_).insert(pos, s, n);
    } else {
      ensure_large();
      std::get<large_storage_type>(storage_).insert(pos, s, n);
    }
    return *this;
  }

  small_string& insert(size_type pos, const char* s) {
    return insert(pos, s, traits_type::length(s));
  }

  small_string& erase(size_type pos = 0, size_type count = npos) {
    if (is_small()) {
      std::get<small_storage_type>(storage_).erase(pos, count);
    } else {
      auto& large = std::get<large_storage_type>(storage_);
      large.erase(pos, count);
      if (large.size() <= small_capacity) {
        small_storage_type small(large.data(), large.size());
        storage_ = std::move(small);
      }
    }
    return *this;
  }

  void resize(size_type count) { resize(count, char{}); }

  void resize(size_type count, char c) {
    if (count <= small_capacity && is_small()) {
      std::get<small_storage_type>(storage_).resize(count, c);
    } else if (count > small_capacity) {
      ensure_large();
      std::get<large_storage_type>(storage_).resize(count, c);
    } else {
      small_storage_type small(count, c);
      storage_ = std::move(small);
    }
  }

  void swap(small_string& other) noexcept { storage_.swap(other.storage_); }

  size_type find(const small_string& str, size_type pos = 0) const noexcept {
    return find(str.data(), pos, str.size());
  }

  size_type find(const char* s, size_type pos, size_type n) const noexcept {
    return std::string_view(data(), size()).find(std::string_view(s, n), pos);
  }

  size_type find(const char* s, size_type pos = 0) const noexcept {
    return find(s, pos, traits_type::length(s));
  }

  size_type find(char c, size_type pos = 0) const noexcept {
    return std::string_view(data(), size()).find(c, pos);
  }

  size_type rfind(const small_string& str,
                  size_type pos = npos) const noexcept {
    return rfind(str.data(), pos, str.size());
  }

  size_type rfind(const char* s, size_type pos, size_type n) const noexcept {
    return std::string_view(data(), size()).rfind(std::string_view(s, n), pos);
  }

  size_type rfind(const char* s, size_type pos = npos) const noexcept {
    return rfind(s, pos, traits_type::length(s));
  }

  size_type rfind(char c, size_type pos = npos) const noexcept {
    return std::string_view(data(), size()).rfind(c, pos);
  }

  int compare(const small_string& str) const noexcept {
    return std::string_view(data(), size())
        .compare(std::string_view(str.data(), str.size()));
  }

  int compare(const char* s) const noexcept {
    return std::string_view(data(), size())
        .compare(std::string_view(s, traits_type::length(s)));
  }

  bool starts_with(std::string_view sv) const noexcept {
    if (sv.size() > size())
      return false;
    return traits_type::compare(data(), sv.data(), sv.size()) == 0;
  }

  bool starts_with(char c) const noexcept { return !empty() && front() == c; }

  bool starts_with(const char* s) const noexcept {
    return starts_with(std::string_view(s));
  }

  bool ends_with(std::string_view sv) const noexcept {
    if (sv.size() > size())
      return false;
    return traits_type::compare(data() + size() - sv.size(), sv.data(),
                                sv.size()) == 0;
  }

  bool ends_with(char c) const noexcept { return !empty() && back() == c; }

  bool ends_with(const char* s) const noexcept {
    return ends_with(std::string_view(s));
  }

  bool contains(std::string_view sv) const noexcept {
    return find(sv.data(), 0, sv.size()) != npos;
  }

  bool contains(char c) const noexcept { return find(c) != npos; }

  bool contains(const char* s) const noexcept { return find(s) != npos; }

  small_string substr(size_type pos = 0, size_type count = npos) const {
    size_type len = std::min(count, size() - pos);
    return small_string(data() + pos, len);
  }

  bool is_small() const noexcept {
    return std::holds_alternative<small_storage_type>(storage_);
  }

private:
  void ensure_large() {
    if (is_small()) {
      auto& small = std::get<small_storage_type>(storage_);
      large_storage_type large(small.data(), small.size());
      storage_ = std::move(large);
    }
  }

  std::variant<small_storage_type, large_storage_type> storage_;
};

template <size_t N>
small_string<N> operator+(const small_string<N>& lhs,
                          const small_string<N>& rhs) {
  small_string<N> result = lhs;
  result += rhs;
  return result;
}

template <size_t N>
small_string<N> operator+(const small_string<N>& lhs, const char* rhs) {
  small_string<N> result = lhs;
  result += rhs;
  return result;
}

template <size_t N>
small_string<N> operator+(const small_string<N>& lhs, char rhs) {
  small_string<N> result = lhs;
  result += rhs;
  return result;
}

template <size_t N>
small_string<N> operator+(const char* lhs, const small_string<N>& rhs) {
  small_string<N> result(lhs);
  result += rhs;
  return result;
}

template <size_t N>
small_string<N> operator+(char lhs, const small_string<N>& rhs) {
  small_string<N> result(1, lhs);
  result += rhs;
  return result;
}

template <size_t N>
bool operator==(const small_string<N>& lhs,
                const small_string<N>& rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

template <size_t N>
bool operator==(const small_string<N>& lhs, const char* rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

template <size_t N>
bool operator==(const char* lhs, const small_string<N>& rhs) noexcept {
  return rhs.compare(lhs) == 0;
}

template <size_t N>
bool operator!=(const small_string<N>& lhs,
                const small_string<N>& rhs) noexcept {
  return lhs.compare(rhs) != 0;
}

template <size_t N>
bool operator!=(const small_string<N>& lhs, const char* rhs) noexcept {
  return lhs.compare(rhs) != 0;
}

template <size_t N>
bool operator!=(const char* lhs, const small_string<N>& rhs) noexcept {
  return rhs.compare(lhs) != 0;
}

template <size_t N>
bool operator<(const small_string<N>& lhs,
               const small_string<N>& rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

template <size_t N>
bool operator<(const small_string<N>& lhs, const char* rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

template <size_t N>
bool operator<(const char* lhs, const small_string<N>& rhs) noexcept {
  return rhs.compare(lhs) > 0;
}

template <size_t N>
bool operator<=(const small_string<N>& lhs,
                const small_string<N>& rhs) noexcept {
  return lhs.compare(rhs) <= 0;
}

template <size_t N>
bool operator<=(const small_string<N>& lhs, const char* rhs) noexcept {
  return lhs.compare(rhs) <= 0;
}

template <size_t N>
bool operator<=(const char* lhs, const small_string<N>& rhs) noexcept {
  return rhs.compare(lhs) >= 0;
}

template <size_t N>
bool operator>(const small_string<N>& lhs,
               const small_string<N>& rhs) noexcept {
  return lhs.compare(rhs) > 0;
}

template <size_t N>
bool operator>(const small_string<N>& lhs, const char* rhs) noexcept {
  return lhs.compare(rhs) > 0;
}

template <size_t N>
bool operator>(const char* lhs, const small_string<N>& rhs) noexcept {
  return rhs.compare(lhs) < 0;
}

template <size_t N>
bool operator>=(const small_string<N>& lhs,
                const small_string<N>& rhs) noexcept {
  return lhs.compare(rhs) >= 0;
}

template <size_t N>
bool operator>=(const small_string<N>& lhs, const char* rhs) noexcept {
  return lhs.compare(rhs) >= 0;
}

template <size_t N>
bool operator>=(const char* lhs, const small_string<N>& rhs) noexcept {
  return rhs.compare(lhs) <= 0;
}

}} // namespace es::string

namespace std {

template <size_t N>
void swap(es::string::small_string<N>& lhs,
          es::string::small_string<N>& rhs) noexcept {
  lhs.swap(rhs);
}

template <size_t N>
std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os,
                                     const es::string::small_string<N>& str) {
  return os << str.data();
}

template <size_t N>
std::basic_istream<char>& operator>>(std::basic_istream<char>& is,
                                     es::string::small_string<N>& str) {
  std::string tmp;
  is >> tmp;
  str = tmp;
  return is;
}

template <size_t N>
std::basic_istream<char>& getline(std::basic_istream<char>& is,
                                  es::string::small_string<N>& str,
                                  char delim) {
  std::string tmp;
  std::getline(is, tmp, delim);
  str = tmp;
  return is;
}

template <size_t N>
std::basic_istream<char>& getline(std::basic_istream<char>& is,
                                  es::string::small_string<N>& str) {
  std::string tmp;
  std::getline(is, tmp);
  str = tmp;
  return is;
}

template <size_t N>
struct hash<es::string::small_string<N>> {
  size_t operator()(const es::string::small_string<N>& str) const {
    return std::hash<std::string_view>()(str);
  }
};

} // namespace std

#endif
