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

/**
 * @brief Base class that enables dynamic casting via runtime type
 * identification.
 *
 * This class provides a foundation for implementing dynamic casting in
 * class hierarchies. Each derived class should:
 * 1. Inherit from Value<UniqueTag>
 * 2. Define a static constexpr unsigned CLASSID member
 * 3. Pass CLASSID to the Value constructor
 *
 * The UniqueTag template parameter ensures type safety across different
 * inheritance hierarchies.
 *
 * @tparam DummyT Unique type tag to distinguish different hierarchies
 *
 * Example usage:
 * @code
 * struct MyHierarchyTag {};
 *
 * class Base : public Value<MyHierarchyTag> {
 * protected:
 *     Base(unsigned uid) : Value<MyHierarchyTag>(uid) {}
 * };
 *
 * class Derived : public Base {
 * public:
 *     static constexpr unsigned CLASSID = 1;
 *     Derived() : Base(CLASSID) {}
 * };
 *
 * // Now you can use dyn_cast
 * Base* obj = new Derived();
 * if (Derived* d = dyn_cast<Derived>(obj)) {
 *     // Successfully cast
 * }
 * @endcode
 */
template <typename DummyT>
class Value {
public:
  using dyn_cast_aux_type = Value<DummyT>;

protected:
  constexpr Value(unsigned uid) : uid(uid) {}
  ~Value() noexcept = default;

  constexpr Value(const Value& other) = default;
  constexpr Value(Value&& other) noexcept = default;
  constexpr Value& operator=(const Value& other) = default;
  constexpr Value& operator=(Value&& other) noexcept = default;

private:
  template <typename T, typename U>
  friend struct isa_traits;

  unsigned uid;
};

namespace __impl_value {
META_HAS_MEMBER_DATA(CLASSID);
}

template <typename T, typename U>
struct isa_traits<T, Value<U>> {
  static_assert(__impl_value::has_member_CLASSID_v<T>);
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
