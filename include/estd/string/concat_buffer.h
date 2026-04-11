#ifndef ESTD_STRING_CONCAT_BUFFER_H
#define ESTD_STRING_CONCAT_BUFFER_H

#include <algorithm>
#include <charconv>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "__algorithm/append.h"
#include "__algorithm/lazy_views.h"
#include "__algorithm/type_traits_string.h"
#include <estd/__bit/bit_pow2.h>
#include <estd/__container/maybe_owned.h>
#include <estd/__meta/array.h>

namespace es { namespace string {

// Storage wrapper for dynamic buffer (STL-like naming)
namespace __impl_concat_buffer {

template <typename Container>
class __storage {
public:
  using container_type = Container;

  static constexpr bool can_reserve =
      __impl_type_traits::has_member_reserve_v<container_type>;
  static constexpr bool can_resize =
      __impl_type_traits::has_member_resize_v<container_type>;
  static constexpr bool can_resize_and_overwrite =
      __impl_type_traits::has_member_resize_and_overwrite_v<container_type>;

  __storage(container_type&& c) : storage_(std::move(c)) {}
  __storage(container_type& c) : storage_(c) {}

  char* data() { return storage_->data(); }
  const char* data() const { return storage_->data(); }

  constexpr size_t capacity() const { return storage_->capacity(); }

  constexpr size_t size() const { return storage_->size(); }

  constexpr void reserve(size_t c) {
    if constexpr (__impl_type_traits::has_member_reserve_v<container_type>) {
      storage_->reserve(c);
    }
  }

  constexpr void resize(size_t s) {
    if constexpr (__impl_type_traits::has_member_resize_v<container_type>) {
      storage_->resize(s);
    }
  }

  template <typename F>
  constexpr void resize_and_overwrite(size_t s, F&& f) {
    if constexpr (__impl_type_traits::has_member_resize_and_overwrite_v<
                      container_type>) {
      storage_->resize_and_overwrite(s, std::forward<F>(f));
    }
  }

private:
  maybe_owned<container_type> storage_;
};

// Base class for concat_buffer using CRTP
template <typename Derived>
class __concat_buffer_base {
protected:
  using Base = __concat_buffer_base<Derived>;

  char* cursor_ = nullptr;

  constexpr __concat_buffer_base() = default;

public:
  template <typename... Args>
  constexpr bool append(Args&&... args) {
    return (append_each(std::forward<Args>(args)) && ...);
  }

  constexpr void clear() { cursor_ = derived()->data(); }

  constexpr size_t size() const {
    return static_cast<size_t>(cursor_ - base());
  }

  constexpr std::string_view view() const { return {base(), size()}; }

  std::string to_string() const { return {base(), size()}; }

  constexpr char* c_str() {
    if (derived()->ensure_capacity(1)) {
      *cursor_ = '\0';
    }
    return derived()->data();
  }

  constexpr const char* c_str() const {
    if (derived()->ensure_capacity(1)) {
      *cursor_ = '\0';
    }
    return derived()->data();
  }

  constexpr explicit operator char*() { return base(); }

  constexpr explicit operator const char*() const { return base(); }

private:
  constexpr bool ensure_capacity(size_t additional) {
    size_t current_size = size();
    size_t required = current_size + additional;
    if (required <= derived()->capacity()) {
      return true;
    }

    if constexpr (Derived::can_reserve) {
      size_t new_capacity =
          std::max(derived()->capacity() * 2, ceil_to_pow2(required));
      char* old_data = base();
      derived()->reserve(new_capacity);
      char* new_data = base();
      if (old_data != new_data) {
        cursor_ = new_data + current_size;
      }
      return true;
    } else {
      return false;
    }
  }

  constexpr void resize_for_raw(size_t additional) {
    size_t new_size = size() + additional;
    if constexpr (Derived::can_resize_and_overwrite) {
      derived()->resize_and_overwrite(
          new_size, [new_size](char* p, size_t s) { return new_size; });
    } else if constexpr (Derived::can_resize) {
      derived()->resize(new_size);
    }
  }

  constexpr bool append_each(char c) {
    if (!ensure_capacity(1))
      return false;

    resize_for_raw(1);
    cursor_ = __impl_append::assign(cursor_, c);
    return true;
  }

  constexpr bool append_each(const char* p, size_t n) {
    if (!ensure_capacity(n))
      return false;
    resize_for_raw(n);
    cursor_ = __impl_append::assign(cursor_, p, n);
    return true;
  }

  constexpr bool append_each(const char* p) {
    size_t n = std::strlen(p);
    return append_each(p, n);
  }

  template <typename T, std::enable_if_t<
                            es::is_iterable_v<T> &&
                                !std::is_integral_v<std::remove_reference_t<T>>,
                            int> = 0>
  constexpr bool append_each(T&& v) {
    bool success = true;
    for (auto m : std::forward<T>(v)) {
      success &= append_each(m);
    }
    return success;
  }

  template <
      typename T, typename = std::enable_if_t<!es::is_iterable_v<T>>,
      typename = std::enable_if_t<__impl_type_traits::has_member_data_v<T>>,
      typename = std::enable_if_t<__impl_type_traits::has_member_size_v<T>>>
  constexpr bool append_each(const T& v) {
    return append_each(v.data(), v.size());
  }

  template <typename T, typename = std::enable_if_t<
                            std::is_integral_v<std::remove_reference_t<T>>>>
  bool append_each(T&& v) {
    using ValueType = remove_cvref_t<T>;
    constexpr size_t max_digits = __impl_append::digits10<ValueType>::value;
    size_t remaining = derived()->capacity() - size();

    if (remaining >= max_digits) {
      resize_for_raw(max_digits);
      auto [ptr, ec] = std::to_chars(cursor_, cursor_ + remaining, v);
      resize_for_raw(ptr - cursor_);
      cursor_ = ptr;
      return true;
    }

    char buffer[32];
    auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), v);
    return append_each(buffer, ptr - buffer);
  }

  constexpr bool append_each(std::string_view sv) {
    return append_each(sv.data(), sv.size());
  }

  constexpr Derived* derived() { return static_cast<Derived*>(this); }

  constexpr const Derived* derived() const {
    return static_cast<const Derived*>(this);
  }

  constexpr char* base() { return derived()->data(); }

  constexpr const char* base() const { return derived()->data(); }

  char* end() { return derived()->end(); }
  const char* end() const { return derived()->end(); }
};

// Fixed buffer implementation (for N > 0)
template <size_t N>
class __concat_buffer_fixed
    : public __concat_buffer_base<__concat_buffer_fixed<N>> {
  using Base = __concat_buffer_base<__concat_buffer_fixed<N>>;
  using Base::cursor_;

public:
  static_assert(N > 0, "N must be greater than 0");

  static constexpr bool can_reserve = false;
  static constexpr bool can_resize = false;
  static constexpr bool can_resize_and_overwrite = false;

  constexpr __concat_buffer_fixed() { cursor_ = data_; }

  constexpr char* data() { return data_; }
  constexpr const char* data() const { return data_; }
  constexpr char* end() { return cursor_; }
  constexpr const char* end() const { return cursor_; }

  constexpr size_t size() const { return static_cast<size_t>(cursor_ - data_); }

  constexpr size_t capacity() const { return N; }

private:
  char data_[N];
};

// Dynamic buffer implementation (for N = 0 with Container)
template <typename Container>
class __concat_buffer_dynamic
    : public __concat_buffer_base<__concat_buffer_dynamic<Container>> {
  using Base = __concat_buffer_base<__concat_buffer_dynamic<Container>>;
  using Base::cursor_;

public:
  static_assert(__impl_type_traits::has_member_data_v<Container> &&
                    __impl_type_traits::has_member_size_v<Container>,
                "Container must have data() and size() methods");

  static constexpr bool can_reserve = __storage<Container>::can_reserve;
  static constexpr bool can_resize = __storage<Container>::can_resize;
  static constexpr bool can_resize_and_overwrite =
      __storage<Container>::can_resize_and_overwrite;

  constexpr __concat_buffer_dynamic() : storage_(Container{}) {
    cursor_ = data();
  }

  constexpr __concat_buffer_dynamic(size_t n) : storage_(Container{}) {
    static_assert(can_reserve,
                  "Container must support reserve() to use this constructor");
    storage_.reserve(n);
    cursor_ = data();
  }

  constexpr __concat_buffer_dynamic(Container& container)
      : storage_(container) {
    cursor_ = data() + storage_.size();
  }

  constexpr __concat_buffer_dynamic(Container&& container)
      : storage_(std::move(container)) {
    cursor_ = data() + storage_.size();
  }

  constexpr char* data() { return storage_.data(); }
  constexpr const char* data() const { return storage_.data(); }

  constexpr char* end() { return cursor_; }
  constexpr const char* end() const { return cursor_; }

  constexpr size_t size() const {
    return static_cast<size_t>(cursor_ - data());
  }

  constexpr size_t capacity() const {
    if constexpr (es::is_c_array_v<Container> ||
                  es::is_std_array_v<Container>) {
      return es::array_traits<Container>::size;
    } else {
      return storage_.capacity();
    }
  }

  constexpr void reserve(size_t capacity) {
    if constexpr (can_reserve) {
      storage_.reserve(capacity);
    }
  }

  constexpr void resize(size_t size) {
    if constexpr (can_resize) {
      storage_.resize(size);
    }
  }

  template <typename F>
  constexpr void resize_and_overwrite(size_t size, F&& f) {
    if constexpr (can_resize_and_overwrite) {
      storage_.resize_and_overwrite(size, std::forward<F>(f));
    }
  }

private:
  __storage<Container> storage_;
};
} // namespace __impl_concat_buffer

// concat_buffer<N, Container> - two parameter template
// - concat_buffer<N> -> fixed buffer with N chars
// - concat_buffer<N, Container> -> dynamic buffer using Container with initial
// capacity N
// - concat_buffer<Container> -> dynamic buffer using Container (backward
// compat)
template <size_t N, typename Container = char[]>
class concat_buffer;

// Specialization: concat_buffer<N> (fixed buffer with N chars)
template <size_t N>
class concat_buffer<N, char[]>
    : public __impl_concat_buffer::__concat_buffer_fixed<N> {
  using Base = __impl_concat_buffer::__concat_buffer_fixed<N>;

public:
  using Base::append;
  using Base::c_str;
  using Base::capacity;
  using Base::clear;
  using Base::data;
  using Base::end;
  using Base::size;
  using Base::to_string;
  using Base::view;
};

// Specialization: concat_buffer<N, Container> (dynamic buffer with initial
// capacity)
template <size_t N, typename Container>
class concat_buffer
    : public __impl_concat_buffer::__concat_buffer_dynamic<Container> {
  using Base = __impl_concat_buffer::__concat_buffer_dynamic<Container>;

public:
  using Base::append;
  using Base::c_str;
  using Base::capacity;
  using Base::clear;
  using Base::data;
  using Base::end;
  using Base::size;
  using Base::to_string;
  using Base::view;

  constexpr concat_buffer() : Base(N) {}
  constexpr concat_buffer(Container& c) : Base(c) {}
  constexpr concat_buffer(Container&& c) : Base(std::move(c)) {}
};

template <typename Container>
concat_buffer(Container& c) -> concat_buffer<0, Container>;

template <typename Container>
concat_buffer(Container&& c) -> concat_buffer<0, Container>;

}} // namespace es::string

#endif // ESTD_STRING_CONCAT_BUFFER_H
