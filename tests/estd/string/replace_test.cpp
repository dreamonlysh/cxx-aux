#include <estd/string/__algorithm/replace.h>
#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <vector>

TEST(ReplaceTest, ReplaceWithCString) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, "Universe");
  ASSERT_EQ(s, "Hello, Universe!");
}

TEST(ReplaceTest, ReplaceWithCStringAndLength) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, "Universe", 3);
  ASSERT_EQ(s, "Hello, Uni!");
}

TEST(ReplaceTest, ReplaceWithStringView) {
  std::string s = "Hello, World!";
  std::string_view sv = "Universe";
  es::string::replace(s, 7, 5, sv);
  ASSERT_EQ(s, "Hello, Universe!");
}

TEST(ReplaceTest, ReplaceWithCharAndCount) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, 3, '*');
  ASSERT_EQ(s, "Hello, ***!");
}

TEST(ReplaceTest, ReplaceWithInitializerList) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e'});
  ASSERT_EQ(s, "Hello, Universe!");
}

TEST(ReplaceTest, ReplaceOutOfRange) {
  std::string s = "Hello, World!";
  ASSERT_THROW(es::string::replace(s, 20, 5, "Test"), std::out_of_range);
}

TEST(ReplaceTest, ReplaceWithEmptyCString) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, "");
  ASSERT_EQ(s, "Hello, !");
}

TEST(ReplaceTest, ReplaceWithEmptyStringView) {
  std::string s = "Hello, World!";
  std::string_view sv = "";
  es::string::replace(s, 7, 5, sv);
  ASSERT_EQ(s, "Hello, !");
}

TEST(ReplaceTest, ReplaceEntireString) {
  std::string s = "Hello, World!";
  es::string::replace(s, 0, s.size(), "Goodbye!");
  ASSERT_EQ(s, "Goodbye!");
}

TEST(ReplaceTest, ReplaceWithLongerString) {
  std::string s = "Hello";
  es::string::replace(s, 0, 5, "Hello, World!");
  ASSERT_EQ(s, "Hello, World!");
}

TEST(ReplaceTest, ReplaceWithShorterString) {
  std::string s = "Hello, World!";
  es::string::replace(s, 0, 13, "Hi");
  ASSERT_EQ(s, "Hi");
}

TEST(ReplaceTest, ReplaceWithSameLengthString) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, "Earth");
  ASSERT_EQ(s, "Hello, Earth!");
}

TEST(ReplaceTest, ReplaceWithPartialCString) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, "Universe", 4);
  ASSERT_EQ(s, "Hello, Univ!");
}

TEST(ReplaceTest, ReplaceWithPartialInitializerList) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e'});
  ASSERT_EQ(s, "Hello, Universe!");
}

TEST(ReplaceTest, ReplaceWithEmptyInitializerList) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, {});
  ASSERT_EQ(s, "Hello, !");
}

TEST(ReplaceTest, ReplaceWithCharRepeated) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, 10, '*');
  ASSERT_EQ(s, "Hello, **********!");
}

TEST(ReplaceTest, ReplaceWithZeroCount) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 0, "Test");
  ASSERT_EQ(s, "Hello, TestWorld!");
}

TEST(ReplaceTest, ReplaceWithZeroLengthCString) {
  std::string s = "Hello, World!";
  es::string::replace(s, 7, 5, "", 0);
  ASSERT_EQ(s, "Hello, !");
}

TEST(ReplaceTest, ReplaceWithZeroLengthStringView) {
  std::string s = "Hello, World!";
  std::string_view sv = "";
  es::string::replace(s, 7, 5, sv);
  ASSERT_EQ(s, "Hello, !");
}
TEST(ReplaceTest, ReplaceWithInputIterator) {
  std::string s = "Hello, World!";
  std::vector<char> replacement = {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e'};
  es::string::replace(s, 7, 5, replacement.begin(), replacement.end());
  ASSERT_EQ(s, "Hello, Universe!");
}

TEST(ReplaceTest, ReplaceWithInputIteratorEmptyRange) {
  std::string s = "Hello, World!";
  std::vector<char> replacement = {};
  es::string::replace(s, 7, 5, replacement.begin(), replacement.end());
  ASSERT_EQ(s, "Hello, !");
}

TEST(ReplaceTest, ReplaceWithInputIteratorPartialRange) {
  std::string s = "Hello, World!";
  std::vector<char> replacement = {'U', 'n', 'i', 'v', 'e', 'r', 's', 'e'};
  es::string::replace(s, 7, 5, replacement.begin(), replacement.begin() + 4);
  ASSERT_EQ(s, "Hello, Univ!");
}

TEST(ReplaceTest, ReplaceWithInputIteratorLongerRange) {
  std::string s = "Hello";
  std::vector<char> replacement = {'H', 'e', 'l', 'l', 'o', ',', ' ',
                                   'W', 'o', 'r', 'l', 'd', '!'};
  es::string::replace(s, 0, 5, replacement.begin(), replacement.end());
  ASSERT_EQ(s, "Hello, World!");
}

TEST(ReplaceTest, ReplaceWithInputIteratorShorterRange) {
  std::string s = "Hello, World!";
  std::vector<char> replacement = {'H', 'i'};
  es::string::replace(s, 0, 13, replacement.begin(), replacement.end());
  ASSERT_EQ(s, "Hi");
}

TEST(ReplaceTest, ReplaceWithInputIteratorZeroCount) {
  std::string s = "Hello, World!";
  std::vector<char> replacement = {'T', 'e', 's', 't'};
  es::string::replace(s, 7, 0, replacement.begin(), replacement.end());
  ASSERT_EQ(s, "Hello, TestWorld!");
}

TEST(ReplaceTest, ReplaceWithInputIteratorOutOfRange) {
  std::string s = "Hello, World!";
  std::vector<char> replacement = {'T', 'e', 's', 't'};
  ASSERT_THROW(
      es::string::replace(s, 20, 5, replacement.begin(), replacement.end()),
      std::out_of_range);
}
