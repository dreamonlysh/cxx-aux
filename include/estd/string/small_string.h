// Copyright (c) 2026 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2.
// You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
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
#include <memory>
#include <string>

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
 * - Union-based storage (no std::variant overhead)
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

  small_string() noexcept : is_small_(true) {
    ::new (static_cast<void*>(&small_)) small_storage_type();
  }

  /**
   * @brief Constructs a string with count copies of character c.
   * @param count Number of characters to fill.
   * @param c Character value to fill with.
   */
  small_string(size_type count, char c) {
    if (count <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(count, c);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(count, c);
    }
  }

  /**
   * @brief Constructs a string from the first count characters of s.
   * @param s Pointer to source character array.
   * @param count Number of characters to copy.
   */
  small_string(const char* s, size_type count) {
    if (count <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(s, count);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(s, count);
    }
  }

  /**
   * @brief Constructs a string from a null-terminated C-string.
   * @param s Pointer to null-terminated source string.
   */
  small_string(const char* s) : small_string(s, traits_type::length(s)) {}

  /**
   * @brief Constructs from a string_view-like object.
   * @tparam StringViewLike Type satisfying the string_view-like concept.
   * @param s Source string_view-like object.
   */
  template <typename StringViewLike,
            typename = std::enable_if_t<
                __impl_type_traits::is_string_view_like_v<StringViewLike>>>
  small_string(const StringViewLike& s) : small_string(s.data(), s.size()) {}

  small_string(const small_string& other) : is_small_(other.is_small_) {
    if (other.is_small_) {
      ::new (static_cast<void*>(&small_)) small_storage_type(other.small_);
    } else {
      ::new (static_cast<void*>(&large_)) large_storage_type(other.large_);
    }
  }

  small_string(small_string&& other) noexcept : is_small_(other.is_small_) {
    if (other.is_small_) {
      ::new (static_cast<void*>(&small_))
          small_storage_type(std::move(other.small_));
    } else {
      ::new (static_cast<void*>(&large_))
          large_storage_type(std::move(other.large_));
    }
  }

  /**
   * @brief Constructs from an initializer list.
   * @param il Initializer list of characters.
   */
  small_string(std::initializer_list<char> il)
      : small_string(il.begin(), il.size()) {}

  ~small_string() { destroy(); }

  small_string& operator=(const small_string& other) {
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

  small_string& operator=(small_string&& other) noexcept {
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

  /**
   * @brief Replaces the contents with the first count characters of s.
   * @param s Pointer to source character array.
   * @param count Number of characters to copy.
   * @return Reference to this string.
   */
  small_string& assign(const char* s, size_type count) {
    destroy();
    if (count <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(s, count);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(s, count);
    }
    return *this;
  }

  /**
   * @brief Replaces the contents with a C-string.
   * @param s Pointer to null-terminated source string.
   * @return Reference to this string.
   */
  small_string& assign(const char* s) {
    return assign(s, traits_type::length(s));
  }

  /**
   * @brief Replaces the contents with count copies of c.
   * @param count Number of characters to fill.
   * @param c Character value to fill with.
   * @return Reference to this string.
   */
  small_string& assign(size_type count, char c) {
    destroy();
    if (count <= small_capacity) {
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type(count, c);
    } else {
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(count, c);
    }
    return *this;
  }

  /**
   * @brief Accesses character at pos with bounds checking.
   * @throws std::out_of_range if pos >= size().
   */
  reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("small_string::at() out of range");
    }
    return data()[pos];
  }

  /**
   * @brief Accesses character at pos with bounds checking.
   * @throws std::out_of_range if pos >= size().
   */
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

  pointer data() noexcept { return is_small_ ? small_.data() : large_.data(); }

  const_pointer data() const noexcept {
    return is_small_ ? small_.data() : large_.data();
  }

  const_pointer c_str() const noexcept { return data(); }

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
    return is_small_ ? small_.size() : large_.size();
  }

  size_type length() const noexcept { return size(); }

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
      std::destroy_at(&large_);
      is_small_ = true;
      ::new (static_cast<void*>(&small_)) small_storage_type();
    }
  }

  /**
   * @brief Reserves storage for at least new_cap characters.
   * May transition from small to large storage.
   */
  void reserve(size_type new_cap) {
    if (new_cap > small_capacity && is_small_) {
      large_storage_type large;
      large.reserve(new_cap);
      large.assign(small_.data(), small_.size());
      std::destroy_at(&small_);
      is_small_ = false;
      ::new (static_cast<void*>(&large_)) large_storage_type(std::move(large));
    } else if (!is_small_) {
      large_.reserve(new_cap);
    }
  }

  /**
   * @brief Reduces capacity. May transition from large to small storage.
   */
  void shrink_to_fit() {
    if (!is_small_) {
      if (large_.size() <= small_capacity) {
        small_storage_type small(large_.data(), large_.size());
        std::destroy_at(&large_);
        is_small_ = true;
        ::new (static_cast<void*>(&small_))
            small_storage_type(std::move(small));
      } else {
        large_.shrink_to_fit();
      }
    }
  }

  /**
   * @brief Appends the first n characters of s.
   * May transition from small to large storage.
   */
  small_string& append(const char* s, size_type n) {
    size_type new_size = size() + n;
    if (new_size <= small_capacity && is_small_) {
      small_.append(s, n);
    } else {
      ensure_large();
      large_.append(s, n);
    }
    return *this;
  }

  /**
   * @brief Appends a C-string.
   */
  small_string& append(const char* s) {
    return append(s, traits_type::length(s));
  }

  /**
   * @brief Appends count copies of c.
   * May transition from small to large storage.
   */
  small_string& append(size_type count, char c) {
    size_type new_size = size() + count;
    if (new_size <= small_capacity && is_small_) {
      small_.append(count, c);
    } else {
      ensure_large();
      large_.append(count, c);
    }
    return *this;
  }

  /**
   * @brief Appends a string_view.
   * May transition from small to large storage.
   */
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

  /**
   * @brief Appends a character. May transition from small to large storage.
   */
  void push_back(char c) {
    size_type new_size = size() + 1;
    if (new_size <= small_capacity && is_small_) {
      small_.push_back(c);
    } else {
      ensure_large();
      large_.push_back(c);
    }
  }

  /**
   * @brief Removes the last character. May transition from large to small.
   */
  void pop_back() {
    if (is_small_) {
      small_.pop_back();
    } else {
      large_.pop_back();
      if (large_.size() <= small_capacity) {
        small_storage_type small(large_.data(), large_.size());
        std::destroy_at(&large_);
        is_small_ = true;
        ::new (static_cast<void*>(&small_))
            small_storage_type(std::move(small));
      }
    }
  }

  /**
   * @brief Inserts characters at position.
   * May transition from small to large storage.
   */
  small_string& insert(size_type pos, const char* s, size_type n) {
    size_type new_size = size() + n;
    if (new_size <= small_capacity && is_small_) {
      small_.insert(pos, s, n);
    } else {
      ensure_large();
      large_.insert(pos, s, n);
    }
    return *this;
  }

  /**
   * @brief Inserts characters at position.
   */
  small_string& insert(size_type pos, const char* s) {
    return insert(pos, s, traits_type::length(s));
  }

  /**
   * @brief Erases characters from position.
   * May transition from large to small storage.
   */
  small_string& erase(size_type pos = 0, size_type count = npos) {
    if (is_small_) {
      small_.erase(pos, count);
    } else {
      large_.erase(pos, count);
      if (large_.size() <= small_capacity) {
        small_storage_type small(large_.data(), large_.size());
        std::destroy_at(&large_);
        is_small_ = true;
        ::new (static_cast<void*>(&small_))
            small_storage_type(std::move(small));
      }
    }
    return *this;
  }

  /**
   * @brief Resizes the string.
   * May transition between small and large storage.
   */
  void resize(size_type count) { resize(count, char{}); }

  /**
   * @brief Resizes the string.
   * May transition from small to large storage, but never large to small.
   */
  void resize(size_type count, char c) {
    if (count <= small_capacity && is_small_) {
      small_.resize(count, c);
    } else if (count > small_capacity) {
      ensure_large();
      large_.resize(count, c);
    } else {
      large_.resize(count, c);
    }
  }

  /**
   * @brief Swaps contents with another small_string.
   */
  void swap(small_string& other) noexcept {
    small_string tmp(std::move(*this));
    *this = std::move(other);
    other = std::move(tmp);
  }

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
      large_storage_type large(small_.data(), small_.size());
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
