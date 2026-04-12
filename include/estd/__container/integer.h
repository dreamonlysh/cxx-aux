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

#ifndef ESTD___CONTAINER_INTEGER_H
#define ESTD___CONTAINER_INTEGER_H
#include <type_traits>

namespace es {

/**
 * @brief A type-safe integer wrapper with unique type identity.
 *
 * This class template provides strong type safety for integers, preventing
 * accidental mixing of different integer types (e.g., indices, bytes, bits).
 * Each instantiation with a different Tag creates a unique type that cannot
 * be implicitly converted to other Integer types.
 *
 * Key features:
 * - Strong type safety with unique type identity
 * - Full arithmetic operator support
 * - Comparison operators
 * - Implicit conversion to underlying type
 * - Zero runtime overhead (wrapper is optimized away)
 *
 * @tparam T Underlying integer type
 * @tparam Tag Type tag for creating unique types (default void allows
 *             direct use)
 *
 * Example usage:
 * @code
 * using Index = Integer<size_t>;
 * using Byte = Integer<size_t, struct ByteTag>;
 *
 * Index i = 10;
 * Byte b = 20;
 * // i = b; // Compile error - different types
 * // auto sum = i + b; // Compile error - cannot mix types
 * @endcode
 */
template <typename T, typename = void,
          typename = std::enable_if_t<std::is_integral_v<T>>>
class Integer {
public:
  using value_type = T;

  /**
   * @brief Default constructor. Initializes value to 0.
   */
  constexpr Integer() = default;

  /**
   * @brief Constructor from a value.
   * @param v Initial value
   */
  constexpr explicit Integer(value_type v) : data(v) {}

  ~Integer() noexcept = default;

  constexpr Integer(const Integer& other) = default;
  constexpr Integer(Integer&& other) noexcept = default;
  constexpr Integer& operator=(const Integer& other) = default;
  constexpr Integer& operator=(Integer&& other) noexcept = default;

  /**
   * @brief Resets the value.
   * @param v New value (default 0)
   */
  constexpr void reset(value_type v = 0) { data = v; }

  /**
   * @brief Swaps values with another Integer.
   * @param other Integer to swap with
   */
  constexpr void swap(Integer& other) noexcept { std::swap(data, other.data); }

  /**
   * @brief Gets the underlying value.
   * @return The underlying integer value
   */
  constexpr value_type value() const noexcept { return data; }

  /**
   * @brief Implicit conversion to the underlying type.
   * @return The underlying integer value
   */
  constexpr explicit operator T() const noexcept { return data; }

  /// @defgroup operator_assignment
  /// Assignment operators
  /// @{

  /**
   * @brief Assignment from underlying type value.
   * @param v Value to assign
   * @return Reference to this
   */
  Integer& operator=(value_type v) {
    data = v;
    return *this;
  }

#define __INTEGER_ASSIGNMENT_OPERTOR(op)                                       \
  template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>    \
  constexpr Integer& operator op(const U & v) {                                \
    data op v;                                                                 \
    return *this;                                                              \
  }                                                                            \
                                                                               \
  constexpr Integer& operator op(const Integer & v) {                          \
    data op v.data;                                                            \
    return *this;                                                              \
  }

  __INTEGER_ASSIGNMENT_OPERTOR(+=)
  __INTEGER_ASSIGNMENT_OPERTOR(-=)
  __INTEGER_ASSIGNMENT_OPERTOR(*=)
  __INTEGER_ASSIGNMENT_OPERTOR(/=)
  __INTEGER_ASSIGNMENT_OPERTOR(%=)
  __INTEGER_ASSIGNMENT_OPERTOR(&=)
  __INTEGER_ASSIGNMENT_OPERTOR(|=)
  __INTEGER_ASSIGNMENT_OPERTOR(^=)
  __INTEGER_ASSIGNMENT_OPERTOR(<<=)
  __INTEGER_ASSIGNMENT_OPERTOR(>>=)

  /// @}

  /// @defgroup operator_incdec
  /// Increment/decrement operators
  /// @{

  constexpr Integer& operator++() noexcept {
    ++data;
    return *this;
  }

  constexpr Integer operator++(int) {
    auto tmp = data;
    this->operator++();
    return Integer(tmp);
  }

  constexpr Integer& operator--() noexcept {
    --data;
    return *this;
  }

  constexpr Integer operator--(int) {
    auto tmp = data;
    this->operator--();
    return Integer(tmp);
  }

  /// @}

private:
  value_type data = 0;
};

/// @defgroup operator_comparison
/// Comparison operators
/// @{

#define __INTEGER_COMPARISION_OPERTOR(op)                                      \
  template <typename T, typename Tag>                                          \
  constexpr bool operator op(const Integer<T, Tag>& lhs,                       \
                             const Integer<T, Tag>& rhs) {                     \
    return lhs.value() op rhs.value();                                         \
  }                                                                            \
                                                                               \
  template <typename T, typename Tag, typename U,                              \
            typename = std::enable_if_t<std::is_integral_v<U>>>                \
  constexpr bool operator op(const Integer<T, Tag>& lhs, const U& rhs) {       \
    return lhs.value() op rhs;                                                 \
  }                                                                            \
                                                                               \
  template <typename T, typename Tag, typename U,                              \
            typename = std::enable_if_t<std::is_integral_v<U>>>                \
  constexpr bool operator op(const U& lhs, const Integer<T, Tag>& rhs) {       \
    return lhs op rhs.value();                                                 \
  }

__INTEGER_COMPARISION_OPERTOR(==)
__INTEGER_COMPARISION_OPERTOR(!=)
__INTEGER_COMPARISION_OPERTOR(<)
__INTEGER_COMPARISION_OPERTOR(>)
__INTEGER_COMPARISION_OPERTOR(<=)
__INTEGER_COMPARISION_OPERTOR(>=)

/// @}

/// @defgroup operator_arithmetic
/// Arithmetic operators
/// @{

#define __INTEGER_ARITHMETIC_UNARY_OPERTOR(op)                                 \
  template <typename T, typename Tag>                                          \
  constexpr Integer<T, Tag> operator op(const Integer<T, Tag>& v) {            \
    return Integer<T, Tag>{op(v.value())};                                     \
  }

#define __INTEGER_ARITHMETIC_BINARY_OPERTOR_II(op)                             \
  template <typename T, typename Tag>                                          \
  constexpr Integer<T, Tag> operator op(const Integer<T, Tag>& lhs,            \
                                        const Integer<T, Tag>& rhs) {          \
    return Integer<T, Tag>{lhs.value() op rhs.value()};                        \
  }

#define __INTEGER_ARITHMETIC_BINARY_OPERTOR_IV(op)                             \
  template <typename T, typename Tag, typename U,                              \
            typename = std::enable_if_t<std::is_integral_v<U>>>                \
  constexpr Integer<T, Tag> operator op(const Integer<T, Tag>& lhs,            \
                                        const U & rhs) {                       \
    auto tmp = lhs.value() op rhs;                                             \
    static_assert(                                                             \
        std::is_same_v<decltype(tmp), T>,                                      \
        "the rhs type should be safely convertable to the lhs value type");    \
    return Integer<T, Tag>{tmp};                                               \
  }

#define __INTEGER_ARITHMETIC_BINARY_OPERTOR_VI2I(op)                           \
  template <typename T, typename Tag, typename U,                              \
            typename = std::enable_if_t<std::is_integral_v<U>>>                \
  constexpr Integer<T, Tag> operator op(const U & lhs,                         \
                                        const Integer<T, Tag>& rhs) {          \
    auto tmp = lhs op rhs.value();                                             \
    static_assert(                                                             \
        std::is_same_v<decltype(tmp), T>,                                      \
        "the lhs type should be safely convertable to the rhs value type");    \
    return Integer<T, Tag>{tmp};                                               \
  }

#define __INTEGER_ARITHMETIC_BINARY_OPERTOR_VI2V(op)                           \
  template <typename T, typename Tag, typename U,                              \
            typename = std::enable_if_t<std::is_integral_v<U>>>                \
  constexpr U operator op(const U& lhs, const Integer<T, Tag>& rhs) {          \
    return lhs op rhs.value();                                                 \
  }

#define __INTEGER_ARITHMETIC_BINARY_OPERTOR(op)                                \
  __INTEGER_ARITHMETIC_BINARY_OPERTOR_II(op)                                   \
  __INTEGER_ARITHMETIC_BINARY_OPERTOR_IV(op)                                   \
  __INTEGER_ARITHMETIC_BINARY_OPERTOR_VI2I(op)

__INTEGER_ARITHMETIC_UNARY_OPERTOR(+)
__INTEGER_ARITHMETIC_UNARY_OPERTOR(-)

__INTEGER_ARITHMETIC_BINARY_OPERTOR(+)
__INTEGER_ARITHMETIC_BINARY_OPERTOR(-)
__INTEGER_ARITHMETIC_BINARY_OPERTOR(*)
__INTEGER_ARITHMETIC_BINARY_OPERTOR(/)
__INTEGER_ARITHMETIC_BINARY_OPERTOR(%)

__INTEGER_ARITHMETIC_UNARY_OPERTOR(~)

__INTEGER_ARITHMETIC_BINARY_OPERTOR(&)
__INTEGER_ARITHMETIC_BINARY_OPERTOR(|)
__INTEGER_ARITHMETIC_BINARY_OPERTOR(^)

#define __INTEGER_ARITHMETIC_BINARY_OPERTOR_RETV(op)                           \
  __INTEGER_ARITHMETIC_BINARY_OPERTOR_II(op)                                   \
  __INTEGER_ARITHMETIC_BINARY_OPERTOR_IV(op)                                   \
  __INTEGER_ARITHMETIC_BINARY_OPERTOR_VI2V(op)

__INTEGER_ARITHMETIC_BINARY_OPERTOR_RETV(<<)
__INTEGER_ARITHMETIC_BINARY_OPERTOR_RETV(>>)

/// @}

/// @defgroup operator_logic
/// Logic operators
/// @{

template <typename T, typename Tag>
constexpr bool operator!(const Integer<T, Tag>& v) {
  return !(v.value());
}

__INTEGER_COMPARISION_OPERTOR(&&)
__INTEGER_COMPARISION_OPERTOR(||)

/// @}

} // namespace es
#endif
