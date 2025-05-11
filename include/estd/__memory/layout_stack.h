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

/// @brief Configure the memory as a backword list with the leading
/// sizeof(void*) bytes used to keep predecessor address
///
/// e.g.
///     header   data
/// m1: |  null  |  {x}B  |
/// m2: |  &m1   |  {y}B  |
/// m3: |  &m2   |  {z}B  |
class layout_stack {
public:
  /// @brief The stack keeps a sequence of memory segment, however size of the
  /// memory segment is unknown at least sizeof(void*)
  using value_type = char*;

  /// @brief The leading memory bytes used by stack, it is useless after pop
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

  /// @brief Get the top memory segment
  /// @return address start from the memory reserved
  value_type top() const { return top_; }

  bool empty() const { return top_ == nullptr; }

  /// @brief Append a memory segment to chain with some leading bytes reserved
  /// for stack
  /// @param v a memory segment
  void push(value_type v, size_t sz = -1) {
    assert(sz >= memory_required_by_stack &&
           "memory to be pushed should not be too small to hold a address");
    *reinterpret_cast<value_type*>(v) = top_;
    top_ = v;
  }

  /// @brief Remove the last memory segment from chain, the reserved leading
  /// bytes are release by stack
  /// @return a memory segment
  value_type pop() {
    value_type ret = top_;
    top_ = *reinterpret_cast<value_type*>(top_);
    return ret;
  }

  /// @brief Move elements from another layout_stack
  /// @param other other layout_stack
  /// @param bottom bottom of this layout_stack to enable fast move
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
