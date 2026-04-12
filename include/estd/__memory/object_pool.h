// Copyright (c) 2024 guyuemeng
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

#ifndef ESTD_MEMORY_OBJECT_POOL_H
#define ESTD_MEMORY_OBJECT_POOL_H
#include "memory_pool_uniform.h"
#include <estd/__bit/bit_pow2.h>
#include <estd/__concurrency/is_mutex_like.h>
#include <estd/__concurrency/mutex_silent.h>
#include <memory>

namespace es { namespace memory {

/**
 * @brief An object pool for efficient allocation and reuse of objects.
 *
 * This class template provides a memory pool optimized for allocating objects
 * of a single type. It uses MemoryPoolUniform for underlying memory management,
 * providing efficient allocation with minimal overhead.
 *
 * Key features:
 * - Type-safe object allocation
 * - Memory reuse through pooling
 * - Support for object construction with arguments
 * - Automatic alignment handling
 * - Manual object destruction and memory return to pool
 *
 * @tparam T The type of objects to pool
 * @tparam Align Alignment for objects in memory (default: alignof(T))
 * @tparam BlockSize Buffer size for each memory block (default: 4KB)
 *
 * @note Objects are constructed in-place using placement new
 * @note Destroyed objects' memory is returned to the pool for reuse
 * @note Thread-unsafe; use SubObjectPool for thread-local pools
 *
 * Example usage:
 * @code
 * struct MyObject {
 *     int x, y;
 *     MyObject(int a, int b) : x(a), y(b) {}
 * };
 *
 * ObjectPool<MyObject> pool;
 *
 * // Allocate and construct an object
 * MyObject* obj1 = pool.operator_new(10, 20);
 *
 * // Use the object
 * std::cout << obj1->x << ", " << obj1->y << std::endl;
 *
 * // Destroy and return to pool
 * pool.operator_delete(obj1);
 *
 * // Or use make_unique for RAII-style management
 * auto obj2 = make_unique(pool, 30, 40);
 * // obj2 is automatically deleted when it goes out of scope
 * @endcode
 */
template <typename T, size_t Align = std::alignment_of_v<T>,
          size_t BlockSize = 4 * 1024>
class ObjectPool : disabled_copy_move {
public:
  static_assert(is_pow2(Align), "The alignment should be pow of 2");

  using object_type = T;

  /**
   * @brief Bytes of a memory block.
   */
  static constexpr size_t block_size = BlockSize;

  /**
   * @brief Bytes of an object in memory (aligned).
   *
   * This is the actual size allocated for each object, rounded up
   * to the nearest alignment boundary.
   */
  static constexpr size_t object_alloc_size =
      (sizeof(T) + (Align - 1)) & (~(Align - 1));

  /**
   * @brief Creates a new object in the pool.
   *
   * Allocates memory from the underlying memory pool and constructs
   * the object in-place using placement new.
   *
   * @tparam Args Constructor argument types
   * @param args Arguments to forward to T's constructor
   * @return Pointer to the newly created object
   */
  template <typename... Args>
  T* operator_new(Args&&... args) {
    void* p = fwp_.allocate();
    return new (p) T(std::forward<Args>(args)...);
  }

  /**
   * @brief Destroys an object and returns its memory to the pool.
   *
   * Calls the object's destructor and returns the memory to the
   * underlying memory pool for reuse.
   *
   * @param p Pointer to the object to destroy
   *
   * @warning p must have been allocated from this pool
   * @warning p must not be null
   */
  void operator_delete(T* p) {
    std::destroy_at(p);
    fwp_.deallocate(p);
  }

  /**
   * @brief Merges another pool's memory into this pool.
   *
   * Transfers all memory blocks from another pool to this pool.
   * The other pool is left empty after the merge.
   *
   * @param other The pool to merge from
   *
   * @warning other must not be used after merge
   */
  void merge(ObjectPool& other) { fwp_.merge(other.fwp_); }

private:
  MemoryPoolUniform<object_alloc_size, block_size> fwp_;
};

/**
 * @brief Helper to create an object in pool with RAII-style management.
 *
 * Creates an object in the pool and returns a unique_ptr with a custom
 * deleter that returns the object to the pool when destroyed.
 *
 * @tparam T Concrete ObjectPool type
 * @tparam Args Constructor argument types
 * @param pool Pool to allocate the object from
 * @param args Arguments to forward to the object's constructor
 * @return unique_ptr to the object with pool-aware deleter
 *
 * Example usage:
 * @code
 * ObjectPool<MyObject> pool;
 * auto obj = make_unique(pool, 10, 20);
 * // obj is automatically returned to pool when it goes out of scope
 * @endcode
 */
template <typename T, typename... Args>
auto make_unique(T& pool, Args&&... args) {
  auto* p = pool.operator_new(std::forward<Args>(args)...);
  auto deleter = [&pool](auto* p) { pool.operator_delete(p); };
  using object_type = typename T::object_type;
  return std::unique_ptr<object_type, decltype(deleter)>(p, deleter);
}

/**
 * @brief A thread-local object pool that merges back to a parent pool.
 *
 * This class provides a thread-local object pool that automatically
 * merges its memory back to a parent pool upon destruction. This enables
 * efficient thread-local allocation while ensuring memory is not leaked.
 *
 * Key features:
 * - Thread-local allocation without contention
 * - Automatic merge back to parent on destruction
 * - Thread-safe merge operation with optional mutex
 *
 * @tparam T The type of objects to pool
 * @tparam Align Alignment for objects in memory
 * @tparam BlockSize Buffer size for each memory block
 * @tparam LockerT Mutex type for thread-safe merge (default: mutex_silent)
 *
 * @note The parent pool must outlive all sub pools
 * @note merge() is disabled to prevent accidental misuse
 *
 * Example usage:
 * @code
 * ObjectPool<MyObject> parent_pool;
 * std::mutex merge_mutex;
 *
 * void thread_function() {
 *     // Create thread-local sub pool
 *     SubObjectPool<MyObject, alignof(MyObject), 4096, std::mutex>
 *         sub_pool(parent_pool, merge_mutex);
 *
 *     // Allocate objects locally
 *     auto obj = sub_pool.operator_new(10, 20);
 *
 *     // Use objects...
 *
 *     // sub_pool automatically merges back to parent_pool on destruction
 * }
 * @endcode
 */
template <typename T, size_t Align, size_t BlockSize,
          typename LockerT = mutex_silent,
          typename = std::enable_if_t<is_mutex_like_v<LockerT>>>
class SubObjectPool : public ObjectPool<T, Align, BlockSize> {
  using object_pool_type = ObjectPool<T, Align, BlockSize>;

public:
  /**
   * @brief Constructs a sub pool with silent merge.
   *
   * Uses mutex_silent for merge, suitable for single-threaded scenarios.
   *
   * @param parent The parent pool to merge back to
   */
  SubObjectPool(object_pool_type& parent)
      : parent_(parent), locker_(default_mutex_silent) {}

  /**
   * @brief Constructs a sub pool with mutex-protected merge.
   *
   * Uses the provided mutex for thread-safe merge operation.
   *
   * @param parent The parent pool to merge back to
   * @param locker Mutex to use for merge synchronization
   */
  SubObjectPool(object_pool_type& parent, LockerT& locker)
      : parent_(parent), locker_(locker) {}

  /**
   * @brief Destructs the sub pool and merges back to parent.
   *
   * Locks the mutex, merges all memory to the parent pool,
   * then unlocks the mutex.
   */
  ~SubObjectPool() noexcept {
    locker_.lock();
    parent_.merge(*this);
    locker_.unlock();
  }

  /**
   * @brief Deleted to prevent accidental misuse.
   *
   * Sub pools should only merge back to their parent on destruction.
   */
  void merge(object_pool_type& other) = delete;

private:
  object_pool_type& parent_;
  LockerT& locker_;
};

/// @brief Helper to make a sub object pool from an object pool
template <typename T, size_t Align, size_t BlockSize>
SubObjectPool(ObjectPool<T, Align, BlockSize>)
    -> SubObjectPool<T, Align, BlockSize>;

/// @brief Helper to make a sub object pool from an object pool
template <typename T, size_t Align, size_t BlockSize, typename LockerT>
SubObjectPool(ObjectPool<T, Align, BlockSize>, LockerT)
    -> SubObjectPool<T, Align, BlockSize, LockerT>;

}} // namespace es::memory
#endif
