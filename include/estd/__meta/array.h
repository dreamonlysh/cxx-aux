#ifndef ESTD_META_ARRAY_H
#define ESTD_META_ARRAY_H

#include <array>
#include <cstddef>
#include <type_traits>

#include <estd/__meta/remove_cvref.h>

namespace es {

template <typename T>
struct is_c_array : std::false_type {};

template <typename T, size_t N>
struct is_c_array<T[N]> : std::true_type {};

template <typename T>
inline constexpr bool is_c_array_v = is_c_array<remove_cvref_t<T>>::value;

template <typename T>
struct is_std_array : std::false_type {};

template <typename T, size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

template <typename T>
inline constexpr bool is_std_array_v = is_std_array<remove_cvref_t<T>>::value;

template <typename T>
struct array_traits {
  using element_type = void;
  static constexpr size_t size = 0;
};

template <typename T, size_t N>
struct array_traits<T[N]> {
  using element_type = T;
  static constexpr size_t size = N;
};

template <typename T, size_t N>
struct array_traits<std::array<T, N>> {
  using element_type = T;
  static constexpr size_t size = N;
};

template <typename T>
struct array_traits<T&> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<const T&> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<T&&> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<const T> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<volatile T> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<const volatile T> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<volatile T&> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<const volatile T&> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<const T&&> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<volatile T&&> : array_traits<remove_cvref_t<T>> {};

template <typename T>
struct array_traits<const volatile T&&> : array_traits<remove_cvref_t<T>> {};

} // namespace es

#endif // ESTD_META_ARRAY_H
