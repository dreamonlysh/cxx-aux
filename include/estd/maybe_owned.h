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
//   using storage_type = std::variant<empty, T, reference_wrapper>;

//   maybe_owned() noexcept;
//   maybe_owned(T&& t);
//   maybe_owned(const T& t);
//   maybe_owned(T& t);

//   maybe_owned(const maybe_owned& other);
//   maybe_owned(maybe_owned&& other) noexcept;
//   ~maybe_owned() noexcept = default;

//   maybe_owned& operator=(const maybe_owned& other);
//   maybe_owned& operator=(maybe_owned&& other) noexcept;

//   maybe_owned& operator=(T&& t);
//   maybe_owned& operator=(const T& t);
//   maybe_owned& operator=(T& t);

//   bool has_value() const;
//   explicit operator bool() const;

//   bool is_owned() const;
//   bool is_reference() const;

//   T& get();
//   const T& get() const;

//   T& operator*();
//   const T& operator*() const;

//   T* operator->();
//   const T* operator->() const;
// };

// template <typename T>
// maybe_owned(T&& t) -> maybe_owned<T>;

// template <typename T>
// maybe_owned(T& t) -> maybe_owned<T>;

#endif
