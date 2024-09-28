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

#ifndef ESTD_UTILITY_H
#define ESTD_UTILITY_H

#include "__utility/type_name.h"
// template <typename T>
// constexpr std::string_view type_name();
// template <typename T>
// constexpr std::string_view type_name(T&&);

#include "__utility/numeric_type.h"
// enum NumericTypeCode : uint16_t {
//   NTC_BOOL = __encodingNumericTypeCode<bool>(),
//   NTC_SIGNED_CHAR = __encodingNumericTypeCode<signed char>(),
//   NTC_UNSIGNED_CHAR = __encodingNumericTypeCode<unsigned char>(),
//   NTC_CHAR = __encodingNumericTypeCode<char>(),
//   NTC_CHAR16 = __encodingNumericTypeCode<char16_t>(),
//   NTC_CHAR32 = __encodingNumericTypeCode<char32_t>(),
//   NTC_WCHAR = __encodingNumericTypeCode<wchar_t>(),
//   NTC_SHORT = __encodingNumericTypeCode<short>(),
//   NTC_UNSIGNED_SHORT = __encodingNumericTypeCode<unsigned short>(),
//   NTC_INT = __encodingNumericTypeCode<int>(),
//   NTC_UNSIGNED_INT = __encodingNumericTypeCode<unsigned int>(),
//   NTC_LONG = __encodingNumericTypeCode<long>(),
//   NTC_UNSIGNED_LONG = __encodingNumericTypeCode<unsigned long>(),
//   NTC_LONGLONG = __encodingNumericTypeCode<long long>(),
//   NTC_UNSIGNED_LONGLONG = __encodingNumericTypeCode<unsigned long long>(),
//   NTC_HALF = __encodingNumericTypeCode<__half>(),
//   NTC_FLOAT = __encodingNumericTypeCode<float>(),
//   NTC_DOUBLE = __encodingNumericTypeCode<double>(),
//   NTC_LONGDOUBLE = __encodingNumericTypeCode<long double>(),
//
//   NTC_INT8 = __encodingNumericTypeCode<int8_t>(),
//   NTC_UINT8 = __encodingNumericTypeCode<uint8_t>(),
//   NTC_INT16 = __encodingNumericTypeCode<int16_t>(),
//   NTC_UINT16 = __encodingNumericTypeCode<uint16_t>(),
//   NTC_INT32 = __encodingNumericTypeCode<int32_t>(),
//   NTC_UINT32 = __encodingNumericTypeCode<uint32_t>(),
//   NTC_INT64 = __encodingNumericTypeCode<int64_t>(),
//   NTC_UINT64 = __encodingNumericTypeCode<uint64_t>()
//   NTC_FLOAT16 = __encodingNumericTypeCode<__half>(),
//   NTC_FLOAT32 = __encodingNumericTypeCode<float>(),
//   NTC_FLOAT64 = __encodingNumericTypeCode<double>()
// };
//
// class NumericType {
// public:
//   template <NumericTypeCode kCode>
//   using type = typename __nt_type_code_config<kCode>::type;
//
//   template <typename T>
//   static constexpr NumericTypeCode type_code();
//   template <typename T>
//   static constexpr NumericTypeCode type_code(T&&);
//
//   static constexpr bool is_unsigned(NumericTypeCode code);
//   static constexpr bool is_signed(NumericTypeCode code);
//   static constexpr bool is_floating_point(NumericTypeCode code);
//   static constexpr bool is_integral(NumericTypeCode code);
//   static constexpr bool is_character(NumericTypeCode code);
//   static constexpr uint16_t bit_size(NumericTypeCode code);
//
//   static constexpr std::string_view name(NumericTypeCode code);
//   static constexpr std::string_view fixed_width_name(NumericTypeCode code);
//   static constexpr std::string_view short_name(NumericTypeCode code);
// };

#include "__utility/disabled_copy_move.h"
// struct disabled_copy {
//   disabled_copy() = default;
//   disabled_copy(const disabled_copy&) = delete;
//   disabled_copy& operator=(const disabled_copy&) = delete;
// };
// struct disabled_move {
//   disabled_move() = default;
//   disabled_move(disabled_copy&&) = delete;
//   disabled_move& operator=(disabled_copy&&) = delete;
// };
// struct disabled_copy_move : disabled_copy, disabled_move {
//   disabled_copy_move() = default;
// };

#include "__utility/overloaded.h"
// template <class... Ts>
// struct overloaded : Ts... {
//   using Ts::operator()...;
// };

#endif
