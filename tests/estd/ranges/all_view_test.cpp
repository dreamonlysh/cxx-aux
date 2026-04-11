#include <estd/__ranges/all_view.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(AllView, AllAdaptor) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto av = es::ranges::all(vec);

  std::vector<int> result;
  for (int x : av) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 5u);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[4], 5);
}

TEST(AllView, AllWithString) {
  std::string str = "hello";

  auto av = es::ranges::all(str);

  std::string result;
  for (char c : av) {
    result += c;
  }

  EXPECT_EQ(result, "hello");
}

TEST(AllView, AllAdaptorChained) {
  std::vector<int> vec = {1, 2, 3, 4, 5};

  auto av = es::ranges::all(vec);
  auto tv = es::ranges::transform([](int x) { return x * 2; })(av);

  std::vector<int> result;
  for (int x : tv) {
    result.push_back(x);
  }

  EXPECT_EQ(result.size(), 5u);
  EXPECT_EQ(result[0], 2);
  EXPECT_EQ(result[4], 10);
}
