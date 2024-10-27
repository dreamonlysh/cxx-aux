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

#ifndef ESTD_MEMORY_H
#define ESTD_MEMORY_H

#include "__memory/layout_bit_mapping.h"
// template <size_t MemoryBytes, size_t MemberBytes,
//           typename MappingType = unsigned long long>
// class layout_bit_mapping {
// public:
//   static constexpr size_t memory_bytes = MemoryBytes;
//   static constexpr size_t member_bytes = MemberBytes;
//   using mapping_type = MappingType;
//
//   static constexpr size_t memory_tail_bytes_unmapping;
//
//   layout_bit_mapping(char* mem) : impl_type(mem) {}
//
//   void reset();
//
//   void* acquire();
//
//   bool release(void* p);
// };

#include "__memory/layout_stack.h"
// class layout_stack {
// public:
//   using value_type = char*;
//
//   static constexpr size_t memory_required_by_stack = sizeof(void*);
//
//   layout_stack() = default;
//   ~layout_stack() noexcept = default;
//   layout_stack(const layout_stack& other);
//   layout_stack(layout_stack&& other) noexcept;
//   layout_stack& operator=(const layout_stack& other);
//   layout_stack& operator=(layout_stack&& other) noexcept;
//
//   value_type top() const;
//
//   bool empty() const;
//
//   void push(value_type v, size_t sz = -1);
//
//   value_type pop();
// };

#include "__memory/memory_pool_uniform.h"
// template <size_t Size, size_t BlockSize = 4 * 1024>
// class MemoryPoolUniform {
// public:
//   static_assert((BlockSize % 1024) == 0,
//                 "The block size should be multi times of 1024 bytes");
//
//   MemoryPoolUniform() = default;
//   ~MemoryPoolUniform() noexcept = default;
//
//   void* allocate();
//
//   void deallocate(void* p);
//
//   void merge(MemoryPoolUniform& other);
// };

#include "__memory/object_pool.h"
// template <typename T, size_t Align = std::alignment_of_v<T>,
//           size_t BlockSize = 4 * 1024>
// class ObjectPool {
// public:
//   static_assert(es::is_pow2(Align), "The alignment should be pow of 2");
//
//   using object_type = T;
//   static constexpr size_t block_size = BlockSize;
//   static constexpr size_t object_alloc_size =
//       (sizeof(T) + (Align - 1)) & (~(Align - 1));
//
//   template <typename... Args>
//   T* operator_new(Args&&... args);
//
//   void operator_delete(T* p);
//
//   void merge(ObjectPool& other);
// };
//
// template <typename T, typename... Args>
// auto make_unique(T& pool, Args&&... args);
//
// template <typename T, size_t Align, size_t BlockSize,
//           typename LockerT = __impl::SubObjectPoolDefaultLocker>
// class SubObjectPool : public ObjectPool<T, Align, BlockSize> {
// public:
//   SubObjectPool(object_pool_type& parent);
//   SubObjectPool(object_pool_type& parent, LockerT& locker);
//   ~SubObjectPool() noexcept;
// };
//
// template <typename T, size_t Align, size_t BlockSize>
// SubObjectPool(ObjectPool<T, Align, BlockSize>)
//     -> SubObjectPool<T, Align, BlockSize>;
// template <typename T, size_t Align, size_t BlockSize, typename LockerT>
// SubObjectPool(ObjectPool<T, Align, BlockSize>, LockerT)
//     -> SubObjectPool<T, Align, BlockSize, LockerT>;

#endif
