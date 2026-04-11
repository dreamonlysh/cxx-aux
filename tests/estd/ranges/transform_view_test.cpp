#include <estd/__ranges/transform_view.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(TransformView, BasicTransform) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto tv = es::ranges::transform([](int x) { return x * 2; })(vec);

  std::vector<int> result;
  for (int x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 5u);
  EXPECT_EQ(result[0], 2);
  EXPECT_EQ(result[4], 10);
}

TEST(TransformView, StringTransform) {
  std::vector<std::string> words = {"hello", "world"};

  auto tv = es::ranges::transform(
      [](const std::string& s) { return s.size(); })(words);

  std::vector<size_t> result;
  for (size_t x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 2u);
  EXPECT_EQ(result[0], 5);
  EXPECT_EQ(result[1], 5);
}

TEST(TransformView, ChainedTransforms) {
  std::vector<int> vec = {1, 2, 3};

  auto tv = es::ranges::transform([](int x) { return x * 2; })(
      es::ranges::transform([](int x) { return x + 1; })(vec));

  std::vector<int> result;
  for (int x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result[0], 4);
  EXPECT_EQ(result[1], 6);
  EXPECT_EQ(result[2], 8);
}

TEST(TransformView, EmptyRange) {
  std::vector<int> vec;

  auto tv = es::ranges::transform([](int x) { return x * 2; })(vec);

  int count = 0;
  for (int x : tv) {
    (void)x;
    ++count;
  }

  EXPECT_EQ(count, 0);
}
