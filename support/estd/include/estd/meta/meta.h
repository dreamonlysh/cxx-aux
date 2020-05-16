#ifndef ESTD_META_H
#define ESTD_META_H
#include <type_traits>

namespace es {

template <typename T, typename U>
struct meta_and
  : std::bool_constant<static_cast<bool>(T::value)
                       && static_cast<bool>(U::value)> {};

template <typename T, typename U>
using meta_and_t = typename meta_and<T, U>::type;

template <typename T, typename U>
struct meta_or
  : std::bool_constant<static_cast<bool>(T::value)
                       || static_cast<bool>(U::value)> {};

template <typename T, typename U>
using meta_or_t = typename meta_or<T, U>::type;

template <typename T>
struct meta_not : std::bool_constant<!static_cast<bool>(T::value)> {};

template <typename T>
using meta_not_t = typename meta_not<T>::type;

}
#endif //ESTD_META_H
