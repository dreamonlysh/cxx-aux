#ifndef ESTD_META_REMOVE_CVREF_H
#define ESTD_META_REMOVE_CVREF_H

#include <type_traits>

namespace es {

template <typename T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

} // namespace es

#endif // ESTD_META_REMOVE_CVREF_H
