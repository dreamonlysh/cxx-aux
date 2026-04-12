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

#include <estd/__meta/array.h>
#include <type_traits>
#include <utility>
#include <variant>

namespace es {

/**
 * @brief A container that can hold either owned data or a reference to external
 * data.
 *
 * This class provides flexible storage semantics where data can be either:
 * - Owned: The container owns and manages the lifetime of the data
 * - Referenced: The container holds a reference to external data
 * - Empty: No data is stored
 *
 * @tparam T The type of data to store. Supports C arrays (converted to
 * std::array internally).
 *
 * @note C arrays are automatically converted to std::array for storage.
 * @note Rvalue construction leads to owned storage, lvalue construction leads
 * to reference storage.
 *
 * Example usage:
 * @code
 * std::string str = "hello";
 * maybe_owned<std::string> ref(str);      // Reference to str
 * maybe_owned<std::string> owned("world"); // Owns the string
 * @endcode
 */
template <typename T>
class maybe_owned {
private:
  // Convert C arrays to std::array for storage
  using __owned_type = to_std_array_t<T>;

  // Reference wrapper for non-owned data
  struct __reference_type {
    T* ptr;

    constexpr explicit __reference_type(T& r) noexcept : ptr(&r) {}
    constexpr __reference_type& operator=(T& r) noexcept {
      ptr = &r;
      return *this;
    }
    constexpr T& get() const noexcept { return *ptr; }
  };

  // Empty state marker
  struct empty {};

public:
  using value_type = T;
  using storage_type = std::variant<empty, __owned_type, __reference_type>;

  /**
   * @brief Default constructor. Creates an empty maybe_owned.
   */
  constexpr maybe_owned() noexcept : storage_(std::in_place_type<empty>) {}

  /**
   * @brief In-place constructor for owned storage.
   *
   * Constructs the owned data in-place using the provided arguments.
   *
   * @tparam Args Constructor argument types
   * @param args Arguments to forward to the value_type constructor
   */
  template <typename... Args,
            typename = std::enable_if_t<
                std::is_constructible_v<__owned_type, Args...>>>
  constexpr maybe_owned(std::in_place_type_t<T>, Args&&... args)
      : storage_(std::in_place_type<__owned_type>,
                 std::forward<Args>(args)...) {}

  /**
   * @brief In-place constructor with initializer list for owned storage.
   *
   * Constructs the owned data in-place using an initializer list.
   *
   * @tparam U Element type of the initializer list
   * @param init Initializer list to construct the value
   */
  template <typename U, typename = std::enable_if_t<std::is_constructible_v<
                            __owned_type, std::initializer_list<U>>>>
  constexpr maybe_owned(std::in_place_type_t<T>, std::initializer_list<U> init)
      : storage_(std::in_place_type<__owned_type>, init) {}

  /**
   * @brief Rvalue constructor. Creates owned storage from an rvalue.
   *
   * When constructed from an rvalue, the container takes ownership of the data.
   *
   * @param t Rvalue reference to the data to own
   */
  template <typename U = T, typename = std::enable_if_t<!std::is_array_v<U>>>
  constexpr maybe_owned(T&& t) noexcept
      : storage_(std::in_place_type<__owned_type>, std::move(t)) {}

  /**
   * @brief Lvalue constructor. Creates reference storage from an lvalue.
   *
   * When constructed from an lvalue, the container holds a reference to the
   * external data.
   *
   * @param t Lvalue reference to external data
   */
  constexpr maybe_owned(T& t) noexcept
      : storage_(std::in_place_type<__reference_type>, t) {}

  constexpr maybe_owned(const maybe_owned& other) : storage_(other.storage_) {}

  constexpr maybe_owned(maybe_owned&& other) noexcept
      : storage_(std::move(other.storage_)) {}

  ~maybe_owned() noexcept = default;

  constexpr maybe_owned& operator=(const maybe_owned& other) {
    if (this != &other) {
      storage_ = other.storage_;
    }
    return *this;
  }

  constexpr maybe_owned& operator=(maybe_owned&& other) noexcept {
    std::swap(storage_, other.storage_);
    return *this;
  }

  template <typename U = T, typename = std::enable_if_t<!std::is_array_v<U>>>
  constexpr maybe_owned& operator=(T&& t) {
    storage_.template emplace<__owned_type>(std::move(t));
    return *this;
  }

  constexpr maybe_owned& operator=(T& t) noexcept {
    storage_.template emplace<__reference_type>(t);
    return *this;
  }

  /**
   * @brief Check if the container holds any value.
   *
   * @return true if the container holds a value (owned or referenced), false if
   * empty
   */
  constexpr bool has_value() const noexcept {
    return !std::holds_alternative<empty>(storage_);
  }

  /**
   * @brief Boolean conversion operator.
   *
   * @return true if the container holds a value, false otherwise
   */
  constexpr explicit operator bool() const noexcept { return has_value(); }

  /**
   * @brief Check if the container owns its data.
   *
   * @return true if the data is owned, false otherwise
   */
  constexpr bool is_owned() const noexcept {
    return std::holds_alternative<__owned_type>(storage_);
  }

  /**
   * @brief Check if the container holds a reference.
   *
   * @return true if the data is a reference, false otherwise
   */
  constexpr bool is_reference() const noexcept {
    return std::holds_alternative<__reference_type>(storage_);
  }

  /**
   * @brief Get a reference to the stored value.
   *
   * @return Reference to the stored value
   * @throws std::bad_variant_access if the container is empty
   */
  constexpr T& get() & {
    if (auto* p = std::get_if<__owned_type>(&storage_)) {
      return __get_from_storage(*p);
    }
    return std::get<__reference_type>(storage_).get();
  }

  /**
   * @brief Get a const reference to the stored value.
   *
   * @return Const reference to the stored value
   * @throws std::bad_variant_access if the container is empty
   */
  constexpr const T& get() const& {
    if (auto* p = std::get_if<__owned_type>(&storage_)) {
      return __get_from_storage(*p);
    }
    return std::get<__reference_type>(storage_).get();
  }

  // Deleted rvalue-qualified overloads to prevent misuse
  constexpr T&& get() && = delete;
  constexpr const T&& get() const&& = delete;

  /**
   * @brief Dereference operator.
   *
   * @return Reference to the stored value
   */
  constexpr T& operator*() & { return get(); }

  /**
   * @brief Const dereference operator.
   *
   * @return Const reference to the stored value
   */
  constexpr const T& operator*() const& { return get(); }

  // Deleted rvalue-qualified overloads to prevent misuse
  constexpr T&& operator*() && = delete;
  constexpr const T&& operator*() const&& = delete;

  /**
   * @brief Member access operator.
   *
   * @return Pointer to the stored value
   */
  constexpr T* operator->() noexcept { return &get(); }

  /**
   * @brief Const member access operator.
   *
   * @return Const pointer to the stored value
   */
  constexpr const T* operator->() const noexcept { return &get(); }

  /**
   * @brief Reset the container to empty state.
   */
  constexpr void reset() noexcept { storage_.template emplace<empty>(); }

  /**
   * @brief Swap contents with another maybe_owned.
   *
   * @param other The other maybe_owned to swap with
   */
  constexpr void swap(maybe_owned& other) noexcept {
    using std::swap;
    swap(storage_, other.storage_);
  }

private:
  // Helper to convert std::array storage back to C array reference when T is an
  // array
  template <typename Storage>
  static constexpr T& __get_from_storage(Storage& s) noexcept {
    if constexpr (std::is_array_v<T>) {
      return reinterpret_cast<T&>(s);
    } else {
      return s;
    }
  }

  template <typename Storage>
  static constexpr const T& __get_from_storage(const Storage& s) noexcept {
    if constexpr (std::is_array_v<T>) {
      return reinterpret_cast<const T&>(s);
    } else {
      return s;
    }
  }

  storage_type storage_;
};

// CTAD deduction guides
template <typename T>
maybe_owned(T&&) -> maybe_owned<T>;

template <typename T>
maybe_owned(T&) -> maybe_owned<T>;

/**
 * @brief Non-member swap function.
 *
 * @param lhs First maybe_owned to swap
 * @param rhs Second maybe_owned to swap
 */
template <typename T>
constexpr void swap(maybe_owned<T>& lhs, maybe_owned<T>& rhs) noexcept {
  lhs.swap(rhs);
}

} // namespace es

#endif // ESTD___CONTAINER_MAYBE_OWNED_H
