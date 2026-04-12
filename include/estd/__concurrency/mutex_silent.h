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

#ifndef ESTD__CONCURRENCY_MUTEX_SILENT_H
#define ESTD__CONCURRENCY_MUTEX_SILENT_H

namespace es {

/**
 * @brief A no-op mutex that provides the mutex interface without actual
 * locking.
 *
 * This class implements the Mutex concept but performs no synchronization.
 * It's useful in scenarios where:
 * - Synchronization is not needed but the code expects a mutex interface
 * - Testing single-threaded code that uses mutex templates
 * - Providing a default "no-lock" behavior in generic code
 *
 * All operations are no-ops:
 * - lock(): Does nothing
 * - unlock(): Does nothing
 * - try_lock(): Always returns false
 *
 * Example usage:
 * @code
 * // Use in generic code that requires a mutex
 * template<typename Mutex = mutex_silent>
 * class ThreadSafeCounter {
 *     Mutex mutex_;
 *     int count_ = 0;
 * public:
 *     void increment() {
 *         std::lock_guard<Mutex> lock(mutex_);
 *         ++count_;
 *     }
 * };
 *
 * // Single-threaded usage (no actual locking)
 * ThreadSafeCounter<> counter1;
 *
 * // Multi-threaded usage (real locking)
 * ThreadSafeCounter<std::mutex> counter2;
 * @endcode
 */
class mutex_silent {
public:
  /**
   * @brief "Acquires" the lock (no-op).
   *
   * This function does nothing and returns immediately.
   */
  void lock() {}

  /**
   * @brief "Attempts" to acquire the lock (always fails).
   *
   * This function always returns false, indicating that the lock
   * could not be acquired.
   *
   * @return Always returns false
   */
  bool try_lock() { return false; }

  /**
   * @brief "Releases" the lock (no-op).
   *
   * This function does nothing.
   */
  void unlock() {}
};

/**
 * @brief Default global instance of mutex_silent.
 *
 * This provides a convenient default mutex instance for use in
 * generic code that expects a mutex reference.
 */
inline mutex_silent default_mutex_silent;

} // namespace es
#endif
