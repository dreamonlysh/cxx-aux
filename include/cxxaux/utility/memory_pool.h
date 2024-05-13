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
#include <algorithm>
#include <cstddef>
#include <type_traits>

namespace cxxaux {

class MemoryPool {
public:
  static void* allocate(size_t bytes);

  static void deallocate(void* ptr, size_t bytes);

  template <typename T, typename... Args>
  static T* operator_new(Args&&... args) {
    void* ptr = allocate(sizeof(T));
    return new (ptr) T(std::forward<Args>(args)...);
  }

  template <typename T>
  static void operator_delete(T* ptr) {
    std::destroy_at(ptr);
    deallocate(ptr, sizeof(T));
  }

  template <class T>
  struct default_delete {
    constexpr default_delete() noexcept = default;

    template <class U>
    default_delete(const default_delete<U>&) noexcept {}

    template <class U>
    default_delete(const default_delete<U[]>&) noexcept {}

    void operator()(T* ptr) const { operator_delete(ptr); }

    template <class U>
    void operator()(U* ptr) const {
      operator_delete(ptr);
    }
  };
};

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

  void deallocate(T* ptr, size_t n) {
    for (size_t i = 0; i < n; ++i) {
      MemoryPool::deallocate(std::next(ptr, n), sizeof(value_type));
    }
  }
};

template <class T1, class T2>
bool operator==(const MemoryPoolAllocator<T1>&,
                const MemoryPoolAllocator<T2>&) noexcept {
  return true;
}

template <class T1, class T2>
bool operator!=(const MemoryPoolAllocator<T1>& lhs,
                const MemoryPoolAllocator<T2>& rhs) noexcept {
  return !(lhs == rhs);
}
} // namespace cxxaux
#endif
