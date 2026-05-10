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

#ifndef CXXAUX_UTILITY_MEMORY_POOL_H
#define CXXAUX_UTILITY_MEMORY_POOL_H
#include <cstddef>
#include <memory>
#include <type_traits>

namespace cxxaux {

/**
 * @brief Global memory pool for uniform-size allocations.
 * Provides static allocate/deallocate methods backed by a shared pool.
 */
class MemoryPool {
public:
  static void* allocate(size_t bytes);

  static void deallocate(void* p, size_t bytes);

  /**
   * @brief Allocates and constructs an object in the pool.
   * @tparam T Object type
   * @tparam Args Constructor argument types
   * @param args Constructor arguments
   * @return Pointer to the newly constructed object
   */
  template <typename T, typename... Args>
  static T* operator_new(Args&&... args) {
    void* p = allocate(sizeof(T));
    return new (p) T(std::forward<Args>(args)...);
  }

  /**
   * @brief Destroys an object and returns its memory to the pool.
   * @tparam T Object type
   * @param p Pointer to the object to destroy
   */
  template <typename T>
  static void operator_delete(T* p) {
    std::destroy_at(p);
    deallocate(p, sizeof(T));
  }

  /**
   * @brief Deleter for use with std::unique_ptr that returns memory to the
   * pool.
   */
  template <class T>
  struct default_delete {
    constexpr default_delete() noexcept = default;

    template <class U>
    default_delete(const default_delete<U>&) noexcept {}

    template <class U>
    default_delete(const default_delete<U[]>&) noexcept {}

    void operator()(T* p) const { operator_delete(p); }

    template <class U>
    void operator()(U* p) const {
      operator_delete(p);
    }
  };
};

/**
 * @brief STL-compatible allocator that uses MemoryPool for allocations.
 * @tparam T Value type to allocate
 */
template <typename T>
class MemoryPoolAllocator {
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  // (C++11)(deprecated in C++23)(removed in C++26)
  using is_always_equal = std::true_type;

  MemoryPoolAllocator() = default;
  ~MemoryPoolAllocator() noexcept = default;

  MemoryPoolAllocator(const MemoryPoolAllocator&) {}
  template <typename U>
  MemoryPoolAllocator(const MemoryPoolAllocator<U>&) {}

  T* allocate(size_t n) {
    return reinterpret_cast<T*>(MemoryPool::allocate(sizeof(value_type) * n));
  }

  void deallocate(T* p, size_t n) {
    std::destroy(p, p + n);
    MemoryPool::deallocate(p, sizeof(value_type) * n);
  }
};

/**
 * @brief All MemoryPoolAllocators are equal (stateless).
 */
template <class T1, class T2>
bool operator==(const MemoryPoolAllocator<T1>&,
                const MemoryPoolAllocator<T2>&) noexcept {
  return true;
}

/**
 * @brief All MemoryPoolAllocators are equal (stateless).
 */
template <class T1, class T2>
bool operator!=(const MemoryPoolAllocator<T1>& lhs,
                const MemoryPoolAllocator<T2>& rhs) noexcept {
  return !(lhs == rhs);
}
} // namespace cxxaux
#endif
