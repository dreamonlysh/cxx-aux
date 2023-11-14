#include <functional>
#include <gtest/gtest.h>
#include <iostream>

template <typename... Args>
int sum(Args&&... args) {
  return (0 + ... + args);
}

template <typename... Args>
int assign(Args&&... args) {
  int result = -1;
  (result = ... = args);
  return result;
}

template <typename... Args>
void print(Args&&... args) {
  (std::cout << ... << args);
}

template <typename T, typename U>
T add(T&& a, U&& b) {
  return a + b;
}

TEST(FoldExpression, All) {
  int step = 1;
  auto increase = [&step](auto&& a) {
    return add(std::forward<decltype(a)>(a), step);
  };
  ASSERT_EQ(increase(1), 2);
  step = 2;
  ASSERT_EQ(increase(1), 3);

  ASSERT_EQ(assign(1, 2, 3, 4), 4);
}
