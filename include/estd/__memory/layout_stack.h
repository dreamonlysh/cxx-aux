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

#ifndef ESTD_MEMORY_LAYOUT_STACK_H
#define ESTD_MEMORY_LAYOUT_STACK_H
#include <cassert>
#include <utility>

namespace es { namespace memory {

/**
 * @brief Memory layout that implements a backward-linked list stack.
 *
 * This class manages memory segments by using the unused object memory itself
 * to store a pointer to the previous unused memory segment. This forms a linked
 * list (stack) of free memory segments.
 *
 * **Why this design works:**
 * - When memory is NOT in use (free), we can reuse its first sizeof(void*)
 * bytes to store a pointer to the previous free segment.
 * - When memory IS in use, the user owns the entire segment including header.
 * - This requires objects to be at least sizeof(void*) bytes.
 *
 * **Memory Layout (each segment uses its header to store predecessor
 * pointer):**
 * ```
 * ┌─────────────────────────────────────────────────────────────────┐
 * │  Segment 1    │  Segment 2    │  Segment 3    │  ...            │
 * │  ┌─────┬────┐ │  ┌─────┬────┐ │  ┌─────┬────┐ │                 │
 * │  │prev │data│ │  │prev │data│ │  │prev │data│ │                 │
 * │  │null │    │ │  │ &S1 │    │ │  │ &S2 │    │ │                 │
 * │  └─────┴────┘ │  └─────┴────┘ │  └─────┴────┘ │                 │
 * └─────────────────────────────────────────────────────────────────┘
 * ```
 *
 * **Memory segment states:**
 * - IN USE:  [  user data  |  user data  |  ...  ]
 * - FREE:    [ prev ptr    |  (unused)   |  ...  ]
 *
 * When a segment is pushed onto the stack:
 * - The first sizeof(void*) bytes store the address of the previous top
 * - The stack's top pointer is updated to this segment
 *
 * @note The size of each memory segment is unknown to the stack
 * @note Minimum segment size is sizeof(void*) for the header
 * @note Best for large objects (>= sizeof(void*)) that can hold a pointer
 * @note For smaller objects, use layout_bit_mapping instead
 *
 * Example usage:
 * @code
 * layout_stack stack;
 * char memory1[1024];
 * char memory2[1024];
 *
 * stack.push(memory1, sizeof(memory1));
 * stack.push(memory2, sizeof(memory2));
 *
 * char* top = stack.top();
 * char* segment = stack.pop();
 * @endcode
 */
class layout_stack {
public:
  /**
   * @brief Type representing a memory segment pointer.
   *
   * The stack keeps a sequence of memory segments, where the size
   * of each segment is at least sizeof(void*).
   */
  using value_type = char*;

  /**
   * @brief Minimum bytes required by the stack for bookkeeping.
   *
   * Each memory segment must have at least this many bytes for
   * storing the predecessor pointer.
   */
  static constexpr size_t memory_required_by_stack = sizeof(void*);

  layout_stack() = default;

  ~layout_stack() noexcept = default;

  layout_stack(const layout_stack& other) { top_ = other.top_; }

  layout_stack(layout_stack&& other) noexcept { std::swap(top_, other.top_); }

  layout_stack& operator=(const layout_stack& other) {
    if (this != &other)
      top_ = other.top_;
    return *this;
  }

  layout_stack& operator=(layout_stack&& other) noexcept {
    std::swap(top_, other.top_);
    return *this;
  }

  /**
   * @brief Gets the top memory segment.
   * @return Pointer to the top memory segment, or nullptr if empty
   */
  value_type top() const { return top_; }

  /**
   * @brief Checks if the stack is empty.
   * @return true if the stack has no segments, false otherwise
   */
  bool empty() const { return top_ == nullptr; }

  /**
   * @brief Pushes a memory segment onto the stack.
   *
   * The leading bytes of the segment are reserved for stack bookkeeping.
   *
   * @param v Pointer to the memory segment to push
   * @param sz Size of the memory segment (must be >= memory_required_by_stack)
   *
   * @warning The memory segment must remain valid for the duration it's on the
   * stack
   */
  void push(value_type v, size_t sz = -1) {
    assert(sz >= memory_required_by_stack &&
           "memory to be pushed should not be too small to hold a address");
    *reinterpret_cast<value_type*>(v) = top_;
    top_ = v;
  }

  /**
   * @brief Pops a memory segment from the stack.
   *
   * Removes and returns the top memory segment. The reserved leading
   * bytes are released by the stack.
   *
   * @return Pointer to the popped memory segment
   *
   * @warning The caller is responsible for managing the popped memory
   */
  value_type pop() {
    value_type ret = top_;
    top_ = *reinterpret_cast<value_type*>(top_);
    return ret;
  }

  /**
   * @brief Moves all segments from another stack to this stack.
   *
   * Transfers all memory segments from `other` to this stack,
   * appending them after the current bottom segment.
   *
   * @param other The source stack to move from (will be empty after operation)
   * @param bottom Optional pointer to the bottom of this stack for fast append
   *
   * @note If bottom is nullptr, the stack will traverse to find the bottom
   */
  void splice(layout_stack& other, value_type bottom = nullptr) {
    if (this == &other || other.empty())
      return;
    if (empty())
      std::swap(top_, other.top_);

    if (bottom == nullptr) {
      bottom = top_;
      while (value_type tmp = *reinterpret_cast<value_type*>(bottom)) {
        bottom = tmp;
      }
    }

    *reinterpret_cast<value_type*>(bottom) = other.top_;
    other.top_ = nullptr;
  }

private:
  value_type top_ = nullptr;
};

}} // namespace es::memory
#endif
