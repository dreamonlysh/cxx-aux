#ifndef ESTD_VOID_H
#define ESTD_VOID_H

namespace estd {

/// To replace void with VoidSubstituteT and none void with NoneVoidSubstituteT.
template <typename MayVoidT,
  typename VoidSubstituteT, typename NoneVoidSubstituteT = MayVoidT>
struct substitute_void {
  using type = std::conditional_t<std::is_void_v<MayVoidT>,
  VoidSubstituteT, NoneVoidSubstituteT>;
};
template <typename MayVoidT,
  typename VoidSubstituteT, typename NoneVoidSubstituteT = MayVoidT>
using substitute_void_t = typename substitute_void<MayVoidT,
                                                   VoidSubstituteT,
                                                   NoneVoidSubstituteT>::type;

/// Choose the initialization value for substitute_void_t.
template <typename MayVoidT,
  typename VoidSubstituteT, typename NoneVoidSubstituteT>
constexpr auto substitute_void_v(VoidSubstituteT&& t1,
                                 NoneVoidSubstituteT&& t2) {
  if constexpr (std::is_void_v<MayVoidT>) {
    return t1;
  } else {
    return t2;
  }
}

}
#endif //ESTD_VOID_H
