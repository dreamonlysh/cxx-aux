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

/**
 * @brief Traits for configuring Pimpl implementation.
 *
 * Specialize this template for each class using Pimpl to define:
 * - impl_type: The actual implementation type
 * - pimpl_type: The Pimpl base type (optional, for multiple inheritance)
 *
 * @tparam T The class that uses Pimpl
 *
 * Example specialization:
 * @code
 * class Widget;
 *
 * template <>
 * struct pimpl_traits<Widget> {
 *     using impl_type = WidgetImpl;
 *     using pimpl_type = Pimpl<Widget, 64>;  // Optional
 * };
 * @endcode
 */
template <typename T>
struct pimpl_traits {
  // using impl_type = TImpl;
  // using pimpl_type = Pimpl<T, N>;
};

/**
 * @brief Performance-optimized Pimpl (Pointer to Implementation) idiom helper.
 *
 * This class template implements the Pimpl idiom with optional inline storage
 * optimization. When storage_size > 0, the implementation is stored inline
 * within the object, avoiding heap allocation.
 *
 * Key features:
 * - Compile-time polymorphism without virtual functions
 * - Inline storage optimization (when storage_size > 0)
 * - Binary compatibility across library boundaries
 * - Reduced compilation dependencies
 *
 * @tparam T The class that inherits from Pimpl
 * @tparam storage_size Bytes to reserve for inline storage (0 = heap
 * allocation)
 *
 * @note When storage_size > 0, sizeof(impl_type) must be <= storage_size
 * @note When storage_size = 0, implementation is heap-allocated
 *
 * Example usage (inline storage):
 * @code
 * // widget.h
 * class Widget : public Pimpl<Widget, 64> {
 * public:
 *     void doSomething();
 * };
 *
 * // widget.cpp
 * class WidgetImpl {
 * public:
 *     void doSomething() {
 *         // Implementation
 *     }
 * private:
 *     int data_[16];  // 64 bytes total
 * };
 *
 * template <>
 * struct pimpl_traits<Widget> {
 *     using impl_type = WidgetImpl;
 * };
 *
 * void Widget::doSomething() {
 *     pimpl_cast(this)->doSomething();
 * }
 * @endcode
 */
template <typename T, unsigned storage_size = 0>
class Pimpl {
public:
  /**
   * @brief Memory bytes allocated for inline storage.
   */
  static constexpr unsigned pimpl_storage_size = storage_size;

  template <typename U>
  friend auto pimpl_cast(U*);

  /**
   * @brief Constructs the implementation object in-place.
   *
   * Forwards all arguments to the implementation's constructor.
   * The implementation is constructed in the reserved storage.
   *
   * @tparam Args Constructor argument types
   * @param args Arguments to forward to implementation constructor
   *
   * @throws Static assertion if storage_size is insufficient
   */
  template <typename... Args>
  Pimpl(Args&&... args) {
    using impl_type = typename pimpl_traits<T>::impl_type;
    static_assert(sizeof(impl_type) <= pimpl_storage_size,
                  "the storage size is not enough to hold the impl");
    new (__pimpl_storage) impl_type(std::forward<Args>(args)...);
  }

  /**
   * @brief Destructs the implementation object.
   *
   * Calls the implementation's destructor in-place.
   */
  ~Pimpl() noexcept {
    using impl_type = typename pimpl_traits<T>::impl_type;
    reinterpret_cast<impl_type*>(__pimpl_storage)->~impl_type();
  }

  Pimpl(const Pimpl&) = delete;

  /**
   * @brief Move constructor.
   *
   * Swaps storage with the other Pimpl object.
   * This is a shallow move suitable for inline storage.
   *
   * @param other Pimpl to move from
   */
  Pimpl(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
  }

  Pimpl& operator=(const Pimpl&) = delete;

  /**
   * @brief Move assignment operator.
   *
   * @param other Pimpl to move from
   * @return Reference to this
   */
  Pimpl& operator=(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
    return *this;
  }

private:
  char __pimpl_storage[pimpl_storage_size];
};

/**
 * @brief Traditional Pimpl with heap allocation.
 *
 * This specialization uses heap allocation for the implementation,
 * suitable when the implementation size is unknown or very large.
 *
 * @tparam T The class that inherits from Pimpl
 *
 * @note Implementation is heap-allocated
 * @note Move operations swap pointers (fast)
 *
 * Example usage (heap allocation):
 * @code
 * // widget.h
 * class Widget : public Pimpl<Widget> {  // storage_size = 0
 * public:
 *     void doSomething();
 * };
 *
 * // widget.cpp
 * class WidgetImpl {
 * public:
 *     void doSomething() {
 *         // Implementation
 *     }
 * private:
 *     std::vector<int> large_data_;  // Can be any size
 * };
 *
 * template <>
 * struct pimpl_traits<Widget> {
 *     using impl_type = WidgetImpl;
 * };
 * @endcode
 */
template <typename T>
class Pimpl<T, 0> {
public:
  /**
   * @brief Memory bytes allocated (0 = heap allocation).
   */
  static constexpr unsigned pimpl_storage_size = 0;

  template <typename U>
  friend auto pimpl_cast(U*);

  /**
   * @brief Constructs the implementation on the heap.
   *
   * Allocates and constructs the implementation object.
   *
   * @tparam Args Constructor argument types
   * @param args Arguments to forward to implementation constructor
   */
  template <typename... Args>
  Pimpl(Args&&... args) {
    using impl_type = typename pimpl_traits<T>::impl_type;
    __pimpl_storage = new impl_type(std::forward<Args>(args)...);
  }

  /**
   * @brief Destructs and deallocates the implementation.
   */
  ~Pimpl() noexcept {
    using impl_type = typename pimpl_traits<T>::impl_type;
    delete static_cast<impl_type*>(__pimpl_storage);
  }

  Pimpl(const Pimpl&) = delete;

  /**
   * @brief Move constructor.
   *
   * Swaps pointers with the other Pimpl object.
   *
   * @param other Pimpl to move from
   */
  Pimpl(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
  }

  Pimpl& operator=(const Pimpl&) = delete;

  /**
   * @brief Move assignment operator.
   *
   * @param other Pimpl to move from
   * @return Reference to this
   */
  Pimpl& operator=(Pimpl&& other) noexcept {
    std::swap(__pimpl_storage, other.__pimpl_storage);
    return *this;
  }

private:
  void* __pimpl_storage = nullptr;
};

META_HAS_MEMBER_TYPE(pimpl_type);

/**
 * @brief Casts from the public class to the implementation pointer.
 *
 * This function template provides access to the implementation object
 * from within the public class's member functions. It handles both
 * single and multiple inheritance scenarios.
 *
 * @tparam T The public class type
 * @param ptr Pointer to the public class object (usually 'this')
 * @return Pointer to the implementation object
 *
 * @note Preserves const-ness: if ptr is const, returns const ImplType*
 * @note Handles multiple Pimpl inheritance via pimpl_type trait
 *
 * Example usage:
 * @code
 * void Widget::doSomething() {
 *     pimpl_cast(this)->doSomething();
 * }
 *
 * void Widget::doSomething() const {
 *     pimpl_cast(this)->doSomething();  // Returns const WidgetImpl*
 * }
 * @endcode
 */
template <typename T>
auto pimpl_cast(T* ptr) {
  using PureT = std::remove_cv_t<T>;
  using ImplType = typename pimpl_traits<PureT>::impl_type;
  using RetT = std::add_pointer_t<es::add_const_as_t<ImplType, T>>;
  // Handle multiple Pimpl inheritance by routing through pimpl_type
  if constexpr (has_member_pimpl_type_v<pimpl_traits<PureT>>) {
    using PimplType = typename pimpl_traits<PureT>::pimpl_type;
    using SrcT = std::add_pointer_t<es::add_const_as_t<PimplType, T>>;
    return reinterpret_cast<RetT>(static_cast<SrcT>(ptr)->__pimpl_storage);
  } else {
    return reinterpret_cast<RetT>(ptr->__pimpl_storage);
  }
}

/**
 * @brief Casts from the public class to the implementation reference.
 *
 * Reference overload of pimpl_cast for convenience.
 *
 * @tparam T The public class type
 * @param ref Reference to the public class object (usually '*this')
 * @return Reference to the implementation object
 *
 * Example usage:
 * @code
 * void Widget::doSomething() {
 *     auto& impl = pimpl_cast(*this);
 *     impl.doSomething();
 * }
 * @endcode
 */
template <typename T>
auto pimpl_cast(T& ref) {
  return *pimpl_cast(&ref);
}

}} // namespace es::idiom
#endif
