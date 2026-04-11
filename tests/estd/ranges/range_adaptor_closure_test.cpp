#include <gtest/gtest.h>

#include <estd/__ranges/range_adaptor_closure.h>

using namespace es::ranges;

// Test a simple adaptor closure for testing
struct TestAdaptor : range_adaptor_closure<TestAdaptor> {
  int value;
  TestAdaptor(int v) : value(v) {}

  template <typename Range>
  auto operator()(Range&& r) const {
    // Simple transformation: add value to each element
    std::vector<typename std::decay_t<Range>::value_type> result;
    for (auto&& elem : r) {
      result.push_back(elem + value);
    }
    return result;
  }
};

TEST(range_adaptor_closure, basic_operation) {
  TestAdaptor add5(5);
  std::vector<int> input = {1, 2, 3, 4, 5};
  auto result = add5(input);
  EXPECT_EQ(result, std::vector<int>({6, 7, 8, 9, 10}));
}

TEST(range_adaptor_closure, pipe_operator) {
  TestAdaptor add5(5);
  std::vector<int> input = {1, 2, 3, 4, 5};
  auto result = input | add5;
  EXPECT_EQ(result, std::vector<int>({6, 7, 8, 9, 10}));
}

TEST(range_adaptor_closure, composition) {
  TestAdaptor add5(5);
  TestAdaptor add3(3);
  auto composed = add5 | add3;
  std::vector<int> input = {1, 2, 3, 4, 5};
  auto result = input | composed;
  EXPECT_EQ(result, std::vector<int>({9, 10, 11, 12, 13}));
}
