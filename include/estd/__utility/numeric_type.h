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

#ifndef ESTD___UTILITY_NUMERIC_TYPE_H
#define ESTD___UTILITY_NUMERIC_TYPE_H
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace es {

// define inner type and flags
enum : uint8_t {
  __NT_BEGIN = 0,
  __NT_BOOL = __NT_BEGIN,
  __NT_CHAR,
  __NT_SHORT,
  __NT_INT,
  __NT_LONG,
  __NT_LONGLONG,
  __NT_FLOAT,
  __NT_DOUBLE,
  __NT_LONG_DOUBLE,
  __NT_WCHAR,
  __NT_SIZE,

  __NT_FLAGS_BEGIN = 0b1 << 4,
  __NT_FLAGS_UNSIGNED = __NT_FLAGS_BEGIN,
  __NT_FLAGS_FLOATING = __NT_FLAGS_BEGIN << 1,
  __NT_FLAGS_CHARACTER = __NT_FLAGS_BEGIN << 2,
};
static_assert(__NT_SIZE < __NT_FLAGS_BEGIN);

// an extened type to represet float 16, which is a c++23 feature
struct __half {
  uint8_t v[2];
};

// config properties for the fixed width type
template <bool IsSigned, uint8_t Bits, bool IsFloat = false>
struct __nt_fixed_width_type_config;

template <>
struct __nt_fixed_width_type_config<false, 1> {
  using type = bool;
  static constexpr const char name[] = "bool";
  static constexpr const char short_name[] = "u1";
};

template <>
struct __nt_fixed_width_type_config<true, 8> {
  using type = int8_t;
  static constexpr const char name[] = "int8_t";
  static constexpr const char short_name[] = "i8";
};

template <>
struct __nt_fixed_width_type_config<false, 8> {
  using type = uint8_t;
  static constexpr const char name[] = "uint8_t";
  static constexpr const char short_name[] = "u8";
};

template <>
struct __nt_fixed_width_type_config<true, 16> {
  using type = int16_t;
  static constexpr const char name[] = "int16_t";
  static constexpr const char short_name[] = "i16";
};

template <>
struct __nt_fixed_width_type_config<false, 16> {
  using type = uint16_t;
  static constexpr const char name[] = "uint16_t";
  static constexpr const char short_name[] = "u16";
};

template <>
struct __nt_fixed_width_type_config<true, 32> {
  using type = int32_t;
  static constexpr const char name[] = "int32_t";
  static constexpr const char short_name[] = "i32";
};

template <>
struct __nt_fixed_width_type_config<false, 32> {
  using type = uint32_t;
  static constexpr const char name[] = "uint32_t";
  static constexpr const char short_name[] = "u32";
};

template <>
struct __nt_fixed_width_type_config<true, 64> {
  using type = int64_t;
  static constexpr const char name[] = "int64_t";
  static constexpr const char short_name[] = "i64";
};

template <>
struct __nt_fixed_width_type_config<false, 64> {
  using type = uint64_t;
  static constexpr const char name[] = "uint64_t";
  static constexpr const char short_name[] = "u64";
};

template <>
struct __nt_fixed_width_type_config<true, 16, true> {
  using type = __half;
  static constexpr const char name[] = "float16_t";
  static constexpr const char short_name[] = "f16";
};

template <>
struct __nt_fixed_width_type_config<true, 32, true> {
  using type = float;
  static constexpr const char name[] = "float32_t";
  static constexpr const char short_name[] = "f32";
};

template <>
struct __nt_fixed_width_type_config<true, 64, true> {
  using type = double;
  static constexpr const char name[] = "float64_t";
  static constexpr const char short_name[] = "f64";
};

template <>
struct __nt_fixed_width_type_config<true, 80, true> {
  using type = long double;
  static constexpr const char name[] = "float80_t";
  static constexpr const char short_name[] = "f80";
};

template <>
struct __nt_fixed_width_type_config<true, 128, true> {
  using type = long double;
  static constexpr const char name[] = "float128_t";
  static constexpr const char short_name[] = "f128";
};

// config properties for the numeric type
template <typename>
struct __nt_type_config;

template <>
struct __nt_type_config<bool> {
  using type = bool;
  static constexpr const char name[] = "bool";
  static constexpr uint8_t base_type_code = __NT_BOOL;
  static constexpr uint8_t flags = __NT_FLAGS_UNSIGNED;
};

template <>
struct __nt_type_config<signed char> {
  using type = signed char;
  static constexpr const char name[] = "signed char";
  static constexpr uint8_t base_type_code = __NT_CHAR;
  static constexpr uint8_t flags = 0;
};

template <>
struct __nt_type_config<unsigned char> {
  using type = unsigned char;
  static constexpr const char name[] = "unsigned char";
  static constexpr uint8_t base_type_code = __NT_CHAR;
  static constexpr uint8_t flags = __NT_FLAGS_UNSIGNED;
};

template <>
struct __nt_type_config<char> {
  using type = char;
  static constexpr const char name[] = "char";
  static constexpr uint8_t base_type_code = __NT_CHAR;
  static constexpr uint8_t flags =
      __NT_FLAGS_CHARACTER |
      (std::is_unsigned_v<type> ? __NT_FLAGS_UNSIGNED : 0);
};

template <>
struct __nt_type_config<char16_t> {
  using type = char16_t;
  static constexpr const char name[] = "char16_t";
  static constexpr uint8_t base_type_code = __NT_CHAR;
  static constexpr uint8_t flags =
      __NT_FLAGS_CHARACTER |
      (std::is_unsigned_v<type> ? __NT_FLAGS_UNSIGNED : 0);
};

template <>
struct __nt_type_config<char32_t> {
  using type = char32_t;
  static constexpr const char name[] = "char32_t";
  static constexpr uint8_t base_type_code = __NT_CHAR;
  static constexpr uint8_t flags =
      __NT_FLAGS_CHARACTER |
      (std::is_unsigned_v<type> ? __NT_FLAGS_UNSIGNED : 0);
};

template <>
struct __nt_type_config<wchar_t> {
  using type = wchar_t;
  static constexpr const char name[] = "wchar_t";
  static constexpr uint8_t base_type_code = __NT_WCHAR;
  static constexpr uint8_t flags =
      __NT_FLAGS_CHARACTER |
      (std::is_unsigned_v<type> ? __NT_FLAGS_UNSIGNED : 0);
};

template <>
struct __nt_type_config<short> {
  using type = short;
  static constexpr const char name[] = "short";
  static constexpr uint8_t base_type_code = __NT_SHORT;
  static constexpr uint8_t flags = 0;
};

template <>
struct __nt_type_config<unsigned short> {
  using type = unsigned short;
  static constexpr const char name[] = "unsigned short";
  static constexpr uint8_t base_type_code = __NT_SHORT;
  static constexpr uint8_t flags = __NT_FLAGS_UNSIGNED;
};

template <>
struct __nt_type_config<int> {
  using type = int;
  static constexpr const char name[] = "int";
  static constexpr uint8_t base_type_code = __NT_INT;
  static constexpr uint8_t flags = 0;
};

template <>
struct __nt_type_config<unsigned int> {
  using type = unsigned int;
  static constexpr const char name[] = "unsigned int";
  static constexpr uint8_t base_type_code = __NT_INT;
  static constexpr uint8_t flags = __NT_FLAGS_UNSIGNED;
};

template <>
struct __nt_type_config<long> {
  using type = long;
  static constexpr const char name[] = "long";
  static constexpr uint8_t base_type_code = __NT_LONG;
  static constexpr uint8_t flags = 0;
};

template <>
struct __nt_type_config<unsigned long> {
  using type = unsigned long;
  static constexpr const char name[] = "unsigned long";
  static constexpr uint8_t base_type_code = __NT_LONG;
  static constexpr uint8_t flags = __NT_FLAGS_UNSIGNED;
};

template <>
struct __nt_type_config<long long> {
  using type = long long;
  static constexpr const char name[] = "long long";
  static constexpr uint8_t base_type_code = __NT_LONGLONG;
  static constexpr uint8_t flags = 0;
};

template <>
struct __nt_type_config<unsigned long long> {
  using type = unsigned long long;
  static constexpr const char name[] = "unsigned long long";
  static constexpr uint8_t base_type_code = __NT_LONGLONG;
  static constexpr uint8_t flags = __NT_FLAGS_UNSIGNED;
};

template <>
struct __nt_type_config<__half> {
  using type = __half;
  static constexpr const char name[] = "half";
  static constexpr uint8_t base_type_code = __NT_FLOAT;
  static constexpr uint8_t flags = __NT_FLAGS_FLOATING;
};

template <>
struct __nt_type_config<float> {
  using type = float;
  static constexpr const char name[] = "float";
  static constexpr uint8_t base_type_code = __NT_FLOAT;
  static constexpr uint8_t flags = __NT_FLAGS_FLOATING;
};

template <>
struct __nt_type_config<double> {
  using type = double;
  static constexpr const char name[] = "double";
  static constexpr uint8_t base_type_code = __NT_DOUBLE;
  static constexpr uint8_t flags = __NT_FLAGS_FLOATING;
};

template <>
struct __nt_type_config<long double> {
  using type = long double;
  static constexpr const char name[] = "long double";
  static constexpr uint8_t base_type_code = __NT_LONG_DOUBLE;
  static constexpr uint8_t flags = __NT_FLAGS_FLOATING;
};

// help to generate NumericTypeCode
constexpr uint16_t __encodingNumericTypeCode(uint8_t typ, uint8_t bits,
                                             uint8_t flags) {
  return (static_cast<uint16_t>(typ | flags) << 8) |
         static_cast<uint16_t>(bits);
}

// help to generate NumericTypeCode
template <typename T>
constexpr uint32_t __encodingNumericTypeCode() {
  constexpr uint8_t bits = std::is_same_v<T, bool> ? 1 : sizeof(T) * 8;
  return __encodingNumericTypeCode(__nt_type_config<T>::base_type_code, bits,
                                   __nt_type_config<T>::flags);
}

/// @brief Numeric type code enumeration for compile-time type identification.
///
/// Each numeric type is encoded as a 16-bit value with the following structure:
/// - Bits 0-7: Size in bits (e.g., 8, 16, 32, 64)
/// - Bits 8-15: Type category and flags (unsigned, floating, character)
///
/// This encoding allows runtime type queries while maintaining compile-time
/// type safety through the type_code system.
enum NumericTypeCode : uint16_t {
  NTC_BOOL = __encodingNumericTypeCode<bool>(),
  NTC_SIGNED_CHAR = __encodingNumericTypeCode<signed char>(),
  NTC_UNSIGNED_CHAR = __encodingNumericTypeCode<unsigned char>(),
  NTC_CHAR = __encodingNumericTypeCode<char>(),
  NTC_CHAR16 = __encodingNumericTypeCode<char16_t>(),
  NTC_CHAR32 = __encodingNumericTypeCode<char32_t>(),
  NTC_WCHAR = __encodingNumericTypeCode<wchar_t>(),
  NTC_SHORT = __encodingNumericTypeCode<short>(),
  NTC_UNSIGNED_SHORT = __encodingNumericTypeCode<unsigned short>(),
  NTC_INT = __encodingNumericTypeCode<int>(),
  NTC_UNSIGNED_INT = __encodingNumericTypeCode<unsigned int>(),
  NTC_LONG = __encodingNumericTypeCode<long>(),
  NTC_UNSIGNED_LONG = __encodingNumericTypeCode<unsigned long>(),
  NTC_LONGLONG = __encodingNumericTypeCode<long long>(),
  NTC_UNSIGNED_LONGLONG = __encodingNumericTypeCode<unsigned long long>(),
  NTC_HALF = __encodingNumericTypeCode<__half>(),
  NTC_FLOAT = __encodingNumericTypeCode<float>(),
  NTC_DOUBLE = __encodingNumericTypeCode<double>(),
  NTC_LONGDOUBLE = __encodingNumericTypeCode<long double>(),

  // alias name with fixed size
  NTC_INT8 = __encodingNumericTypeCode<int8_t>(),
  NTC_UINT8 = __encodingNumericTypeCode<uint8_t>(),
  NTC_INT16 = __encodingNumericTypeCode<int16_t>(),
  NTC_UINT16 = __encodingNumericTypeCode<uint16_t>(),
  NTC_INT32 = __encodingNumericTypeCode<int32_t>(),
  NTC_UINT32 = __encodingNumericTypeCode<uint32_t>(),
  NTC_INT64 = __encodingNumericTypeCode<int64_t>(),
  NTC_UINT64 = __encodingNumericTypeCode<uint64_t>(),
  NTC_FLOAT16 = __encodingNumericTypeCode<__half>(),
  NTC_FLOAT32 = __encodingNumericTypeCode<float>(),
  NTC_FLOAT64 = __encodingNumericTypeCode<double>()
};

// config properties from NumericTypeCode to numeric type
template <NumericTypeCode kCode>
struct __nt_type_code_config;

template <>
struct __nt_type_code_config<NTC_BOOL> {
  using type = bool;
};

template <>
struct __nt_type_code_config<NTC_SIGNED_CHAR> {
  using type = signed char;
};

template <>
struct __nt_type_code_config<NTC_UNSIGNED_CHAR> {
  using type = unsigned char;
};

template <>
struct __nt_type_code_config<NTC_CHAR> {
  using type = char;
};

template <>
struct __nt_type_code_config<NTC_CHAR16> {
  using type = char16_t;
};

template <>
struct __nt_type_code_config<NTC_CHAR32> {
  using type = char32_t;
};

template <>
struct __nt_type_code_config<NTC_WCHAR> {
  using type = wchar_t;
};

template <>
struct __nt_type_code_config<NTC_SHORT> {
  using type = short int;
};

template <>
struct __nt_type_code_config<NTC_UNSIGNED_SHORT> {
  using type = unsigned short int;
};

template <>
struct __nt_type_code_config<NTC_INT> {
  using type = int;
};

template <>
struct __nt_type_code_config<NTC_UNSIGNED_INT> {
  using type = unsigned int;
};

template <>
struct __nt_type_code_config<NTC_LONG> {
  using type = long int;
};

template <>
struct __nt_type_code_config<NTC_UNSIGNED_LONG> {
  using type = unsigned long int;
};

template <>
struct __nt_type_code_config<NTC_LONGLONG> {
  using type = long long int;
};

template <>
struct __nt_type_code_config<NTC_UNSIGNED_LONGLONG> {
  using type = unsigned long long int;
};

template <>
struct __nt_type_code_config<NTC_HALF> {
  using type = __half;
};

template <>
struct __nt_type_code_config<NTC_FLOAT> {
  using type = float;
};

template <>
struct __nt_type_code_config<NTC_DOUBLE> {
  using type = double;
};

template <>
struct __nt_type_code_config<NTC_LONGDOUBLE> {
  using type = long double;
};

/// @brief Class providing utilities for numeric type introspection and
/// manipulation.
///
/// This class offers a comprehensive interface for working with numeric types
/// at both compile-time and runtime. It provides:
/// - Type-to-code and code-to-type mappings
/// - Type property queries (signed/unsigned, floating/integral, bit size)
/// - Type name retrieval (full name, fixed-width name, short name)
///
/// Example usage:
/// @code
/// // Get type code from a type
/// constexpr auto code = NumericType::type_code<int>();
/// static_assert(code == NTC_INT);
///
/// // Get type from a code
/// using IntType = NumericType::type<NTC_INT>;
/// static_assert(std::is_same_v<IntType, int>);
///
/// // Query type properties
/// static_assert(NumericType::is_signed(NTC_INT));
/// static_assert(NumericType::is_unsigned(NTC_UINT32));
/// static_assert(NumericType::is_floating_point(NTC_FLOAT));
/// static_assert(NumericType::bit_size(NTC_INT32) == 32);
///
/// // Get type names
/// std::cout << NumericType::name(NTC_UNSIGNED_SHORT);      // "unsigned short"
/// std::cout << NumericType::fixed_width_name(NTC_UINT16);  // "uint16_t"
/// std::cout << NumericType::short_name(NTC_INT32);         // "i32"
/// @endcode
class NumericType {
public:
  /**
   * @brief Gets the numeric type associated with a type code.
   *
   * @tparam kCode The numeric type code
   * @return The corresponding type
   */
  template <NumericTypeCode kCode>
  using type = typename __nt_type_code_config<kCode>::type;

  /**
   * @brief Gets the numeric type code for a given type.
   *
   * @tparam T The numeric type
   * @return The corresponding numeric type code
   */
  template <typename T>
  static constexpr NumericTypeCode type_code() {
    return static_cast<NumericTypeCode>(__encodingNumericTypeCode<T>());
  }

  /**
   * @brief Gets the numeric type code from a value (type deduction).
   *
   * @tparam T The numeric type (deduced from the argument)
   * @param Unused parameter for type deduction
   * @return The corresponding numeric type code
   */
  template <typename T>
  static constexpr NumericTypeCode type_code(T&&) {
    return type_code<T>();
  }

  /**
   * @brief Checks if a type is unsigned.
   * @param code Numeric type code
   * @return true if the type is unsigned
   */
  static constexpr bool is_unsigned(NumericTypeCode code) {
    return ((code >> 8) & __NT_FLAGS_UNSIGNED) != 0;
  }

  /**
   * @brief Checks if a type is signed.
   * @param code Numeric type code
   * @return true if the type is signed
   */
  static constexpr bool is_signed(NumericTypeCode code) {
    return !is_unsigned(code);
  }

  /**
   * @brief Checks if a type is floating-point.
   * @param code Numeric type code
   * @return true if the type is floating-point
   */
  static constexpr bool is_floating_point(NumericTypeCode code) {
    return ((code >> 8) & __NT_FLAGS_FLOATING) != 0;
  }

  /**
   * @brief Checks if a type is integral.
   * @param code Numeric type code
   * @return true if the type is integral
   */
  static constexpr bool is_integral(NumericTypeCode code) {
    return !is_floating_point(code);
  }

  /**
   * @brief Checks if a type is a character type.
   *
   * Character types include: char, char16_t, char32_t, wchar_t
   *
   * @param code Numeric type code
   * @return true if the type is a character type
   */
  static constexpr bool is_character(NumericTypeCode code) {
    return ((code >> 8) & __NT_FLAGS_CHARACTER) != 0;
  }

  /**
   * @brief Gets the bit size of a type.
   * @param code Numeric type code
   * @return Size in bits (e.g., 8, 16, 32, 64)
   */
  static constexpr uint16_t bit_size(NumericTypeCode code) {
    return code & 0xff;
  }

  /**
   * @brief Gets the full name of a type.
   * @param code Numeric type code
   * @return Full type name (e.g., "unsigned short", "long long")
   */
  static constexpr std::string_view name(NumericTypeCode code) {
    return traits_by_code<std::string_view, name_traits>(code);
  }

  /**
   * @brief Gets the fixed-width name of a type.
   * @param code Numeric type code
   * @return Fixed-width type name (e.g., "uint16_t", "int32_t")
   */
  static constexpr std::string_view fixed_width_name(NumericTypeCode code) {
    return traits_by_code<std::string_view, fixed_width_name_traits>(code);
  }

  /**
   * @brief Gets the short name of a type.
   * @param code Numeric type code
   * @return Short type name with size (e.g., "u8", "i16", "f32")
   */
  static constexpr std::string_view short_name(NumericTypeCode code) {
    return traits_by_code<std::string_view, short_name_traits>(code);
  }

private:
  template <typename T>
  struct name_traits {
    static constexpr std::string_view doit(NumericTypeCode code) {
      return __nt_type_config<T>::name;
    }
  };

  template <typename T>
  struct fixed_width_name_traits {
    static constexpr std::string_view doit(NumericTypeCode code) {
      constexpr uint8_t bits = std::is_same_v<T, bool> ? 1 : sizeof(T) * 8;
      if constexpr (std::is_same_v<T, __half>)
        return __nt_fixed_width_type_config<true, bits, true>::name;
      else
        return __nt_fixed_width_type_config<std::is_signed_v<T>, bits,
                                            std::is_floating_point_v<T>>::name;
    }
  };

  template <typename T>
  struct short_name_traits {
    static constexpr std::string_view doit(NumericTypeCode code) {
      constexpr uint8_t bits = std::is_same_v<T, bool> ? 1 : sizeof(T) * 8;
      if constexpr (std::is_same_v<T, __half>)
        return __nt_fixed_width_type_config<true, bits, true>::short_name;
      else
        return __nt_fixed_width_type_config<
            std::is_signed_v<T>, bits, std::is_floating_point_v<T>>::short_name;
    }
  };

  template <typename RetT, template <typename> class TraitsT>
  static constexpr RetT traits_by_code(NumericTypeCode code) {
    switch (code) {
    case NTC_BOOL:
      return TraitsT<bool>::doit(code);
    case NTC_SIGNED_CHAR:
      return TraitsT<signed char>::doit(code);
    case NTC_UNSIGNED_CHAR:
      return TraitsT<unsigned char>::doit(code);
    case NTC_CHAR:
      return TraitsT<char>::doit(code);
    case NTC_CHAR16:
      return TraitsT<char16_t>::doit(code);
    case NTC_CHAR32:
      return TraitsT<char32_t>::doit(code);
    case NTC_WCHAR:
      return TraitsT<wchar_t>::doit(code);
    case NTC_SHORT:
      return TraitsT<short>::doit(code);
    case NTC_UNSIGNED_SHORT:
      return TraitsT<unsigned short>::doit(code);
    case NTC_INT:
      return TraitsT<int>::doit(code);
    case NTC_UNSIGNED_INT:
      return TraitsT<unsigned int>::doit(code);
    case NTC_LONG:
      return TraitsT<long>::doit(code);
    case NTC_UNSIGNED_LONG:
      return TraitsT<unsigned long>::doit(code);
    case NTC_LONGLONG:
      return TraitsT<long long>::doit(code);
    case NTC_UNSIGNED_LONGLONG:
      return TraitsT<unsigned long long>::doit(code);
    case NTC_HALF:
      return TraitsT<__half>::doit(code);
    case NTC_FLOAT:
      return TraitsT<float>::doit(code);
    case NTC_DOUBLE:
      return TraitsT<double>::doit(code);
    case NTC_LONGDOUBLE:
      return TraitsT<long double>::doit(code);
    default:
      return {};
    }
  }
};

} // namespace es
#endif
