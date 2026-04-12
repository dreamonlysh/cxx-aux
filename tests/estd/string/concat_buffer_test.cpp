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

TEST_F(ConcatBufferTest, FixedBuffer_SingleChar) {
  concat_buffer<1> cb;
  EXPECT_TRUE(cb.append('A'));
  EXPECT_FALSE(cb.append('B'));
  EXPECT_EQ(cb.view(), "A");
}

TEST_F(ConcatBufferTest, FixedBuffer_ExactFit) {
  concat_buffer<5> cb;
  EXPECT_TRUE(cb.append("Hello"));
  EXPECT_EQ(cb.size(), 5);
  EXPECT_FALSE(cb.append('!'));
}

TEST_F(ConcatBufferTest, FixedBuffer_PartialOverflow) {
  concat_buffer<5> cb;
  cb.append("He");
  EXPECT_FALSE(cb.append("llo World"));
  EXPECT_EQ(cb.view(), "He");
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

TEST_F(ConcatBufferTest, DynamicBuffer_String_ZeroCapacity) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
  // N=0 means no initial reservation, capacity may be small
}

TEST_F(ConcatBufferTest, DynamicBuffer_String_ExplicitCapacity) {
  concat_buffer<0, std::string> cb(200);
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
  EXPECT_GE(cb.capacity(), 200);
}

TEST_F(ConcatBufferTest, DynamicBuffer_LvalueRef_ReserveCapacity) {
  std::string str;
  concat_buffer<100, std::string> cb(str);
  EXPECT_GE(cb.capacity(), 100);
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "Hello");
  EXPECT_EQ(str, "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_RvalueRef_ReserveCapacity) {
  std::string str = "test";
  concat_buffer<100, std::string> cb(std::move(str));
  EXPECT_GE(cb.capacity(), 100);
  cb.append("Hello");
  EXPECT_EQ(cb.view(), "testHello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_LvalueRef_ExistingCapacity) {
  std::string str;
  str.reserve(200);
  concat_buffer<50, std::string> cb(str);
  // Should keep existing capacity if it's already >= N
  EXPECT_GE(cb.capacity(), 200);
}

TEST_F(ConcatBufferTest, DynamicBuffer_RvalueRef_ExistingCapacity) {
  std::string str;
  str.reserve(200);
  str = "test";
  concat_buffer<50, std::string> cb(std::move(str));
  // Should keep existing capacity if it's already >= N
  EXPECT_GE(cb.capacity(), 200);
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

TEST_F(ConcatBufferTest, DynamicBuffer_VectorAutoExpand) {
  concat_buffer<0, std::vector<char>> cb;
  for (int i = 0; i < 100; ++i) {
    cb.append("x");
  }
  EXPECT_EQ(cb.size(), 100);
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

TEST_F(ConcatBufferTest, Append_Short) {
  concat_buffer<0, std::string> cb;
  cb.append(short(123));
  EXPECT_EQ(cb.view(), "123");
}

TEST_F(ConcatBufferTest, Append_Long) {
  concat_buffer<0, std::string> cb;
  cb.append(123456789L);
  EXPECT_EQ(cb.view(), "123456789");
}

TEST_F(ConcatBufferTest, Append_UnsignedLong) {
  concat_buffer<0, std::string> cb;
  cb.append(123456789UL);
  EXPECT_EQ(cb.view(), "123456789");
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
  EXPECT_EQ(std::string_view(cb.begin(), cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, DynamicBuffer_Data) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(std::string_view(cb.begin(), cb.size()), "Hello");
}

TEST_F(ConcatBufferTest, FixedBuffer_End) {
  concat_buffer<20> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.end(), cb.begin() + 5);
}

TEST_F(ConcatBufferTest, DynamicBuffer_End) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  EXPECT_EQ(cb.end(), cb.begin() + 5);
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

TEST_F(ConcatBufferTest, RangeBasedForLoop) {
  concat_buffer<20> cb;
  cb.append("Hello");

  std::string result;
  for (char c : cb) {
    result += c;
  }
  EXPECT_EQ(result, "Hello");
}

TEST_F(ConcatBufferTest, ConstBuffer) {
  concat_buffer<20> cb;
  cb.append("Hello");
  const auto& const_cb = cb;

  EXPECT_EQ(const_cb.view(), "Hello");
  EXPECT_EQ(const_cb.size(), 5);
  EXPECT_EQ(const_cb.capacity(), 20);
}

TEST_F(ConcatBufferTest, CopyConstructor) {
  concat_buffer<0, std::string> cb1;
  cb1.append("Hello");

  concat_buffer<0, std::string> cb2(cb1);
  EXPECT_EQ(cb2.view(), "Hello");
  EXPECT_EQ(cb1.view(), "Hello");
}

TEST_F(ConcatBufferTest, MoveConstructor) {
  concat_buffer<0, std::string> cb1;
  cb1.append("Hello");

  concat_buffer<0, std::string> cb2(std::move(cb1));
  EXPECT_EQ(cb2.view(), "Hello");
}

TEST_F(ConcatBufferTest, CopyAssignment) {
  concat_buffer<0, std::string> cb1;
  cb1.append("Hello");

  concat_buffer<0, std::string> cb2;
  cb2 = cb1;
  EXPECT_EQ(cb2.view(), "Hello");
  EXPECT_EQ(cb1.view(), "Hello");
}

TEST_F(ConcatBufferTest, MoveAssignment) {
  concat_buffer<0, std::string> cb1;
  cb1.append("Hello");

  concat_buffer<0, std::string> cb2;
  cb2 = std::move(cb1);
  EXPECT_EQ(cb2.view(), "Hello");
}

TEST_F(ConcatBufferTest, SelfAssignment) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  cb = cb;
  EXPECT_EQ(cb.view(), "Hello");
}

TEST_F(ConcatBufferTest, ClearAndReuse) {
  concat_buffer<0, std::string> cb;
  cb.append("First");
  EXPECT_EQ(cb.view(), "First");

  cb.clear();
  cb.append("Second");
  EXPECT_EQ(cb.view(), "Second");

  cb.clear();
  cb.append("Third");
  EXPECT_EQ(cb.view(), "Third");
}

TEST_F(ConcatBufferTest, AppendAfterClearFixed) {
  concat_buffer<10> cb;
  cb.append("Hello");
  cb.clear();
  EXPECT_TRUE(cb.append("World"));
  EXPECT_EQ(cb.view(), "World");
}

TEST_F(ConcatBufferTest, ViewAfterClear) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  cb.clear();
  EXPECT_TRUE(cb.view().empty());
}

TEST_F(ConcatBufferTest, CapacityAfterAppend) {
  concat_buffer<0, std::string> cb;
  size_t initial_cap = cb.capacity();
  cb.append("Hello");
  EXPECT_GE(cb.capacity(), initial_cap);
}

TEST_F(ConcatBufferTest, AppendNullCString) {
  concat_buffer<0, std::string> cb;
  cb.append("");
  EXPECT_EQ(cb.size(), 0);
}

TEST_F(ConcatBufferTest, FixedBufferAppendExactCapacity) {
  concat_buffer<10> cb;
  EXPECT_TRUE(cb.append("1234567890"));
  EXPECT_EQ(cb.size(), 10);
  EXPECT_FALSE(cb.append("1"));
}

TEST_F(ConcatBufferTest, DynamicBufferRealloc) {
  concat_buffer<0, std::string> cb;
  cb.append("Start");
  char* old_begin = cb.begin();

  for (int i = 0; i < 1000; ++i) {
    cb.append("x");
  }

  char* new_begin = cb.begin();
  EXPECT_NE(old_begin, new_begin);
  EXPECT_EQ(cb.size(), 1005);
}

TEST_F(ConcatBufferTest, OwnedContainerClear) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  cb.clear();
  EXPECT_TRUE(cb.view().empty());
}

TEST_F(ConcatBufferTest, IteratorBeginEnd) {
  concat_buffer<20> cb;
  cb.append("Hello");

  auto begin = cb.begin();
  auto end = cb.end();

  EXPECT_EQ(*begin, 'H');
  EXPECT_EQ(end - begin, 5);
}

TEST_F(ConcatBufferTest, ConstIterator) {
  concat_buffer<20> cb;
  cb.append("Hello");
  const auto& const_cb = cb;

  auto begin = const_cb.begin();
  auto end = const_cb.end();

  EXPECT_EQ(*begin, 'H');
  EXPECT_EQ(end - begin, 5);
}

TEST_F(ConcatBufferTest, EmptyBeginEnd) {
  concat_buffer<20> cb;
  EXPECT_EQ(cb.begin(), cb.end());
}

TEST_F(ConcatBufferTest, CStrNullTermination) {
  concat_buffer<20> cb;
  cb.append("Hello");
  const char* cstr = cb.c_str();
  EXPECT_EQ(cstr[5], '\0');
}

TEST_F(ConcatBufferTest, CStrPreservesContent) {
  concat_buffer<0, std::string> cb;
  cb.append("Hello");
  const char* cstr = cb.c_str();
  EXPECT_EQ(std::string(cstr), "Hello");
}

TEST_F(ConcatBufferTest, AppendBoolFalse) {
  concat_buffer<5> cb;
  cb.append("Hello");
  bool result = cb.append("World");
  EXPECT_FALSE(result);
}

TEST_F(ConcatBufferTest, FixedBufferReturnsFalseOnOverflow) {
  concat_buffer<5> cb;
  EXPECT_TRUE(cb.append("test"));
  EXPECT_FALSE(cb.append("test"));
}

TEST_F(ConcatBufferTest, AppendCharMultipleTimes) {
  concat_buffer<0, std::string> cb;
  for (int i = 0; i < 100; ++i) {
    cb.append('x');
  }
  EXPECT_EQ(cb.size(), 100);
  EXPECT_EQ(std::string(cb.begin(), cb.end()), std::string(100, 'x'));
}

TEST_F(ConcatBufferTest, MixedTypesAppend) {
  concat_buffer<0, std::string> cb;
  cb.append("int:", 42, " short:", short(10), " long:", 100L);
  EXPECT_EQ(cb.view(), "int:42 short:10 long:100");
}
