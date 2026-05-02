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

/**
 * @brief Base class to disable copy operations.
 *
 * Inherit from this class to make derived classes non-copyable.
 * Move operations remain enabled.
 *
 * Example usage:
 * @code
 * class MyClass : private disabled_copy {
 *     // Copy constructor and copy assignment are deleted
 *     // Move constructor and move assignment are still available
 * };
 * @endcode
 */
struct disabled_copy {
  disabled_copy() = default;
  disabled_copy(const disabled_copy&) = delete;
  disabled_copy& operator=(const disabled_copy&) = delete;
};

/**
 * @brief Base class to disable move operations.
 *
 * Inherit from this class to make derived classes non-movable.
 * Copy operations remain enabled.
 *
 * Example usage:
 * @code
 * class MyClass : private disabled_move {
 *     // Move constructor and move assignment are deleted
 *     // Copy constructor and copy assignment are still available
 * };
 * @endcode
 */
struct disabled_move {
  disabled_move() = default;
  disabled_move(disabled_move&&) = delete;
  disabled_move& operator=(disabled_move&&) = delete;
};

/**
 * @brief Base class to disable both copy and move operations.
 *
 * Inherit from this class to make derived classes neither copyable nor movable.
 * This is useful for types that manage unique resources or have
 * non-transferable state.
 *
 * Example usage:
 * @code
 * class UniqueResource : private disabled_copy_move {
 *     // Neither copy nor move operations are available
 * };
 * @endcode
 */
struct disabled_copy_move : disabled_copy, disabled_move {
  disabled_copy_move() = default;
};

} // namespace es
#endif
