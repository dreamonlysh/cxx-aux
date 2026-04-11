#include "estd/string/concat_buffer.h"
#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <vector>

using namespace es::string;

class ConcatBufferTest : public ::testing::Test {};

TEST_F(ConcatBufferTest, FixedBuffer_Basic) {
  concat_buffer<20> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
  EXPECT_EQ(cb.size(), 5);
}

TEST_F(ConcatBufferTest, FixedBuffer_AppendChar) {
  concat_buffer<20> cb;
  cb.append('H');
  cb.append('e');
  cb.append('l');
  cb.append('l');
  cb.append('o');
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, FixedBuffer_AppendMultiple) {
  concat_buffer<20> cb;
  cb.append("Hello", ' ', "World");
  EXPECT_EQ(cb.view(), "Hello World");
}

TEST_F(ConcatBufferTest, FixedBuffer_AppendIntegral) {
  concat_buffer<20> cb;
  cb.append("Value: ");
  cb.append(42);
  EXPECT_EQ(cb.view(), "Value: 42");
}

TEST_F(ConcatBufferTest, FixedBuffer_AppendIntegralNegative) {
  concat_buffer<20> cb;
  cb.append("Value: ");
  cb.append(-123);
  EXPECT_EQ(cb.view(), "Value: -123");
}

TEST_F(ConcatBufferTest, FixedBuffer_Overflow_ReturnsFalse) {
  concat_buffer<10> cb;
  cb.append("Hello");
  bool result = cb.append(" World");
  EXPECT_FALSE(result);
  EXPECT_EQ(cb.size(), 5);
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, FixedBuffer_Overflow_CharByChar) {
  concat_buffer<3> cb;
  EXPECT_TRUE(cb.append('A'));
  EXPECT_TRUE(cb.append('B'));
  EXPECT_TRUE(cb.append('C'));
  EXPECT_FALSE(cb.append('D'));
  EXPECT_EQ(cb.size(), 3);
  EXPECT_EQ(cb.view(), "ABC");
}

TEST_F(ConcatBufferTest, FixedBuffer_Capacity) {
  concat_buffer<20> cb;
  EXPECT_EQ(cb.capacity(), 20);
  EXPECT_FALSE(decltype(cb)::can_reserve);
}

TEST_F(ConcatBufferTest, FixedBuffer_CanReserve) {
  concat_buffer<20> cb;
  EXPECT_FALSE(cb.can_reserve);
}

TEST_F(ConcatBufferTest, FixedBuffer_Clear) {
  concat_buffer<20> cb;
  cb.append("Hello");
  cb.clear();
  EXPECT_EQ(cb.size(), 0);
  cb.append("World");
  EXPECT_EQ(cb.view(), "World");
}

TEST_F(ConcatBufferTest, FixedBuffer_ClearEmpty) {
  concat_buffer<20> cb;
  cb.clear();
  EXPECT_EQ(cb.size(), 0);
}

TEST_F(ConcatBufferTest, FixedBuffer_ViewAfterOverflow) {
  concat_buffer<5> cb;
  cb.append("Hello");
  cb.append("World");
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, FixedBuffer_IntegralOverflow) {
  concat_buffer<8> cb;
  cb.append("Val: ");
  EXPECT_TRUE(cb.append(123));
  EXPECT_FALSE(cb.append(123456789));
}

TEST_F(ConcatBufferTest, DynamicBuffer_String_Basic) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_String_WithInitialCapacity) {
  concat_buffer<100, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
  EXPECT_GE(cb.capacity(), 100);
}

TEST_F(ConcatBufferTest, DynamicBuffer_String_AutoExpand) {
  concat_buffer<0, std::string> cb;
  for (int i = 0; i < 100; ++i) {
    cb.append("abc");
  }
  EXPECT_EQ(cb.size(), 300);
  EXPECT_EQ(cb.view().substr(0, 6), "abcabc");
}

TEST_F(ConcatBufferTest, DynamicBuffer_Vector) {
  concat_buffer<0, std::vector<char>> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_ReferencedContainer) {
  std::string str;
  concat_buffer cb(str);
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
  EXPECT_EQ(str, "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_RvalueContainer) {
  concat_buffer cb(std::string("initial"));
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "initialHello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_MoveContainer) {
  std::string str = "test";
  concat_buffer cb(std::move(str));
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "testHello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_Reserve) {
  concat_buffer<0, std::string> cb;
  cb.reserve(100);
  EXPECT_GE(cb.capacity(), 100);
}

TEST_F(ConcatBufferTest, DynamicBuffer_ReserveTwice) {
  concat_buffer<0, std::string> cb;
  cb.reserve(50);
  cb.reserve(100);
  EXPECT_GE(cb.capacity(), 100);
}

TEST_F(ConcatBufferTest, DynamicBuffer_CanReserve) {
  concat_buffer<0, std::string> cb;
  EXPECT_TRUE(decltype(cb)::can_reserve);
  EXPECT_TRUE(decltype(cb)::can_resize);
}

TEST_F(ConcatBufferTest, DynamicBuffer_CanResize) {
  concat_buffer<0, std::string> cb;
  EXPECT_TRUE(cb.can_resize);
}

TEST_F(ConcatBufferTest, Append_Char) {
  concat_buffer<0, std::string> cb;
  cb.append('A');
  cb.append('B');
  cb.append('C');
  EXPECT_EQ(cb.view(), "ABC");
}

TEST_F(ConcatBufferTest, Append_CString) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.size(), 5);
}

TEST_F(ConcatBufferTest, Append_EmptyCString) {
  concat_buffer<0, std::string> cb;
  cb.append("");
  EXPECT_EQ(cb.size(), 0);
}

TEST_F(ConcatBufferTest, Append_StringView) {
  concat_buffer<0, std::string> cb;
  std::string_view sv = "World";
  cb.append("Hello ");
  cb.append(sv);
  EXPECT_EQ(cb.view(), "Hello World");
}

TEST_F(ConcatBufferTest, Append_EmptyStringView) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  std::string_view sv = "";
  cb.append(sv);
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, Append_Vector) {
  concat_buffer<0, std::string> cb;
  std::vector<char> vec = {'H', 'e', 'l', 'l', 'o'};
  cb.append(vec);
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, Append_EmptyVector) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  std::vector<char> vec;
  cb.append(vec);
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, Append_MultipleArgs) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello", ' ', "World", '!', 123);
  EXPECT_EQ(cb.view(), "Hello World!123");
}

TEST_F(ConcatBufferTest, Append_ManyArgs) {
  concat_buffer<0, std::string> cb;
  cb.append("A", "B", "C", "D", "E");
  EXPECT_EQ(cb.view(), "ABCDE");
}

TEST_F(ConcatBufferTest, Append_LazyView_ToUpper) {
  concat_buffer<0, std::string> cb;
  cb.append("hello" | views::to_upper);
  EXPECT_EQ(cb.view(), "HELLO");
}

TEST_F(ConcatBufferTest, Append_LazyView_Chain) {
  concat_buffer<0, std::string> cb;
  cb.append("a.b.c" | views::dot_to_underline);
  EXPECT_EQ(cb.view(), "a_b_c");
}

TEST_F(ConcatBufferTest, Append_Integral_Positive) {
  concat_buffer<0, std::string> cb;
  cb.append("Value: ");
  cb.append(42);
  EXPECT_EQ(cb.view(), "Value: 42");
}

TEST_F(ConcatBufferTest, Append_Integral_Negative) {
  concat_buffer<0, std::string> cb;
  cb.append("Value: ");
  cb.append(-42);
  EXPECT_EQ(cb.view(), "Value: -42");
}

TEST_F(ConcatBufferTest, Append_Integral_Zero) {
  concat_buffer<0, std::string> cb;
  cb.append(0);
  EXPECT_EQ(cb.view(), "0");
}

TEST_F(ConcatBufferTest, Append_Integral_One) {
  concat_buffer<0, std::string> cb;
  cb.append(1);
  EXPECT_EQ(cb.view(), "1");
}

TEST_F(ConcatBufferTest, Append_Integral_Large) {
  concat_buffer<0, std::string> cb;
  cb.append(999999999999999999LL);
  EXPECT_EQ(cb.view(), "999999999999999999");
}

TEST_F(ConcatBufferTest, Append_Integral_Max) {
  concat_buffer<0, std::string> cb;
  cb.append(INT64_MAX);
  EXPECT_EQ(cb.view(), "9223372036854775807");
}

TEST_F(ConcatBufferTest, Append_Integral_Min) {
  concat_buffer<0, std::string> cb;
  cb.append(INT64_MIN);
  EXPECT_EQ(cb.view(), "-9223372036854775808");
}

TEST_F(ConcatBufferTest, Append_Unsigned) {
  concat_buffer<0, std::string> cb;
  cb.append(42u);
  EXPECT_EQ(cb.view(), "42");
}

TEST_F(ConcatBufferTest, Clear) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.size(), 5);
  cb.clear();
  EXPECT_EQ(cb.size(), 0);
}

TEST_F(ConcatBufferTest, ClearMultipleTimes) {
  concat_buffer<0, std::string> cb;
  cb.append("A");
  cb.clear();
  cb.append("B");
  cb.clear();
  cb.append("C");
  EXPECT_EQ(cb.view(), "C");
}

TEST_F(ConcatBufferTest, EmptyBuffer) {
  concat_buffer<0, std::string> cb;
  EXPECT_EQ(cb.size(), 0);
  EXPECT_TRUE(cb.view().empty());
  cb.append("");
  EXPECT_EQ(cb.size(), 0);
}

TEST_F(ConcatBufferTest, ToString) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello World");
  std::string result = cb.to_string();
  EXPECT_EQ(result, "Hello World");
}

TEST_F(ConcatBufferTest, ToStringEmpty) {
  concat_buffer<0, std::string> cb;
  std::string result = cb.to_string();
  EXPECT_TRUE(result.empty());
}

TEST_F(ConcatBufferTest, NestedAppends) {
  concat_buffer<0, std::string> cb;
  cb.append("Start: ");

  concat_buffer<0, std::string> temp;
  temp.append("nested");
  cb.append(temp.view());

  cb.append(" :End");
  EXPECT_EQ(cb.view(), "Start: nested :End");
}

TEST_F(ConcatBufferTest, NestedAppendsDeep) {
  concat_buffer<0, std::string> cb;
  cb.append("A");
  {
    concat_buffer<0, std::string> inner;
    inner.append("B");
    cb.append(inner.view());
  }
  cb.append("C");
  EXPECT_EQ(cb.view(), "ABC");
}

TEST_F(ConcatBufferTest, Append_ReturnsBool_Success) {
  concat_buffer<0, std::string> cb;
  bool result = cb.append("Hello");
  EXPECT_TRUE(result);
}

TEST_F(ConcatBufferTest, Append_ReturnsBool_Failure) {
  concat_buffer<5> cb;
  cb.append("Hello");
  bool result = cb.append(" World");
  EXPECT_FALSE(result);
}

TEST_F(ConcatBufferTest, FixedBuffer_OperatorCharPointer) {
  concat_buffer<20> cb;
  cb.append("Hello");
  auto* ptr = static_cast<const char*>(cb);
  EXPECT_EQ(std::string_view(ptr, cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_OperatorCharPointer) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  auto* ptr = static_cast<const char*>(cb);
  EXPECT_EQ(std::string_view(ptr, cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, FixedBuffer_CStr) {
  concat_buffer<20> cb;
  cb.append("Hello");
  const char* ptr = cb.c_str();
  EXPECT_EQ(std::string_view(ptr, cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_CStr) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  const char* ptr = cb.c_str();
  EXPECT_EQ(std::string_view(ptr, cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, FixedBuffer_Data) {
  concat_buffer<20> cb;
  cb.append("Hello");
  EXPECT_EQ(std::string_view(cb.data(), cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_Data) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(std::string_view(cb.data(), cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, FixedBuffer_End) {
  concat_buffer<20> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.end(), cb.data() + 5);
}

TEST_F(ConcatBufferTest, DynamicBuffer_End) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.end(), cb.data() + 5);
}

TEST_F(ConcatBufferTest, LargeData) {
  concat_buffer<0, std::string> cb;
  std::string expected;
  for (int i = 0; i < 1000; ++i) {
    cb.append("a");
    expected += "a";
  }
  EXPECT_EQ(cb.view(), expected);
}

TEST_F(ConcatBufferTest, MixedOperations) {
  concat_buffer<0, std::string> cb;
  cb.append("Start");
  cb.append(' ');
  cb.append(42);
  cb.append(" - ");
  cb.append("end");
  EXPECT_EQ(cb.view(), "Start 42 - end");
}
