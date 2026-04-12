// Copyright (c) 2026 guyuemeng
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

#ifndef ESTD_MAYBE_OWNED_H
#define ESTD_MAYBE_OWNED_H

#include "__container/maybe_owned.h"

// template <typename T>
// class maybe_owned {
// public:
//   using value_type = T;
//   using storage_type = std::variant<empty, to_std_array_t<T>,
//   __reference_type>;
//
//   // Constructors
//   maybe_owned() noexcept;
//
//   template <typename... Args>
//   maybe_owned(std::in_place_type_t<T>, Args&&... args);
//
//   maybe_owned(T&& t);  // Rvalue -> owned storage
//   maybe_owned(T& t);   // Lvalue -> reference storage
//
//   maybe_owned(const maybe_owned& other);
//   maybe_owned(maybe_owned&& other) noexcept;
//   ~maybe_owned() noexcept = default;
//
//   // Assignment operators
//   maybe_owned& operator=(const maybe_owned& other);
//   maybe_owned& operator=(maybe_owned&& other) noexcept;
//
//   maybe_owned& operator=(T&& t);  // Rvalue -> owned storage
//   maybe_owned& operator=(T& t);   // Lvalue -> reference storage
//
//   // Observers
//   bool has_value() const noexcept;
//   explicit operator bool() const noexcept;
//
//   bool is_owned() const noexcept;
//   bool is_reference() const noexcept;
//
//   // Accessors
//   T& get() &;
//   const T& get() const&;
//   T&& get() && = delete;  // Deleted to prevent misuse
//   const T&& get() const&& = delete;
//
//   T& operator*() &;
//   const T& operator*() const&;
//   T&& operator*() && = delete;
//   const T&& operator*() const&& = delete;
//
//   T* operator->() noexcept;
//   const T* operator->() const noexcept;
//
//   // Modifiers
//   void reset() noexcept;
//   void swap(maybe_owned& other) noexcept;
// };
//
// // Deduction guides
// template <typename T>
// maybe_owned(T&&) -> maybe_owned<T>;
//
// template <typename T>
// maybe_owned(T&) -> maybe_owned<T>;
//
// // Non-member functions
// template <typename T>
// void swap(maybe_owned<T>& lhs, maybe_owned<T>& rhs) noexcept;

#endif
