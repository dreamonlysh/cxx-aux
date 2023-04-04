// Copyright (c) 2023 guyuemeng
//
// Tony is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD_VOID_H
#define ESTD_VOID_H

namespace es {

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
  if constexpr (std::is_void_v<MayVoidT>) return t1;
  else                                    return t2;
}

}
#endif //ESTD_VOID_H
