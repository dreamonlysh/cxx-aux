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

#ifndef ESTD___RANGES_VIEW_INTERFACE_H
#define ESTD___RANGES_VIEW_INTERFACE_H
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>

namespace es { namespace ranges {

/**
 * @brief CRTP base class for view types providing common interface.
 *
 * This class template uses the Curiously Recurring Template Pattern (CRTP)
 * to provide a common interface for view types. Derived classes must implement
 * begin() and end() methods, and this base class provides additional utility
 * methods based on the iterator capabilities.
 *
 * The interface adapts to the iterator capabilities:
 * - empty(), operator bool(): Always available
 * - cbegin(), cend(): Always available
 * - data(): Available for contiguous iterators
 * - size(): Available for forward iterators or better
 * - front(): Always available
 * - back(): Available for bidirectional iterators or better
 * - operator[]: Available for random access iterators
 *
 * @tparam Derived The derived view type (CRTP pattern)
 *
 * @note Derived must implement begin() and end() methods
 *
 * Example usage:
 * @code
 * class my_view : public view_interface<my_view> {
 * public:
 *     auto begin() { return data_.begin(); }
 *     auto end() { return data_.end(); }
 * private:
 *     std::vector<int> data_;
 * };
 *
 * my_view v;
 * if (!v.empty()) {
 *     auto first = v.front();
 *     auto size = v.size();
 * }
 * @endcode
 */
template <typename Derived>
class view_interface {
private:
  // CRTP helper to access the derived class
  constexpr Derived& derived() { return static_cast<Derived&>(*this); }
  constexpr const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }

public:
  /**
   * @brief Checks if the view is empty.
   * @return true if the view is empty, false otherwise
   */
  constexpr bool empty() { return derived().begin() == derived().end(); }

  /**
   * @brief Checks if the view is empty (const version).
   * @return true if the view is empty, false otherwise
   */
  constexpr bool empty() const { return derived().begin() == derived().end(); }

  /**
   * @brief Gets a const iterator to the beginning.
   * @return Const iterator to the first element
   */
  constexpr auto cbegin() { return std::cbegin(derived()); }

  /**
   * @brief Gets a const iterator to the beginning (const version).
   * @return Const iterator to the first element
   */
  constexpr auto cbegin() const { return std::cbegin(derived()); }

  /**
   * @brief Gets a const iterator to the end.
   * @return Const iterator past the last element
   */
  constexpr auto cend() { return std::cend(derived()); }

  /**
   * @brief Gets a const iterator to the end (const version).
   * @return Const iterator past the last element
   */
  constexpr auto cend() const { return std::cend(derived()); }

  /**
   * @brief Boolean conversion operator.
   * @return true if the view is not empty, false otherwise
   */
  constexpr explicit operator bool() { return !empty(); }

  /**
   * @brief Boolean conversion operator (const version).
   * @return true if the view is not empty, false otherwise
   */
  constexpr explicit operator bool() const { return !empty(); }

  /**
   * @brief Gets a pointer to the underlying data.
   *
   * Only available for contiguous iterators.
   *
   * @return Pointer to the first element
   * @throws Static assertion failure if iterator is not contiguous
   */
  constexpr auto data() {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "data() requires contiguous_iterator");
    return std::addressof(*derived().begin());
  }

  /**
   * @brief Gets a pointer to the underlying data (const version).
   *
   * Only available for contiguous iterators.
   *
   * @return Const pointer to the first element
   * @throws Static assertion failure if iterator is not contiguous
   */
  constexpr auto data() const {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "data() requires contiguous_iterator");
    return std::addressof(*derived().begin());
  }

  /**
   * @brief Gets the size of the view.
   * @return Number of elements in the view
   */
  constexpr size_t size() { return std::as_const(derived()).size(); }

  /**
   * @brief Gets the size of the view (const version).
   * @return Number of elements in the view
   */
  constexpr size_t size() const {
    return static_cast<size_t>(
        std::distance(derived().begin(), derived().end()));
  }

  /**
   * @brief Gets a reference to the first element.
   * @return Reference to the first element
   */
  decltype(auto) front() { return *derived().begin(); }

  /**
   * @brief Gets a const reference to the first element.
   * @return Const reference to the first element
   */
  decltype(auto) front() const { return *derived().begin(); }

  /**
   * @brief Gets a reference to the last element.
   *
   * Only available for bidirectional iterators or better.
   *
   * @return Reference to the last element
   * @throws Static assertion failure if iterator is not bidirectional
   */
  decltype(auto) back() {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category> ||
            std::is_base_of_v<std::bidirectional_iterator_tag,
                              iterator_category>,
        "back() not supported for non-bidirectional view");
    auto it = derived().end();
    --it;
    return *it;
  }

  /**
   * @brief Gets a const reference to the last element.
   *
   * Only available for bidirectional iterators or better.
   *
   * @return Const reference to the last element
   * @throws Static assertion failure if iterator is not bidirectional
   */
  decltype(auto) back() const {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category> ||
            std::is_base_of_v<std::bidirectional_iterator_tag,
                              iterator_category>,
        "back() not supported for non-bidirectional view");
    auto it = derived().end();
    --it;
    return *it;
  }

  /**
   * @brief Accesses an element by index.
   *
   * Only available for random access iterators.
   *
   * @param n Index of the element to access
   * @return Reference to the element at position n
   * @throws Static assertion failure if iterator is not random access
   */
  template <typename Self = Derived>
  decltype(auto) operator[](size_t n) {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "operator[] requires random_access_iterator");
    auto it = derived().begin();
    it += n;
    return *it;
  }

  /**
   * @brief Accesses an element by index (const version).
   *
   * Only available for random access iterators.
   *
   * @param n Index of the element to access
   * @return Const reference to the element at position n
   * @throws Static assertion failure if iterator is not random access
   */
  template <typename Self = Derived>
  decltype(auto) operator[](size_t n) const {
    using iterator = decltype(derived().begin());
    using iterator_category =
        typename std::iterator_traits<iterator>::iterator_category;
    static_assert(
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category>,
        "operator[] requires random_access_iterator");
    auto it = derived().begin();
    it += n;
    return *it;
  }
};

}} // namespace es::ranges
#endif
