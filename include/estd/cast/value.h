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

#ifndef ESTD_CAST_VALUE_H
#define ESTD_CAST_VALUE_H
#include "dyn_cast.h"

namespace es {

/// @brief Base class with uid to enable the dyn_cast
///
/// the final concret class should has an static constexprt unsigned member
/// CLASSID
/// @tparam DummyT Unique type
template <typename DummyT>
class Value {
public:
  using dyn_cast_aux_type = Value<DummyT>;

protected:
  Value(unsigned uid) : uid(uid) {}
  ~Value() noexcept = default;

  Value(const Value& other) = default;
  Value(Value&& other) noexcept = default;
  Value& operator=(const Value& other) = default;
  Value& operator=(Value&& other) noexcept = default;

private:
  template <typename T, typename U>
  friend struct isa_traits;

  unsigned uid;
};

namespace __value_impl {
META_HAS_MEMBER_DATA(CLASSID);
}

template <typename T, typename U>
struct isa_traits<T, Value<U>> {
  static_assert(__value_impl::has_member_CLASSID_v<T>);
  static_assert(std::is_base_of_v<Value<U>, T>);

  static bool doit(const Value<U>& v) { return T::CLASSID == v.uid; }
};

template <typename T, typename U>
struct cast_traits<T, Value<U>> {
  static_assert(std::is_base_of_v<Value<U>, T>);

  static T& doit(Value<U>& v) { return static_cast<T&>(v); }

  static const T& doit(const Value<U>& v) { return static_cast<const T&>(v); }
};

} // namespace es
#endif
