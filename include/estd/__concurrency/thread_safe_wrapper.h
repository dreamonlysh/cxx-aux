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

#ifndef ESTD__CONCURRENCY_THREAD_SAFE_WRAPPER_H
#define ESTD__CONCURRENCY_THREAD_SAFE_WRAPPER_H
#include "is_mutex_like.h"

namespace es {
namespace __impl {
template <typename LockerT>
struct ThreadSafeWrapperLockerGuard {
  ThreadSafeWrapperLockerGuard(LockerT& locker) : locker_(locker) {
    locker.lock();
  }
  ~ThreadSafeWrapperLockerGuard() noexcept { locker_.unlock(); }

  LockerT& locker_;
};
} // namespace __impl

/// @brief Wrapper of an object for thread safe access
///
/// each invoke starts with locker.lock() and ends with locker.unlock()
/// @tparam LockerT a mutex like locker
/// @tparam T object to lock
template <typename LockerT, typename T>
class ThreadSafeObjectWrapper {
  static_assert(is_mutex_like_v<LockerT>);

public:
  ThreadSafeObjectWrapper(T& obj, LockerT& locker)
      : obj_(obj), locker_(locker) {}
  ~ThreadSafeObjectWrapper() noexcept = default;

  template <typename FnT, typename... Args>
  auto invoke(FnT&& fn, Args&&... args) {
    static_assert(std::is_member_function_pointer_v<FnT>);

    __impl::ThreadSafeWrapperLockerGuard _(locker_);
    return std::invoke(std::forward<FnT>(fn), obj_,
                       std::forward<Args>(args)...);
  }

private:
  T& obj_;
  LockerT& locker_;
};

template <typename LockerT, typename T>
ThreadSafeObjectWrapper(T, LockerT&) -> ThreadSafeObjectWrapper<LockerT, T>;

/// @brief Wrapper of a member function binding an object for thread safe access
///
/// each call starts with locker.lock() and ends with locker.unlock()
/// @tparam LockerT a mutex like locker
/// @tparam T object binding to the member function
/// @tparam FnT function to lock
template <typename LockerT, typename T, typename FnT>
class ThreadSafeFunctionWrapper {
  static_assert(is_mutex_like_v<LockerT>);

public:
  ThreadSafeFunctionWrapper(T& obj, FnT fn, LockerT& locker)
      : obj_(obj), fn_(fn), locker_(locker) {
    static_assert(std::is_member_function_pointer_v<FnT>);
  }
  ~ThreadSafeFunctionWrapper() noexcept = default;

  template <typename... Args>
  auto operator()(Args&&... args) {
    __impl::ThreadSafeWrapperLockerGuard _(locker_);
    return std::invoke(fn_, obj_, std::forward<Args>(args)...);
  }

private:
  T& obj_;
  FnT fn_;
  LockerT& locker_;
};

/// @brief Wrapper of a function like call for thread safe access
///
/// each call starts with locker.lock() and ends with locker.unlock()
/// @tparam LockerT a mutex like locker
/// @tparam FnT function or its reference_wrapper to lock(by std::ref())
template <typename LockerT, typename FnT>
class ThreadSafeFunctionWrapper<LockerT, void, FnT> {
public:
  static_assert(is_mutex_like_v<LockerT>);

public:
  ThreadSafeFunctionWrapper(FnT fn, LockerT& locker)
      : fn_(std::forward<FnT>(fn)), locker_(locker) {}
  ~ThreadSafeFunctionWrapper() noexcept = default;

  template <typename... Args>
  auto operator()(Args&&... args) {
    __impl::ThreadSafeWrapperLockerGuard _(locker_);
    return std::invoke(fn_, std::forward<Args>(args)...);
  }

private:
  FnT fn_;
  LockerT& locker_;
};

template <typename LockerT, typename T, typename FnT>
ThreadSafeFunctionWrapper(T&, FnT, LockerT&)
    -> ThreadSafeFunctionWrapper<LockerT, T, FnT>;

template <typename LockerT, typename FnT>
ThreadSafeFunctionWrapper(FnT, LockerT&)
    -> ThreadSafeFunctionWrapper<LockerT, void, FnT>;

} // namespace es
#endif
