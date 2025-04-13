#include "estd/string/__algorithm/append.h"
#include <gtest/gtest.h>
#include <string>
#include <string_view>

using namespace es::string;

TEST(AppendTest, AppendSingleCharacter) {
  std::string s = "Hello";
  append(s, '!');
  EXPECT_EQ(s, "Hello!");
}

TEST(AppendTest, AppendMultipleCharacters) {
  std::string s = "Hello";
  append(s, 3, '!');
  EXPECT_EQ(s, "Hello!!!");
}

TEST(AppendTest, AppendCString) {
  std::string s = "Hello";
  append(s, " World");
  EXPECT_EQ(s, "Hello World");
}

TEST(AppendTest, AppendCStringWithLength) {
  std::string s = "Hello";
  append(s, " World", 5);
  EXPECT_EQ(s, "Hello Worl");
}

TEST(AppendTest, AppendStringView) {
  std::string s = "Hello";
  std::string_view sv = " World";
  append(s, sv);
  EXPECT_EQ(s, "Hello World");
}

TEST(AppendTest, AppendStringViewWithPositionAndLength) {
  std::string s = "Hello";
  std::string_view sv = "Amazing World";
  append(s, sv, 7, 6);
  EXPECT_EQ(s, "Hello World");
}

TEST(AppendTest, AppendIntegral) {
  std::string s = "Value: ";
  append(s, 42);
  EXPECT_EQ(s, "Value: 42");
}

TEST(AppendTest, AppendInitializerList) {
  std::string s = "Hello";
  append(s, {' ', 'W', 'o', 'r', 'l', 'd'});
  EXPECT_EQ(s, "Hello World");
}

TEST(AppendTest, AppendWithIterators) {
  std::string s = "Hello";
  std::string to_append = " World";
  append(s, to_append.begin(), to_append.end());
  EXPECT_EQ(s, "Hello World");
}

TEST(AppendNTest, AppendMultipleArguments) {
  std::string s = "Hello";
  append_n(s, " ", "World", '!', 123);
  EXPECT_EQ(s, "Hello World!123");
}
