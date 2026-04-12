#ifndef ESTD_META_REMOVE_CVREF_H
#define ESTD_META_REMOVE_CVREF_H

#include <type_traits>

namespace es {

/**
 * @brief Removes cv-qualifiers and references from a type.
 *
 * This type trait combines the effects of std::remove_cv and
 * std::remove_reference, removing both const/volatile qualifiers and references
 * (both lvalue and rvalue) from the given type.
 *
 * @tparam T Type to strip
 *
 * Example usage:
 * @code
 * using T1 = remove_cvref_t<const int&>;      // T1 is int
 * using T2 = remove_cvref_t<volatile int&&>;  // T2 is int
 * using T3 = remove_cvref_t<int*>;            // T3 is int* (pointers
 * unchanged) using T4 = remove_cvref_t<const int[10]>;   // T4 is int[10]
 *
 * // Useful in perfect forwarding
 * template<typename T>
 * void process(T&& value) {
 *     using CleanType = remove_cvref_t<T>;
 *     // CleanType is the unqualified, non-reference type
 * }
 * @endcode
 */
template <typename T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

/**
 * @brief Helper type for remove_cvref.
 * @tparam T Type to strip
 */
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

} // namespace es

#endif // ESTD_META_REMOVE_CVREF_H
