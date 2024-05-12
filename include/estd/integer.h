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

#ifndef ESTD_INTEGER_H
#define ESTD_INTEGER_H

#include "__container/integer.h"
// template <typename T, typename = void,
//           typename = std::enable_if_t<std::is_integral_v<T>>>
// class Integer {
// public:
//   using value_type = T;
//
//   constexpr Integer() = default;
//
//   constexpr explicit Integer(value_type v);
//
//   constexpr Integer(const Integer& other);
//
//   constexpr Integer(Integer&& other) noexcept;
//
//   constexpr ~Integer() noexcept;
//
//   constexpr Integer& operator=(const Integer& other);
//
//   constexpr Integer& operator=(Integer&& other) noexcept;
//
//   constexpr void reset(value_type v = 0);
//
//   constexpr void swap(Integer& other) noexcept;
//
//   constexpr value_type get() const noexcept;
//
//   constexpr explicit operator T() const noexcept;
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator+=(const U& v)
//   constexpr Integer& operator+=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator-=(const U& v)
//   constexpr Integer& operator-=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator*=(const U& v)
//   constexpr Integer& operator*=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator/=(const U& v)
//   constexpr Integer& operator/=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator%=(const U& v)
//   constexpr Integer& operator%=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator&=(const U& v)
//   constexpr Integer& operator&=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator|=(const U& v)
//   constexpr Integer& operator|=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator^=(const U& v)
//   constexpr Integer& operator^=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator<<=(const U& v)
//   constexpr Integer& operator<<=(const Integer& v)
//
//   template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
//   constexpr Integer& operator>>=(const U& v)
//   constexpr Integer& operator>>=(const Integer& v)
//
//   constexpr Integer& operator++() noexcept;
//   constexpr Integer operator++(int);
//   constexpr Integer& operator--() noexcept;
//   constexpr Integer operator--(int);
// };
//
// template <typename T, typename Tag>
// constexpr bool operator==(const Integer<T, Tag>& lhs,
//                           const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator==(const Integer<T, Tag>& lhs, const U& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator==(const U& lhs, const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr bool operator!=(const Integer<T, Tag>& lhs,
//                           const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator!=(const Integer<T, Tag>& lhs, const U& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator!=(const U& lhs, const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr bool operator<(const Integer<T, Tag>& lhs,
//                          const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator<(const Integer<T, Tag>& lhs, const U& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator<(const U& lhs, const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr bool operator>(const Integer<T, Tag>& lhs,
//                          const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator>(const Integer<T, Tag>& lhs, const U& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator>(const U& lhs, const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr bool operator<=(const Integer<T, Tag>& lhs,
//                           const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator<=(const Integer<T, Tag>& lhs, const U& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator<=(const U& lhs, const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr bool operator>=(const Integer<T, Tag>& lhs,
//                           const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator>=(const Integer<T, Tag>& lhs, const U& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr bool operator>=(const U& lhs, const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator+(const Integer<T, Tag>& v);
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator-(const Integer<T, Tag>& v);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator+(const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator+(const Integer<T, Tag>& lhs,
//                                     const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator+(const U & lhs,
//                                     const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator-(const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator-(const Integer<T, Tag>& lhs,
//                                     const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator-(const U & lhs,
//                                     const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator*(const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator*(const Integer<T, Tag>& lhs,
//                                     const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator*(const U & lhs,
//                                     const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator/(const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator/(const Integer<T, Tag>& lhs,
//                                     const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator/(const U & lhs,
//                                     const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator~(const Integer<T, Tag>& v);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator&(const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator&(const Integer<T, Tag>& lhs,
//                                     const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator&(const U & lhs,
//                                     const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator|(const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator|(const Integer<T, Tag>& lhs,
//                                     const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator|(const U & lhs,
//                                     const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator^(const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator^(const Integer<T, Tag>& lhs,
//                                     const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator^(const U & lhs,
//                                     const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator<<(const Integer<T, Tag>& lhs,
//                                      const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator<<(const Integer<T, Tag>& lhs,
//                                      const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator<<(const U & lhs,
//                                      const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator>>const Integer<T, Tag>& lhs,
//                                     const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator>>(const Integer<T, Tag>& lhs,
//                                      const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator>>(const U & lhs,
//                                      const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator!(const Integer<T, Tag>& v);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator&&(const Integer<T, Tag>& lhs,
//                                      const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator&&(const Integer<T, Tag>& lhs,
//                                      const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator&&(const U & lhs,
//                                      const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag>
// constexpr Integer<T, Tag> operator||(const Integer<T, Tag>& lhs,
//                                      const Integer<T, Tag>& rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator||(const Integer<T, Tag>& lhs,
//                                      const U & rhs);
// template <typename T, typename Tag, typename U,
//           std::enable_if_t<std::is_integral_v<U>>>
// constexpr Integer<T, Tag> operator||(const U & lhs,
//                                      const Integer<T, Tag>& rhs);
//
// template <typename T, typename Tag, typename OS>
// constexpr OS& operator<<(OS& os, const Integer<T, Tag>& v);

namespace es { namespace units {

class __BitTag;
using Bit = Integer<uint32_t, __BitTag>;
constexpr Bit operator""_b(unsigned long long int v) {
  return Bit{static_cast<uint32_t>(v)};
}

class __ByteTag;
using Byte = Integer<uint32_t, __ByteTag>;
constexpr Byte operator""_B(unsigned long long int v) {
  return Byte{static_cast<uint32_t>(v)};
}

class __KiloByteTag;
using KiloByte = Integer<uint32_t, __KiloByteTag>;
constexpr KiloByte operator""_KB(unsigned long long int v) {
  return KiloByte{static_cast<uint32_t>(v)};
}

class __MegaByteTag;
using MegaByte = Integer<uint32_t, __MegaByteTag>;
constexpr MegaByte operator""_MB(unsigned long long int v) {
  return MegaByte{static_cast<uint32_t>(v)};
}

class __GigaByteTag;
using GigaByte = Integer<uint32_t, __GigaByteTag>;
constexpr GigaByte operator""_GB(unsigned long long int v) {
  return GigaByte{static_cast<uint32_t>(v)};
}

class __TeraByteTag;
using TeraByte = Integer<uint32_t, __TeraByteTag>;
constexpr TeraByte operator""_TB(unsigned long long int v) {
  return TeraByte{static_cast<uint32_t>(v)};
}

class __PetaByteTag;
using PetaByte = Integer<uint32_t, __PetaByteTag>;
constexpr PetaByte operator""_PB(unsigned long long int v) {
  return PetaByte{static_cast<uint32_t>(v)};
}

}} // namespace es::units
#endif
