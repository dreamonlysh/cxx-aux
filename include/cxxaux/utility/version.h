// Copyright (c) 2026 guyuemeng
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

#ifndef CXXAUX_UTILITY_VERSION_H
#define CXXAUX_UTILITY_VERSION_H

#include "estd/__bit/bit_get.h"
#include <cstddef>
#include <estd/bit.h>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace cxxaux {

/**
 * @brief Represents a single version identifier as a bit position.
 *
 * Each VersionID corresponds to a single bit in a bitset, enabling
 * efficient set operations and storage.
 *
 * @tparam BitSetT Unsigned integer type for the underlying bitset
 * @tparam UniqueT Optional tag type for creating distinct VersionID types
 *
 * Example:
 * @code
 * using VID = VersionID<uint32_t>;
 * VID v1(0);  // Version at offset 0
 * VID v2(5);  // Version at offset 5
 * @endcode
 */
template <typename BitSetT, typename UniqueT = void>
class VersionID {
public:
  using bitset_type = BitSetT;

  static_assert(std::is_unsigned_v<bitset_type>,
                "BitSetT must be an unsigned integer type");

  /** @brief Default constructor, creates an invalid VersionID */
  constexpr VersionID() noexcept = default;

  /**
   * @brief Constructs a VersionID from a bit offset.
   * @param offset The bit position (0-based)
   */
  constexpr explicit VersionID(unsigned offset) noexcept
      : bit_(bitset_type{1} << offset) {}

  /**
   * @brief Sets the bit position.
   * @param offset The bit position (0-based)
   */
  constexpr void set(unsigned offset) noexcept {
    bit_ = bitset_type{1} << offset;
  }

  /** @brief Returns true if this is a valid (non-zero) VersionID */
  constexpr bool valid() const noexcept { return bit_ != 0; }

  /** @brief Returns the underlying bit value */
  constexpr bitset_type value() const noexcept { return bit_; }

  /** @brief Returns the bit offset (position) */
  constexpr unsigned offset() const noexcept { return es::countr_bit0(bit_); }

  constexpr bool operator==(VersionID other) const noexcept {
    return bit_ == other.bit_;
  }

  constexpr bool operator!=(VersionID other) const noexcept {
    return bit_ != other.bit_;
  }

private:
  bitset_type bit_ = 0;
};

/**
 * @brief A set of VersionIDs represented as a bitset.
 *
 * Provides efficient set operations (union, intersection) and iteration
 * over contained VersionIDs.
 *
 * @tparam VersionID The VersionID type this set contains
 *
 * Example:
 * @code
 * using VIDSet = VersionIDSet<VersionID<uint32_t>>;
 * VIDSet set(VID(0), VID(2), VID(4));
 * EXPECT_EQ(set.size(), 3u);
 * set.foreach([](VID id) { std::cout << id.offset() << "\n"; });
 * @endcode
 */
template <typename VersionIDT>
class VersionIDSet {
public:
  using bitset_type = typename VersionIDT::bitset_type;

  /** @brief Default constructor, creates an empty set */
  constexpr VersionIDSet() noexcept = default;

  /** @brief Constructs a set from a single VersionID */
  constexpr explicit VersionIDSet(VersionIDT id) noexcept
      : bitmap_(id.value()) {}

  /** @brief Constructs a set from multiple VersionIDs */
  template <typename... IDs>
  constexpr VersionIDSet(VersionIDT id, IDs... rest) noexcept
      : bitmap_(id.value() | (rest.value() | ...)) {}

  /** @brief Adds a VersionID to the set */
  constexpr void add(VersionIDT id) noexcept { bitmap_ |= id.value(); }

  /** @brief Removes a VersionID from the set */
  constexpr void remove(VersionIDT id) noexcept { bitmap_ &= ~id.value(); }

  /** @brief Returns true if the set contains the given VersionID */
  constexpr bool has(VersionIDT id) const noexcept {
    return (bitmap_ & id.value()) != 0;
  }

  /** @brief Returns true if the set contains all given VersionIDs */
  constexpr bool has(VersionIDSet ids) const noexcept {
    return (bitmap_ & ids.bitmap_) == ids.bitmap_;
  }

  /** @brief Returns true if the set contains any of the given VersionIDs */
  constexpr bool has_any(VersionIDSet ids) const noexcept {
    return (bitmap_ & ids.bitmap_) != 0;
  }

  /** @brief Returns the number of VersionIDs in the set */
  constexpr unsigned size() const noexcept { return es::count_bit1(bitmap_); }

  /** @brief Returns true if the set is empty */
  constexpr bool empty() const noexcept { return bitmap_ == 0; }

  /**
   * @brief Returns the VersionID at the given index.
   * @param index 0-based index (0 = first set bit)
   */
  constexpr VersionIDT at(unsigned index) const noexcept {
    return VersionIDT(es::countr_bit0(es::get_nth_bit(bitmap_, index)));
  }

  /** @brief Returns the first (lowest offset) VersionID in the set */
  constexpr VersionIDT first() const noexcept {
    return VersionIDT(es::countr_bit0(es::hl_first_bit1(bitmap_)));
  }

  /** @brief Same as at(index) */
  constexpr VersionIDT operator[](unsigned index) const noexcept {
    return at(index);
  }

  /** @brief Set union */
  constexpr VersionIDSet operator|(VersionIDSet other) const noexcept {
    return VersionIDSet(bitmap_ | other.bitmap_);
  }

  /** @brief Set union with a single VersionID */
  constexpr VersionIDSet operator|(VersionIDT id) const noexcept {
    return VersionIDSet(bitmap_ | id.value());
  }

  /** @brief Set union assignment */
  constexpr VersionIDSet& operator|=(VersionIDSet other) noexcept {
    bitmap_ |= other.bitmap_;
    return *this;
  }

  /** @brief Set union assignment with a single VersionID */
  constexpr VersionIDSet& operator|=(VersionIDT id) noexcept {
    bitmap_ |= id.value();
    return *this;
  }

  /** @brief Set intersection */
  constexpr VersionIDSet operator&(VersionIDSet other) const noexcept {
    return VersionIDSet(bitmap_ & other.bitmap_);
  }

  /** @brief Set intersection with a single VersionID */
  constexpr VersionIDSet operator&(VersionIDT id) const noexcept {
    return VersionIDSet(bitmap_ & id.value());
  }

  /** @brief Set intersection assignment */
  constexpr VersionIDSet& operator&=(VersionIDSet other) noexcept {
    bitmap_ &= other.bitmap_;
    return *this;
  }

  /** @brief Set intersection assignment with a single VersionID */
  constexpr VersionIDSet& operator&=(VersionIDT id) noexcept {
    bitmap_ &= id.value();
    return *this;
  }

  /**
   * @brief Iterates over all VersionIDs in the set.
   * @param func Callable taking a VersionID parameter
   */
  template <typename Func>
  constexpr void foreach (Func&& func) const noexcept {
    bitset_type remaining = bitmap_;
    while (remaining) {
      unsigned off = es::countr_bit0(remaining);
      func(VersionIDT(off));
      remaining = es::reset_first(remaining);
    }
  }

private:
  constexpr explicit VersionIDSet(bitset_type bits) noexcept : bitmap_(bits) {}

  bitset_type bitmap_ = 0;
};

/**
 * @brief Base struct for version information.
 *
 * Can be extended with additional fields by inheritance.
 *
 * @tparam VersionID The VersionID type for this info
 *
 * Example:
 * @code
 * struct MyVersionInfo : VersionInfo<VersionID<uint32_t>> {
 *     int major, minor, patch;
 * };
 * @endcode
 */
template <typename VersionIDT>
struct VersionInfo {
  VersionIDT id;              /**< The version identifier */
  const char* name = nullptr; /**< Optional version name */

  /** @brief Default constructor */
  constexpr VersionInfo() noexcept = default;

  /**
   * @brief Constructs a VersionInfo with id and name.
   * @param v The version identifier
   * @param n The version name (optional)
   */
  constexpr VersionInfo(VersionIDT v, const char* n) noexcept
      : id(v), name(n) {}

  /**
   * @brief Casts this to a derived type.
   * @tparam Derived The derived VersionInfo type
   */
  template <typename Derived>
  const Derived* as() const noexcept {
    return static_cast<const Derived*>(this);
  }

  template <typename Derived>
  Derived* as() noexcept {
    return static_cast<Derived*>(this);
  }
};

/**
 * @brief Manages registration and lookup of version information.
 *
 * Provides efficient storage and retrieval of VersionInfo objects
 * indexed by VersionID. Manages object lifetime using placement new.
 *
 * @tparam VersionInfo The VersionInfo type to manage
 *
 * Example:
 * @code
 * using VID = VersionID<uint32_t>;
 * using VInfo = VersionInfo<VID>;
 * VersionManager<VInfo> mgr;
 *
 * auto [ptr, inserted] = mgr.emplace(VID(0), VID(0), "v1.0");
 * if (inserted) {
 *     std::cout << "Added version: " << ptr->name << "\n";
 * }
 *
 * if (auto* info = mgr.find(VID(0))) {
 *     std::cout << "Found: " << info->name << "\n";
 * }
 * @endcode
 */
template <typename VersionInfoT>
class VersionManager {
public:
  using VersionIDT = decltype(VersionInfoT::id);
  using VersionIDSetT = VersionIDSet<VersionIDT>;

  /** @brief Default constructor */
  constexpr VersionManager() noexcept = default;

  /** @brief Destructor, destroys all managed VersionInfo objects */
  ~VersionManager() noexcept {
    ids_.foreach ([this](VersionIDT id) { std::destroy_at(at(id.offset())); });
  }

  /** @brief Returns the maximum number of versions that can be stored */
  static constexpr unsigned capacity() noexcept {
    return sizeof(typename VersionIDT::bitset_type) * 8;
  }

  /** @brief Returns the number of registered versions */
  unsigned size() const noexcept { return ids_.size(); }

  /**
   * @brief Returns the VersionInfo at the given offset.
   * @param index The bit offset (not validated)
   * @return Pointer to the VersionInfo (may be uninitialized if not registered)
   */
  VersionInfoT* at(unsigned index) noexcept {
    return reinterpret_cast<VersionInfoT*>(infos_ +
                                           index * sizeof(VersionInfoT));
  }

  const VersionInfoT* at(unsigned index) const noexcept {
    return reinterpret_cast<const VersionInfoT*>(infos_ +
                                                 index * sizeof(VersionInfoT));
  }

  /**
   * @brief Constructs a VersionInfo in-place.
   * @param id The version identifier
   * @param args Arguments to forward to VersionInfo constructor
   * @return Pair of (pointer, inserted) like std::map::insert
   */
  template <typename... Args>
  std::pair<VersionInfoT*, bool> emplace(VersionIDT id, Args&&... args) {
    if (ids_.has(id)) {
      return {at(id.offset()), false};
    }
    auto off = id.offset();
    VersionInfoT* ptr = at(off);
    ::new (static_cast<void*>(ptr)) VersionInfoT(std::forward<Args>(args)...);
    ids_ |= id;
    return {ptr, true};
  }

  std::pair<VersionInfoT*, bool> add(VersionIDT id, VersionInfoT info) {
    if (ids_.has(id)) {
      return {at(id.offset()), false};
    }
    VersionInfoT* ptr = at(id.offset());
    ::new (static_cast<void*>(ptr)) VersionInfoT(std::move(info));
    ids_ |= id;
    return {ptr, true};
  }

  /**
   * @brief Finds a VersionInfo by VersionID.
   * @param id The version identifier to find
   * @return Pointer to the VersionInfo, or nullptr if not found
   */
  VersionInfoT* find(VersionIDT id) noexcept {
    if (ids_.has(id)) {
      return at(id.offset());
    }
    return nullptr;
  }

  const VersionInfoT* find(VersionIDT id) const noexcept {
    if (ids_.has(id)) {
      return at(id.offset());
    }
    return nullptr;
  }

  /**
   * @brief Iterates over all registered VersionInfo objects.
   * @param func Callable taking a VersionInfo& parameter
   */
  template <typename Func>
  void foreach (Func&& func) noexcept {
    ids_.foreach ([this, &func](VersionIDT id) { func(*at(id.offset())); });
  }

  template <typename Func>
  void foreach (Func&& func) const noexcept {
    ids_.foreach ([this, &func](VersionIDT id) { func(*at(id.offset())); });
  }

  /** @brief Returns true if the given VersionID is registered */
  bool has(VersionIDT id) const noexcept { return ids_.has(id); }

  /** @brief Returns true if all given VersionIDs are registered */
  bool has(VersionIDSetT ids) const noexcept { return ids_.has(ids); }

  /** @brief Returns true if any of the given VersionIDs are registered */
  bool has_any(VersionIDSetT ids) const noexcept { return ids_.has_any(ids); }

private:
  VersionIDSetT ids_;
  alignas(VersionInfoT) std::byte infos_[sizeof(VersionInfoT) * capacity()];
};

} // namespace cxxaux

#endif // CXXAUX_UTILITY_VERSION_H
