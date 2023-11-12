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

#ifndef ESTD_CAST_DYN_CAST_H
#define ESTD_CAST_DYN_CAST_H
#include "type_id.h"

namespace es {

template <typename BaseT>
class Object {
public:
  template <typename T>
  bool typeOf(T&&) const {
    static_assert(std::is_base_of_v<BaseT, T>);
    return type_id<T>() == id;
  }

protected:
  Object() = default;
  ~Object() noexcept = default;

private:
  size_t id;
};

} // namespace es

#endif // ESTD_CAST_DYN_CAST_H
