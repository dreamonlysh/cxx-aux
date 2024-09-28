// Copyright (c) 2023 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD___UTILITY_DISABLED_COPY_MOV_H
#define ESTD___UTILITY_DISABLED_COPY_MOV_H

namespace es {

/// @brief Inherit as a base to disable copy
struct disabled_copy {
  disabled_copy() = default;
  disabled_copy(const disabled_copy&) = delete;
  disabled_copy& operator=(const disabled_copy&) = delete;
};

/// @brief Inherit as a base to disable move
struct disabled_move {
  disabled_move() = default;
  disabled_move(disabled_copy&&) = delete;
  disabled_move& operator=(disabled_copy&&) = delete;
};

/// @brief Inherit as a base to disable copy and move
struct disabled_copy_move : disabled_copy, disabled_move {
  disabled_copy_move() = default;
};

} // namespace es
#endif
