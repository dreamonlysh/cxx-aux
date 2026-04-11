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

#ifndef ESTD_IDIOM_PIMPL_H
#define ESTD_IDIOM_PIMPL_H
#include <estd/meta.h>
#include <utility>

namespace es { namespace idiom {

/// @brief traits of pimpl to get the impl and pimpl type
///
/// impl_type is the T impl type, it must be defined.
/// pimpl_type is a must for multi Pimpl inheritance, it is always the direct
/// inherited Pimpl type.
///
/// @tparam T type derived the Pimpl
template <typename T>
struct pimpl_traits {
  // using impl_type = TImpl;
  // using pimpl_type = Pimpl<T, N>;
};

/// @brief a performance optimized pimpl aux class
///
/// e.g.
/// a.h
/// class A : public Pimpl<A, 8> {
/// public:
///   void func();
/// };
///
/// a.cpp:
/// class AImpl {
/// public:
///   void func() {
///     // do something
///   }
///   uint64_t data;
/// };
///
/// template <>
/// struct pimpl_traits<A> {
///   using impl_type = AImpl;
///   using pimpl_type = Pimpl<A, 8>; // optional
/// };
///
/// void A::func() {
///   pimpl_cast(this)->func();
/// }
///
/// @tparam T type derived the Pimpl
/// @tparam storage_size memory byte size enough to hold the impl class
template <typename T, unsigned storage_size = 0>
class Pimpl {
public:
  /// @brief memory byte size alloced
  static constexpr unsigned pimpl_storage_size = storage_size;

  template <typename U>
  friend auto pimpl_cast(U*);

  /// @brief construct of pimpl with args passed to the impl class
  /// @tparam Args args type of the impl class construct
  /// @param args args of the impl class construct
  template <typename... Args>
  Pimpl(Args&&... args) {
    using impl_type = typename pimpl_traits<T>::impl_type;
    static_assert(sizeof(impl_type) <= pimpl_storage_size,
                  "the storage size is not enough to hold the impl");
    new (__pimpl_storage) impl_type(std::forward<Args>(args)...);
  }

  ~Pimpl() noexcept {
    using impl_type = typename pimpl_traits<T>::impl_type;
    reinterpret_cast<impl_type*>(__pimpl_storage)->~impl_type();
  }

  Pimpl(const Pimpl&) = delete;

  Pimpl(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
  }

  Pimpl& operator=(const Pimpl&) = delete;

  Pimpl& operator=(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
    return *this;
  }

private:
  char __pimpl_storage[pimpl_storage_size];
};

/// @brief a traditional pimpl aux class
/// @tparam T type derived the Pimpl
template <typename T>
class Pimpl<T, 0> {
public:
  /// @brief memory byte size alloced
  static constexpr unsigned pimpl_storage_size = 0;

  template <typename U>
  friend auto pimpl_cast(U*);

  /// @brief construct of pimpl with args passed to the impl class
  /// @tparam Args args type of the impl class construct
  /// @param args args of the impl class construct
  template <typename... Args>
  Pimpl(Args&&... args) {
    using impl_type = typename pimpl_traits<T>::impl_type;
    __pimpl_storage = new impl_type(std::forward<Args>(args)...);
  }

  ~Pimpl() noexcept {
    using impl_type = typename pimpl_traits<T>::impl_type;
    delete static_cast<impl_type*>(__pimpl_storage);
  }

  Pimpl(const Pimpl&) = delete;

  Pimpl(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
  }

  Pimpl& operator=(const Pimpl&) = delete;

  Pimpl& operator=(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
    return *this;
  }

private:
  void* __pimpl_storage = nullptr;
};

META_HAS_MEMBER_TYPE(pimpl_type);

/// @brief aux to get the pointer of the impl
/// @tparam T type derived the Pimpl
/// @param ptr T type object, always be this
/// @return pointer to the T impl
template <typename T>
auto pimpl_cast(T* ptr) {
  using PureT = std::remove_cv_t<T>;
  using ImplType = typename pimpl_traits<PureT>::impl_type;
  using RetT = std::add_pointer_t<es::add_const_as_t<ImplType, T>>;
  // traits pimpl type, designed for multi Pimpl inheritance case
  if constexpr (has_member_pimpl_type_v<pimpl_traits<PureT>>) {
    using PimplType = typename pimpl_traits<PureT>::pimpl_type;
    using SrcT = std::add_pointer_t<es::add_const_as_t<PimplType, T>>;
    return reinterpret_cast<RetT>(static_cast<SrcT>(ptr)->__pimpl_storage);
  } else {
    return reinterpret_cast<RetT>(ptr->__pimpl_storage);
  }
}

/// @brief aux to get the reference of the impl
/// @tparam T type derived the Pimpl
/// @param ref T type object, always be this
/// @return reference to the T impl
template <typename T>
auto pimpl_cast(T& ref) {
  return *pimpl_cast(&ref);
}

}} // namespace es::idiom
#endif
