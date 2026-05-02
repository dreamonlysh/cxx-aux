#include <array>
#include <estd/__meta/array.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(ArrayTest, IsCArray_BasicTypes) {
  EXPECT_TRUE(es::is_c_array_v<int[5]>);
  EXPECT_TRUE(es::is_c_array_v<double[10]>);
  EXPECT_TRUE(es::is_c_array_v<char[1]>);
  EXPECT_TRUE(es::is_c_array_v<std::string[3]>);

  EXPECT_FALSE(es::is_c_array_v<int>);
  EXPECT_FALSE(es::is_c_array_v<double>);
  EXPECT_FALSE(es::is_c_array_v<std::string>);
  EXPECT_FALSE(es::is_c_array_v<std::vector<int>>);
}

TEST(ArrayTest, IsCArray_WithCVQualifiers) {
  EXPECT_TRUE(es::is_c_array_v<const int[5]>);
  EXPECT_TRUE(es::is_c_array_v<volatile double[10]>);
  EXPECT_TRUE(es::is_c_array_v<const volatile char[1]>);

  EXPECT_TRUE((es::is_c_array_v<int (&)[5]>));
  EXPECT_TRUE((es::is_c_array_v<const int (&)[5]>));
  EXPECT_TRUE((es::is_c_array_v<int (&&)[5]>));
}

TEST(ArrayTest, IsStdArray_BasicTypes) {
  EXPECT_TRUE((es::is_std_array_v<std::array<int, 5>>));
  EXPECT_TRUE((es::is_std_array_v<std::array<double, 10>>));
  EXPECT_TRUE((es::is_std_array_v<std::array<char, 1>>));
  EXPECT_TRUE((es::is_std_array_v<std::array<std::string, 3>>));

  EXPECT_FALSE(es::is_std_array_v<int[5]>);
  EXPECT_FALSE(es::is_std_array_v<int>);
  EXPECT_FALSE((es::is_std_array_v<std::vector<int>>));
}

TEST(ArrayTest, IsStdArray_WithCVQualifiers) {
  EXPECT_TRUE((es::is_std_array_v<const std::array<int, 5>>));
  EXPECT_TRUE((es::is_std_array_v<volatile std::array<double, 10>>));
  EXPECT_TRUE((es::is_std_array_v<const volatile std::array<char, 1>>));

  EXPECT_TRUE((es::is_std_array_v<std::array<int, 5>&>));
  EXPECT_TRUE((es::is_std_array_v<const std::array<int, 5>&>));
  EXPECT_TRUE((es::is_std_array_v<std::array<int, 5>&&>));
}

TEST(ArrayTest, ArrayTraits_CArray) {
  using traits1 = es::array_traits<int[5]>;
  EXPECT_TRUE((std::is_same_v<traits1::element_type, int>));
  EXPECT_EQ(traits1::size, 5u);

  using traits2 = es::array_traits<double[10]>;
  EXPECT_TRUE((std::is_same_v<traits2::element_type, double>));
  EXPECT_EQ(traits2::size, 10u);

  using traits3 = es::array_traits<std::string[3]>;
  EXPECT_TRUE((std::is_same_v<traits3::element_type, std::string>));
  EXPECT_EQ(traits3::size, 3u);
}

TEST(ArrayTest, ArrayTraits_StdArray) {
  using traits1 = es::array_traits<std::array<int, 5>>;
  EXPECT_TRUE((std::is_same_v<traits1::element_type, int>));
  EXPECT_EQ(traits1::size, 5u);

  using traits2 = es::array_traits<std::array<double, 10>>;
  EXPECT_TRUE((std::is_same_v<traits2::element_type, double>));
  EXPECT_EQ(traits2::size, 10u);

  using traits3 = es::array_traits<std::array<std::string, 3>>;
  EXPECT_TRUE((std::is_same_v<traits3::element_type, std::string>));
  EXPECT_EQ(traits3::size, 3u);
}

TEST(ArrayTest, ArrayTraits_NonArray) {
  using traits = es::array_traits<int>;
  EXPECT_TRUE((std::is_same_v<traits::element_type, void>));
  EXPECT_EQ(traits::size, 0u);
}

TEST(ArrayTest, ArrayTraits_WithCVQualifiers) {
  using traits1 = es::array_traits<const int[5]>;
  EXPECT_TRUE((std::is_same_v<traits1::element_type, int>));
  EXPECT_EQ(traits1::size, 5u);

  using traits2 = es::array_traits<int (&)[5]>;
  EXPECT_TRUE((std::is_same_v<traits2::element_type, int>));
  EXPECT_EQ(traits2::size, 5u);

  using traits3 = es::array_traits<const std::array<double, 10>&>;
  EXPECT_TRUE((std::is_same_v<traits3::element_type, double>));
  EXPECT_EQ(traits3::size, 10u);
}

TEST(ArrayTest, ToStdArray_BasicTypes) {
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<int[5]>, std::array<int, 5>>));
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<double[10]>, std::array<double, 10>>));
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<char[1]>, std::array<char, 1>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<std::string[3]>,
                              std::array<std::string, 3>>));
}

TEST(ArrayTest, ToStdArray_NonArrayTypes) {
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<int>, int>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<double>, double>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<std::string>, std::string>));
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<std::vector<int>>, std::vector<int>>));
}

TEST(ArrayTest, ToStdArray_StdArrayUnchanged) {
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<std::array<int, 5>>,
                              std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<std::array<double, 10>>,
                              std::array<double, 10>>));
}

TEST(ArrayTest, ToStdArray_MultiDimensional) {
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int[3][4]>, std::array<int[4], 3>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<double[2][3][4]>,
                              std::array<double[3][4], 2>>));
}

TEST(ArrayTest, ToStdArray_ElementTypes) {
  struct CustomType {};
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<CustomType[5]>,
                              std::array<CustomType, 5>>));

  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int* [3]>, std::array<int*, 3>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<int (*)[3]>, int (*)[3]>));
}

TEST(ArrayTest, ToStdArray_WithCVQualifiers) {
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<const int[5]>, std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<volatile double[10]>,
                              std::array<double, 10>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const volatile char[1]>,
                              std::array<char, 1>>));
}

TEST(ArrayTest, ToStdArray_WithReferences) {
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int (&)[5]>, std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const int (&)[5]>,
                              std::array<int, 5>>));
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int (&&)[5]>, std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const volatile int (&)[5]>,
                              std::array<int, 5>>));
}

TEST(ArrayTest, ToStdArray_CompileTimeVerification) {
  static_assert(std::is_same_v<es::to_std_array_t<int[5]>, std::array<int, 5>>,
                "int[5] should convert to std::array<int, 5>");
  static_assert(std::is_same_v<es::to_std_array_t<double>, double>,
                "double should remain unchanged");
  static_assert(std::is_same_v<es::to_std_array_t<std::array<char, 10>>,
                               std::array<char, 10>>,
                "std::array should remain unchanged");

  using converted_type = es::to_std_array_t<int[100]>;
  static_assert(std::is_same_v<converted_type, std::array<int, 100>>,
                "Type conversion should work in type aliases");
}

TEST(ArrayTest, ArrayTraits_LargeArray) {
  using traits = es::array_traits<double[1000]>;
  EXPECT_TRUE((std::is_same_v<traits::element_type, double>));
  EXPECT_EQ(traits::size, 1000u);
}

TEST(ArrayTest, ToStdArray_LargeArray) {
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int[10000]>, std::array<int, 10000>>));
}

TEST(ArrayTest, ArrayTraits_FunctionPointer) {
  using FuncPtr = void (*)();
  using traits = es::array_traits<FuncPtr[5]>;
  EXPECT_TRUE((std::is_same_v<traits::element_type, FuncPtr>));
  EXPECT_EQ(traits::size, 5u);
}

TEST(ArrayTest, ToStdArray_FunctionPointer) {
  using FuncPtr = void (*)();
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<FuncPtr[3]>, std::array<FuncPtr, 3>>));
}

TEST(ArrayTest, ArrayTraits_MemberPointer) {
  struct Test {
    int member;
  };
  using MemberPtr = int Test::*;
  using traits = es::array_traits<MemberPtr[4]>;
  EXPECT_TRUE((std::is_same_v<traits::element_type, MemberPtr>));
  EXPECT_EQ(traits::size, 4u);
}

TEST(ArrayTest, ArrayTraits_ArrayOfArrays) {
  using traits = es::array_traits<int[3][4]>;
  EXPECT_TRUE((std::is_same_v<traits::element_type, int[4]>));
  EXPECT_EQ(traits::size, 3u);

  using traits2 = es::array_traits<double[2][3][4]>;
  EXPECT_TRUE((std::is_same_v<traits2::element_type, double[3][4]>));
  EXPECT_EQ(traits2::size, 2u);
}

TEST(ArrayTest, ToStdArray_ArrayOfArrays) {
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int[3][4]>, std::array<int[4], 3>>));
}

TEST(ArrayTest, IsCArray_FunctionArray) {
  using FuncPtr = void (*)();
  using FuncArray = FuncPtr[5];
  EXPECT_TRUE(es::is_c_array_v<FuncArray>);
}

TEST(ArrayTest, IsStdArray_NestedStdArray) {
  using Nested = std::array<std::array<int, 3>, 4>;
  EXPECT_TRUE(es::is_std_array_v<Nested>);
  EXPECT_TRUE(es::is_std_array_v<const Nested&>);
}

TEST(ArrayTest, ArrayTraits_NestedStdArray) {
  using Nested = std::array<std::array<int, 3>, 4>;
  using traits = es::array_traits<Nested>;
  EXPECT_TRUE((std::is_same_v<traits::element_type, std::array<int, 3>>));
  EXPECT_EQ(traits::size, 4u);
}

TEST(ArrayTest, ToStdArray_AllCVRefCombinations) {
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<int[5]>, std::array<int, 5>>));
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<const int[5]>, std::array<int, 5>>));
  EXPECT_TRUE((
      std::is_same_v<es::to_std_array_t<volatile int[5]>, std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const volatile int[5]>,
                              std::array<int, 5>>));
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int (&)[5]>, std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const int (&)[5]>,
                              std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<volatile int (&)[5]>,
                              std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const volatile int (&)[5]>,
                              std::array<int, 5>>));
  EXPECT_TRUE(
      (std::is_same_v<es::to_std_array_t<int (&&)[5]>, std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const int (&&)[5]>,
                              std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<volatile int (&&)[5]>,
                              std::array<int, 5>>));
  EXPECT_TRUE((std::is_same_v<es::to_std_array_t<const volatile int (&&)[5]>,
                              std::array<int, 5>>));
}
