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
// class layout_bit_mapping
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

#endif
