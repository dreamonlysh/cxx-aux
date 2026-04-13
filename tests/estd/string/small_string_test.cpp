#include <estd/string/small_string.h>
#include <gtest/gtest.h>
#include <string>

using namespace es::string;

TEST(SmallStringTest, DefaultConstructor) {
  small_string<16> str;
  EXPECT_TRUE(str.empty());
  EXPECT_EQ(str.size(), 0);
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, ConstructorWithSizeAndChar) {
  small_string<16> str(5, 'a');
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "aaaaa");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, ConstructorWithSizeAndCharOverflow) {
  small_string<16> str(20, 'a');
  EXPECT_EQ(str.size(), 20);
  EXPECT_FALSE(str.is_small());
}

TEST(SmallStringTest, ConstructorWithCString) {
  small_string<16> str("hello");
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, ConstructorWithCStringOverflow) {
  small_string<16> str("this is a very long string");
  EXPECT_EQ(str.size(), 26);
  EXPECT_FALSE(str.is_small());
}

TEST(SmallStringTest, ConstructorWithCStringAndSize) {
  small_string<16> str("hello world", 5);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, ConstructorWithStringView) {
  std::string_view sv("hello");
  small_string<16> str(sv);
  EXPECT_EQ(str, "hello");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, ConstructorWithInitializerList) {
  small_string<16> str{'h', 'e', 'l', 'l', 'o'};
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, CopyConstructor) {
  small_string<16> str1("hello");
  small_string<16> str2(str1);
  EXPECT_EQ(str2, "hello");
  EXPECT_TRUE(str2.is_small());
}

TEST(SmallStringTest, MoveConstructor) {
  small_string<16> str1("hello");
  small_string<16> str2(std::move(str1));
  EXPECT_EQ(str2, "hello");
}

TEST(SmallStringTest, AssignmentOperator) {
  small_string<16> str1("hello");
  small_string<16> str2;
  str2 = str1;
  EXPECT_EQ(str2, "hello");
}

TEST(SmallStringTest, MoveAssignmentOperator) {
  small_string<16> str1("hello");
  small_string<16> str2;
  str2 = std::move(str1);
  EXPECT_EQ(str2, "hello");
}

TEST(SmallStringTest, AssignmentOperatorWithCString) {
  small_string<16> str;
  str = "hello";
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(SmallStringTest, AssignmentOperatorWithChar) {
  small_string<16> str;
  str = 'a';
  EXPECT_EQ(str.size(), 1);
  EXPECT_EQ(str, "a");
}

TEST(SmallStringTest, AssignmentOperatorWithInitializerList) {
  small_string<16> str;
  str = {'h', 'e', 'l', 'l', 'o'};
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(SmallStringTest, AssignWithNChar) {
  small_string<16> str;
  str.assign(5, 'a');
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "aaaaa");
}

TEST(SmallStringTest, AssignWithCStringSize) {
  small_string<16> str;
  str.assign("hello world", 5);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(SmallStringTest, AssignWithCString) {
  small_string<16> str;
  str.assign("hello");
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(SmallStringTest, At) {
  small_string<16> str("hello");
  EXPECT_EQ(str.at(0), 'h');
  EXPECT_EQ(str.at(4), 'o');
  EXPECT_THROW(str.at(5), std::out_of_range);
}

TEST(SmallStringTest, SubscriptOperator) {
  small_string<16> str("hello");
  EXPECT_EQ(str[0], 'h');
  EXPECT_EQ(str[4], 'o');
}

TEST(SmallStringTest, FrontAndBack) {
  small_string<16> str("hello");
  EXPECT_EQ(str.front(), 'h');
  EXPECT_EQ(str.back(), 'o');
}

TEST(SmallStringTest, DataAndCStr) {
  small_string<16> str("hello");
  EXPECT_STREQ(str.data(), "hello");
  EXPECT_STREQ(str.c_str(), "hello");
}

TEST(SmallStringTest, ImplicitConversionToStringView) {
  small_string<16> str("hello");
  std::string_view sv = str;
  EXPECT_EQ(sv, "hello");
}

TEST(SmallStringTest, BeginEnd) {
  small_string<16> str("hello");
  std::string result;
  for (char c : str) {
    result += c;
  }
  EXPECT_EQ(result, "hello");
}

TEST(SmallStringTest, Clear) {
  small_string<16> str("hello");
  str.clear();
  EXPECT_TRUE(str.empty());
  EXPECT_EQ(str.size(), 0);
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, PushBack) {
  small_string<16> str("hello");
  str.push_back('!');
  EXPECT_EQ(str, "hello!");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, PushBackOverflow) {
  small_string<6> str("hello");
  EXPECT_TRUE(str.is_small());
  str.push_back('!');
  EXPECT_EQ(str, "hello!");
  EXPECT_FALSE(str.is_small());
}

TEST(SmallStringTest, PopBack) {
  small_string<16> str("hello!");
  str.pop_back();
  EXPECT_EQ(str, "hello");
}

TEST(SmallStringTest, PopBackTransitionToSmall) {
  small_string<6> str("hello!");
  EXPECT_FALSE(str.is_small());
  str.pop_back();
  EXPECT_EQ(str, "hello");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, AppendCString) {
  small_string<16> str("hello");
  str.append(" world");
  EXPECT_EQ(str, "hello world");
}

TEST(SmallStringTest, AppendOverflow) {
  small_string<8> str("hello");
  str.append(" world");
  EXPECT_EQ(str, "hello world");
  EXPECT_FALSE(str.is_small());
}

TEST(SmallStringTest, AppendNChar) {
  small_string<16> str("hello");
  str.append(3, '!');
  EXPECT_EQ(str, "hello!!!");
}

TEST(SmallStringTest, OperatorPlusEqual) {
  small_string<16> str("hello");
  str += " world";
  EXPECT_EQ(str, "hello world");
}

TEST(SmallStringTest, OperatorPlusEqualChar) {
  small_string<16> str("hello");
  str += '!';
  EXPECT_EQ(str, "hello!");
}

TEST(SmallStringTest, Insert) {
  small_string<16> str("hello");
  str.insert(5, " world");
  EXPECT_EQ(str, "hello world");
}

TEST(SmallStringTest, InsertOverflow) {
  small_string<8> str("hello");
  str.insert(5, " world");
  EXPECT_EQ(str, "hello world");
  EXPECT_FALSE(str.is_small());
}

TEST(SmallStringTest, Erase) {
  small_string<16> str("hello world");
  str.erase(5, 6);
  EXPECT_EQ(str, "hello");
}

TEST(SmallStringTest, EraseTransitionToSmall) {
  small_string<8> str("hello world");
  EXPECT_FALSE(str.is_small());
  str.erase(5, 6);
  EXPECT_EQ(str, "hello");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, Resize) {
  small_string<16> str("hello");
  str.resize(3);
  EXPECT_EQ(str.size(), 3);
  EXPECT_EQ(str, "hel");
}

TEST(SmallStringTest, ResizeWithChar) {
  small_string<16> str("hello");
  str.resize(8, '!');
  EXPECT_EQ(str.size(), 8);
  EXPECT_EQ(str, "hello!!!");
}

TEST(SmallStringTest, ResizeOverflow) {
  small_string<8> str("hello");
  str.resize(20, '!');
  EXPECT_EQ(str.size(), 20);
  EXPECT_FALSE(str.is_small());
}

TEST(SmallStringTest, Swap) {
  small_string<16> str1("hello");
  small_string<16> str2("world");
  str1.swap(str2);
  EXPECT_EQ(str1, "world");
  EXPECT_EQ(str2, "hello");
}

TEST(SmallStringTest, Find) {
  small_string<16> str("hello world");
  EXPECT_EQ(str.find("world"), 6);
  EXPECT_EQ(str.find("xyz"), small_string<16>::npos);
}

TEST(SmallStringTest, FindChar) {
  small_string<16> str("hello world");
  EXPECT_EQ(str.find('o'), 4);
  EXPECT_EQ(str.find('x'), small_string<16>::npos);
}

TEST(SmallStringTest, RFind) {
  small_string<16> str("hello world");
  EXPECT_EQ(str.rfind("o"), 7);
}

TEST(SmallStringTest, Compare) {
  small_string<16> str1("hello");
  small_string<16> str2("hello");
  small_string<16> str3("world");
  EXPECT_EQ(str1.compare(str2), 0);
  EXPECT_LT(str1.compare(str3), 0);
  EXPECT_GT(str3.compare(str1), 0);
}

TEST(SmallStringTest, StartsWith) {
  small_string<16> str("hello world");
  EXPECT_TRUE(str.starts_with("hello"));
  EXPECT_FALSE(str.starts_with("world"));
  EXPECT_TRUE(str.starts_with('h'));
}

TEST(SmallStringTest, EndsWith) {
  small_string<16> str("hello world");
  EXPECT_TRUE(str.ends_with("world"));
  EXPECT_FALSE(str.ends_with("hello"));
  EXPECT_TRUE(str.ends_with('d'));
}

TEST(SmallStringTest, Contains) {
  small_string<16> str("hello world");
  EXPECT_TRUE(str.contains("lo wo"));
  EXPECT_FALSE(str.contains("xyz"));
  EXPECT_TRUE(str.contains('w'));
}

TEST(SmallStringTest, Substr) {
  small_string<16> str("hello world");
  auto sub = str.substr(0, 5);
  EXPECT_EQ(sub, "hello");
}

TEST(SmallStringTest, OperatorPlus) {
  small_string<16> str1("hello");
  small_string<16> str2(" world");
  auto result = str1 + str2;
  EXPECT_EQ(result, "hello world");
}

TEST(SmallStringTest, OperatorPlusCString) {
  small_string<16> str("hello");
  auto result = str + " world";
  EXPECT_EQ(result, "hello world");
}

TEST(SmallStringTest, OperatorPlusChar) {
  small_string<16> str("hello");
  auto result = str + '!';
  EXPECT_EQ(result, "hello!");
}

TEST(SmallStringTest, OperatorEquality) {
  small_string<16> str1("hello");
  small_string<16> str2("hello");
  small_string<16> str3("world");
  EXPECT_TRUE(str1 == str2);
  EXPECT_FALSE(str1 == str3);
  EXPECT_TRUE(str1 == "hello");
}

TEST(SmallStringTest, OperatorInequality) {
  small_string<16> str1("hello");
  small_string<16> str2("world");
  EXPECT_TRUE(str1 != str2);
  EXPECT_FALSE(str1 != str1);
}

TEST(SmallStringTest, OperatorLessThan) {
  small_string<16> str1("apple");
  small_string<16> str2("banana");
  EXPECT_TRUE(str1 < str2);
  EXPECT_FALSE(str2 < str1);
}

TEST(SmallStringTest, OperatorLessThanOrEqual) {
  small_string<16> str1("apple");
  small_string<16> str2("banana");
  small_string<16> str3("apple");
  EXPECT_TRUE(str1 <= str2);
  EXPECT_TRUE(str1 <= str3);
}

TEST(SmallStringTest, OperatorGreaterThan) {
  small_string<16> str1("banana");
  small_string<16> str2("apple");
  EXPECT_TRUE(str1 > str2);
  EXPECT_FALSE(str2 > str1);
}

TEST(SmallStringTest, OperatorGreaterThanOrEqual) {
  small_string<16> str1("banana");
  small_string<16> str2("apple");
  small_string<16> str3("banana");
  EXPECT_TRUE(str1 >= str2);
  EXPECT_TRUE(str1 >= str3);
}

TEST(SmallStringTest, Reserve) {
  small_string<16> str("hello");
  str.reserve(100);
  EXPECT_GE(str.capacity(), 100);
  EXPECT_FALSE(str.is_small());
}

TEST(SmallStringTest, ShrinkToFit) {
  small_string<8> str("hello world");
  EXPECT_FALSE(str.is_small());
  str.shrink_to_fit();
  EXPECT_EQ(str, "hello world");
}

TEST(SmallStringTest, ShrinkToFitTransitionToSmall) {
  small_string<8> str("hello world");
  EXPECT_FALSE(str.is_small());
  str.erase(5, 6);
  str.shrink_to_fit();
  EXPECT_EQ(str, "hello");
  EXPECT_TRUE(str.is_small());
}

TEST(SmallStringTest, STDSwap) {
  small_string<16> str1("hello");
  small_string<16> str2("world");
  std::swap(str1, str2);
  EXPECT_EQ(str1, "world");
  EXPECT_EQ(str2, "hello");
}

TEST(SmallStringTest, STDOStream) {
  small_string<16> str("hello world");
  std::ostringstream oss;
  oss << str;
  EXPECT_EQ(oss.str(), "hello world");
}

TEST(SmallStringTest, STDHash) {
  small_string<16> str1("hello");
  small_string<16> str2("hello");
  std::hash<small_string<16>> hash_fn;
  EXPECT_EQ(hash_fn(str1), hash_fn(str2));
}

TEST(SmallStringTest, TransitionSmallToLarge) {
  small_string<16> str;
  EXPECT_TRUE(str.is_small());

  str = "hello";
  EXPECT_TRUE(str.is_small());

  str += " world and more";
  EXPECT_FALSE(str.is_small());
  EXPECT_EQ(str, "hello world and more");
}

TEST(SmallStringTest, TransitionLargeToSmall) {
  small_string<16> str("this is a very long string");
  EXPECT_FALSE(str.is_small());

  str = "hello";
  EXPECT_TRUE(str.is_small());
  EXPECT_EQ(str, "hello");
}

TEST(SmallStringTest, CapacitySmallVsLarge) {
  small_string<16> str1("hello");
  EXPECT_TRUE(str1.is_small());
  EXPECT_EQ(str1.capacity(), 15);

  small_string<16> str2("this is a very long string");
  EXPECT_FALSE(str2.is_small());
  EXPECT_GE(str2.capacity(), str2.size());
}
