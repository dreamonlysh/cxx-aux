// Copyright (c) 2023 guyuemeng
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

#ifndef ESTD_CASE_TYPE_ID_H
#define ESTD_CASE_TYPE_ID_H
#include <cstddef>

namespace es {

namespace __es_impl {
template <typename T>
size_t __type_id() noexcept {
  static const char id = '\0';
  return reinterpret_cast<size_t>(&id);
}
} // namespace __es_impl

template <typename T>
size_t type_id() noexcept {
  static_assert(std::is_class_v<T>);
  return __es_impl::__type_id<std::remove_cv_t<T>>();
}

template <typename T>
size_t type_id(T&&) noexcept {
  return type_id<std::remove_reference_t<T>>();
}

} // namespace es

#endif // ESTD_CASE_TYPE_ID_H
