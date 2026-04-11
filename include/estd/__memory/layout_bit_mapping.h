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

#ifndef ESTD_MEMORY_LAYOUT_BIT_MAPPING_H
#define ESTD_MEMORY_LAYOUT_BIT_MAPPING_H
#include <cassert>
#include <cstring>
#include <estd/__bit/bit_count.h>
#include <estd/__bit/bit_set_reset.h>
#include <estd/__utility/bits_of.h>
#include <estd/__utility/bytes_of.h>
#include <iterator>

namespace es { namespace memory {

namespace __impl_layout_bit_mapping {

template <size_t MemoryBytes, size_t MemberBytes, typename MappingType,
          unsigned Hierarchy>
class layout_bit_mapping_impl;

template <size_t MemoryBytes, size_t MemberBytes, typename MappingType>
class layout_bit_mapping_impl<MemoryBytes, MemberBytes, MappingType, 1> {
public:
  // `bytes_of<MappingType> + n_member * MemberBytes = MemoryBytes`
  // the maximum of `n_member` is bits_of<MappingType>
  static_assert(
      MemoryBytes <=
          (bytes_of<MappingType> + bits_of<MappingType> * MemberBytes),
      "The memory bytes is provided too large to do a mapping. The maxium "
      "MemoryBytes should be `bytesof(MappingType) + bitsof(MappingType) * "
      "MemberBytes)`.");

  static constexpr size_t member_bytes_offset = bytes_of<MappingType>;

  static constexpr size_t member_capacity =
      (MemoryBytes - member_bytes_offset) / MemberBytes;

  static constexpr size_t memory_tail_bytes_unmapping =
      MemoryBytes - member_bytes_offset - member_capacity * MemberBytes;

  layout_bit_mapping_impl(char* mem) : memory_(mem) {}

  void reset() {
    auto* bit_mapping = reinterpret_cast<MappingType*>(memory_);
    // clear the bits mapping that maps a member for later acquire
    // set the bits mapping that maps no member just as used
    *bit_mapping = set_bits(MappingType{}, member_capacity,
                            bits_of<MappingType> - member_capacity);
  }

  void* acquire() {
    auto* bit_mapping = reinterpret_cast<MappingType*>(memory_);
    if (*bit_mapping == std::numeric_limits<MappingType>::max())
      return nullptr;

    // get the index that mapping a member not acquired
    auto n = countr_bit1(*bit_mapping);
    // mark the member mapping used
    *bit_mapping = set_bit(*bit_mapping, n);
    // return the member
    return std::next(memory_, member_bytes_offset + n * MemberBytes);
  }

  bool release(void* p) {
    auto distance = std::distance(memory_, static_cast<char*>(p));
    if (distance < static_cast<std::ptrdiff_t>(member_bytes_offset) ||
        distance >= (MemoryBytes - memory_tail_bytes_unmapping))
      return false;

    assert((distance - member_bytes_offset) % MemberBytes == 0 &&
           "can not release a member with a non-header pointer");

    auto* bit_mapping = reinterpret_cast<MappingType*>(memory_);
    // get the index that the member mapping
    auto offset = (distance - member_bytes_offset) / MemberBytes;
    // mark the member mapping unused
    *bit_mapping &= ~set_bit(MappingType{}, offset);
    return true;
  }

private:
  char* memory_ = nullptr;
};

template <size_t MemoryBytes, size_t MemberBytes, typename MappingType>
class layout_bit_mapping_impl<MemoryBytes, MemberBytes, MappingType, 2> {
public:
  // clang-format off
  // bytes_of<MappingType> + n_bit_mapping * bytes_of<MappingType> + n_member * MemberBytes = MemoryBytes
  // the maximum of `n_bit_mapping` is `bits_of<MappingType>`
  // the maximum of `n_member` is `bits_of<MappingType> * bits_of<MappingType>`
  // clang-format on
  static_assert(
      MemoryBytes <=
          (bytes_of<MappingType> +
           bits_of<MappingType> * bytes_of<MappingType> +
           bits_of<MappingType> * bits_of<MappingType> * MemberBytes),
      "The memory bytes is provided too large to do a mapping. The maxium "
      "MemoryBytes should be `bytesof(MappingType) + bitsof(MappingType) * "
      "bytesof(MappingType) + bitsof(MappingType) * bitsof(MappingType) * "
      "MemberBytes)`.");

  // clang-format off
  // bytes_of<MappingType> + n_bit_mapping * bytes_of<MappingType> + n_member * MemberBytes = MemoryBytes
  // => n_member = (MemoryBytes - bytes_of<MappingType> - n_bit_mapping * bytes_of<MappingType>) / MemberBytes
  //
  // the maximum of `n_bit_mapping` is `bits_of<MappingType>`
  // the maximum of `n_member` is `bits_of<MappingType> * bits_of<MappingType>`

  // n_bit_mapping * bits_of<MappingType> >= n_member
  // => n_bit_mapping * bits_of<MappingType> >=
  //    (MemoryBytes - bytes_of<MappingType> - n_bit_mapping * bytes_of<MappingType>) / MemberBytes
  // => n_bit_mapping >= (MemoryBytes - bytes_of<MappingType>) /
  //                     (bits_of<MappingType> * MemberBytes + bytes_of<MappingType>)
  // => n_bit_mapping = (MemoryBytes - bytes_of<MappingType> + bits_of<MappingType> * MemberBytes + bytes_of<MappingType> - 1) /
  //                    (bits_of<MappingType> * MemberBytes + bytes_of<MappingType>)
  // => n_bit_mapping = (MemoryBytes + bits_of<MappingType> * MemberBytes -1) /
  //                    (bits_of<MappingType> * MemberBytes + bytes_of<MappingType>)
  // clang-format on
  static constexpr size_t h2_bit_mapping_count =
      (MemoryBytes + bits_of<MappingType> * MemberBytes - 1) /
      (bits_of<MappingType> * MemberBytes + bytes_of<MappingType>);

  static constexpr size_t member_bytes_offset =
      (1 + h2_bit_mapping_count) * bytes_of<MappingType>;

  static constexpr size_t member_capacity =
      (MemoryBytes - member_bytes_offset) / MemberBytes;

  static constexpr size_t memory_tail_bytes_unmapping =
      MemoryBytes - member_bytes_offset - member_capacity * MemberBytes;

  layout_bit_mapping_impl(char* mem) : memory_(mem) {}

  void reset() {
    constexpr size_t h1_bit_valid =
        (h2_bit_mapping_count + bits_of<MappingType> - 1) /
        bits_of<MappingType>;
    constexpr size_t h2_tail_bit_valid =
        h2_bit_mapping_count % bits_of<MappingType>;

    auto* h1_bit_mapping = reinterpret_cast<MappingType*>(memory_);
    // clear the 1st bits mapping that maps the 2nd bits mappings
    // set the 1st mapping bits that maps no 2nd mapping just as used
    *h1_bit_mapping = set_bits(MappingType{}, h1_bit_valid,
                               bits_of<MappingType> - h1_bit_valid);

    MappingType* h2_bit_mapping = std::next(h1_bit_mapping);
    // clear the 2nd bits mapping that maps a member for later acquire
    // set the bits mapping that maps no member just as used
    std::memset(h2_bit_mapping, 0,
                h2_bit_mapping_count * bytes_of<MappingType>);
    h2_bit_mapping[h2_bit_mapping_count - 1] =
        set_bits(MappingType{}, h2_tail_bit_valid,
                 bits_of<MappingType> - h2_tail_bit_valid);
  }

  void* acquire() {
    auto* h1_bit_mapping = reinterpret_cast<MappingType*>(memory_);
    if (*h1_bit_mapping == std::numeric_limits<MappingType>::max())
      return nullptr;

    // get the index that mapping a member not acquired
    auto n = countr_bit1(*h1_bit_mapping);
    MappingType* h2_bit_mapping = std::next(h1_bit_mapping, 1 + n);
    auto m = countr_bit1(*h2_bit_mapping);
    // mark the member mapping used
    *h2_bit_mapping = set_bit(*h2_bit_mapping, m);
    if (*h2_bit_mapping == std::numeric_limits<MappingType>::max())
      *h1_bit_mapping = set_bit(*h1_bit_mapping, n);
    // return the member
    return std::next(memory_, member_bytes_offset +
                                  (n * bits_of<MappingType> + m) * MemberBytes);
  }

  bool release(void* p) {
    auto distance = std::distance(memory_, static_cast<char*>(p));
    if (distance < static_cast<std::ptrdiff_t>(member_bytes_offset) ||
        distance >= (MemoryBytes - memory_tail_bytes_unmapping))
      return false;

    assert((distance - member_bytes_offset) % MemberBytes == 0 &&
           "can not release a member with a non-header pointer");

    auto* h1_bit_mapping = reinterpret_cast<MappingType*>(memory_);
    MappingType* h2_bit_mapping = std::next(h1_bit_mapping);

    // get the index that the member mapping
    auto offset = (distance - member_bytes_offset) / MemberBytes;
    auto n = offset / bits_of<MappingType>;
    auto m = offset % bits_of<MappingType>;
    // mark the member mapping unused
    *std::next(h2_bit_mapping, n) &= ~set_bit(MappingType{}, m);
    *h1_bit_mapping &= ~set_bit(MappingType{}, n);
    return true;
  }

private:
  char* memory_ = nullptr;
};

template <size_t MemoryBytes, size_t MemberBytes, typename MappingType>
struct layout_bit_mapping_selector {
  // to deduce which hierarchy is better
  static constexpr unsigned hierarchy =
      MemoryBytes <=
              (sizeof(MappingType) + sizeof(MappingType) * 8 * MemberBytes)
          ? 1
          : 2;
  using value_type =
      layout_bit_mapping_impl<MemoryBytes, MemberBytes, MappingType, hierarchy>;
};
template <size_t MemoryBytes, size_t MemberBytes, typename MappingType>
using layout_bit_mapping_selector_t =
    typename layout_bit_mapping_selector<MemoryBytes, MemberBytes,
                                         MappingType>::value_type;

} // namespace __impl_layout_bit_mapping

/// @brief Configure the memory with the leading N bits mapping the following
/// fixed size member
///
/// For the hierarchy is one, the memory will be designed as
///     header                 data             tail
/// m1: | bits_of<MappingType> | {n}MemberBytes | ? |
/// tips: the tail unmapping is small than on MemberBytes that can not alloc
///
/// For the hierarchy is two, the memory will be designed as
///     1st header             2st headers               data             tail
/// m1: | bits_of<MappingType> | {m}bits_of<MappingType> | {n}MemberBytes | ? |
/// tips: the 1st header is a mapping to 2st headers
///
/// @tparam MemoryBytes total memory bytes can be used to design
/// @tparam MemberBytes bytes of each member
/// @tparam MappingType type to hold the mapping info in header
template <size_t MemoryBytes, size_t MemberBytes,
          typename MappingType = unsigned long long>
class layout_bit_mapping
    : __impl_layout_bit_mapping::layout_bit_mapping_selector_t<
          MemoryBytes, MemberBytes, MappingType> {
  using impl_type = __impl_layout_bit_mapping::layout_bit_mapping_selector_t<
      MemoryBytes, MemberBytes, MappingType>;

public:
  static constexpr size_t memory_bytes = MemoryBytes;

  static constexpr size_t member_bytes = MemberBytes;

  using mapping_type = MappingType;

  /// @brief Debug usage, the bytes offset is byte distance form the begin of
  /// member data to the memory begin
  static constexpr size_t member_bytes_offset = impl_type::member_bytes_offset;

  /// @brief Debug usage, the capacity that memory can provide
  static constexpr size_t member_capacity = impl_type::member_capacity;

  /// @brief There may be some memory less that a member bytes can not be used,
  /// it is reported here for later use
  static constexpr size_t memory_tail_bytes_unmapping =
      impl_type::memory_tail_bytes_unmapping;

  layout_bit_mapping(char* mem) : impl_type(mem) {}

  /// @brief Initial the memory a new bit mapping with all headers pre-setted
  void reset() { impl_type::reset(); }

  /// @brief Acquire a member from the memory
  /// @return a memory address with MemberBytes, or nullptr that no valid memory
  void* acquire() { return impl_type::acquire(); }

  /// @brief Release a member to the memory which can be acquired later
  /// @param p pointer to a member that may in the memory
  /// @return true: succeed to release, flase: not belong to the memory
  bool release(void* p) { return impl_type::release(p); }
};

}} // namespace es::memory
#endif
