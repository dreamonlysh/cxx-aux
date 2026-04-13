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

#ifndef CXXAUX_UTILITY_GUARD_H
#define CXXAUX_UTILITY_GUARD_H
#include <utility>

namespace cxxaux {

template <typename T>
class __Guard {
public:
  __Guard(T&& destruct) : __destruct(std::forward<T>(destruct)) {}
  ~__Guard() noexcept { __destruct(); }

private:
  T __destruct;
};

template <typename T>
__Guard(T&&) -> __Guard<T>;

/// this is a general guard like a lambda to release objects
#define CXXAUX_GUARD(obj) cxxaux::__Guard __guard_dummy_##__COUNTER__ = [&obj]

/// this ia a special guard to release a pointer
#define CXXAUX_OBJECT_GUARD(p)                                                 \
  CXXAUX_GUARD(p) { delete p; }

/// this ia a special guard to release a pointer that is not by delete
#define CXXAUX_OBJECT_GUARD_WITH_DELETER(p, deleter)                           \
  CXXAUX_GUARD(p) { deleter(p); }

} // namespace cxxaux
#endif
