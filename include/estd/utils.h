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

#ifndef ESTD_UTILS_H
#define ESTD_UTILS_H

namespace es {

template <typename T>
constexpr bool not_null(T* p) {
  return p != nullptr;
}

struct disabled_copy_move {
  disabled_copy_move() = default;
  disabled_copy_move(const disabled_copy_move&) = delete;
  disabled_copy_move& operator=(const disabled_copy_move&) = delete;
  disabled_copy_move(disabled_copy_move&&) = delete;
  disabled_copy_move& operator=(disabled_copy_move&&) = delete;
};

}
#endif //ESTD_UTILS_H
