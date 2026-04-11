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

#ifndef ESTD___CONTAINER_MAYBE_OWNED_H
#define ESTD___CONTAINER_MAYBE_OWNED_H

#include <type_traits>
#include <utility>
#include <variant>

namespace es {

template <typename T>
class maybe_owned {
private:
  struct reference_wrapper {
    T* ptr;

    constexpr reference_wrapper(T& r) noexcept : ptr(&r) {}
    constexpr reference_wrapper& operator=(T& r) noexcept {
      ptr = &r;
      return *this;
    }
    constexpr T& get() const noexcept { return *ptr; }
  };

  struct empty {};

public:
  using value_type = T;
  using storage_type = std::variant<empty, T, reference_wrapper>;

  maybe_owned() : storage_(std::in_place_type<empty>) {}

  maybe_owned(T&& t) : storage_(std::in_place_type<T>, std::move(t)) {}

  maybe_owned(const T& t) : storage_(std::in_place_type<T>, t) {}

  maybe_owned(T& t) : storage_(std::in_place_type<reference_wrapper>, t) {}

  maybe_owned(const maybe_owned& other) : storage_(other.storage_) {}

  maybe_owned(maybe_owned&& other) noexcept
      : storage_(std::move(other.storage_)) {}

  ~maybe_owned() noexcept = default;

  maybe_owned& operator=(const maybe_owned& other) {
    if (this != &other) {
      storage_ = other.storage_;
    }
    return *this;
  }

  maybe_owned& operator=(maybe_owned&& other) noexcept {
    std::swap(storage_, other.storage_);
    return *this;
  }

  maybe_owned& operator=(T&& t) {
    storage_.template emplace<T>(std::move(t));
    return *this;
  }

  maybe_owned& operator=(const T& t) {
    storage_.template emplace<T>(t);
    return *this;
  }

  maybe_owned& operator=(T& t) {
    storage_.template emplace<reference_wrapper>(t);
    return *this;
  }

  bool has_value() const { return !std::holds_alternative<empty>(storage_); }

  explicit operator bool() const { return has_value(); }

  bool is_owned() const { return std::holds_alternative<T>(storage_); }

  bool is_reference() const {
    return std::holds_alternative<reference_wrapper>(storage_);
  }

  T& get() {
    if (auto* p = std::get_if<T>(&storage_)) {
      return *p;
    }
    return std::get<reference_wrapper>(storage_).get();
  }

  const T& get() const {
    if (auto* p = std::get_if<T>(&storage_)) {
      return *p;
    }
    return std::get<reference_wrapper>(storage_).get();
  }

  T& operator*() { return get(); }

  const T& operator*() const { return get(); }

  T* operator->() { return &get(); }

  const T* operator->() const { return &get(); }

private:
  storage_type storage_;
};

template <typename T>
maybe_owned(T&&) -> maybe_owned<T>;

template <typename T>
maybe_owned(T&) -> maybe_owned<T>;

} // namespace es

#endif // ESTD___CONTAINER_MAYBE_OWNED_H
