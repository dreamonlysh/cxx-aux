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

#ifndef ESTD_MEMORY_MEMORY_POOL_UNIFORM_H
#define ESTD_MEMORY_MEMORY_POOL_UNIFORM_H
#include "layout_bit_mapping.h"
#include "layout_stack.h"
#include <cstdlib>
#include <estd/__utility/disabled_copy_move.h>
#include <forward_list>

namespace es { namespace memory {
namespace __impl_memory_pool_uniform {

template <size_t Size, size_t BlockSize>
class __MemoryPoolUniformSmall {
  using LayoutType = layout_bit_mapping<BlockSize, Size>;

public:
  __MemoryPoolUniformSmall() = default;
  ~__MemoryPoolUniformSmall() noexcept = default;

  void* allocate() {
    for (auto& block : allocator_) {
      if (void* ret = block.layout().acquire(); ret != nullptr)
        return ret;
    }

    allocator_.emplace_front();
    auto layout = allocator_.front().layout();
    layout.reset();
    return layout.acquire();
  }

  void deallocate(void* p) {
    for (auto& block : allocator_) {
      if (block.layout().release(p))
        return;
    }
  }

  void merge(__MemoryPoolUniformSmall& other) {
    allocator_.splice_after(allocator_.cbefore_begin(), other.allocator_);
  }

private:
  struct Block {
    char data[BlockSize];

    LayoutType layout() { return {data}; }
  };

  std::forward_list<Block> allocator_;
};

template <size_t Size, size_t BlockSize>
class __MemoryPoolUniformLarge {
public:
  __MemoryPoolUniformLarge() = default;
  ~__MemoryPoolUniformLarge() noexcept = default;

  void* allocate() {
    if (idle_.empty()) {
      allocator_.emplace_front();
      bottom_ = allocator_.front().data;
      for (size_t i = 0; i < (BlockSize / Size); ++i) {
        idle_.push(std::next(allocator_.front().data, i * Size));
      }
    }

    return idle_.pop();
  }

  void deallocate(void* p) {
    if (idle_.empty()) [[unlikely]]
      bottom_ = static_cast<char*>(p);
    idle_.push(static_cast<char*>(p));
  }

  void merge(__MemoryPoolUniformLarge& other) {
    allocator_.splice_after(allocator_.cbefore_begin(), other.allocator_);
    idle_.splice(other.idle_, bottom_);
  }

private:
  struct Block {
    char data[BlockSize];
  };
  std::forward_list<Block> allocator_;

  layout_stack idle_;
  layout_stack::value_type bottom_ = nullptr;
};

// Use std::forward_list as the container of the memory, there ware thought an
// in-place ptr used as the next. To align each memory with a page size, the
// actual block size will be `BlockSize - sizeof(void*)`
template <size_t Size, size_t BlockSize>
using MemoryPoolUniformSelector =
    std::conditional_t<std::less()(Size,
                                   layout_stack::memory_required_by_stack),
                       __impl_memory_pool_uniform::__MemoryPoolUniformSmall<
                           Size, BlockSize - sizeof(void*)>,
                       __impl_memory_pool_uniform::__MemoryPoolUniformLarge<
                           Size, BlockSize - sizeof(void*)>>;

} // namespace __impl_memory_pool_uniform

/**
 * @brief A memory pool for fixed-size allocations.
 *
 * This class template provides a memory pool optimized for allocating
 * memory blocks of a uniform size. It automatically selects the optimal
 * implementation based on the allocation size:
 * - Small allocations (< pointer size): Uses bitmap-based allocation
 * - Large allocations (>= pointer size): Uses stack-based allocation
 *
 * Key features:
 * - Fixed-size allocations only
 * - Automatic block management
 * - Memory reuse through pooling
 * - Support for pool merging
 *
 * @tparam Size Size in bytes for each allocation
 * @tparam BlockSize Buffer size for each memory block (default: 4KB)
 *
 * @note BlockSize must be a multiple of 1024 bytes
 * @note Thread-unsafe; use external synchronization if needed
 *
 * Example usage:
 * @code
 * // Pool for 64-byte allocations
 * MemoryPoolUniform<64> pool;
 *
 * // Allocate memory
 * void* mem1 = pool.allocate();
 * void* mem2 = pool.allocate();
 *
 * // Use memory...
 * memset(mem1, 0, 64);
 *
 * // Deallocate memory
 * pool.deallocate(mem1);
 * pool.deallocate(mem2);
 * @endcode
 */
template <size_t Size, size_t BlockSize = 4 * 1024>
class MemoryPoolUniform
    : __impl_memory_pool_uniform::MemoryPoolUniformSelector<Size, BlockSize>,
      disabled_copy_move {
  using impl_type =
      __impl_memory_pool_uniform::MemoryPoolUniformSelector<Size, BlockSize>;

public:
  static_assert((BlockSize % 1024) == 0,
                "The block size should be multi times of 1024 bytes");

  MemoryPoolUniform() = default;
  ~MemoryPoolUniform() noexcept = default;

  /**
   * @brief Allocates a memory block of Size bytes.
   *
   * If there's available memory in existing blocks, reuses it.
   * Otherwise, allocates a new block and returns memory from it.
   *
   * @return Pointer to a memory block of Size bytes
   * @note Never returns nullptr; throws on allocation failure
   */
  void* allocate() { return impl_type::allocate(); }

  /**
   * @brief Deallocates a memory block.
   *
   * Returns the memory to the pool for reuse. The pointer must have
   * been allocated from this pool.
   *
   * @param p Pointer to the memory block to deallocate
   *
   * @warning p must have been allocated from this pool
   * @warning p must not be null
   */
  void deallocate(void* p) { impl_type::deallocate(p); }

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
  void merge(MemoryPoolUniform& other) { impl_type::merge(other); }
};

}} // namespace es::memory
#endif
