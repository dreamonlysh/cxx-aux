#ifndef ESTD_CONDITIONAL_H
#define ESTD_CONDITIONAL_H
#include <type_traits>

namespace estd {

template<bool B, typename IfT, typename ThenT>
using conditional_t = std::conditional<B, IfT, ThenT>;

template <bool B,
  typename IfT, typename ThenT>
constexpr auto conditional_v(IfT&& t1, ThenT&& t2) {
  if constexpr (B) {
    return t1;
  } else {
    return t2;
  }
}

template <bool B1, bool B2,
  typename IfT, typename ThenIfT, typename ThenT>
constexpr auto conditional_v(IfT&& t1, ThenIfT&& t2, ThenT&& t3) {
  return conditional_v<B1>(t1, conditional_v<B2>(t2, t3));
}

}

#endif //ESTD_CONDITIONAL_H
