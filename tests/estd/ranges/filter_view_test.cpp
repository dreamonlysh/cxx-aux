#include <estd/__ranges/filter_view.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(FilterView, BasicFilter) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  auto fv = es::ranges::filter([](int x) { return x % 2 == 0; })(vec);

  std::vector<int> result;
  for (int x : fv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 5u);
  EXPECT_EQ(result[0], 2);
  EXPECT_EQ(result[1], 4);
  EXPECT_EQ(result[2], 6);
  EXPECT_EQ(result[3], 8);
  EXPECT_EQ(result[4], 10);
}

TEST(FilterView, FilterAll) {
  std::vector<int> vec = {2, 4, 6, 8};

  auto fv = es::ranges::filter([](int x) { return x % 2 == 0; })(vec);

  std::vector<int> result;
  for (int x : fv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 4u);
}

TEST(FilterView, FilterNone) {
  std::vector<int> vec = {1, 3, 5, 7};

  auto fv = es::ranges::filter([](int x) { return x % 2 == 0; })(vec);

  std::vector<int> result;
  for (int x : fv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 0u);
}

TEST(FilterView, FilterWithString) {
  std::vector<std::string> words = {"hello", "world", "foo", "bar"};

  auto fv = es::ranges::filter(
      [](const std::string& s) { return s.size() > 3; })(words);

  std::vector<std::string> result;
  for (const auto& s : fv) {
    result.push_back(s);
  }

  EXPECT_EQ(result.size(), 2u);
  EXPECT_EQ(result[0], "hello");
  EXPECT_EQ(result[1], "world");
}
