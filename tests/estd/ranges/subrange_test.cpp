#include <estd/__ranges/subrange.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(Subrange, BasicConstruction) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto sr = es::ranges::subrange(vec.begin(), vec.begin() + 3);

  EXPECT_EQ(sr.begin() - vec.begin(), 0);
  EXPECT_EQ(sr.end() - vec.begin(), 3);
}

TEST(Subrange, EmptyRange) {
  std::vector<int> vec = {1, 2, 3};

  auto sr = es::ranges::subrange(vec.begin(), vec.begin());

  EXPECT_TRUE(sr.begin() == sr.end());
}

TEST(Subrange, FullRange) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto sr = es::ranges::subrange(vec.begin(), vec.end());

  EXPECT_EQ(sr.size(), 5u);
}

TEST(Subrange, ForwardIteration) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto sr = es::ranges::subrange(vec.begin() + 1, vec.begin() + 4);

  std::vector<int> result;
  for (auto it = sr.begin(); it != sr.end(); ++it) {
    result.push_back(*it);
  }

  EXPECT_EQ(result.size(), 3u);
  EXPECT_EQ(result[0], 2);
  EXPECT_EQ(result[1], 3);
  EXPECT_EQ(result[2], 4);
}

TEST(Subrange, RangeBasedForLoop) {
  std::string str = "hello";

  auto sr = es::ranges::subrange(str.begin(), str.begin() + 3);

  std::string result;
  for (char c : sr) {
    result += c;
  }

  EXPECT_EQ(result, "hel");
}

TEST(Subrange, DataAccess) {
  std::vector<int> vec = {10, 20, 30, 40, 50};

  auto sr = es::ranges::subrange(vec.begin() + 1, vec.begin() + 4);

  EXPECT_EQ(*sr.data(), 20);
}
