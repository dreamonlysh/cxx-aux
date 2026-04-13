#include <estd/__container/small_vector.h>
#include <gtest/gtest.h>
#include <vector>

TEST(SmallVectorTest, DefaultConstructor) {
  es::small_vector<16, int> vec;
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, ConstructorWithSize) {
  es::small_vector<16, int> vec(5);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, ConstructorWithSizeOverflow) {
  es::small_vector<8, int> vec(20);
  EXPECT_EQ(vec.size(), 20);
  EXPECT_FALSE(vec.is_small());
}

TEST(SmallVectorTest, ConstructorWithSizeAndValue) {
  es::small_vector<16, int> vec(5, 42);
  EXPECT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(vec[i], 42);
  }
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, ConstructorWithInitializerList) {
  es::small_vector<16, int> vec{1, 2, 3, 4, 5};
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[4], 5);
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, ConstructorWithInitializerListOverflow) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5, 6, 7, 8};
  EXPECT_EQ(vec.size(), 8);
  EXPECT_FALSE(vec.is_small());
}

TEST(SmallVectorTest, CopyConstructor) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2(vec1);
  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec2[0], 1);
  EXPECT_TRUE(vec2.is_small());
}

TEST(SmallVectorTest, MoveConstructor) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2(std::move(vec1));
  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec2[0], 1);
}

TEST(SmallVectorTest, AssignmentOperator) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2;
  vec2 = vec1;
  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec2[0], 1);
}

TEST(SmallVectorTest, MoveAssignmentOperator) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2;
  vec2 = std::move(vec1);
  EXPECT_EQ(vec2.size(), 3);
}

TEST(SmallVectorTest, At) {
  es::small_vector<16, int> vec{1, 2, 3};
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(2), 3);
  EXPECT_THROW(vec.at(3), std::out_of_range);
}

TEST(SmallVectorTest, SubscriptOperator) {
  es::small_vector<16, int> vec{1, 2, 3};
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[2], 3);
}

TEST(SmallVectorTest, FrontAndBack) {
  es::small_vector<16, int> vec{1, 2, 3};
  EXPECT_EQ(vec.front(), 1);
  EXPECT_EQ(vec.back(), 3);
}

TEST(SmallVectorTest, Data) {
  es::small_vector<16, int> vec{1, 2, 3};
  int* p = vec.data();
  EXPECT_EQ(p[0], 1);
  EXPECT_EQ(p[2], 3);
}

TEST(SmallVectorTest, Iterators) {
  es::small_vector<16, int> vec{1, 2, 3};
  int sum = 0;
  for (int v : vec) {
    sum += v;
  }
  EXPECT_EQ(sum, 6);
}

TEST(SmallVectorTest, ReverseIterators) {
  es::small_vector<16, int> vec{1, 2, 3};
  std::vector<int> reversed(vec.rbegin(), vec.rend());
  EXPECT_EQ(reversed.size(), 3);
  EXPECT_EQ(reversed[0], 3);
  EXPECT_EQ(reversed[2], 1);
}

TEST(SmallVectorTest, Clear) {
  es::small_vector<16, int> vec{1, 2, 3};
  vec.clear();
  EXPECT_TRUE(vec.empty());
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, PushBack) {
  es::small_vector<16, int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, PushBackOverflow) {
  es::small_vector<4, int> vec;
  for (int i = 0; i < 10; ++i) {
    vec.push_back(i);
  }
  EXPECT_EQ(vec.size(), 10);
  EXPECT_FALSE(vec.is_small());
}

TEST(SmallVectorTest, EmplaceBack) {
  es::small_vector<16, int> vec;
  vec.emplace_back(42);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec[0], 42);
}

TEST(SmallVectorTest, PopBack) {
  es::small_vector<16, int> vec{1, 2, 3};
  vec.pop_back();
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec.back(), 2);
}

TEST(SmallVectorTest, PopBackTransitionToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec.pop_back();
  vec.pop_back();
  EXPECT_EQ(vec.size(), 3);
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, Insert) {
  es::small_vector<16, int> vec{1, 3, 4};
  vec.insert(vec.begin() + 1, 2);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[1], 2);
}

TEST(SmallVectorTest, InsertOverflow) {
  es::small_vector<4, int> vec{1, 2, 3, 4};
  vec.insert(vec.begin(), 0);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_FALSE(vec.is_small());
}

TEST(SmallVectorTest, Erase) {
  es::small_vector<16, int> vec{1, 2, 3, 4};
  vec.erase(vec.begin() + 1);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[1], 3);
}

TEST(SmallVectorTest, EraseRange) {
  es::small_vector<16, int> vec{1, 2, 3, 4, 5};
  vec.erase(vec.begin() + 1, vec.begin() + 3);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 4);
}

TEST(SmallVectorTest, EraseTransitionToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5, 6};
  EXPECT_FALSE(vec.is_small());
  vec.erase(vec.begin() + 4, vec.end());
  EXPECT_EQ(vec.size(), 4);
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, Resize) {
  es::small_vector<16, int> vec{1, 2, 3};
  vec.resize(5, 42);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[4], 42);
}

TEST(SmallVectorTest, ResizeOverflow) {
  es::small_vector<4, int> vec{1, 2, 3};
  vec.resize(10, 42);
  EXPECT_EQ(vec.size(), 10);
  EXPECT_FALSE(vec.is_small());
}

TEST(SmallVectorTest, ResizeShrink) {
  es::small_vector<16, int> vec{1, 2, 3, 4, 5};
  vec.resize(3);
  EXPECT_EQ(vec.size(), 3);
}

TEST(SmallVectorTest, Reserve) {
  es::small_vector<16, int> vec{1, 2, 3};
  vec.reserve(100);
  EXPECT_GE(vec.capacity(), 100);
  EXPECT_FALSE(vec.is_small());
}

TEST(SmallVectorTest, ShrinkToFit) {
  es::small_vector<8, int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  EXPECT_FALSE(vec.is_small());
  vec.shrink_to_fit();
  EXPECT_EQ(vec.size(), 10);
}

TEST(SmallVectorTest, ShrinkToFitTransitionToSmall) {
  es::small_vector<8, int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  EXPECT_FALSE(vec.is_small());
  vec.resize(5);
  vec.shrink_to_fit();
  EXPECT_EQ(vec.size(), 5);
  EXPECT_TRUE(vec.is_small());
}

TEST(SmallVectorTest, Swap) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2{4, 5, 6, 7};
  vec1.swap(vec2);
  EXPECT_EQ(vec1.size(), 4);
  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec1[0], 4);
  EXPECT_EQ(vec2[0], 1);
}

TEST(SmallVectorTest, OperatorEquality) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2{1, 2, 3};
  es::small_vector<16, int> vec3{1, 2, 4};
  EXPECT_TRUE(vec1 == vec2);
  EXPECT_FALSE(vec1 == vec3);
}

TEST(SmallVectorTest, OperatorInequality) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2{4, 5, 6};
  EXPECT_TRUE(vec1 != vec2);
}

TEST(SmallVectorTest, OperatorLessThan) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2{1, 2, 4};
  EXPECT_TRUE(vec1 < vec2);
}

TEST(SmallVectorTest, STDSwap) {
  es::small_vector<16, int> vec1{1, 2, 3};
  es::small_vector<16, int> vec2{4, 5, 6};
  std::swap(vec1, vec2);
  EXPECT_EQ(vec1[0], 4);
  EXPECT_EQ(vec2[0], 1);
}

TEST(SmallVectorTest, STDErase) {
  es::small_vector<16, int> vec{1, 2, 3, 2, 4};
  std::erase(vec, 2);
  EXPECT_EQ(vec.size(), 3);
}

TEST(SmallVectorTest, STDEraseIf) {
  es::small_vector<16, int> vec{1, 2, 3, 4, 5};
  std::erase_if(vec, [](int x) { return x % 2 == 0; });
  EXPECT_EQ(vec.size(), 3);
}

TEST(SmallVectorTest, TransitionSmallToLarge) {
  es::small_vector<4, int> vec;
  EXPECT_TRUE(vec.is_small());

  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  EXPECT_TRUE(vec.is_small());

  vec.push_back(4);
  EXPECT_TRUE(vec.is_small());

  vec.push_back(5);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
}

TEST(SmallVectorTest, TransitionLargeToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5, 6};
  EXPECT_FALSE(vec.is_small());

  vec.pop_back();
  vec.pop_back();
  EXPECT_TRUE(vec.is_small());
  EXPECT_EQ(vec.size(), 4);
}

TEST(SmallVectorTest, CapacitySmallVsLarge) {
  es::small_vector<4, int> vec1{1, 2, 3};
  EXPECT_TRUE(vec1.is_small());
  EXPECT_EQ(vec1.capacity(), 4);

  es::small_vector<4, int> vec2{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec2.is_small());
  EXPECT_GE(vec2.capacity(), 5);
}

TEST(SmallVectorTest, Assign) {
  es::small_vector<16, int> vec;
  vec.assign(5, 42);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 42);
}

TEST(SmallVectorTest, AssignRange) {
  es::small_vector<16, int> vec;
  std::vector<int> src{1, 2, 3, 4, 5};
  vec.assign(src.begin(), src.end());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
}

TEST(SmallVectorTest, Emplace) {
  es::small_vector<16, int> vec{1, 3, 4};
  vec.emplace(vec.begin() + 1, 2);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[1], 2);
}
