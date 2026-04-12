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
namespace __impl_thread_safe_wrapper {
template <typename LockerT>
struct ThreadSafeWrapperLockerGuard {
  ThreadSafeWrapperLockerGuard(LockerT& locker) : locker_(locker) {
    locker.lock();
  }
  ~ThreadSafeWrapperLockerGuard() noexcept { locker_.unlock(); }

  LockerT& locker_;
};
} // namespace __impl_thread_safe_wrapper

/**
 * @brief Wrapper for thread-safe object method invocation.
 *
 * This class template provides thread-safe access to an object's methods
 * by automatically locking a mutex before each invocation and unlocking
 * after completion.
 *
 * Key features:
 * - Automatic mutex locking/unlocking
 * - RAII-style lock management
 * - Support for member function invocation with arguments
 *
 * @tparam LockerT Mutex type (must satisfy is_mutex_like)
 * @tparam T Object type to wrap
 *
 * @note Each invoke() call locks and unlocks the mutex
 * @note The mutex must outlive the wrapper
 *
 * Example usage:
 * @code
 * class Counter {
 * public:
 *     void increment() { ++count_; }
 *     int get() const { return count_; }
 * private:
 *     int count_ = 0;
 * };
 *
 * Counter counter;
 * std::mutex mtx;
 * ThreadSafeObjectWrapper<std::mutex, Counter> wrapper(counter, mtx);
 *
 * // Thread-safe invocation
 * wrapper.invoke(&Counter::increment);
 * int value = wrapper.invoke(&Counter::get);
 * @endcode
 */
template <typename LockerT, typename T>
class ThreadSafeObjectWrapper {
  static_assert(is_mutex_like_v<LockerT>);

public:
  /**
   * @brief Constructs a thread-safe wrapper.
   *
   * @param obj Reference to the object to wrap
   * @param locker Reference to the mutex for synchronization
   */
  ThreadSafeObjectWrapper(T& obj, LockerT& locker)
      : obj_(obj), locker_(locker) {}
  ~ThreadSafeObjectWrapper() noexcept = default;

  /**
   * @brief Invokes a member function with automatic locking.
   *
   * Locks the mutex, invokes the member function, then unlocks.
   * The lock is held for the duration of the invocation.
   *
   * @tparam FnT Member function pointer type
   * @tparam Args Argument types
   * @param fn Member function to invoke
   * @param args Arguments to forward to the function
   * @return Result of the function call
   *
   * @note FnT must be a member function pointer
   */
  template <typename FnT, typename... Args>
  auto invoke(FnT&& fn, Args&&... args) {
    static_assert(std::is_member_function_pointer_v<FnT>);

    __impl_thread_safe_wrapper::ThreadSafeWrapperLockerGuard _(locker_);
    return std::invoke(std::forward<FnT>(fn), obj_,
                       std::forward<Args>(args)...);
  }

private:
  T& obj_;
  LockerT& locker_;
};

template <typename LockerT, typename T>
ThreadSafeObjectWrapper(T, LockerT&) -> ThreadSafeObjectWrapper<LockerT, T>;

/**
 * @brief Wrapper for thread-safe member function binding.
 *
 * This class template binds an object and a member function together,
 * providing a callable object that automatically locks a mutex before
 * each call and unlocks after completion.
 *
 * Key features:
 * - Binds object and member function
 * - Automatic mutex locking/unlocking
 * - Callable like a regular function
 *
 * @tparam LockerT Mutex type (must satisfy is_mutex_like)
 * @tparam T Object type (use void for free functions)
 * @tparam FnT Function type (member function pointer or callable)
 *
 * @note Each operator() call locks and unlocks the mutex
 * @note The mutex must outlive the wrapper
 *
 * Example usage:
 * @code
 * class Counter {
 * public:
 *     void increment() { ++count_; }
 * private:
 *     int count_ = 0;
 * };
 *
 * Counter counter;
 * std::mutex mtx;
 *
 * // Bind counter and increment method
 * ThreadSafeFunctionWrapper<std::mutex, Counter, decltype(&Counter::increment)>
 *     wrapper(counter, &Counter::increment, mtx);
 *
 * // Call like a function
 * wrapper();  // Thread-safe increment
 * @endcode
 */
template <typename LockerT, typename T, typename FnT>
class ThreadSafeFunctionWrapper {
  static_assert(is_mutex_like_v<LockerT>);

public:
  /**
   * @brief Constructs a thread-safe function wrapper.
   *
   * @param obj Reference to the object to bind
   * @param fn Member function pointer to bind
   * @param locker Reference to the mutex for synchronization
   *
   * @note FnT must be a member function pointer
   */
  ThreadSafeFunctionWrapper(T& obj, FnT fn, LockerT& locker)
      : obj_(obj), fn_(fn), locker_(locker) {
    static_assert(std::is_member_function_pointer_v<FnT>);
  }
  ~ThreadSafeFunctionWrapper() noexcept = default;

  /**
   * @brief Calls the bound function with automatic locking.
   *
   * Locks the mutex, calls the bound member function, then unlocks.
   *
   * @tparam Args Argument types
   * @param args Arguments to forward to the function
   * @return Result of the function call
   */
  template <typename... Args>
  auto operator()(Args&&... args) {
    __impl_thread_safe_wrapper::ThreadSafeWrapperLockerGuard _(locker_);
    return std::invoke(fn_, obj_, std::forward<Args>(args)...);
  }

private:
  T& obj_;
  FnT fn_;
  LockerT& locker_;
};

/**
 * @brief Specialization for free functions (T = void).
 *
 * This specialization wraps free functions or callable objects
 * (not bound to any object) with automatic mutex locking.
 *
 * @tparam LockerT Mutex type
 * @tparam FnT Function type (callable)
 *
 * Example usage:
 * @code
 * std::mutex mtx;
 *
 * // Wrap a lambda
 * auto unsafe_print = []() { std::cout << "Hello" << std::endl; };
 * ThreadSafeFunctionWrapper<std::mutex, void, decltype(unsafe_print)>
 *     safe_print(unsafe_print, mtx);
 *
 * safe_print();  // Thread-safe print
 * @endcode
 */
template <typename LockerT, typename FnT>
class ThreadSafeFunctionWrapper<LockerT, void, FnT> {
public:
  static_assert(is_mutex_like_v<LockerT>);

public:
  /**
   * @brief Constructs a thread-safe function wrapper.
   *
   * @param fn Callable to wrap
   * @param locker Reference to the mutex for synchronization
   */
  ThreadSafeFunctionWrapper(FnT fn, LockerT& locker)
      : fn_(std::forward<FnT>(fn)), locker_(locker) {}
  ~ThreadSafeFunctionWrapper() noexcept = default;

  /**
   * @brief Calls the wrapped function with automatic locking.
   *
   * Locks the mutex, calls the wrapped function, then unlocks.
   *
   * @tparam Args Argument types
   * @param args Arguments to forward to the function
   * @return Result of the function call
   */
  template <typename... Args>
  auto operator()(Args&&... args) {
    __impl_thread_safe_wrapper::ThreadSafeWrapperLockerGuard _(locker_);
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
