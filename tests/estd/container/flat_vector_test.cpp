#include "estd/__container/flat_vector.h"
#include <gtest/gtest.h>

TEST(FlatVectorTest, DefaultConstructor) {
  es::flat_vector<10, int> vec;
  ASSERT_TRUE(vec.empty());
  ASSERT_EQ(vec.size(), 0);
  ASSERT_EQ(vec.capacity(), 10);
}

TEST(FlatVectorTest, SizeConstructor) {
  es::flat_vector<10, int> vec(5);
  ASSERT_FALSE(vec.empty());
  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec.capacity(), 10);
}

TEST(FlatVectorTest, SizeValueConstructor) {
  es::flat_vector<10, int> vec(5, 42);
  ASSERT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], 42);
  }
}

TEST(FlatVectorTest, RangeConstructor) {
  std::vector<int> source = {1, 2, 3, 4, 5};
  es::flat_vector<10, int> vec(source.begin(), source.end());
  ASSERT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], source[i]);
  }
}

TEST(FlatVectorTest, InitializerListConstructor) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(FlatVectorTest, CopyConstructor) {
  es::flat_vector<10, int> vec1 = {1, 2, 3, 4, 5};
  es::flat_vector<10, int> vec2(vec1);
  ASSERT_EQ(vec2.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec2[i], vec1[i]);
  }
}

TEST(FlatVectorTest, MoveConstructor) {
  es::flat_vector<10, int> vec1 = {1, 2, 3, 4, 5};
  es::flat_vector<10, int> vec2(std::move(vec1));
  ASSERT_EQ(vec2.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec2[i], i + 1);
  }
}

TEST(FlatVectorTest, CopyAssignment) {
  es::flat_vector<10, int> vec1 = {1, 2, 3, 4, 5};
  es::flat_vector<10, int> vec2;
  vec2 = vec1;
  ASSERT_EQ(vec2.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec2[i], vec1[i]);
  }
}

TEST(FlatVectorTest, MoveAssignment) {
  es::flat_vector<10, int> vec1 = {1, 2, 3, 4, 5};
  es::flat_vector<10, int> vec2;
  vec2 = std::move(vec1);
  ASSERT_EQ(vec2.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec2[i], i + 1);
  }
}

TEST(FlatVectorTest, Assign) {
  es::flat_vector<10, int> vec;
  vec.assign(5, 42);
  ASSERT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], 42);
  }
}

TEST(FlatVectorTest, AssignRange) {
  std::vector<int> source = {1, 2, 3, 4, 5};
  es::flat_vector<10, int> vec;
  vec.assign(source.begin(), source.end());
  ASSERT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], source[i]);
  }
}

TEST(FlatVectorTest, AssignInitializerList) {
  es::flat_vector<10, int> vec;
  vec.assign({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(FlatVectorTest, At) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(vec.at(2), 3);
  ASSERT_THROW(vec.at(5), std::out_of_range);
}

TEST(FlatVectorTest, OperatorSquareBrackets) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(vec[2], 3);
}

TEST(FlatVectorTest, Front) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(vec.front(), 1);
}

TEST(FlatVectorTest, Back) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(vec.back(), 5);
}

TEST(FlatVectorTest, Data) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(vec.data()[2], 3);
}

TEST(FlatVectorTest, BeginEnd) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(*(vec.begin() + 2), 3);
  ASSERT_EQ(*(vec.end() - 1), 5);
}

TEST(FlatVectorTest, RBeginREnd) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  ASSERT_EQ(*(vec.rbegin()), 5);
  ASSERT_EQ(*(vec.rend() - 1), 1);
}

TEST(FlatVectorTest, Empty) {
  es::flat_vector<10, int> vec;
  ASSERT_TRUE(vec.empty());
  vec.push_back(1);
  ASSERT_FALSE(vec.empty());
}

TEST(FlatVectorTest, Size) {
  es::flat_vector<10, int> vec;
  ASSERT_EQ(vec.size(), 0);
  vec.push_back(1);
  ASSERT_EQ(vec.size(), 1);
}

TEST(FlatVectorTest, MaxSize) {
  es::flat_vector<10, int> vec;
  ASSERT_EQ(vec.max_size(), 10);
}

TEST(FlatVectorTest, Clear) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  vec.clear();
  ASSERT_TRUE(vec.empty());
}

TEST(FlatVectorTest, Insert) {
  es::flat_vector<10, int> vec = {1, 2, 4, 5};
  vec.insert(vec.begin() + 2, 3);
  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec[2], 3);
}

TEST(FlatVectorTest, Emplace) {
  es::flat_vector<10, int> vec = {1, 2, 4, 5};
  vec.emplace(vec.begin() + 2, 3);
  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec[2], 3);
}

TEST(FlatVectorTest, Erase) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  vec.erase(vec.begin() + 2);
  ASSERT_EQ(vec.size(), 4);
  ASSERT_EQ(vec[2], 4);
}

TEST(FlatVectorTest, PushBack) {
  es::flat_vector<10, int> vec;
  vec.push_back(1);
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec.back(), 1);
}

TEST(FlatVectorTest, EmplaceBack) {
  es::flat_vector<10, int> vec;
  vec.emplace_back(1);
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec.back(), 1);
}

TEST(FlatVectorTest, PopBack) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  vec.pop_back();
  ASSERT_EQ(vec.size(), 4);
  ASSERT_EQ(vec.back(), 4);
}

TEST(FlatVectorTest, Resize) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec.resize(5, 42);
  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec[3], 42);
  ASSERT_EQ(vec[4], 42);
}

TEST(FlatVectorTest, Swap) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {4, 5, 6};
  vec1.swap(vec2);
  ASSERT_EQ(vec1[0], 4);
  ASSERT_EQ(vec2[0], 1);
}

TEST(FlatVectorTest, EqualityOperator) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec1 == vec2);
}

TEST(FlatVectorTest, InequalityOperator) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {4, 5, 6};
  ASSERT_TRUE(vec1 != vec2);
}

TEST(FlatVectorTest, LessThanOperator) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {4, 5, 6};
  ASSERT_TRUE(vec1 < vec2);
}

TEST(FlatVectorTest, LessThanOrEqualOperator) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec1 <= vec2);
}

TEST(FlatVectorTest, GreaterThanOperator) {
  es::flat_vector<10, int> vec1 = {4, 5, 6};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec1 > vec2);
}

TEST(FlatVectorTest, GreaterThanOrEqualOperator) {
  es::flat_vector<10, int> vec1 = {4, 5, 6};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec1 >= vec2);
}

TEST(FlatVectorTest, StdSwap) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {4, 5, 6};
  std::swap(vec1, vec2);
  ASSERT_EQ(vec1[0], 4);
  ASSERT_EQ(vec2[0], 1);
}

TEST(FlatVectorTest, StdErase) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  std::erase(vec, 3);
  ASSERT_EQ(vec.size(), 4);
  ASSERT_EQ(vec[2], 4);
}

TEST(FlatVectorTest, StdEraseIf) {
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  std::erase_if(vec, [](int value) { return value % 2 == 0; });
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 3);
  ASSERT_EQ(vec[2], 5);
}
