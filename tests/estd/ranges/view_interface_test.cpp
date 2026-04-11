#include <gtest/gtest.h>

#include <estd/__ranges/view_interface.h>
#include <list>
#include <vector>

using namespace es::ranges;

namespace {
// Test view that implements begin() and end()
template <typename T>
struct TestView : view_interface<TestView<T>> {
  std::vector<T> data_;
  TestView(std::initializer_list<T> init) : data_(init) {}
  auto begin() { return data_.begin(); }
  auto end() { return data_.end(); }
  auto begin() const { return data_.begin(); }
  auto end() const { return data_.end(); }
  size_t size() const { return data_.size(); }
};

// Test view without size() method
template <typename T>
struct TestViewNoSize : view_interface<TestViewNoSize<T>> {
  std::vector<T> data_;
  TestViewNoSize(std::initializer_list<T> init) : data_(init) {}
  auto begin() { return data_.begin(); }
  auto end() { return data_.end(); }
  auto begin() const { return data_.begin(); }
  auto end() const { return data_.end(); }
};
} // namespace

TEST(view_interface, empty) {
  TestView<int> empty_view({});
  TestView<int> non_empty_view({1, 2, 3});
  EXPECT_TRUE(empty_view.empty());
  EXPECT_FALSE(non_empty_view.empty());
}

TEST(view_interface, bool_operator) {
  TestView<int> empty_view({});
  TestView<int> non_empty_view({1, 2, 3});
  EXPECT_FALSE(empty_view);
  EXPECT_TRUE(non_empty_view);
}

TEST(view_interface, front) {
  TestView<int> view({1, 2, 3, 4, 5});
  EXPECT_EQ(view.front(), 1);
}

TEST(view_interface, back) {
  TestView<int> view({1, 2, 3, 4, 5});
  EXPECT_EQ(view.back(), 5);
}

TEST(view_interface, operator_subscript) {
  TestView<int> view({1, 2, 3, 4, 5});
  EXPECT_EQ(view[0], 1);
  EXPECT_EQ(view[2], 3);
  EXPECT_EQ(view[4], 5);
}

TEST(view_interface, data) {
  TestView<int> view({1, 2, 3, 4, 5});
  EXPECT_EQ(view.data(), &view.data_[0]);
  EXPECT_EQ(*view.data(), 1);
}

TEST(view_interface, size_with_size_method) {
  TestView<int> view({1, 2, 3, 4, 5});
  EXPECT_EQ(view.size(), 5);
}

TEST(view_interface, size_without_size_method) {
  TestViewNoSize<int> view({1, 2, 3, 4, 5});
  EXPECT_EQ(view.size(), 5);
}
