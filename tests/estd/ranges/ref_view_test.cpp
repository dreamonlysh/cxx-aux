#include <estd/__ranges/ref_view.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(RefView, BasicUsage) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  es::ranges::ref_view rv(vec);

  std::vector<int> result;
  for (int x : rv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 5u);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[4], 5);
}

TEST(RefView, ModifyOriginal) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  es::ranges::ref_view rv(vec);

  for (int& x : rv) {
    x *= 2;
  }

  EXPECT_EQ(vec[0], 2);
  EXPECT_EQ(vec[4], 10);
}

TEST(RefView, StringView) {
  std::string str = "hello";

  es::ranges::ref_view rv(str);

  std::string result;
  for (char c : rv) {
    result += c;
  }

  EXPECT_EQ(result, "hello");
}

TEST(RefView, ConstAccess) {
  const std::vector<int> vec = {1, 2, 3, 4, 5};

  es::ranges::ref_view rv(vec);

  auto it = rv.begin();
  EXPECT_EQ(*it, 1);
}
