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

#ifndef ESTD_CONCURRENCY_H
#define ESTD_CONCURRENCY_H

#include "__concurrency/is_mutex_like.h"
// - META_HAS_MEMBER_FUNCTION(lock);
// - META_HAS_MEMBER_FUNCTION(try_lock);
// - META_HAS_MEMBER_FUNCTION(unlock);
//   Defined: is_mutex_like, is_mutex_like_v

#include "__concurrency/mutex_silent.h"
// class mutex_silent {
// public:
//   void lock();
//   bool try_lock();
//   void unlock();
// };
// inline mutex_silent defalut_mutex_silent;

#endif
