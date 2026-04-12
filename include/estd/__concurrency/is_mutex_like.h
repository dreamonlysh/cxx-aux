// Copyright (c) 2024 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD__CONCURRENCY_IS_MUTEX_LIKE_H
#define ESTD__CONCURRENCY_IS_MUTEX_LIKE_H
#include <estd/__meta/has_member.h>

namespace es {
namespace __impl_is_mutex_like {
META_HAS_MEMBER_FUNCTION(lock);
META_HAS_MEMBER_FUNCTION(unlock);
META_HAS_MEMBER_FUNCTION(try_lock);
} // namespace __impl_is_mutex_like

/**
 * @brief Type trait to check if a type satisfies the Mutex concept.
 *
 * A type is considered mutex-like if it provides the following member
 * functions:
 * - lock(): Acquires the lock
 * - unlock(): Releases the lock
 * - try_lock(): Attempts to acquire the lock without blocking
 *
 * This trait is useful for template metaprogramming to ensure that a type
 * can be used as a mutex in synchronization contexts.
 *
 * @tparam T The type to check
 *
 * Example usage:
 * @code
 * static_assert(is_mutex_like_v<std::mutex>);
 * static_assert(is_mutex_like_v<std::shared_mutex>);
 * static_assert(!is_mutex_like_v<int>);
 *
 * template<typename Mutex>
 * std::enable_if_t<is_mutex_like_v<Mutex>> lock_and_do_work(Mutex& m) {
 *     std::lock_guard<Mutex> lock(m);
 *     // do work
 * }
 * @endcode
 */
template <typename T>
struct is_mutex_like
    : std::bool_constant<__impl_is_mutex_like::has_member_lock_v<T> &&
                         __impl_is_mutex_like::has_member_unlock_v<T> &&
                         __impl_is_mutex_like::has_member_try_lock_v<T>> {};

/**
 * @brief Helper variable template for is_mutex_like.
 * @tparam T The type to check
 */
template <typename T>
constexpr bool is_mutex_like_v = is_mutex_like<T>::value;

} // namespace es
#endif
