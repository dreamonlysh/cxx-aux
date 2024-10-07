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

/// @brief type unique integer
///
/// used to define units like index/byte/bit/metre/kilometre and so on
///
/// @tparam T integer type used to keep data
/// @tparam   unique type to make each Integer unique
template <typename T, typename = void,
          typename = std::enable_if_t<std::is_integral_v<T>>>
class Integer {
public:
  using value_type = T;

  constexpr Integer() = default;

  constexpr explicit Integer(value_type v) : data(v) {}

  constexpr Integer(const Integer& other) : data(other.data) {}

  constexpr Integer(Integer&& other) noexcept : data(other.data) {}

  ~Integer() noexcept = default;

  constexpr Integer& operator=(const Integer& other) {
    if (this != &other)
      data = other.data;
    return *this;
  }

  constexpr Integer& operator=(Integer&& other) noexcept {
    data = other.data;
    return *this;
  }

  constexpr void reset(value_type v = 0) { data = v; }

  constexpr void swap(Integer& other) noexcept { std::swap(data, other.data); }

  constexpr value_type value() const noexcept { return data; }

  constexpr explicit operator T() const noexcept { return data; }

  /// @defgroup operator_assignment
  /// Assignment operators
  /// @{

  Integer& operator=(value_type v) { data = v; }

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
