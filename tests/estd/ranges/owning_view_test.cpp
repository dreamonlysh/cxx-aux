#include <estd/__ranges/owning_view.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(OwningView, BasicOwnership) {
  auto ov = es::ranges::owning_view(std::vector<int>{1, 2, 3, 4, 5});

  std::vector<int> result;
  for (int x : ov) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 5u);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[4], 5);
}

TEST(OwningView, MoveOwnership) {
  std::vector<int> source = {1, 2, 3};
  auto ov = es::ranges::owning_view(std::move(source));

  std::vector<int> result;
  for (int x : ov) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 3u);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[2], 3);
}

TEST(OwningView, StringOwnership) {
  auto ov = es::ranges::owning_view(std::string("hello"));

  std::string result;
  for (char c : ov) {
    result += c;
  }

  EXPECT_EQ(result, "hello");
}

TEST(OwningView, ConstAccess) {
  auto ov = es::ranges::owning_view(std::vector<int>{10, 20, 30});

  auto it = ov.begin();
  EXPECT_EQ(*it, 10);
}
