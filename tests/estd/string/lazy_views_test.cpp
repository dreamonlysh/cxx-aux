#include <estd/string/__algorithm/lazy_views.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(LazyViews, ToUpperClosure) {
  std::string input = "hello";
  auto result = es::string::views::to_upper(input);

  std::string output;
  for (char c : result) {
    output += c;
  }
  EXPECT_EQ(output, "HELLO");
}

TEST(LazyViews, ToUpperCString) {
  const char* input = "hello world";
  auto result = es::string::views::to_upper(input);

  std::string output;
  for (char c : result) {
    output += c;
  }
  EXPECT_EQ(output, "HELLO WORLD");
}

TEST(LazyViews, ToLowerClosure) {
  std::string input = "HELLO";
  auto result = es::string::views::to_lower(input);

  std::string output;
  for (char c : result) {
    output += c;
  }
  EXPECT_EQ(output, "hello");
}

TEST(LazyViews, ToLowerCString) {
  const char* input = "HELLO WORLD";
  auto result = es::string::views::to_lower(input);

  std::string output;
  for (char c : result) {
    output += c;
  }
  EXPECT_EQ(output, "hello world");
}

TEST(LazyViews, ChainedViews) {
  std::string input = "hello";
  auto upper_result = es::string::views::to_upper(input);

  std::string upper_output;
  for (char c : upper_result) {
    upper_output += c;
  }
  EXPECT_EQ(upper_output, "HELLO");
}
