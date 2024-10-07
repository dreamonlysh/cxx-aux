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

#ifndef CXXAUX_POOL_OBJECT_POOL_H
#define CXXAUX_POOL_OBJECT_POOL_H
#include "memory_pool_uniform.h"
#include <estd/bit.h>
#include <memory>

namespace cxxaux {

/// @brief An object pool
/// @tparam T object type
/// @tparam Align alignment for the T to keep in memory
/// @tparam BlockSize buffer size designed for a block
template <typename T, size_t Align = std::alignment_of_v<T>,
          size_t BlockSize = 4 * 1024>
class ObjectPool : es::disabled_copy_move {
public:
  static_assert(es::is_pow2(Align), "The alignment should be pow of 2");

  using object_type = T;

  /// @brief Bytes of a memory block
  static constexpr size_t block_size = BlockSize;
  /// @brief Bytes of an object in memory
  static constexpr size_t object_alloc_size =
      (sizeof(T) + (Align - 1)) & (~(Align - 1));

  /// @brief Create an object in pool
  /// @tparam ...Args args type to construct the object
  /// @param ...args args to construct the object
  /// @return an object in pool
  template <typename... Args>
  T* operator_new(Args&&... args) {
    void* p = fwp_.allocate();
    return new (p) T(std::forward<Args>(args)...);
  }

  /// @brief Delete an object and release the memory to pool
  /// @param p an object in pool
  void operator_delete(T* p) {
    std::destroy_at(p);
    fwp_.deallocate(p);
  }

  /// @brief Merge the members from other to this
  /// @param other the pool to be moved and clean
  void merge(ObjectPool& other) { fwp_.merge(other.fwp_); }

private:
  MemoryPoolUniform<object_alloc_size, block_size> fwp_;
};

/// @brief Helper to create an object in pool like std::make_unique
/// @tparam ...Args args type to construct the object
/// @tparam T Concrete ObjectPool
/// @param pool pool to hold the object
/// @param ...args args to construct the object
/// @return an object in pool
template <typename T, typename... Args>
auto make_unique(T& pool, Args&&... args) {
  auto* p = pool.operator_new(std::forward<Args>(args)...);
  auto deleter = [&pool](auto* p) { pool.operator_delete(p); };
  using object_type = typename T::object_type;
  return std::unique_ptr<object_type, decltype(deleter)>(p, deleter);
}

namespace __impl {
struct SubObjectPoolDefaultLocker {
  void lock() {}
  void unlock() {}
};
inline SubObjectPoolDefaultLocker default_locker;
} // namespace __impl

/// @brief An object pool that belongs a main object pool
///
/// The purpose of the sub object pool is safe thread local
/// @tparam T Concrete ObjectPool
/// @tparam LockerT locker to make it safe to release the sub to main
/// @tparam Align alignment for the T to keep in memory
/// @tparam BlockSize buffer size designed for a block
template <typename T, size_t Align, size_t BlockSize,
          typename LockerT = __impl::SubObjectPoolDefaultLocker>
class SubObjectPool : public ObjectPool<T, Align, BlockSize> {
  using object_pool_type = ObjectPool<T, Align, BlockSize>;

public:
  SubObjectPool(object_pool_type& parent)
      : parent_(parent), locker_(__impl::default_locker) {}
  SubObjectPool(object_pool_type& parent, LockerT& locker)
      : parent_(parent), locker_(locker) {}
  ~SubObjectPool() noexcept {
    locker_.lock();
    parent_.merge(*this);
    locker_.unlock();
  }

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

} // namespace cxxaux
#endif
