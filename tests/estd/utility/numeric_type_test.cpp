#include <estd/__utility/numeric_type.h>
#include <gtest/gtest.h>
#include <type_traits>

using es::NumericTypeCode;

TEST(NumericType, Type) {
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_BOOL>, bool>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_SIGNED_CHAR>, signed char>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_UNSIGNED_CHAR>, unsigned char>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_CHAR>, char>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_CHAR16>, char16_t>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_CHAR32>, char32_t>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_WCHAR>, wchar_t>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_SHORT>, short>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_UNSIGNED_SHORT>, unsigned short>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_INT>, int>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_UNSIGNED_INT>, unsigned int>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_LONG>, long>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_UNSIGNED_LONG>, unsigned long>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_LONGLONG>, long long>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_UNSIGNED_LONGLONG>, unsigned long long>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_HALF>, es::__half>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_FLOAT>, float>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_DOUBLE>, double>));
  ASSERT_TRUE((std::is_same_v<typename es::NumericType::type<es::NTC_LONGDOUBLE>, long double>));
}

TEST(NumericType, TypeCode) {
  ASSERT_EQ(es::NumericType::type_code<bool>(), es::NTC_BOOL);
  ASSERT_EQ(es::NumericType::type_code<signed char>(), es::NTC_SIGNED_CHAR);
  ASSERT_EQ(es::NumericType::type_code<unsigned char>(), es::NTC_UNSIGNED_CHAR);
  ASSERT_EQ(es::NumericType::type_code<char>(), es::NTC_CHAR);
  ASSERT_EQ(es::NumericType::type_code<char16_t>(), es::NTC_CHAR16);
  ASSERT_EQ(es::NumericType::type_code<char32_t>(), es::NTC_CHAR32);
  ASSERT_EQ(es::NumericType::type_code<wchar_t>(), es::NTC_WCHAR);
  ASSERT_EQ(es::NumericType::type_code<short>(), es::NTC_SHORT);
  ASSERT_EQ(es::NumericType::type_code<unsigned short>(), es::NTC_UNSIGNED_SHORT);
  ASSERT_EQ(es::NumericType::type_code<int>(), es::NTC_INT);
  ASSERT_EQ(es::NumericType::type_code<unsigned int>(), es::NTC_UNSIGNED_INT);
  ASSERT_EQ(es::NumericType::type_code<long>(), es::NTC_LONG);
  ASSERT_EQ(es::NumericType::type_code<unsigned long>(), es::NTC_UNSIGNED_LONG);
  ASSERT_EQ(es::NumericType::type_code<long long>(), es::NTC_LONGLONG);
  ASSERT_EQ(es::NumericType::type_code<unsigned long long>(), es::NTC_UNSIGNED_LONGLONG);
  ASSERT_EQ(es::NumericType::type_code<es::__half>(), es::NTC_HALF);
  ASSERT_EQ(es::NumericType::type_code<float>(), es::NTC_FLOAT);
  ASSERT_EQ(es::NumericType::type_code<double>(), es::NTC_DOUBLE);
  ASSERT_EQ(es::NumericType::type_code<long double>(), es::NTC_LONGDOUBLE);

  ASSERT_EQ(es::NumericType::type_code(0), es::NTC_INT);
  ASSERT_EQ(es::NumericType::type_code(0u), es::NTC_UNSIGNED_INT);
}

TEST(NumericType, IsSigned) {
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_BOOL), std::is_signed_v<bool>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_SIGNED_CHAR), std::is_signed_v<signed char>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_UNSIGNED_CHAR), std::is_signed_v<unsigned char>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_CHAR), std::is_signed_v<char>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_CHAR16), std::is_signed_v<char16_t>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_CHAR32), std::is_signed_v<char32_t>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_WCHAR), std::is_signed_v<wchar_t>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_SHORT), std::is_signed_v<short>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_UNSIGNED_SHORT), std::is_signed_v<unsigned short>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_INT), std::is_signed_v<int>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_UNSIGNED_INT), std::is_signed_v<unsigned int>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_LONG), std::is_signed_v<long>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_UNSIGNED_LONG), std::is_signed_v<unsigned long>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_LONGLONG), std::is_signed_v<long long>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_UNSIGNED_LONGLONG), std::is_signed_v<unsigned long long>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_HALF), std::is_signed_v<float>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_FLOAT), std::is_signed_v<float>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_DOUBLE), std::is_signed_v<double>);
  ASSERT_EQ(es::NumericType::is_signed(es::NTC_LONGDOUBLE), std::is_signed_v<long double>);
}

TEST(NumericType, IsUnsigned) {
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_BOOL), std::is_unsigned_v<bool>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_SIGNED_CHAR), std::is_unsigned_v<signed char>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_UNSIGNED_CHAR), std::is_unsigned_v<unsigned char>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_CHAR), std::is_unsigned_v<char>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_CHAR16), std::is_unsigned_v<char16_t>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_CHAR32), std::is_unsigned_v<char32_t>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_WCHAR), std::is_unsigned_v<wchar_t>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_SHORT), std::is_unsigned_v<short>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_UNSIGNED_SHORT), std::is_unsigned_v<unsigned short>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_INT), std::is_unsigned_v<int>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_UNSIGNED_INT), std::is_unsigned_v<unsigned int>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_LONG), std::is_unsigned_v<long>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_UNSIGNED_LONG), std::is_unsigned_v<unsigned long>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_LONGLONG), std::is_unsigned_v<long long>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_UNSIGNED_LONGLONG), std::is_unsigned_v<unsigned long long>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_HALF), std::is_unsigned_v<float>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_FLOAT), std::is_unsigned_v<float>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_DOUBLE), std::is_unsigned_v<double>);
  ASSERT_EQ(es::NumericType::is_unsigned(es::NTC_LONGDOUBLE), std::is_unsigned_v<long double>);
}

TEST(NumericType, IsFloat) {
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_BOOL), std::is_floating_point_v<bool>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_SIGNED_CHAR), std::is_floating_point_v<signed char>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_UNSIGNED_CHAR), std::is_floating_point_v<unsigned char>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_CHAR), std::is_floating_point_v<char>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_CHAR16), std::is_floating_point_v<char16_t>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_CHAR32), std::is_floating_point_v<char32_t>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_WCHAR), std::is_floating_point_v<wchar_t>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_SHORT), std::is_floating_point_v<short>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_UNSIGNED_SHORT), std::is_floating_point_v<unsigned short>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_INT), std::is_floating_point_v<int>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_UNSIGNED_INT), std::is_floating_point_v<unsigned int>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_LONG), std::is_floating_point_v<long>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_UNSIGNED_LONG), std::is_floating_point_v<unsigned long>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_LONGLONG), std::is_floating_point_v<long long>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_UNSIGNED_LONGLONG), std::is_floating_point_v<unsigned long long>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_HALF), std::is_floating_point_v<float>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_FLOAT), std::is_floating_point_v<float>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_DOUBLE), std::is_floating_point_v<double>);
  ASSERT_EQ(es::NumericType::is_floating_point(es::NTC_LONGDOUBLE), std::is_floating_point_v<long double>);
}

TEST(NumericType, IsIntegral) {
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_BOOL), std::is_integral_v<bool>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_SIGNED_CHAR), std::is_integral_v<signed char>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_UNSIGNED_CHAR), std::is_integral_v<unsigned char>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_CHAR), std::is_integral_v<char>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_CHAR16), std::is_integral_v<char16_t>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_CHAR32), std::is_integral_v<char32_t>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_WCHAR), std::is_integral_v<wchar_t>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_SHORT), std::is_integral_v<short>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_UNSIGNED_SHORT), std::is_integral_v<unsigned short>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_INT), std::is_integral_v<int>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_UNSIGNED_INT), std::is_integral_v<unsigned int>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_LONG), std::is_integral_v<long>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_UNSIGNED_LONG), std::is_integral_v<unsigned long>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_LONGLONG), std::is_integral_v<long long>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_UNSIGNED_LONGLONG), std::is_integral_v<unsigned long long>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_HALF), std::is_integral_v<float>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_FLOAT), std::is_integral_v<float>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_DOUBLE), std::is_integral_v<double>);
  ASSERT_EQ(es::NumericType::is_integral(es::NTC_LONGDOUBLE), std::is_integral_v<long double>);
}

TEST(NumericType, IsCharacter) {
  ASSERT_EQ(es::NumericType::is_character(es::NTC_BOOL), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_SIGNED_CHAR), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_UNSIGNED_CHAR), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_CHAR), true);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_CHAR16), true);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_CHAR32), true);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_WCHAR), true);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_SHORT), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_UNSIGNED_SHORT), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_INT), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_UNSIGNED_INT), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_LONG), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_UNSIGNED_LONG), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_LONGLONG), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_UNSIGNED_LONGLONG), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_HALF), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_FLOAT), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_DOUBLE), false);
  ASSERT_EQ(es::NumericType::is_character(es::NTC_LONGDOUBLE), false);
}

TEST(NumericType, Name) {
  ASSERT_EQ(es::NumericType::name(es::NTC_BOOL), "bool");
  ASSERT_EQ(es::NumericType::name(es::NTC_SIGNED_CHAR), "signed char");
  ASSERT_EQ(es::NumericType::name(es::NTC_UNSIGNED_CHAR), "unsigned char");
  ASSERT_EQ(es::NumericType::name(es::NTC_CHAR), "char");
  ASSERT_EQ(es::NumericType::name(es::NTC_CHAR16), "char16_t");
  ASSERT_EQ(es::NumericType::name(es::NTC_CHAR32), "char32_t");
  ASSERT_EQ(es::NumericType::name(es::NTC_WCHAR), "wchar_t");
  ASSERT_EQ(es::NumericType::name(es::NTC_SHORT), "short");
  ASSERT_EQ(es::NumericType::name(es::NTC_UNSIGNED_SHORT), "unsigned short");
  ASSERT_EQ(es::NumericType::name(es::NTC_INT), "int");
  ASSERT_EQ(es::NumericType::name(es::NTC_UNSIGNED_INT), "unsigned int");
  ASSERT_EQ(es::NumericType::name(es::NTC_LONG), "long");
  ASSERT_EQ(es::NumericType::name(es::NTC_UNSIGNED_LONG), "unsigned long");
  ASSERT_EQ(es::NumericType::name(es::NTC_LONGLONG), "long long");
  ASSERT_EQ(es::NumericType::name(es::NTC_UNSIGNED_LONGLONG), "unsigned long long");
  ASSERT_EQ(es::NumericType::name(es::NTC_HALF), "half");
  ASSERT_EQ(es::NumericType::name(es::NTC_FLOAT), "float");
  ASSERT_EQ(es::NumericType::name(es::NTC_DOUBLE), "double");
  ASSERT_EQ(es::NumericType::name(es::NTC_LONGDOUBLE), "long double");
}

TEST(NumericType, FixedWidthName) {
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_BOOL), "bool");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_SIGNED_CHAR), "int8_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_UNSIGNED_CHAR), "uint8_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_CHAR), std::is_signed_v<char> ? "int8_t" : "uint8_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_CHAR16), "uint16_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_CHAR32), "uint32_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_WCHAR),  sizeof(int) == 4 ? "int32_t" : "int16_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_SHORT), "int16_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_UNSIGNED_SHORT), "uint16_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_INT), sizeof(int) == 4 ? "int32_t" : "int16_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_UNSIGNED_INT), sizeof(int) == 4 ? "uint32_t" : "uint16_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_LONG), (sizeof(long) == 4 ? "int32_t" : "int64_t"));
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_UNSIGNED_LONG), (sizeof(long) == 4 ? "uint32_t" : "uint64_t"));
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_LONGLONG), "int64_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_UNSIGNED_LONGLONG), "uint64_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_HALF), "float16_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_FLOAT), "float32_t");
  ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_DOUBLE), "float64_t");
  if constexpr (sizeof(long double) == 8)
    ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_LONGDOUBLE), "float64_t");
  else if (sizeof(long double) == 16)
    ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_LONGDOUBLE), "float128_t");
  else
    ASSERT_EQ(es::NumericType::fixed_width_name(es::NTC_LONGDOUBLE), "float80_t");
}

TEST(NumericType, ShortName) {
  ASSERT_EQ(es::NumericType::short_name(es::NTC_BOOL), "u1");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_SIGNED_CHAR), "i8");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_UNSIGNED_CHAR), "u8");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_CHAR), std::is_signed_v<char> ? "i8" : "u8");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_CHAR16), "u16");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_CHAR32), "u32");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_WCHAR),  sizeof(int) == 4 ? "i32" : "i16");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_SHORT), "i16");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_UNSIGNED_SHORT), "u16");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_INT), sizeof(int) == 4 ? "i32" : "i16");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_UNSIGNED_INT), sizeof(int) == 4 ? "u32" : "u16");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_LONG), (sizeof(long) == 4 ? "i32" : "i64"));
  ASSERT_EQ(es::NumericType::short_name(es::NTC_UNSIGNED_LONG), (sizeof(long) == 4 ? "u32" : "u64"));
  ASSERT_EQ(es::NumericType::short_name(es::NTC_LONGLONG), "i64");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_UNSIGNED_LONGLONG), "u64");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_FLOAT), "f32");
  ASSERT_EQ(es::NumericType::short_name(es::NTC_DOUBLE), "f64");
  if constexpr (sizeof(long double) == 64)
    ASSERT_EQ(es::NumericType::short_name(es::NTC_LONGDOUBLE), "f64");
  else if constexpr (sizeof(long double) == 80)
    ASSERT_EQ(es::NumericType::short_name(es::NTC_LONGDOUBLE), "f80");
  else
    ASSERT_EQ(es::NumericType::short_name(es::NTC_LONGDOUBLE), "f128");
}
