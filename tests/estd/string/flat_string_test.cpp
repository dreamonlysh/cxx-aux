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
  EXPECT_EQ(pos, 7);
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
