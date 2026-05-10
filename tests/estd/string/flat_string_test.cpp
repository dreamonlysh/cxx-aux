#include <estd/string/flat_string.h>
#include <gtest/gtest.h>
#include <string>

using namespace es::string;

TEST(FlatStringTest, DefaultConstructor) {
  flat_string<16> str;
  EXPECT_TRUE(str.empty());
  EXPECT_EQ(str.size(), 0);
  EXPECT_EQ(str.capacity(), 15);
}

TEST(FlatStringTest, ConstructorWithSizeAndChar) {
  flat_string<16> str(5, 'a');
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "aaaaa");
}

TEST(FlatStringTest, ConstructorWithRange) {
  const char* data = "hello world";
  flat_string<16> str(data, data + 5);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, ConstructorWithCString) {
  flat_string<16> str("hello");
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, ConstructorWithCStringAndSize) {
  flat_string<16> str("hello world", 5);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, ConstructorWithStringViewLike) {
  std::string_view sv("hello");
  flat_string<16> str(sv);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, ConstructorWithStringViewLikeAndSize) {
  std::string_view sv("hello world");
  flat_string<16> str(sv, 0, 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, CopyConstructor) {
  flat_string<16> str1("hello");
  flat_string<16> str2(str1);
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, MoveConstructor) {
  flat_string<16> str1("hello");
  flat_string<16> str2(std::move(str1));
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, CopyConstructorWithSize) {
  flat_string<16> str1("hello world");
  flat_string<16> str2(str1, 5);
  EXPECT_EQ(str2, " world");
}

TEST(FlatStringTest, MoveConstructorWithSize) {
  flat_string<16> str1("hello world");
  flat_string<16> str2(std::move(str1), 5);
  EXPECT_EQ(str2, " world");
}

TEST(FlatStringTest, CopyConstructorWithPosAndSize) {
  flat_string<16> str1("hello world");
  flat_string<16> str2(str1, 6, 5);
  EXPECT_EQ(str2.size(), 5);
  EXPECT_EQ(str2, "world");
}

TEST(FlatStringTest, MoveConstructorWithPosAndSize) {
  flat_string<16> str1("hello world");
  flat_string<16> str2(std::move(str1), 6, 5);
  EXPECT_EQ(str2.size(), 5);
  EXPECT_EQ(str2, "world");
}

TEST(FlatStringTest, ConstructorWithInitializerList) {
  flat_string<16> str{'h', 'e', 'l', 'l', 'o'};
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignmentOperator) {
  flat_string<16> str1("hello");
  flat_string<16> str2;
  str2 = str1;
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, MoveAssignmentOperator) {
  flat_string<16> str1("hello");
  flat_string<16> str2;
  str2 = std::move(str1);
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, AssignmentOperatorWithCString) {
  flat_string<16> str;
  str = "hello";
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignmentOperatorWithChar) {
  flat_string<16> str;
  str = 'a';
  EXPECT_EQ(str.size(), 1);
  EXPECT_EQ(str, "a");
}

TEST(FlatStringTest, AssignmentOperatorWithInitializerList) {
  std::string_view sv("hello");
  flat_string<16> str;
  str = sv;
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignmentOperatorWithStringViewLike) {
  std::string_view sv("hello");
  flat_string<16> str;
  str = sv;
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignWithFlatString) {
  flat_string<16> str1("hello");
  flat_string<16> str2;
  str2.assign(str1);
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, AssignWithMoveFlatString) {
  flat_string<16> str1("hello");
  flat_string<16> str2;
  str2.assign(std::move(str1));
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, AssignWithNChar) {
  flat_string<16> str;
  str.assign(5, 'a');
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "aaaaa");
}

TEST(FlatStringTest, AssignWithCStringSize) {
  flat_string<16> str;
  str.assign("hello world", 5);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignWithCString) {
  flat_string<16> str;
  str.assign("hello");
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignWithStringViewLike) {
  flat_string<16> str;
  std::string_view sv("hello");
  str.assign(sv);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignWithStringViewLikeAndPosAndSize) {
  flat_string<16> str;
  std::string_view sv("hello world");
  str.assign(sv, 6, 5);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "world");
}

TEST(FlatStringTest, AssignWithMoveFlatStringPosAndSize) {
  flat_string<16> str1("hello world");
  flat_string<16> str2;
  str2.assign(std::move(str1), 6, 5);
  EXPECT_EQ(str2, "world");
}

TEST(FlatStringTest, AssignWithIterator) {
  flat_string<16> str;
  const char* data = "hello";
  str.assign(data, data + 5);
  EXPECT_EQ(str.size(), 5);
}

TEST(FlatStringTest, AssignWithInitializerList) {
  flat_string<16> str;
  str.assign({'h', 'e', 'l', 'l', 'o'});
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AssignWithRange) {
  flat_string<16> str;
  const char* data = "hello world";
  str.assign(data, data + 5);
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, At) {
  flat_string<16> str("hello");
  EXPECT_EQ(str.at(0), 'h');
  EXPECT_EQ(str.at(4), 'o');
  EXPECT_THROW(str.at(5), std::out_of_range);
}

TEST(FlatStringTest, SubscriptOperator) {
  flat_string<16> str("hello");
  EXPECT_EQ(str[0], 'h');
  EXPECT_EQ(str[4], 'o');
}

TEST(FlatStringTest, FrontAndBack) {
  flat_string<16> str("hello");
  EXPECT_EQ(str.front(), 'h');
  EXPECT_EQ(str.back(), 'o');
}

TEST(FlatStringTest, DataAndCStr) {
  flat_string<16> str("hello");
  EXPECT_STREQ(str.data(), "hello");
  EXPECT_STREQ(str.c_str(), "hello");
}

TEST(FlatStringTest, ImplicitConversionToStringView) {
  flat_string<16> str("hello");
  std::string_view sv = str;
  EXPECT_EQ(sv, "hello");
}

TEST(FlatStringTest, Begin) {
  flat_string<16> str("hello");
  auto it = str.begin();
  EXPECT_EQ(*it, 'h');
  ++it;
  EXPECT_EQ(*it, 'e');
  ++it;
  EXPECT_EQ(*it, 'l');
  ++it;
  EXPECT_EQ(*it, 'l');
  ++it;
  EXPECT_EQ(*it, 'o');
  ++it;
  EXPECT_EQ(it, str.end());
}

TEST(FlatStringTest, End) {
  flat_string<16> str("hello");
  auto it = str.end();
  --it;
  EXPECT_EQ(*it, 'o');
  --it;
  EXPECT_EQ(*it, 'l');
  --it;
  EXPECT_EQ(*it, 'l');
  --it;
  EXPECT_EQ(*it, 'e');
  --it;
  EXPECT_EQ(*it, 'h');
  EXPECT_EQ(it, str.begin());
}

TEST(FlatStringTest, RBegin) {
  flat_string<16> str("hello");
  auto it = str.rbegin();
  EXPECT_EQ(*it, 'o');
  ++it;
  EXPECT_EQ(*it, 'l');
  ++it;
  EXPECT_EQ(*it, 'l');
  ++it;
  EXPECT_EQ(*it, 'e');
  ++it;
  EXPECT_EQ(*it, 'h');
  ++it;
  EXPECT_EQ(it, str.rend());
}

TEST(FlatStringTest, REnd) {
  flat_string<16> str("hello");
  auto it = str.rend();
  --it;
  EXPECT_EQ(*it, 'h');
  --it;
  EXPECT_EQ(*it, 'e');
  --it;
  EXPECT_EQ(*it, 'l');
  --it;
  EXPECT_EQ(*it, 'l');
  --it;
  EXPECT_EQ(*it, 'o');
  EXPECT_EQ(it, str.rbegin());
}

TEST(FlatStringTest, Clear) {
  flat_string<16> str("hello");
  str.clear();
  EXPECT_TRUE(str.empty());
  EXPECT_EQ(str.size(), 0);
}

TEST(FlatStringTest, InsertWithNChar) {
  flat_string<16> str("hello");
  str.insert(5, 3, '!');
  EXPECT_EQ(str, "hello!!!");
}

TEST(FlatStringTest, InsertWithCString) {
  flat_string<16> str("hello");
  str.insert(5, " world", 6);
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, InsertWithCStringAndSize) {
  flat_string<16> str("hello");
  str.insert(5, " world", 5);
  EXPECT_EQ(str, "hello worl");
}

TEST(FlatStringTest, InsertWithFlatString) {
  flat_string<16> str("hello");
  flat_string<16> str2(" world");
  str.insert(5, str2);
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, InsertWithFlatStringPosAndSize) {
  flat_string<16> str("hello");
  flat_string<16> str2(" world");
  str.insert(5, str2, 0, 5);
  EXPECT_EQ(str, "hello worl");
}

TEST(FlatStringTest, InsertWithChar) {
  flat_string<16> str("hello");
  str.insert(str.begin() + 5, '!');
  EXPECT_EQ(str, "hello!");
}

TEST(FlatStringTest, InsertByIteratorWithNChar) {
  flat_string<16> str("hello");
  str.insert(str.end(), 3, '!');
  EXPECT_EQ(str, "hello!!!");
}

TEST(FlatStringTest, InsertByIteratorWithBeginEnd) {
  flat_string<16> str("hello");
  flat_string<16> str2(" world");
  str.insert(str.end(), str2.begin(), str2.end());
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, InsertByIteratorWithInitializerList) {
  flat_string<16> str("hello");
  str.insert(str.end(), {' ', 'w', 'o', 'r', 'l', 'd'});
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, InsertByIteratorWithStringViewLike) {
  flat_string<16> str("hello");
  std::string_view sv(" world");
  str.insert(str.end(), sv.begin(), sv.end());
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, InsertByIteratorWithStringViewLikePosAndSize) {
  flat_string<16> str("hello");
  std::string_view sv(" world");
  str.insert(str.end(), sv.data(), sv.data() + 5);
  EXPECT_EQ(str, "hello worl");
}

TEST(FlatStringTest, InsertWithRange) {
  flat_string<16> str("hello");
  const char data[] = " world";
  str.insert(5, data);
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, Erase) {
  flat_string<16> str("hello world");
  str.erase(5, 6);
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, EraseWithRange) {
  flat_string<16> str("hello world");
  str.erase(str.begin() + 5, str.end());
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, EraseWithIterator) {
  flat_string<16> str("hello world");
  str.erase(str.begin() + 5);
  EXPECT_EQ(str, "helloworld");
}

TEST(FlatStringTest, PushBack) {
  flat_string<16> str("hello");
  str.push_back('!');
  EXPECT_EQ(str, "hello!");
}

TEST(FlatStringTest, PopBack) {
  flat_string<16> str("hello!");
  str.pop_back();
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, AppendWithNChar) {
  flat_string<16> str("hello");
  str.append(3, '!');
  EXPECT_EQ(str, "hello!!!");
}

TEST(FlatStringTest, AppendWithCString) {
  flat_string<16> str("hello");
  str.append(" world");
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, AppendWithCStringAndSize) {
  flat_string<16> str("hello");
  str.append(" world", 5);
  EXPECT_EQ(str, "hello worl");
}

TEST(FlatStringTest, AppendWithStringViewLike) {
  flat_string<16> str("hello");
  std::string_view sv(" world");
  str.append(sv);
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, AppendWithStringViewLikeAndPosAndSize) {
  flat_string<16> str("hello");
  std::string_view sv(" world");
  str.append(sv, 0, 5);
  EXPECT_EQ(str, "hello worl");
}

TEST(FlatStringTest, AppendWithFlatString) {
  flat_string<16> str("hello");
  flat_string<16> str2(" world");
  str.append(str2);
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, AppendWithFlatStringPosAndSize) {
  flat_string<16> str("hello");
  flat_string<16> str2(" world");
  str.append(str2, 0, 5);
  EXPECT_EQ(str, "hello worl");
}

TEST(FlatStringTest, AppendWithIterator) {
  flat_string<16> str("hello");
  const char* data = " world";
  str.append(data, data + 6);
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, AppendWithInitializerList) {
  flat_string<16> str("hello");
  str.append({' ', 'w', 'o', 'r', 'l', 'd'});
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, AppendWithRange) {
  flat_string<16> str("hello");
  const char data[] = " world";
  str.append(data);
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, OperatorPlusEqualFlatString) {
  flat_string<16> str1("hello");
  flat_string<16> str2(" world");
  str1 += str2;
  EXPECT_EQ(str1, "hello world");
}

TEST(FlatStringTest, OperatorPlusEqualChar) {
  flat_string<16> str("hello");
  str += '!';
  EXPECT_EQ(str, "hello!");
}

TEST(FlatStringTest, OperatorPlusEqualCString) {
  flat_string<16> str("hello");
  str += " world";
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, OperatorPlusEqualInitializerList) {
  flat_string<16> str("hello");
  str += {' ', 'w', 'o', 'r', 'l', 'd'};
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, OperatorPlusEqualStringViewLike) {
  flat_string<16> str("hello");
  std::string_view sv(" world");
  str += sv;
  EXPECT_EQ(str, "hello world");
}

TEST(FlatStringTest, ReplaceWithPosAndCount) {
  flat_string<16> str("hello world");
  str.replace(6, 5, "everyone");
  EXPECT_EQ(str, "hello everyone");

  str.replace(0, 5, "hi");
  EXPECT_EQ(str, "hi everyone");
  str.replace(0, 2, "hello");
  EXPECT_EQ(str, "hello everyone");
  str.replace(0, 5, "hi");
  EXPECT_EQ(str, "hi everyone");
}

TEST(FlatStringTest, ReplaceWithRange) {
  flat_string<32> str("hello world");
  str.replace(str.begin() + 6, str.end(), "everyone");
  EXPECT_EQ(str, "hello everyone");

  str.replace(str.begin(), str.begin() + 2, "hi");
  EXPECT_EQ(str, "hillo everyone");
  str.replace(str.begin(), str.begin() + 2, "hello");
  EXPECT_EQ(str, "hellollo everyone");
  str.replace(str.begin(), str.begin() + 5, "hi");
  EXPECT_EQ(str, "hillo everyone");
}

TEST(FlatStringTest, ReplacePosAndCountWithOtherFlatStringPosAndCount) {
  flat_string<16> str1("hello world");
  flat_string<16> str2("everyone");
  str1.replace(6, 5, str2, 0, 8);
  EXPECT_EQ(str1, "hello everyone");

  str1.replace(0, 5, str2, 0, 2);
  EXPECT_EQ(str1, "ev everyone");
}

TEST(FlatStringTest, ReplacePosAndCountWithCStringCount) {
  flat_string<16> str("hello world");
  str.replace(6, 5, "everyone", 8);
  EXPECT_EQ(str, "hello everyone");
  str.replace(0, 5, "hi");
  EXPECT_EQ(str, "hi everyone");
  str.replace(0, 2, "hello");
  EXPECT_EQ(str, "hello everyone");
  str.replace(0, 5, "hi");
  EXPECT_EQ(str, "hi everyone");
}

TEST(FlatStringTest, ReplaceIteratorWithCStringCount) {
  flat_string<16> str("hello world");
  str.replace(str.begin() + 6, str.end(), "everyone", 8);
  EXPECT_EQ(str, "hello everyone");
}

TEST(FlatStringTest, ReplaceIteratorWithCString) {
  flat_string<32> str("hello world");
  str.replace(str.begin() + 6, str.end(), "everyone");
  EXPECT_EQ(str, "hello everyone");

  str.replace(str.begin(), str.begin() + 2, "hi");
  EXPECT_EQ(str, "hillo everyone");
  str.replace(str.begin(), str.begin() + 2, "hello");
  EXPECT_EQ(str, "hellollo everyone");
  str.replace(str.begin(), str.begin() + 5, "hi");
  EXPECT_EQ(str, "hillo everyone");
}

TEST(FlatStringTest, ReplacePosAndCountWithNChar) {
  flat_string<16> str("hello world");
  str.replace(6, 5, 3, '!');
  EXPECT_EQ(str, "hello !!!");
}

TEST(FlatStringTest, ReplaceIteratorWithNChar) {
  flat_string<16> str("hello world");
  str.replace(str.begin() + 6, str.end(), 3, '!');
  EXPECT_EQ(str, "hello !!!");
}

TEST(FlatStringTest, ReplaceIteratorWithRange) {
  flat_string<16> str("hello world");
  const char data[] = "everyone";
  str.replace(str.begin() + 6, str.end(), data, data + 8);
  EXPECT_EQ(str, "hello everyone");
}

TEST(FlatStringTest, ReplaceIteratorWithInitializerList) {
  flat_string<16> str("hello world");
  str.replace(str.begin() + 6, str.end(),
              {'e', 'v', 'e', 'r', 'y', 'o', 'n', 'e'});
  EXPECT_EQ(str, "hello everyone");
}

TEST(FlatStringTest, ReplaceIteratorWithStringViewLike) {
  flat_string<16> str("hello world");
  std::string_view sv("everyone");
  str.replace(str.begin() + 6, str.end(), sv);
  EXPECT_EQ(str, "hello everyone");
}

TEST(FlatStringTest, ReplacePosAndCountWithStringViewLikePosAndSize) {
  flat_string<16> str("hello world");
  std::string_view sv("everyone");
  str.replace(6, 5, sv, 0, 8);
  EXPECT_EQ(str, "hello everyone");

  str.replace(0, 5, sv, 0, 2);
  EXPECT_EQ(str, "ev everyone");
}

TEST(FlatStringTest, ReplaceWithRangeByPosAndCount) {
  flat_string<16> str("hello world");
  std::string_view data = "everyone";
  str.replace_with_range(str.begin() + 6, str.end(), data);
  EXPECT_EQ(str, "hello everyone");
  str.replace_with_range(str.begin(), str.begin() + 5, std::string_view("hi"));
  EXPECT_EQ(str, "hi everyone");
  str.replace_with_range(str.begin(), str.begin() + 2,
                         std::string_view("hello"));
  EXPECT_EQ(str, "hello everyone");
  str.replace_with_range(str.begin(), str.begin() + 5, std::string_view("hi"));
  EXPECT_EQ(str, "hi everyone");
}

TEST(FlatStringTest, Copy) {
  flat_string<16> str("hello");
  char buffer[10];
  str.copy(buffer, 5);
  buffer[5] = '\0';
  EXPECT_STREQ(buffer, "hello");
}

TEST(FlatStringTest, Resize) {
  flat_string<16> str("hello");
  str.resize(3);
  EXPECT_EQ(str.size(), 3);
  EXPECT_EQ(str, "hel");
}

TEST(FlatStringTest, ResizeWithChar) {
  flat_string<16> str("hello");
  str.resize(8, '!');
  EXPECT_EQ(str.size(), 8);
  EXPECT_EQ(str, "hello!!!");
}

TEST(FlatStringTest, ResizeAndOverwrite) {
  flat_string<16> str("hello");
  str.resize(3, '!');
  EXPECT_EQ(str.size(), 3);
  EXPECT_EQ(str, "hel");
  str.resize_and_overwrite(5, [](char* data, size_t size) {
    for (size_t i = 0; i < 5; ++i) {
      data[i] = '!';
    }
    return 5;
  });
  EXPECT_EQ(str.size(), 5);
  EXPECT_EQ(str, "!!!!!");
}

TEST(FlatStringTest, Swap) {
  flat_string<16> str1("hello");
  flat_string<16> str2("world");
  str1.swap(str2);
  EXPECT_EQ(str1, "world");
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, FindFlatStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find("lo", 2);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, FindCStringWithStartPosAndCount) {
  flat_string<16> str("hello world");
  auto pos = str.find("lo", 2, 2);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, FindCStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find("lo", 2);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, FindCharWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find('o', 2);
  EXPECT_EQ(pos, 4);
}

TEST(FlatStringTest, FindStringViewLikeWithStartPos) {
  flat_string<16> str("hello world");
  std::string_view sv("lo");
  auto pos = str.find(sv, 2);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, RFindFlatStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.rfind("lo", 8);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, RFindCStringWithStartPosAndCount) {
  flat_string<16> str("hello world");
  auto pos = str.rfind("lo", 8, 2);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, RFindCStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.rfind("lo", 8);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, RFindCharWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.rfind('o', 8);
  EXPECT_EQ(pos, 7);
}

TEST(FlatStringTest, RFindStringViewLikeWithStartPos) {
  flat_string<16> str("hello world");
  std::string_view sv("lo");
  auto pos = str.rfind(sv, 8);
  EXPECT_EQ(pos, 3);
}

TEST(FlatStringTest, FindFirstOfFlatStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_of("lo", 2);
  EXPECT_EQ(pos, 2);
}
TEST(FlatStringTest, FindFirstOfCStringWithStartPosAndCount) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_of("lo", 2, 2);
  EXPECT_EQ(pos, 2);
}
TEST(FlatStringTest, FindFirstOfCStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_of("lo", 2);
  EXPECT_EQ(pos, 2);
}
TEST(FlatStringTest, FindFirstOfCharWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_of('o', 2);
  EXPECT_EQ(pos, 4);
}
TEST(FlatStringTest, FindFirstOfStringViewLikeWithStartPos) {
  flat_string<16> str("hello world");
  std::string_view sv("lo");
  auto pos = str.find_first_of(sv, 2);
  EXPECT_EQ(pos, 2);
}
TEST(FlatStringTest, FindFirstNotOfFlatStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_not_of("lo", 2);
  EXPECT_EQ(pos, 5);
}
TEST(FlatStringTest, FindFirstNotOfCStringWithStartPosAndCount) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_not_of("lo", 2, 2);
  EXPECT_EQ(pos, 5);
}
TEST(FlatStringTest, FindFirstNotOfCStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_not_of("lo", 2);
  EXPECT_EQ(pos, 5);
}
TEST(FlatStringTest, FindFirstNotOfCharWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_first_not_of('o', 2);
  EXPECT_EQ(pos, 2);
}
TEST(FlatStringTest, FindFirstNotOfStringViewLikeWithStartPos) {
  flat_string<16> str("hello world");
  std::string_view sv("lo");
  auto pos = str.find_first_not_of(sv, 2);
  EXPECT_EQ(pos, 5);
}

TEST(FlatStringTest, FindLastOfFlatStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_of("lo", 8);
  EXPECT_EQ(pos, 7);
}
TEST(FlatStringTest, FindLastOfCStringWithStartPosAndCount) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_of("lo", 8, 2);
  EXPECT_EQ(pos, 7);
}
TEST(FlatStringTest, FindLastOfCStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_of("lo", 8);
  EXPECT_EQ(pos, 7);
}
TEST(FlatStringTest, FindLastOfCharWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_of('o', 8);
  EXPECT_EQ(pos, 7);
}
TEST(FlatStringTest, FindLastOfStringViewLikeWithStartPos) {
  flat_string<16> str("hello world");
  std::string_view sv("lo");
  auto pos = str.find_last_of(sv, 8);
  EXPECT_EQ(pos, 7);
}
TEST(FlatStringTest, FindLastNotOfFlatStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_not_of("lo", 8);
  EXPECT_EQ(pos, 8);
}
TEST(FlatStringTest, FindLastNotOfCStringWithStartPosAndCount) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_not_of("lo", 8, 2);
  EXPECT_EQ(pos, 8);
}
TEST(FlatStringTest, FindLastNotOfCStringWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_not_of("lo", 8);
  EXPECT_EQ(pos, 8);
}
TEST(FlatStringTest, FindLastNotOfCharWithStartPos) {
  flat_string<16> str("hello world");
  auto pos = str.find_last_not_of('o', 8);
  EXPECT_EQ(pos, 8);
}
TEST(FlatStringTest, FindLastNotOfStringViewLikeWithStartPos) {
  flat_string<16> str("hello world");
  std::string_view sv("lo");
  auto pos = str.find_last_not_of(sv, 8);
  EXPECT_EQ(pos, 8);
}

TEST(FlatStringTest, CompareFlatString) {
  flat_string<16> str1("hello");
  flat_string<16> str2("hello");
  EXPECT_EQ(str1.compare(str2), 0);
}

TEST(FlatStringTest, ComparePosCountFlatString) {
  flat_string<16> str1("hello world");
  flat_string<16> str2("hello");
  EXPECT_EQ(str1.compare(0, 5, str2), 0);
  EXPECT_EQ(str1.compare(0, 5, "hello"), 0);
  EXPECT_TRUE(str1.compare(0, 5, "world") < 0);
  EXPECT_TRUE(str1.compare(0, 5, "hello world") < 0);
}

TEST(FlatStringTest, ComparePosCountFlatStringPosCount) {
  flat_string<16> str1("hello world");
  flat_string<16> str2("hello");
  EXPECT_EQ(str1.compare(0, 5, str2, 0, 5), 0);
  EXPECT_TRUE(str1.compare(0, 5, str2, 0, 2) > 0);
  EXPECT_EQ(str1.compare(0, 5, str2, 0, 10), 0);
}

TEST(FlatStringTest, CompareCString) {
  flat_string<16> str("hello");
  EXPECT_EQ(str.compare("hello"), 0);
  EXPECT_TRUE(str.compare("world") < 0);
  EXPECT_EQ(str.compare("hello world"), -1);
}

TEST(FlatStringTest, ComparePosCountCString) {
  flat_string<16> str("hello world");
  EXPECT_EQ(str.compare(0, 5, "hello"), 0);
  EXPECT_TRUE(str.compare(0, 5, "world") < 0);
  EXPECT_EQ(str.compare(0, 5, "hello world"), -1);
}

TEST(FlatStringTest, ComparePosCountCStringPosCount) {
  flat_string<16> str1("hello world");
  EXPECT_EQ(str1.compare(0, 5, "hello", 5), 0);
  EXPECT_TRUE(str1.compare(0, 5, "hello", 2) > 0);
  EXPECT_EQ(str1.compare(0, 5, "hello", 10), -1);
}

TEST(FlatStringTest, CompareStringViewLike) {
  flat_string<16> str("hello");
  std::string_view sv("hello");
  EXPECT_EQ(str.compare(sv), 0);
  EXPECT_TRUE(str.compare("world") < 0);
  EXPECT_EQ(str.compare("hello world"), -1);
}

TEST(FlatStringTest, ComparePosCountStringViewLike) {
  flat_string<16> str("hello world");
  std::string_view sv("hello");
  EXPECT_EQ(str.compare(0, 5, sv), 0);
  EXPECT_TRUE(str.compare(0, 5, "world") < 0);
  EXPECT_TRUE(str.compare(0, 5, "hello world") < 0);
}

TEST(FlatStringTest, ComparePosCountStringViewLikePosCount) {
  flat_string<16> str1("hello world");
  std::string_view sv("hello");
  EXPECT_EQ(str1.compare(0, 5, sv, 0, 5), 0);
  EXPECT_EQ(str1.compare(0, 5, sv, 0, 2), 1);
  EXPECT_EQ(str1.compare(0, 5, sv, 0, 10), 0);
}

TEST(FlatStringTest, StartsWith) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str.starts_with("he"));
  EXPECT_FALSE(str.starts_with("wo"));
}

TEST(FlatStringTest, StartWithCString) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str.starts_with("he"));
  EXPECT_FALSE(str.starts_with("wo"));
}

TEST(FlatStringTest, StartsWithStringViewLike) {
  flat_string<16> str("hello");
  std::string_view sv("he");
  EXPECT_TRUE(str.starts_with(sv));
  EXPECT_FALSE(str.starts_with("wo"));
}

TEST(FlatStringTest, StartsWithChar) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str.starts_with('h'));
  EXPECT_FALSE(str.starts_with('w'));
}

TEST(FlatStringTest, EndsWith) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str.ends_with("lo"));
  EXPECT_FALSE(str.ends_with("he"));
}

TEST(FlatStringTest, EndsWithCString) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str.ends_with("lo"));
  EXPECT_FALSE(str.ends_with("he"));
}

TEST(FlatStringTest, EndsWithStringViewLike) {
  flat_string<16> str("hello");
  std::string_view sv("lo");
  EXPECT_TRUE(str.ends_with(sv));
  EXPECT_FALSE(str.ends_with("he"));
}

TEST(FlatStringTest, EndsWithChar) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str.ends_with('o'));
  EXPECT_FALSE(str.ends_with('h'));
}

TEST(FlatStringTest, Contains) {
  flat_string<16> str("hello world");
  EXPECT_TRUE(str.contains(flat_string<16>{"lo"}));
  EXPECT_FALSE(str.contains(flat_string<16>{"ow"}));
}

TEST(FlatStringTest, ContainsCString) {
  flat_string<16> str("hello world");
  EXPECT_TRUE(str.contains("lo"));
  EXPECT_FALSE(str.contains("ow"));
}

TEST(FlatStringTest, ContainsStringViewLike) {
  flat_string<16> str("hello world");
  std::string_view sv("lo");
  EXPECT_TRUE(str.contains(sv));
  sv = "ow";
  EXPECT_FALSE(str.contains(sv));
}

TEST(FlatStringTest, ContainsChar) {
  flat_string<16> str("hello world");
  EXPECT_TRUE(str.contains('l'));
  EXPECT_FALSE(str.contains('x'));
}

TEST(FlatStringTest, Substr) {
  flat_string<16> str("hello");
  auto sub = str.substr(1, 3);
  EXPECT_EQ(sub, "ell");
}

TEST(FlatStringTest, SubstrWithLRef) {
  auto sub = flat_string<16>("hello").substr(1, 3);
  EXPECT_EQ(sub, "ell");
}

TEST(FlatStringTest, SubstrWithRRef) {
  flat_string<16> str("hello");
  auto sub = std::move(str).substr(1, 3);
  EXPECT_EQ(sub, "ell");
}

TEST(FlatStringTest, SubstrWithPos) {
  flat_string<16> str("hello");
  auto sub = str.substr(1);
  EXPECT_EQ(sub, "ello");
}

TEST(FlatStringTest, OperatorPlusFlatAndFlatString) {
  flat_string<16> str1("hello");
  flat_string<16> str2("world");
  auto result = str1 + str2;
  EXPECT_EQ(result, "helloworld");
}

TEST(FlatStringTest, OperatorPlusFlatAndCString) {
  flat_string<16> str("hello");
  auto result = str + " world";
  EXPECT_EQ(result, "hello world");
}

TEST(FlatStringTest, OperatorPlusFlatAndStringViewLike) {
  flat_string<16> str("hello");
  std::string_view sv(" world");
  auto result = str + sv;
  EXPECT_EQ(result, "hello world");
}

TEST(FlatStringTest, OperatorPlusCStringAndFlatString) {
  flat_string<16> str("hello");
  auto result = "world" + str;
  EXPECT_EQ(result, "worldhello");
}

TEST(FlatStringTest, OperatorPlusCharAndFlatString) {
  flat_string<16> str("hello");
  auto result = '!' + str;
  EXPECT_EQ(result, "!hello");
}

TEST(FlatStringTest, OperatorPlusStringViewLikeAndFlatString) {
  flat_string<16> str("hello");
  std::string_view sv(" world");
  auto result = sv + str;
  EXPECT_EQ(result, " worldhello");
}

TEST(FlatStringTest, OperatorPlusFlatAndFlatStringRR) {
  auto result = flat_string<16>("hello") + flat_string<16>(" world");
  EXPECT_EQ(result, "hello world");
}

TEST(FlatStringTest, OperatorPlusFlatAndFlatStringRL) {
  flat_string<16> str(" world");
  auto result = flat_string<16>("hello") + str;
  EXPECT_EQ(result, "hello world");
}

TEST(FlatStringTest, OperatorPlusFlatAndCStringR) {
  auto result = flat_string<16>("hello") + " world";
  EXPECT_EQ(result, "hello world");
}

TEST(FlatStringTest, OperatorPlusFlatAndCharR) {
  auto result = flat_string<16>("hello") + '!';
  EXPECT_EQ(result, "hello!");
}

TEST(FlatStringTest, OperatorPlusFlatAndStringViewLikeR) {
  std::string_view sv(" world");
  auto result = flat_string<16>("hello") + sv;
  EXPECT_EQ(result, "hello world");
}

TEST(FlatStringTest, OperatorPlusFlatAndFlatStringLR) {
  flat_string<16> str("hello");
  auto result = str + flat_string<16>(" world");
  EXPECT_EQ(result, "hello world");
}

TEST(FlatStringTest, OperatorPlusCStringAndFlatStringR) {
  auto result = " world" + flat_string<16>("hello");
  EXPECT_EQ(result, " worldhello");
}

TEST(FlatStringTest, OperatorPlusCharAndFlatStringR) {
  auto result = '!' + flat_string<16>("hello");
  EXPECT_EQ(result, "!hello");
}

TEST(FlatStringTest, OperatorPlusStringViewLikeAndFlatStringR) {
  std::string_view sv(" world");
  auto result = sv + flat_string<16>("hello");
  EXPECT_EQ(result, " worldhello");
}

TEST(FlatStringTest, OperatorEqualityFlatAndFlatString) {
  flat_string<16> str1("hello");
  flat_string<16> str2("hello");
  flat_string<16> str3("world");
  EXPECT_TRUE(str1 == str2);
  EXPECT_FALSE(str1 == str3);
}

TEST(FlatStringTest, OperatorEqualityFlatAndCString) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str == "hello");
  EXPECT_FALSE(str == "world");
}

TEST(FlatStringTest, OperatorEqualityCStringAndFlatString) {
  flat_string<16> str("hello");
  EXPECT_TRUE("hello" == str);
  EXPECT_FALSE("world" == str);
}

TEST(FlatStringTest, OperatorInequalityFlatAndFlatString) {
  flat_string<16> str1("hello");
  flat_string<16> str2("world");
  flat_string<16> str3("hello");
  EXPECT_TRUE(str1 != str2);
  EXPECT_FALSE(str1 != str3);
}

TEST(FlatStringTest, OperatorInequalityFlatAndCString) {
  flat_string<16> str("hello");
  EXPECT_TRUE(str != "world");
  EXPECT_FALSE(str != "hello");
}

TEST(FlatStringTest, OperatorInequalityCStringAndFlatString) {
  flat_string<16> str("hello");
  EXPECT_TRUE("world" != str);
  EXPECT_FALSE("hello" != str);
}

TEST(FlatStringTest, OperatorLessThanFlatAndFlatString) {
  flat_string<16> str1("apple");
  flat_string<16> str2("banana");
  EXPECT_TRUE(str1 < str2);
  EXPECT_FALSE(str2 < str1);
}

TEST(FlatStringTest, OperatorLessThanFlatAndCString) {
  flat_string<16> str("apple");
  EXPECT_TRUE(str < "banana");
  EXPECT_FALSE(str < "apple");
}

TEST(FlatStringTest, OperatorLessThanCStringAndFlatString) {
  flat_string<16> str("banana");
  EXPECT_TRUE("apple" < str);
  EXPECT_FALSE("banana" < str);
}

TEST(FlatStringTest, OperatorLessThanOrEqualFlatAndFlatString) {
  flat_string<16> str1("apple");
  flat_string<16> str2("banana");
  flat_string<16> str3("apple");
  EXPECT_TRUE(str1 <= str2);
  EXPECT_TRUE(str1 <= str3);
  EXPECT_FALSE(str2 <= str1);
}

TEST(FlatStringTest, OperatorLessThanOrEqualFlatAndCString) {
  flat_string<16> str("apple");
  EXPECT_TRUE(str <= "banana");
  EXPECT_TRUE(str <= "apple");
  EXPECT_TRUE(str <= "apples");
}

TEST(FlatStringTest, OperatorLessThanOrEqualCStringAndFlatString) {
  flat_string<16> str("banana");
  EXPECT_TRUE("apple" <= str);
  EXPECT_TRUE("banana" <= str);
}

TEST(FlatStringTest, OperatorGreaterThanFlatAndFlatString) {
  flat_string<16> str1("banana");
  flat_string<16> str2("apple");
  EXPECT_TRUE(str1 > str2);
  EXPECT_FALSE(str2 > str1);
}

TEST(FlatStringTest, OperatorGreaterThanFlatAndCString) {
  flat_string<16> str("banana");
  EXPECT_TRUE(str > "apple");
  EXPECT_FALSE(str > "banana");
}

TEST(FlatStringTest, OperatorGreaterThanCStringAndFlatString) {
  flat_string<16> str("apple");
  EXPECT_TRUE("banana" > str);
  EXPECT_FALSE("apple" > str);
}

TEST(FlatStringTest, OperatorGreaterThanOrEqualFlatAndFlatString) {
  flat_string<16> str1("banana");
  flat_string<16> str2("apple");
  flat_string<16> str3("banana");
  EXPECT_TRUE(str1 >= str2);
  EXPECT_TRUE(str1 >= str3);
  EXPECT_FALSE(str2 >= str1);
}

TEST(FlatStringTest, OperatorGreaterThanOrEqualFlatAndCString) {
  flat_string<16> str("banana");
  EXPECT_TRUE(str >= "apple");
  EXPECT_TRUE(str >= "banana");
  EXPECT_FALSE(str >= "bananas");
}

TEST(FlatStringTest, OperatorGreaterThanOrEqualCStringAndFlatString) {
  flat_string<16> str("apple");
  EXPECT_TRUE("banana" >= str);
  EXPECT_TRUE("apple" >= str);
}

TEST(FlatStringTest, STDSwap) {
  flat_string<16> str1("hello");
  flat_string<16> str2("world");
  std::swap(str1, str2);
  EXPECT_EQ(str1, "world");
  EXPECT_EQ(str2, "hello");
}

TEST(FlatStringTest, STDErase) {
  flat_string<16> str("hello world");
  std::erase(str, 'o');
  EXPECT_EQ(str, "hell wrld");
}

TEST(FlatStringTest, STDEraseIf) {
  flat_string<16> str("hello world");
  std::erase_if(str, [](char c) { return c == 'o'; });
  EXPECT_EQ(str, "hell wrld");
}

TEST(FlatStringTest, STDIStream) {
  flat_string<16> str;
  std::istringstream iss("hello world");
  iss >> str;
  EXPECT_EQ(str, "hello");
}

TEST(FlatStringTest, STDOStream) {
  flat_string<16> str("hello world");
  std::ostringstream oss;
  oss << str;
  EXPECT_EQ(oss.str(), "hello world");
}

TEST(FlatStringTest, STDGetLine) {
  flat_string<16> str;
  std::istringstream iss("hello world");
  std::getline(iss, str);
  EXPECT_EQ(str, "hello world");
  EXPECT_EQ(str.size(), 11);
}

TEST(FlatStringTest, STDGetLineWithDelimiter) {
  flat_string<16> str;
  std::istringstream iss("hello world\n");
  std::getline(iss, str, ' ');
  EXPECT_EQ(str, "hello");
  EXPECT_EQ(str.size(), 5);
}

TEST(FlatStringTest, STDGetLineR) {
  flat_string<16> str;
  std::istringstream iss("hello world");
  std::getline(std::move(iss), str);
  EXPECT_EQ(str, "hello world");
  EXPECT_EQ(str.size(), 11);
}

TEST(FlatStringTest, STDGetLineWithDelimiterR) {
  flat_string<16> str;
  std::istringstream iss("hello world\n");
  std::getline(std::move(iss), str, ' ');
  EXPECT_EQ(str, "hello");
  EXPECT_EQ(str.size(), 5);
}

TEST(FlatStringTest, STDStoi) {
  flat_string<16> str("123");
  int value = std::stoi(str);
  EXPECT_EQ(value, 123);
}

TEST(FlatStringTest, STDStol) {
  flat_string<32> str("1234567890123456789");
  long value = std::stol(str);
  EXPECT_EQ(value, 1234567890123456789L);
}

TEST(FlatStringTest, STDStoll) {
  flat_string<32> str("1234567890123456789");
  long long value = std::stoll(str);
  EXPECT_EQ(value, 1234567890123456789LL);
}

TEST(FlatStringTest, STDStoul) {
  flat_string<32> str("12345678901234567890");
  unsigned long value = std::stoul(str);
  EXPECT_EQ(value, 12345678901234567890UL);
}

TEST(FlatStringTest, STDStoull) {
  flat_string<32> str("12345678901234567890");
  unsigned long long value = std::stoull(str);
  EXPECT_EQ(value, 12345678901234567890ULL);
}

TEST(FlatStringTest, STDStof) {
  flat_string<16> str("123.45");
  float value = std::stof(str);
  EXPECT_EQ(value, 123.45f);
}

TEST(FlatStringTest, STDStod) {
  flat_string<32> str("123.4567890123456789");
  double value = std::stod(str);
  EXPECT_EQ(value, 123.4567890123456789);
}

TEST(FlatStringTest, STDStold) {
  flat_string<32> str("123.456789012345678901234567890");
  long double value = std::stold(str);
  EXPECT_EQ(value, 123.456789012345678901234567890L);
}

class FlatStringNullTermTest : public ::testing::Test {
protected:
  void check_null_terminated(const flat_string<16>& s) {
    EXPECT_EQ(s.data()[s.size()], '\0')
        << "null terminator missing after size=" << s.size();
    EXPECT_EQ(s.c_str()[s.size()], '\0')
        << "c_str null terminator missing after size=" << s.size();
  }
};

TEST_F(FlatStringNullTermTest, DefaultConstructor) {
  flat_string<16> s;
  check_null_terminated(s);
  EXPECT_EQ(s[0], '\0');
}

TEST_F(FlatStringNullTermTest, ConstructFromCString) {
  flat_string<16> s("abc");
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, ConstructFromNChar) {
  flat_string<16> s(5, 'x');
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, ConstructFromInitializerList) {
  flat_string<16> s{'a', 'b', 'c'};
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, CopyConstructor) {
  flat_string<16> s1("hello");
  flat_string<16> s2(s1);
  check_null_terminated(s2);
}

TEST_F(FlatStringNullTermTest, MoveConstructor) {
  flat_string<16> s1("hello");
  flat_string<16> s2(std::move(s1));
  check_null_terminated(s2);
}

TEST_F(FlatStringNullTermTest, CopyConstructorWithPos) {
  flat_string<16> s1("hello world");
  flat_string<16> s2(s1, 6);
  check_null_terminated(s2);
}

TEST_F(FlatStringNullTermTest, CopyConstructorWithPosAndCount) {
  flat_string<16> s1("hello world");
  flat_string<16> s2(s1, 6, 3);
  check_null_terminated(s2);
}

TEST_F(FlatStringNullTermTest, AssignCString) {
  flat_string<16> s("abc");
  s = "hello";
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, AssignChar) {
  flat_string<16> s("abc");
  s = 'z';
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, AssignNChar) {
  flat_string<16> s;
  s.assign(5, 'a');
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, PushBack) {
  flat_string<16> s("abc");
  s.push_back('d');
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, PopBack) {
  flat_string<16> s("abc");
  s.pop_back();
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, InsertAtEnd) {
  flat_string<16> s("abc");
  s.insert(3, "def");
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, InsertAtBeginning) {
  flat_string<16> s("world");
  s.insert(0, "hello ");
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, InsertCharIterator) {
  flat_string<16> s("abc");
  s.insert(s.begin() + 1, 'X');
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, EraseMiddle) {
  flat_string<16> s("hello world");
  s.erase(5, 6);
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, EraseIterator) {
  flat_string<16> s("abc");
  s.erase(s.begin() + 1);
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, EraseRange) {
  flat_string<16> s("abcde");
  s.erase(s.begin() + 1, s.begin() + 4);
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, Clear) {
  flat_string<16> s("hello");
  s.clear();
  check_null_terminated(s);
  EXPECT_EQ(s[0], '\0');
}

TEST_F(FlatStringNullTermTest, ResizeGrow) {
  flat_string<16> s("abc");
  s.resize(6, 'x');
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, ResizeShrink) {
  flat_string<16> s("abcdef");
  s.resize(3);
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, ResizeAndOverwrite) {
  flat_string<16> s("hello");
  s.resize_and_overwrite(3, [](char*, size_t) { return 3; });
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, Swap) {
  flat_string<16> s1("hello");
  flat_string<16> s2("world");
  s1.swap(s2);
  check_null_terminated(s1);
  check_null_terminated(s2);
}

TEST_F(FlatStringNullTermTest, SwapEmptyAndNonEmpty) {
  flat_string<16> s1("hello");
  flat_string<16> s2;
  s1.swap(s2);
  check_null_terminated(s1);
  check_null_terminated(s2);
}

TEST_F(FlatStringNullTermTest, AppendOperations) {
  flat_string<16> s("abc");
  s.append("def");
  check_null_terminated(s);
  s.append(2, 'g');
  check_null_terminated(s);
  s += 'h';
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, ReplaceOperations) {
  flat_string<32> s("hello world");
  s.replace(6, 5, "there");
  check_null_terminated(s);
}

TEST_F(FlatStringNullTermTest, SelfAssignment) {
  flat_string<16> s("hello");
  s = s;
  check_null_terminated(s);
  EXPECT_EQ(s, "hello");
}

TEST_F(FlatStringNullTermTest, OperatorSubscriptAtSize) {
  flat_string<16> s("abc");
  EXPECT_EQ(s[s.size()], '\0');
}

class FlatStringFindTest : public ::testing::Test {};

TEST_F(FlatStringFindTest, FindEmptyString) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.find(""), 0);
  EXPECT_EQ(s.find("", 3), 3);
  EXPECT_EQ(s.find("", 10), 10);
}

TEST_F(FlatStringFindTest, FindNotFound) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.find("xyz"), flat_string<16>::npos);
  EXPECT_EQ(s.find('z'), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindEmptySubstring) {
  flat_string<16> s("abc");
  EXPECT_EQ(s.find(flat_string<16>("")), 0);
}

TEST_F(FlatStringFindTest, RFindEmptyString) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.rfind(""), 5);
  EXPECT_EQ(s.rfind("", 3), 3);
  EXPECT_EQ(s.rfind("", 10), 5);
}

TEST_F(FlatStringFindTest, RFindNotFound) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.rfind("xyz"), flat_string<16>::npos);
  EXPECT_EQ(s.rfind('z'), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, RFindCharAtEnd) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.rfind('o'), 4);
  EXPECT_EQ(s.rfind('o', 4), 4);
  EXPECT_EQ(s.rfind('o', 3), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindFirstOfEmptySet) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.find_first_of(""), flat_string<16>::npos);
  EXPECT_EQ(s.find_first_of("", 2), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindFirstOfNotFound) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.find_first_of("xyz"), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindFirstNotOfAllChars) {
  flat_string<16> s("aaa");
  EXPECT_EQ(s.find_first_not_of('a'), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindFirstNotOfEmptySet) {
  flat_string<16> s("abc");
  EXPECT_EQ(s.find_first_not_of(""), 0);
}

TEST_F(FlatStringFindTest, FindLastOfEmptySet) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.find_last_of(""), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindLastOfNotFound) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.find_last_of("xyz"), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindLastNotOfAllChars) {
  flat_string<16> s("aaa");
  EXPECT_EQ(s.find_last_not_of('a'), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindLastNotOfCharBasic) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.find_last_not_of('d'), 9);
  EXPECT_EQ(s.find_last_not_of('h'), 10);
  EXPECT_EQ(s.find_last_not_of('x'), 10);
}

TEST_F(FlatStringFindTest, FindLastNotOfCharNpos) {
  flat_string<16> s("aaa");
  EXPECT_EQ(s.find_last_not_of('a'), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindLastNotOfStringBasic) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.find_last_not_of("hd"), 9);
  EXPECT_EQ(s.find_last_not_of("helo wrd"), flat_string<16>::npos);
}

TEST_F(FlatStringFindTest, FindOnEmptyString) {
  flat_string<16> s;
  EXPECT_EQ(s.find('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find("a"), flat_string<16>::npos);
  EXPECT_EQ(s.rfind('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_first_of('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_first_not_of('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_last_of('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_last_not_of('a'), flat_string<16>::npos);
}

class FlatStringCompareTest : public ::testing::Test {};

TEST_F(FlatStringCompareTest, EmptyStringCompare) {
  flat_string<16> s1;
  flat_string<16> s2;
  EXPECT_EQ(s1.compare(s2), 0);
  EXPECT_EQ(s1.compare(""), 0);
}

TEST_F(FlatStringCompareTest, ComparePosAtSize) {
  flat_string<16> s("abc");
  EXPECT_EQ(s.compare(3, 0, ""), 0);
}

TEST_F(FlatStringCompareTest, ComparePosOutOfRange) {
  flat_string<16> s("abc");
  EXPECT_THROW(s.compare(4, 0, ""), std::out_of_range);
}

TEST_F(FlatStringCompareTest, CompareSelf) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.compare(s), 0);
}

TEST_F(FlatStringCompareTest, ComparePrefix) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.compare(0, 5, "hello"), 0);
  EXPECT_LT(s.compare(0, 5, "hello!"), 0);
}

TEST_F(FlatStringCompareTest, CompareSuffix) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.compare(6, 5, "world"), 0);
}

TEST_F(FlatStringCompareTest, CompareDifferentLengths) {
  flat_string<16> s1("abc");
  flat_string<16> s2("abcd");
  EXPECT_LT(s1.compare(s2), 0);
  EXPECT_GT(s2.compare(s1), 0);
}

TEST_F(FlatStringCompareTest, CompareStringViewLikePosCount) {
  flat_string<16> s("hello world");
  std::string_view sv("world");
  EXPECT_EQ(s.compare(6, 5, sv), 0);
  EXPECT_EQ(s.compare(6, 5, sv, 0, 5), 0);
}

class FlatStringEdgeTest : public ::testing::Test {};

TEST_F(FlatStringEdgeTest, FillToCapacity) {
  flat_string<8> s;
  EXPECT_EQ(s.capacity(), 7);
  s.assign(7, 'x');
  EXPECT_EQ(s.size(), 7);
  EXPECT_EQ(s, "xxxxxxx");
  EXPECT_EQ(s.data()[7], '\0');
}

TEST_F(FlatStringEdgeTest, EmptyStringOperations) {
  flat_string<16> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
  EXPECT_STREQ(s.c_str(), "");
  EXPECT_STREQ(s.data(), "");
  EXPECT_EQ(s[0], '\0');
}

TEST_F(FlatStringEdgeTest, SingleCharString) {
  flat_string<16> s(1, 'a');
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s, "a");
  EXPECT_EQ(s.front(), 'a');
  EXPECT_EQ(s.back(), 'a');
  EXPECT_EQ(s.data()[1], '\0');
}

TEST_F(FlatStringEdgeTest, InsertAtBeginning) {
  flat_string<16> s("world");
  s.insert(0, "hello ");
  EXPECT_EQ(s, "hello world");
}

TEST_F(FlatStringEdgeTest, InsertInMiddle) {
  flat_string<16> s("helo");
  s.insert(2, "l");
  EXPECT_EQ(s, "hello");
}

TEST_F(FlatStringEdgeTest, EraseAll) {
  flat_string<16> s("hello");
  s.erase(0, flat_string<16>::npos);
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.data()[0], '\0');
}

TEST_F(FlatStringEdgeTest, EraseSingleChar) {
  flat_string<16> s("abc");
  s.erase(1, 1);
  EXPECT_EQ(s, "ac");
}

TEST_F(FlatStringEdgeTest, ResizeToZero) {
  flat_string<16> s("hello");
  s.resize(0);
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.data()[0], '\0');
}

TEST_F(FlatStringEdgeTest, ResizeGrowAndShrink) {
  flat_string<16> s("abc");
  s.resize(6, 'x');
  EXPECT_EQ(s, "abcxxx");
  s.resize(2);
  EXPECT_EQ(s, "ab");
  EXPECT_EQ(s.data()[2], '\0');
}

TEST_F(FlatStringEdgeTest, SelfAssignment) {
  flat_string<16> s("hello");
  s = s;
  EXPECT_EQ(s, "hello");
  EXPECT_EQ(s.data()[5], '\0');
}

TEST_F(FlatStringEdgeTest, SelfSwap) {
  flat_string<16> s("hello");
  s.swap(s);
  EXPECT_EQ(s, "hello");
  EXPECT_EQ(s.data()[5], '\0');
}

TEST_F(FlatStringEdgeTest, CopyWithPos) {
  flat_string<16> s("hello world");
  char buf[6];
  auto n = s.copy(buf, 5, 6);
  EXPECT_EQ(n, 5);
  buf[5] = '\0';
  EXPECT_STREQ(buf, "world");
}

TEST_F(FlatStringEdgeTest, CopyOutOfRange) {
  flat_string<16> s("hello");
  char buf[1];
  EXPECT_THROW(s.copy(buf, 1, 10), std::out_of_range);
}

TEST_F(FlatStringEdgeTest, SubstrFullString) {
  flat_string<16> s("hello");
  auto sub = s.substr(0);
  EXPECT_EQ(sub, "hello");
}

TEST_F(FlatStringEdgeTest, SubstrNpos) {
  flat_string<16> s("hello world");
  auto sub = s.substr(6);
  EXPECT_EQ(sub, "world");
}

TEST_F(FlatStringEdgeTest, StartsWithEmpty) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.starts_with(""));
}

TEST_F(FlatStringEdgeTest, StartsWithFullString) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.starts_with("hello"));
}

TEST_F(FlatStringEdgeTest, StartsWithTooLong) {
  flat_string<16> s("hello");
  EXPECT_FALSE(s.starts_with("hello world"));
}

TEST_F(FlatStringEdgeTest, StartsWithEmptyString) {
  flat_string<16> s;
  EXPECT_TRUE(s.starts_with(""));
  EXPECT_FALSE(s.starts_with("a"));
}

TEST_F(FlatStringEdgeTest, EndsWithEmpty) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.ends_with(""));
}

TEST_F(FlatStringEdgeTest, EndsWithFullString) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.ends_with("hello"));
}

TEST_F(FlatStringEdgeTest, EndsWithTooLong) {
  flat_string<16> s("hello");
  EXPECT_FALSE(s.ends_with("hello world"));
}

TEST_F(FlatStringEdgeTest, ContainsEmpty) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.contains(""));
}

TEST_F(FlatStringEdgeTest, ContainsOnEmpty) {
  flat_string<16> s;
  EXPECT_TRUE(s.contains(""));
  EXPECT_FALSE(s.contains("a"));
  EXPECT_FALSE(s.contains('a'));
}

TEST_F(FlatStringEdgeTest, AtConstAndNonConst) {
  flat_string<16> s("hello");
  s.at(0) = 'H';
  EXPECT_EQ(s, "Hello");
  const flat_string<16>& cs = s;
  EXPECT_EQ(cs.at(0), 'H');
}

TEST_F(FlatStringEdgeTest, OperatorSubscriptConst) {
  const flat_string<16> s("hello");
  EXPECT_EQ(s[0], 'h');
  EXPECT_EQ(s[4], 'o');
  EXPECT_EQ(s[5], '\0');
}

TEST_F(FlatStringEdgeTest, IteratorDistance) {
  flat_string<16> s("hello");
  EXPECT_EQ(std::distance(s.begin(), s.end()), 5);
  EXPECT_EQ(std::distance(s.cbegin(), s.cend()), 5);
  EXPECT_EQ(std::distance(s.rbegin(), s.rend()), 5);
}

TEST_F(FlatStringEdgeTest, RangeForLoop) {
  flat_string<16> s("abc");
  std::string result;
  for (auto c : s) {
    result += c;
  }
  EXPECT_EQ(result, "abc");
}

TEST_F(FlatStringEdgeTest, StringViewConversion) {
  flat_string<16> s("hello");
  std::string_view sv = s;
  EXPECT_EQ(sv.size(), 5);
  EXPECT_EQ(sv, "hello");
}

TEST_F(FlatStringEdgeTest, HashConsistency) {
  flat_string<16> s1("hello");
  flat_string<16> s2("hello");
  EXPECT_EQ(std::hash<flat_string<16>>()(s1), std::hash<flat_string<16>>()(s2));
}

TEST_F(FlatStringEdgeTest, MultipleAppendChain) {
  flat_string<32> s;
  s += "hello";
  s += ' ';
  s += "world";
  s.append(3, '!');
  EXPECT_EQ(s, "hello world!!!");
  EXPECT_EQ(s.data()[s.size()], '\0');
}

TEST_F(FlatStringEdgeTest, ReplaceShrink) {
  flat_string<32> s("hello world");
  s.replace(5, 6, "!");
  EXPECT_EQ(s, "hello!");
  EXPECT_EQ(s.data()[6], '\0');
}

TEST_F(FlatStringEdgeTest, ReplaceGrow) {
  flat_string<32> s("hi");
  s.replace(0, 2, "hello");
  EXPECT_EQ(s, "hello");
  EXPECT_EQ(s.data()[5], '\0');
}

TEST_F(FlatStringEdgeTest, ReplaceSameSize) {
  flat_string<32> s("hello");
  s.replace(0, 5, "world");
  EXPECT_EQ(s, "world");
  EXPECT_EQ(s.data()[5], '\0');
}

class FlatStringNullTerminationTest : public ::testing::Test {
protected:
  void expect_null_terminated(const flat_string<16>& s) {
    EXPECT_EQ(s.data()[s.size()], '\0');
    EXPECT_EQ(s.c_str()[s.size()], '\0');
  }
};

TEST_F(FlatStringNullTerminationTest, DefaultConstruction) {
  flat_string<16> s;
  EXPECT_STREQ(s.c_str(), "");
  EXPECT_EQ(s.data()[s.size()], '\0');
}

TEST_F(FlatStringNullTerminationTest, AfterPushBack) {
  flat_string<16> s;
  s.push_back('x');
  expect_null_terminated(s);
  EXPECT_EQ(s.data()[1], '\0');
}

TEST_F(FlatStringNullTerminationTest, AfterAppend) {
  flat_string<16> s;
  s.append("hello");
  expect_null_terminated(s);
  EXPECT_EQ(s.data()[5], '\0');
}

TEST_F(FlatStringNullTerminationTest, AfterPopBack) {
  flat_string<16> s("hello");
  s.pop_back();
  expect_null_terminated(s);
  EXPECT_EQ(s.size(), 4);
}

TEST_F(FlatStringNullTerminationTest, AfterErase) {
  flat_string<16> s("hello world");
  s.erase(5, 6);
  expect_null_terminated(s);
  EXPECT_EQ(s, "hello");
}

TEST_F(FlatStringNullTerminationTest, AfterResizeGrow) {
  flat_string<16> s("hi");
  s.resize(5, 'x');
  expect_null_terminated(s);
  EXPECT_EQ(s.data()[5], '\0');
}

TEST_F(FlatStringNullTerminationTest, AfterResizeShrink) {
  flat_string<16> s("hello");
  s.resize(2);
  expect_null_terminated(s);
  EXPECT_EQ(s.data()[2], '\0');
}

TEST_F(FlatStringNullTerminationTest, OperatorSubscriptAtSize) {
  flat_string<16> s("hello");
  EXPECT_EQ(s[s.size()], '\0');
}

class FlatStringFindLastNotOfTest : public ::testing::Test {};

TEST_F(FlatStringFindLastNotOfTest, FindsLastCharNotEqualToValue) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.find_last_not_of('d'), 9);
}

TEST_F(FlatStringFindLastNotOfTest, AllCharsMatchReturnsNpos) {
  flat_string<16> s("aaa");
  EXPECT_EQ(s.find_last_not_of('a'), flat_string<16>::npos);
}

TEST_F(FlatStringFindLastNotOfTest, FindsDifferentCharAtEnd) {
  flat_string<16> s("aab");
  EXPECT_EQ(s.find_last_not_of('a'), 2);
}

TEST_F(FlatStringFindLastNotOfTest, SkipsTrailingMatchingChars) {
  flat_string<16> s("  hello  ");
  EXPECT_EQ(s.find_last_not_of(' '), 6);
}

class FlatStringStartsEndsContainsTest : public ::testing::Test {};

TEST_F(FlatStringStartsEndsContainsTest, StartsWithCString) {
  flat_string<16> s("hello world");
  EXPECT_TRUE(s.starts_with("hello"));
  EXPECT_FALSE(s.starts_with("world"));
}

TEST_F(FlatStringStartsEndsContainsTest, StartsWithChar) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.starts_with('h'));
  EXPECT_FALSE(s.starts_with('w'));
}

TEST_F(FlatStringStartsEndsContainsTest, StartsWithStringView) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.starts_with(std::string_view("hel")));
  EXPECT_FALSE(s.starts_with(std::string_view("wor")));
}

TEST_F(FlatStringStartsEndsContainsTest, EndsWithCString) {
  flat_string<16> s("hello world");
  EXPECT_TRUE(s.ends_with("world"));
  EXPECT_FALSE(s.ends_with("hello"));
}

TEST_F(FlatStringStartsEndsContainsTest, EndsWithChar) {
  flat_string<16> s("world");
  EXPECT_TRUE(s.ends_with('d'));
  EXPECT_FALSE(s.ends_with('w'));
}

TEST_F(FlatStringStartsEndsContainsTest, ContainsCString) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.contains("llo"));
  EXPECT_FALSE(s.contains("xyz"));
}

TEST_F(FlatStringStartsEndsContainsTest, ContainsChar) {
  flat_string<16> s("hello");
  EXPECT_TRUE(s.contains('e'));
  EXPECT_FALSE(s.contains('x'));
}

TEST_F(FlatStringStartsEndsContainsTest, StartsWithNegativeOnShorter) {
  flat_string<16> s("hi");
  EXPECT_FALSE(s.starts_with("hello"));
}

class FlatStringCompareBasicTest : public ::testing::Test {};

TEST_F(FlatStringCompareBasicTest, EqualStringsReturnZero) {
  flat_string<16> s1("hello");
  flat_string<16> s2("hello");
  EXPECT_EQ(s1.compare(s2), 0);
}

TEST_F(FlatStringCompareBasicTest, LessReturnsNegative) {
  flat_string<16> s1("apple");
  flat_string<16> s2("banana");
  EXPECT_LT(s1.compare(s2), 0);
}

TEST_F(FlatStringCompareBasicTest, GreaterReturnsPositive) {
  flat_string<16> s1("banana");
  flat_string<16> s2("apple");
  EXPECT_GT(s1.compare(s2), 0);
}

class FlatStringSubstrTest : public ::testing::Test {};

TEST_F(FlatStringSubstrTest, SubstrFromZeroReturnsFull) {
  flat_string<16> s("hello");
  EXPECT_EQ(s.substr(0), "hello");
}

TEST_F(FlatStringSubstrTest, SubstrFromMiddlePosition) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.substr(3), "lo world");
}

TEST_F(FlatStringSubstrTest, SubstrWithCount) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.substr(0, 3), "hel");
}

TEST_F(FlatStringSubstrTest, SubstrWithNposCount) {
  flat_string<16> s("hello world");
  EXPECT_EQ(s.substr(6, flat_string<16>::npos), "world");
}

class FlatStringResizeAndOverwriteTest : public ::testing::Test {};

TEST_F(FlatStringResizeAndOverwriteTest, WriteContentAndReturnActualSize) {
  flat_string<16> s("hello");
  s.resize_and_overwrite(8, [](char* buf, size_t) {
    const char* src = "abcdefgh";
    for (size_t i = 0; i < 8; ++i) {
      buf[i] = src[i];
    }
    return 8;
  });
  EXPECT_EQ(s.size(), 8);
  EXPECT_EQ(s, "abcdefgh");
}

TEST_F(FlatStringResizeAndOverwriteTest, ReturnSizeLessThanCount) {
  flat_string<16> s("hello");
  s.resize_and_overwrite(10, [](char* buf, size_t) {
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    return 3;
  });
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s, "abc");
  EXPECT_EQ(s.data()[3], '\0');
}

class FlatStringAssignOverloadTest : public ::testing::Test {};

TEST_F(FlatStringAssignOverloadTest, AssignCountChar) {
  flat_string<16> s("hello");
  s.assign(3, 'z');
  EXPECT_EQ(s, "zzz");
}

TEST_F(FlatStringAssignOverloadTest, AssignCStringWithSize) {
  flat_string<16> s("hello");
  s.assign("world", 3);
  EXPECT_EQ(s, "wor");
}

TEST_F(FlatStringAssignOverloadTest, AssignCString) {
  flat_string<16> s("hello");
  s.assign("world");
  EXPECT_EQ(s, "world");
}

TEST_F(FlatStringAssignOverloadTest, AssignStringViewLike) {
  flat_string<16> s("hello");
  s.assign(std::string_view("world"));
  EXPECT_EQ(s, "world");
}

TEST_F(FlatStringAssignOverloadTest, AssignFlatString) {
  flat_string<16> s1("hello");
  flat_string<16> s2("world");
  s1.assign(s2);
  EXPECT_EQ(s1, "world");
}

class FlatStringEdgeCaseTest : public ::testing::Test {};

TEST_F(FlatStringEdgeCaseTest, EmptyStringFindOperations) {
  flat_string<16> s;
  EXPECT_EQ(s.find('a'), flat_string<16>::npos);
  EXPECT_EQ(s.rfind('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_first_of('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_first_not_of('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_last_of('a'), flat_string<16>::npos);
  EXPECT_EQ(s.find_last_not_of('a'), flat_string<16>::npos);
}

TEST_F(FlatStringEdgeCaseTest, EmptyStringAppendAndErase) {
  flat_string<16> s;
  s.append("a");
  EXPECT_EQ(s, "a");
  s.erase(0, 1);
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.data()[0], '\0');
}

TEST_F(FlatStringEdgeCaseTest, FullCapacityString) {
  flat_string<8> s;
  s.assign(7, 'x');
  EXPECT_EQ(s.size(), s.capacity());
  EXPECT_EQ(s, "xxxxxxx");
  EXPECT_EQ(s.data()[7], '\0');
}

TEST_F(FlatStringEdgeCaseTest, MinimalCapacityString) {
  flat_string<1> s;
  EXPECT_EQ(s.capacity(), 0);
  EXPECT_TRUE(s.empty());
  EXPECT_STREQ(s.c_str(), "");
  EXPECT_EQ(s.data()[0], '\0');
  EXPECT_EQ(s[s.size()], '\0');
}

TEST_F(FlatStringEdgeCaseTest, FullCapacityResizeThrows) {
  flat_string<4> s;
  s.assign(3, 'x');
  EXPECT_EQ(s.size(), s.capacity());
  EXPECT_THROW(s.resize(4), std::out_of_range);
}
