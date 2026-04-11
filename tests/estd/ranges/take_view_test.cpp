#include <estd/__ranges/take_view.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(TakeView, BasicTake) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto tv = es::ranges::take(3)(vec);

  std::vector<int> result;
  for (int x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 3u);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[1], 2);
  EXPECT_EQ(result[2], 3);
}

TEST(TakeView, TakeMoreThanAvailable) {
  std::vector<int> vec = {1, 2, 3};

  auto tv = es::ranges::take(10)(vec);

  std::vector<int> result;
  for (int x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 3u);
}

TEST(TakeView, TakeZero) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto tv = es::ranges::take(0)(vec);

  std::vector<int> result;
  for (int x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 0u);
}

TEST(TakeView, TakeAll) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto tv = es::ranges::take(5)(vec);

  std::vector<int> result;
  for (int x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 5u);
}

TEST(TakeView, StringTake) {
  std::string str = "hello world";

  auto tv = es::ranges::take(5)(str);

  std::string result;
  for (char c : tv) {
    result += c;
  }

  EXPECT_EQ(result, "hello");
}
