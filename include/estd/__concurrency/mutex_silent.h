// Copyright (c) 2024 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD__CONCURRENCY_MUTEX_SILENT_H
#define ESTD__CONCURRENCY_MUTEX_SILENT_H

namespace es {

/// @brief This is a helper for no lock
class mutex_silent {
public:
  void lock() {}

  bool try_lock() { return false; }

  void unlock() {}
};

inline mutex_silent default_mutex_silent;

} // namespace es
#endif
