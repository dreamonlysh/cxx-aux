#ifndef ESTD_META_ARRAY_H
#define ESTD_META_ARRAY_H

#include <array>
#include <cstddef>
#include <type_traits>

#include <estd/__meta/remove_cvref.h>

namespace es {

/**
 * @brief Type trait to check if a type is a C-style array.
 *
 * @tparam T Type to check
 *
 * Example usage:
 * @code
 * static_assert(is_c_array_v<int[10]>);
 * static_assert(!is_c_array_v<std::array<int, 10>>);
 * static_assert(!is_c_array_v<int>);
 * @endcode
 */
template <typename T>
struct is_c_array : std::false_type {};

template <typename T, size_t N>
struct is_c_array<T[N]> : std::true_type {};

/**
 * @brief Helper variable template for is_c_array.
 * @tparam T Type to check
 */
template <typename T>
inline constexpr bool is_c_array_v = is_c_array<remove_cvref_t<T>>::value;

/**
 * @brief Type trait to check if a type is a std::array.
 *
 * @tparam T Type to check
 *
 * Example usage:
 * @code
 * static_assert(is_std_array_v<std::array<int, 10>>);
 * static_assert(!is_std_array_v<int[10]>);
 * static_assert(!is_std_array_v<int>);
 * @endcode
 */
template <typename T>
struct is_std_array : std::false_type {};

template <typename T, size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

/**
 * @brief Helper variable template for is_std_array.
 * @tparam T Type to check
 */
template <typename T>
inline constexpr bool is_std_array_v = is_std_array<remove_cvref_t<T>>::value;

namespace __impl_array_traits {

template <typename T>
struct array_traits_impl {
  using element_type = void;
  static constexpr size_t size = 0;
};

template <typename T, size_t N>
struct array_traits_impl<T[N]> {
  using element_type = T;
  static constexpr size_t size = N;
};

template <typename T, size_t N>
struct array_traits_impl<std::array<T, N>> {
  using element_type = T;
  static constexpr size_t size = N;
};

} // namespace __impl_array_traits

/**
 * @brief Type traits for array types.
 *
 * Provides information about array types (both C-style arrays and std::array):
 * - element_type: The type of elements in the array
 * - size: The number of elements in the array
 *
 * For non-array types, element_type is void and size is 0.
 *
 * @tparam T Type to analyze
 *
 * Example usage:
 * @code
 * using traits1 = array_traits<int[10]>;
 * static_assert(std::is_same_v<traits1::element_type, int>);
 * static_assert(traits1::size == 10);
 *
 * using traits2 = array_traits<std::array<double, 5>>;
 * static_assert(std::is_same_v<traits2::element_type, double>);
 * static_assert(traits2::size == 5);
 *
 * using traits3 = array_traits<int>;
 * static_assert(std::is_same_v<traits3::element_type, void>);
 * static_assert(traits3::size == 0);
 * @endcode
 */
template <typename T>
struct array_traits
    : __impl_array_traits::array_traits_impl<remove_cvref_t<T>> {};

namespace __impl_to_std_array {

template <typename T>
struct to_std_array_impl {
  using type = T;
};

template <typename T, size_t N>
struct to_std_array_impl<T[N]> {
  using type = std::array<T, N>;
};

} // namespace __impl_to_std_array

/**
 * @brief Type trait that converts C-style arrays to std::array.
 *
 * For C-style array types T[N], provides std::array<T, N> as the type.
 * For all other types, leaves the type unchanged.
 *
 * @tparam T Type to potentially convert
 *
 * Example usage:
 * @code
 * using type1 = to_std_array_t<int[10]>;        // std::array<int, 10>
 * using type2 = to_std_array_t<double[5]>;      // std::array<double, 5>
 * using type3 = to_std_array_t<std::string>;    // std::string (unchanged)
 * @endcode
 */
template <typename T>
struct to_std_array
    : __impl_to_std_array::to_std_array_impl<remove_cvref_t<T>> {};

/**
 * @brief Helper type for to_std_array.
 * @tparam T Type to potentially convert
 */
template <typename T>
using to_std_array_t = typename to_std_array<T>::type;

} // namespace es

#endif // ESTD_META_ARRAY_H
