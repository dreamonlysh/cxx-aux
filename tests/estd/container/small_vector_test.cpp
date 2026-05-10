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

TEST(SmallVectorTest, PopBackNotTransitionToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec.pop_back();
  vec.pop_back();
  EXPECT_EQ(vec.size(), 3);
  EXPECT_FALSE(vec.is_small());
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

TEST(SmallVectorTest, EraseNotTransitionToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5, 6};
  EXPECT_FALSE(vec.is_small());
  vec.erase(vec.begin() + 4, vec.end());
  EXPECT_EQ(vec.size(), 4);
  EXPECT_FALSE(vec.is_small());
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

TEST(SmallVectorTest, NotTransitionLargeToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5, 6};
  EXPECT_FALSE(vec.is_small());

  vec.pop_back();
  vec.pop_back();
  EXPECT_FALSE(vec.is_small());
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

struct TrackedObj {
  static int alive_count;
  static int move_count;
  static int copy_count;
  int value;

  explicit TrackedObj(int v = 0) : value(v) { ++alive_count; }
  TrackedObj(const TrackedObj& o) : value(o.value) {
    ++copy_count;
    ++alive_count;
  }
  TrackedObj(TrackedObj&& o) noexcept : value(o.value) {
    ++move_count;
    ++alive_count;
  }
  TrackedObj& operator=(const TrackedObj& o) {
    if (this != &o) {
      value = o.value;
      ++copy_count;
    }
    return *this;
  }
  TrackedObj& operator=(TrackedObj&& o) noexcept {
    if (this != &o) {
      value = o.value;
      ++move_count;
    }
    return *this;
  }
  bool operator==(const TrackedObj& o) const { return value == o.value; }
  ~TrackedObj() { --alive_count; }
  static void reset() {
    alive_count = 0;
    move_count = 0;
    copy_count = 0;
  }
};

int TrackedObj::alive_count = 0;
int TrackedObj::move_count = 0;
int TrackedObj::copy_count = 0;

class SmallVectorTrackedTest : public ::testing::Test {
protected:
  void SetUp() override { TrackedObj::reset(); }
};

TEST_F(SmallVectorTrackedTest, DefaultConstructor) {
  es::small_vector<4, TrackedObj> vec;
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(TrackedObj::alive_count, 0);
}

TEST_F(SmallVectorTrackedTest, PushBackLValue) {
  es::small_vector<4, TrackedObj> vec;
  TrackedObj obj(1);
  int alive_before = TrackedObj::alive_count;
  vec.push_back(obj);
  EXPECT_EQ(TrackedObj::alive_count, alive_before + 1);
  EXPECT_EQ(TrackedObj::copy_count, 1);
}

TEST_F(SmallVectorTrackedTest, PushBackRValue) {
  es::small_vector<4, TrackedObj> vec;
  TrackedObj::reset();
  vec.push_back(TrackedObj(1));
  EXPECT_GE(TrackedObj::move_count, 1);
}

TEST_F(SmallVectorTrackedTest, EmplaceBack) {
  es::small_vector<4, TrackedObj> vec;
  TrackedObj::reset();
  vec.emplace_back(42);
  EXPECT_EQ(TrackedObj::alive_count, 1);
  EXPECT_EQ(vec[0].value, 42);
}

TEST_F(SmallVectorTrackedTest, PopBackDestroys) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  EXPECT_EQ(TrackedObj::alive_count, 2);
  vec.pop_back();
  EXPECT_EQ(TrackedObj::alive_count, 1);
}

TEST_F(SmallVectorTrackedTest, ClearDestroys) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  vec.emplace_back(3);
  EXPECT_EQ(TrackedObj::alive_count, 3);
  vec.clear();
  EXPECT_EQ(TrackedObj::alive_count, 0);
}

TEST_F(SmallVectorTrackedTest, CopyConstructor) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  int alive_before = TrackedObj::alive_count;
  auto vec2 = vec;
  EXPECT_EQ(TrackedObj::alive_count, alive_before + 2);
  EXPECT_EQ(TrackedObj::copy_count, 2);
  EXPECT_EQ(vec2[0].value, 1);
  EXPECT_EQ(vec2[1].value, 2);
}

TEST_F(SmallVectorTrackedTest, MoveConstructor) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  TrackedObj::reset();
  auto vec2 = std::move(vec);
  EXPECT_EQ(TrackedObj::alive_count, 2);
  EXPECT_GE(TrackedObj::move_count, 2);
  EXPECT_EQ(vec2[0].value, 1);
}

TEST_F(SmallVectorTrackedTest, CopyAssignment) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  es::small_vector<4, TrackedObj> vec2;
  int alive_before = TrackedObj::alive_count;
  vec2 = vec;
  EXPECT_EQ(TrackedObj::alive_count, alive_before + 2);
  EXPECT_EQ(TrackedObj::copy_count, 2);
}

TEST_F(SmallVectorTrackedTest, MoveAssignment) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  es::small_vector<4, TrackedObj> vec2;
  TrackedObj::reset();
  vec2 = std::move(vec);
  EXPECT_EQ(TrackedObj::alive_count, 2);
  EXPECT_GE(TrackedObj::move_count, 2);
}

TEST_F(SmallVectorTrackedTest, TransitionSmallToLargeMoves) {
  es::small_vector<2, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  EXPECT_TRUE(vec.is_small());
  TrackedObj::reset();
  vec.emplace_back(3);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0].value, 1);
  EXPECT_EQ(vec[1].value, 2);
  EXPECT_EQ(vec[2].value, 3);
  EXPECT_GE(TrackedObj::move_count, 2);
}

TEST_F(SmallVectorTrackedTest, DestructorCleansUp) {
  {
    es::small_vector<4, TrackedObj> vec;
    vec.emplace_back(1);
    vec.emplace_back(2);
    EXPECT_EQ(TrackedObj::alive_count, 2);
  }
  EXPECT_EQ(TrackedObj::alive_count, 0);
}

TEST_F(SmallVectorTrackedTest, DestructorLargeStorage) {
  {
    es::small_vector<2, TrackedObj> vec;
    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    EXPECT_FALSE(vec.is_small());
    EXPECT_EQ(TrackedObj::alive_count, 3);
  }
  EXPECT_EQ(TrackedObj::alive_count, 0);
}

TEST_F(SmallVectorTrackedTest, InsertLValue) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(3);
  TrackedObj val(2);
  TrackedObj::reset();
  vec.insert(vec.begin() + 1, val);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[1].value, 2);
}

TEST_F(SmallVectorTrackedTest, InsertRValue) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(3);
  TrackedObj::reset();
  vec.insert(vec.begin() + 1, TrackedObj(2));
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[1].value, 2);
}

TEST_F(SmallVectorTrackedTest, EraseDestroys) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  vec.emplace_back(3);
  EXPECT_EQ(TrackedObj::alive_count, 3);
  vec.erase(vec.begin() + 1);
  EXPECT_EQ(TrackedObj::alive_count, 2);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[0].value, 1);
  EXPECT_EQ(vec[1].value, 3);
}

TEST_F(SmallVectorTrackedTest, ResizeShrinkDestroys) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  vec.emplace_back(3);
  EXPECT_EQ(TrackedObj::alive_count, 3);
  vec.resize(1);
  EXPECT_EQ(TrackedObj::alive_count, 1);
  EXPECT_EQ(vec[0].value, 1);
}

TEST_F(SmallVectorTrackedTest, ResizeGrow) {
  es::small_vector<4, TrackedObj> vec;
  vec.emplace_back(1);
  vec.resize(3, TrackedObj(9));
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0].value, 1);
  EXPECT_EQ(vec[1].value, 9);
  EXPECT_EQ(vec[2].value, 9);
}

class SmallVectorStringTest : public ::testing::Test {};

TEST_F(SmallVectorStringTest, PushBackStrings) {
  es::small_vector<4, std::string> vec;
  vec.push_back("hello");
  vec.push_back("world");
  EXPECT_EQ(vec[0], "hello");
  EXPECT_EQ(vec[1], "world");
  EXPECT_TRUE(vec.is_small());
}

TEST_F(SmallVectorStringTest, TransitionToLarge) {
  es::small_vector<2, std::string> vec;
  vec.push_back("a");
  vec.push_back("b");
  EXPECT_TRUE(vec.is_small());
  vec.push_back("c");
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec[0], "a");
  EXPECT_EQ(vec[1], "b");
  EXPECT_EQ(vec[2], "c");
}

TEST_F(SmallVectorStringTest, CopyConstructor) {
  es::small_vector<4, std::string> vec;
  vec.push_back("hello");
  vec.push_back("world");
  auto vec2 = vec;
  EXPECT_EQ(vec2[0], "hello");
  EXPECT_EQ(vec2[1], "world");
}

TEST_F(SmallVectorStringTest, MoveConstructor) {
  es::small_vector<4, std::string> vec;
  vec.push_back("hello");
  auto vec2 = std::move(vec);
  EXPECT_EQ(vec2[0], "hello");
}

TEST_F(SmallVectorStringTest, CopyAssignmentSmallToLarge) {
  es::small_vector<4, std::string> vec1;
  vec1.push_back("a");
  es::small_vector<4, std::string> vec2;
  vec2.push_back("x");
  vec2.push_back("y");
  vec2.push_back("z");
  vec2.push_back("w");
  vec2.push_back("q");
  EXPECT_FALSE(vec2.is_small());
  vec2 = vec1;
  EXPECT_TRUE(vec2.is_small());
  EXPECT_EQ(vec2.size(), 1);
  EXPECT_EQ(vec2[0], "a");
}

TEST_F(SmallVectorStringTest, CopyAssignmentLargeToSmall) {
  es::small_vector<4, std::string> vec1;
  vec1.push_back("a");
  vec1.push_back("b");
  vec1.push_back("c");
  vec1.push_back("d");
  vec1.push_back("e");
  EXPECT_FALSE(vec1.is_small());
  es::small_vector<4, std::string> vec2;
  vec2.push_back("x");
  EXPECT_TRUE(vec2.is_small());
  vec2 = vec1;
  EXPECT_FALSE(vec2.is_small());
  EXPECT_EQ(vec2.size(), 5);
  EXPECT_EQ(vec2[0], "a");
}

TEST_F(SmallVectorStringTest, ReserveMovesData) {
  es::small_vector<2, std::string> vec;
  vec.push_back("hello");
  vec.push_back("world");
  EXPECT_TRUE(vec.is_small());
  vec.reserve(100);
  EXPECT_FALSE(vec.is_small());
  EXPECT_GE(vec.capacity(), 100);
  EXPECT_EQ(vec[0], "hello");
  EXPECT_EQ(vec[1], "world");
}

TEST_F(SmallVectorStringTest, ShrinkToFitBackToSmall) {
  es::small_vector<4, std::string> vec;
  vec.push_back("a");
  vec.push_back("b");
  vec.push_back("c");
  vec.push_back("d");
  vec.push_back("e");
  EXPECT_FALSE(vec.is_small());
  vec.resize(2);
  vec.shrink_to_fit();
  EXPECT_TRUE(vec.is_small());
  EXPECT_EQ(vec[0], "a");
  EXPECT_EQ(vec[1], "b");
}

TEST_F(SmallVectorStringTest, SwapSmallAndLarge) {
  es::small_vector<4, std::string> vec1;
  vec1.push_back("a");
  es::small_vector<4, std::string> vec2;
  vec2.push_back("x");
  vec2.push_back("y");
  vec2.push_back("z");
  vec2.push_back("w");
  vec2.push_back("q");
  EXPECT_TRUE(vec1.is_small());
  EXPECT_FALSE(vec2.is_small());
  vec1.swap(vec2);
  EXPECT_FALSE(vec1.is_small());
  EXPECT_TRUE(vec2.is_small());
  EXPECT_EQ(vec1.size(), 5);
  EXPECT_EQ(vec2.size(), 1);
  EXPECT_EQ(vec1[0], "x");
  EXPECT_EQ(vec2[0], "a");
}

TEST_F(SmallVectorStringTest, InsertOverflow) {
  es::small_vector<2, std::string> vec;
  vec.push_back("a");
  vec.push_back("b");
  EXPECT_TRUE(vec.is_small());
  vec.insert(vec.begin() + 1, "X");
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec[0], "a");
  EXPECT_EQ(vec[1], "X");
  EXPECT_EQ(vec[2], "b");
}

TEST_F(SmallVectorStringTest, EraseRange) {
  es::small_vector<8, std::string> vec;
  vec.push_back("a");
  vec.push_back("b");
  vec.push_back("c");
  vec.push_back("d");
  vec.push_back("e");
  vec.erase(vec.begin() + 1, vec.begin() + 4);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[0], "a");
  EXPECT_EQ(vec[1], "e");
}

class SmallVectorTransitionTest : public ::testing::Test {};

TEST_F(SmallVectorTransitionTest, InsertCountOverflow) {
  es::small_vector<4, int> vec{1, 2};
  vec.insert(vec.begin(), 3, 0);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 0);
  EXPECT_EQ(vec[2], 0);
  EXPECT_EQ(vec[3], 1);
  EXPECT_EQ(vec[4], 2);
}

TEST_F(SmallVectorTransitionTest, InsertRangeOverflow) {
  es::small_vector<4, int> vec{1, 2};
  std::vector<int> extra{10, 20, 30};
  vec.insert(vec.begin() + 1, extra.begin(), extra.end());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 10);
  EXPECT_EQ(vec[2], 20);
  EXPECT_EQ(vec[3], 30);
  EXPECT_EQ(vec[4], 2);
}

TEST_F(SmallVectorTransitionTest, EmplaceOverflow) {
  es::small_vector<4, int> vec{1, 2, 3, 4};
  EXPECT_TRUE(vec.is_small());
  vec.emplace(vec.begin(), 0);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 0);
}

TEST_F(SmallVectorTransitionTest, ResizeLargeToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec.resize(3, 0);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  vec.shrink_to_fit();
  EXPECT_TRUE(vec.is_small());
}

TEST_F(SmallVectorTransitionTest, AssignLargeToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec.assign(3, 9);
  EXPECT_TRUE(vec.is_small());
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 9);
}

TEST_F(SmallVectorTransitionTest, AssignSmallToLarge) {
  es::small_vector<4, int> vec{1, 2};
  EXPECT_TRUE(vec.is_small());
  vec.assign(10, 7);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 10);
  EXPECT_EQ(vec[0], 7);
}

TEST_F(SmallVectorTransitionTest, AssignRangeOverflow) {
  es::small_vector<4, int> vec{1, 2};
  std::vector<int> src{10, 20, 30, 40, 50};
  vec.assign(src.begin(), src.end());
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 10);
}

TEST_F(SmallVectorTransitionTest, AssignInitializerListOverflow) {
  es::small_vector<4, int> vec{1, 2};
  vec.assign({10, 20, 30, 40, 50});
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 10);
}

TEST_F(SmallVectorTransitionTest, OperatorAssignInitializerListOverflow) {
  es::small_vector<4, int> vec{1, 2};
  vec = {10, 20, 30, 40, 50};
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
}

TEST_F(SmallVectorTransitionTest, CopyConstructorLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  auto vec2 = vec;
  EXPECT_FALSE(vec2.is_small());
  EXPECT_EQ(vec2.size(), 5);
  EXPECT_EQ(vec2[0], 1);
}

TEST_F(SmallVectorTransitionTest, MoveConstructorLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  auto vec2 = std::move(vec);
  EXPECT_FALSE(vec2.is_small());
  EXPECT_EQ(vec2.size(), 5);
  EXPECT_EQ(vec2[0], 1);
}

TEST_F(SmallVectorTransitionTest, CopyAssignLargeToLarge) {
  es::small_vector<4, int> vec1{1, 2, 3, 4, 5};
  es::small_vector<4, int> vec2{10, 20, 30, 40, 50};
  vec2 = vec1;
  EXPECT_FALSE(vec2.is_small());
  EXPECT_EQ(vec2[0], 1);
  EXPECT_EQ(vec2[4], 5);
}

class SmallVectorEdgeTest : public ::testing::Test {};

TEST_F(SmallVectorEdgeTest, EmptyVectorOperations) {
  es::small_vector<4, int> vec;
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
  EXPECT_TRUE(vec.is_small());
  EXPECT_EQ(vec.begin(), vec.end());
}

TEST_F(SmallVectorEdgeTest, SingleElement) {
  es::small_vector<4, int> vec;
  vec.push_back(42);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec.front(), 42);
  EXPECT_EQ(vec.back(), 42);
  EXPECT_EQ(vec[0], 42);
  vec.pop_back();
  EXPECT_TRUE(vec.empty());
}

TEST_F(SmallVectorEdgeTest, SelfAssignment) {
  es::small_vector<4, int> vec{1, 2, 3};
  vec = vec;
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
}

TEST_F(SmallVectorEdgeTest, SelfAssignmentLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec = vec;
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
}

TEST_F(SmallVectorEdgeTest, SelfSwap) {
  es::small_vector<4, int> vec{1, 2, 3};
  vec.swap(vec);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
}

TEST_F(SmallVectorEdgeTest, AtThrowsOnLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_THROW(vec.at(5), std::out_of_range);
}

TEST_F(SmallVectorEdgeTest, ConstAccess) {
  es::small_vector<4, int> vec{1, 2, 3};
  const auto& cvec = vec;
  EXPECT_EQ(cvec[0], 1);
  EXPECT_EQ(cvec.at(1), 2);
  EXPECT_EQ(cvec.front(), 1);
  EXPECT_EQ(cvec.back(), 3);
  EXPECT_EQ(*cvec.data(), 1);
  EXPECT_EQ(cvec.begin(), cvec.cbegin());
  EXPECT_EQ(cvec.end(), cvec.cend());
}

TEST_F(SmallVectorEdgeTest, IteratorDistance) {
  es::small_vector<4, int> vec{1, 2, 3};
  EXPECT_EQ(std::distance(vec.begin(), vec.end()), 3);
  EXPECT_EQ(std::distance(vec.cbegin(), vec.cend()), 3);
  EXPECT_EQ(std::distance(vec.rbegin(), vec.rend()), 3);
}

TEST_F(SmallVectorEdgeTest, RangeForLoop) {
  es::small_vector<4, int> vec{10, 20, 30};
  int sum = 0;
  for (auto& v : vec) {
    sum += v;
  }
  EXPECT_EQ(sum, 60);
}

TEST_F(SmallVectorEdgeTest, PushBackMoveValue) {
  es::small_vector<4, std::string> vec;
  std::string s = "hello";
  vec.push_back(std::move(s));
  EXPECT_EQ(vec[0], "hello");
}

TEST_F(SmallVectorEdgeTest, InsertAtEnd) {
  es::small_vector<8, int> vec{1, 2, 3};
  vec.insert(vec.end(), 4);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[3], 4);
}

TEST_F(SmallVectorEdgeTest, InsertAtBeginning) {
  es::small_vector<8, int> vec{1, 2, 3};
  vec.insert(vec.begin(), 0);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 1);
}

TEST_F(SmallVectorEdgeTest, InsertCountAtBeginning) {
  es::small_vector<8, int> vec{1, 2, 3};
  vec.insert(vec.begin(), 3, 0);
  EXPECT_EQ(vec.size(), 6);
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[3], 1);
}

TEST_F(SmallVectorEdgeTest, EraseFirstElement) {
  es::small_vector<8, int> vec{1, 2, 3, 4};
  vec.erase(vec.begin());
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 2);
}

TEST_F(SmallVectorEdgeTest, EraseLastElement) {
  es::small_vector<8, int> vec{1, 2, 3, 4};
  vec.erase(vec.end() - 1);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.back(), 3);
}

TEST_F(SmallVectorEdgeTest, ResizeToZero) {
  es::small_vector<4, int> vec{1, 2, 3};
  vec.resize(0);
  EXPECT_TRUE(vec.empty());
  EXPECT_TRUE(vec.is_small());
}

TEST_F(SmallVectorEdgeTest, ResizeGrowWithValue) {
  es::small_vector<8, int> vec{1, 2};
  vec.resize(5, 99);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[2], 99);
  EXPECT_EQ(vec[4], 99);
}

TEST_F(SmallVectorEdgeTest, ReserveNoOpWhenSmallAndCapWithin) {
  es::small_vector<4, int> vec{1, 2};
  EXPECT_TRUE(vec.is_small());
  vec.reserve(4);
  EXPECT_TRUE(vec.is_small());
}

TEST_F(SmallVectorEdgeTest, ClearLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec.clear();
  EXPECT_TRUE(vec.empty());
  EXPECT_FALSE(vec.is_small());
}

TEST_F(SmallVectorEdgeTest, ComparisonOperators) {
  es::small_vector<4, int> a{1, 2, 3};
  es::small_vector<4, int> b{1, 2, 3};
  es::small_vector<4, int> c{1, 2, 4};
  es::small_vector<4, int> d{1, 2};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
  EXPECT_TRUE(a != c);
  EXPECT_TRUE(a < c);
  EXPECT_TRUE(c > a);
  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(a >= b);
  EXPECT_TRUE(d < a);
  EXPECT_TRUE(a > d);
  EXPECT_TRUE(d <= a);
  EXPECT_TRUE(a >= d);
}

TEST_F(SmallVectorEdgeTest, MultipleTransitions) {
  es::small_vector<4, int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  vec.push_back(5);
  EXPECT_FALSE(vec.is_small());
  vec.resize(2);
  vec.shrink_to_fit();
  EXPECT_TRUE(vec.is_small());
  vec.push_back(3);
  vec.push_back(4);
  vec.push_back(5);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[4], 5);
}

TEST_F(SmallVectorEdgeTest, SwapBothLarge) {
  es::small_vector<4, int> vec1{1, 2, 3, 4, 5};
  es::small_vector<4, int> vec2{10, 20, 30, 40, 50, 60};
  EXPECT_FALSE(vec1.is_small());
  EXPECT_FALSE(vec2.is_small());
  vec1.swap(vec2);
  EXPECT_EQ(vec1.size(), 6);
  EXPECT_EQ(vec2.size(), 5);
  EXPECT_EQ(vec1[0], 10);
  EXPECT_EQ(vec2[0], 1);
}

TEST_F(SmallVectorEdgeTest, SwapBothSmall) {
  es::small_vector<4, int> vec1{1, 2};
  es::small_vector<4, int> vec2{3, 4, 5};
  EXPECT_TRUE(vec1.is_small());
  EXPECT_TRUE(vec2.is_small());
  vec1.swap(vec2);
  EXPECT_EQ(vec1.size(), 3);
  EXPECT_EQ(vec2.size(), 2);
  EXPECT_EQ(vec1[0], 3);
  EXPECT_EQ(vec2[0], 1);
}

TEST_F(SmallVectorEdgeTest, STDSwapSmallAndLarge) {
  es::small_vector<4, int> vec1{1, 2};
  es::small_vector<4, int> vec2{10, 20, 30, 40, 50};
  std::swap(vec1, vec2);
  EXPECT_EQ(vec1.size(), 5);
  EXPECT_EQ(vec2.size(), 2);
  EXPECT_EQ(vec1[0], 10);
  EXPECT_EQ(vec2[0], 1);
}

TEST_F(SmallVectorEdgeTest, STDEraseOnLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 2, 4, 5};
  EXPECT_FALSE(vec.is_small());
  std::erase(vec, 2);
  EXPECT_EQ(vec.size(), 4);
}

TEST_F(SmallVectorEdgeTest, STDEraseIfOnLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5, 6};
  EXPECT_FALSE(vec.is_small());
  std::erase_if(vec, [](int x) { return x % 2 == 0; });
  EXPECT_EQ(vec.size(), 3);
}

class SmallVectorSVOTransitionTest : public ::testing::Test {};

TEST_F(SmallVectorSVOTransitionTest, PushBackNPlus1TriggersSmallToLarge) {
  es::small_vector<4, int> vec;
  for (int i = 0; i < 4; ++i) vec.push_back(i);
  EXPECT_TRUE(vec.is_small());
  vec.push_back(4);
  EXPECT_FALSE(vec.is_small());
  for (int i = 0; i < 5; ++i) EXPECT_EQ(vec[i], i);
}

TEST_F(SmallVectorSVOTransitionTest, ReserveTriggersSmallToLarge) {
  es::small_vector<4, int> vec{1, 2, 3};
  EXPECT_TRUE(vec.is_small());
  vec.reserve(10);
  EXPECT_FALSE(vec.is_small());
  EXPECT_GE(vec.capacity(), 10);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[2], 3);
}

TEST_F(SmallVectorSVOTransitionTest, ShrinkToFitAfterOverflowTriggersLargeToSmall) {
  es::small_vector<4, int> vec;
  for (int i = 0; i < 6; ++i) vec.push_back(i);
  EXPECT_FALSE(vec.is_small());
  vec.resize(3);
  vec.shrink_to_fit();
  EXPECT_TRUE(vec.is_small());
  for (int i = 0; i < 3; ++i) EXPECT_EQ(vec[i], i);
}

TEST_F(SmallVectorSVOTransitionTest, InsertBeyondCapacityTriggersSmallToLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4};
  EXPECT_TRUE(vec.is_small());
  vec.insert(vec.begin() + 2, 99);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[2], 99);
}

TEST_F(SmallVectorSVOTransitionTest, ResizeBeyondNTriggersSmallToLarge) {
  es::small_vector<4, int> vec{1, 2};
  EXPECT_TRUE(vec.is_small());
  vec.resize(8, 0);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 8);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
}

TEST_F(SmallVectorSVOTransitionTest, ResizeBackAndShrinkToFitTriggersLargeToSmall) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec.resize(2);
  vec.shrink_to_fit();
  EXPECT_TRUE(vec.is_small());
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
}

struct TrackedString {
  static int alive_count;
  static int move_count;
  static int copy_count;
  std::string value;

  explicit TrackedString(const std::string& v = "") : value(v) { ++alive_count; }
  TrackedString(const TrackedString& o) : value(o.value) {
    ++copy_count;
    ++alive_count;
  }
  TrackedString(TrackedString&& o) noexcept : value(std::move(o.value)) {
    ++move_count;
    ++alive_count;
  }
  TrackedString& operator=(const TrackedString& o) {
    if (this != &o) {
      value = o.value;
      ++copy_count;
    }
    return *this;
  }
  TrackedString& operator=(TrackedString&& o) noexcept {
    if (this != &o) {
      value = std::move(o.value);
      ++move_count;
    }
    return *this;
  }
  bool operator==(const TrackedString& o) const { return value == o.value; }
  ~TrackedString() { --alive_count; }
  static void reset() {
    alive_count = 0;
    move_count = 0;
    copy_count = 0;
  }
};

int TrackedString::alive_count = 0;
int TrackedString::move_count = 0;
int TrackedString::copy_count = 0;

class SmallVectorTrackedStringTest : public ::testing::Test {
protected:
  void SetUp() override { TrackedString::reset(); }
};

TEST_F(SmallVectorTrackedStringTest, DestructorDestroysAllSmall) {
  {
    es::small_vector<4, TrackedString> vec;
    vec.emplace_back("a");
    vec.emplace_back("b");
    vec.emplace_back("c");
    EXPECT_EQ(TrackedString::alive_count, 3);
  }
  EXPECT_EQ(TrackedString::alive_count, 0);
}

TEST_F(SmallVectorTrackedStringTest, DestructorDestroysAllLarge) {
  {
    es::small_vector<2, TrackedString> vec;
    vec.emplace_back("a");
    vec.emplace_back("b");
    vec.emplace_back("c");
    EXPECT_FALSE(vec.is_small());
    EXPECT_EQ(TrackedString::alive_count, 3);
  }
  EXPECT_EQ(TrackedString::alive_count, 0);
}

TEST_F(SmallVectorTrackedStringTest, ClearDestroysAllSmall) {
  es::small_vector<4, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  EXPECT_EQ(TrackedString::alive_count, 2);
  vec.clear();
  EXPECT_EQ(TrackedString::alive_count, 0);
}

TEST_F(SmallVectorTrackedStringTest, ClearDestroysAllLarge) {
  es::small_vector<2, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  vec.emplace_back("c");
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(TrackedString::alive_count, 3);
  vec.clear();
  EXPECT_EQ(TrackedString::alive_count, 0);
}

TEST_F(SmallVectorTrackedStringTest, EraseDestroysElement) {
  es::small_vector<4, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  vec.emplace_back("c");
  EXPECT_EQ(TrackedString::alive_count, 3);
  vec.erase(vec.begin() + 1);
  EXPECT_EQ(TrackedString::alive_count, 2);
  EXPECT_EQ(vec[0].value, "a");
  EXPECT_EQ(vec[1].value, "c");
}

TEST_F(SmallVectorTrackedStringTest, PopBackDestroysElement) {
  es::small_vector<4, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  EXPECT_EQ(TrackedString::alive_count, 2);
  vec.pop_back();
  EXPECT_EQ(TrackedString::alive_count, 1);
  EXPECT_EQ(vec[0].value, "a");
}

TEST_F(SmallVectorTrackedStringTest, CopyConstructorSmall) {
  es::small_vector<4, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  int alive_before = TrackedString::alive_count;
  auto vec2 = vec;
  EXPECT_EQ(TrackedString::alive_count, alive_before + 2);
  EXPECT_EQ(TrackedString::copy_count, 2);
  EXPECT_EQ(vec2[0].value, "a");
  EXPECT_EQ(vec2[1].value, "b");
}

TEST_F(SmallVectorTrackedStringTest, CopyConstructorLarge) {
  es::small_vector<2, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  vec.emplace_back("c");
  EXPECT_FALSE(vec.is_small());
  int alive_before = TrackedString::alive_count;
  auto vec2 = vec;
  EXPECT_EQ(TrackedString::alive_count, alive_before + 3);
  EXPECT_EQ(TrackedString::copy_count, 3);
}

TEST_F(SmallVectorTrackedStringTest, MoveConstructorSmall) {
  es::small_vector<4, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  TrackedString::reset();
  auto vec2 = std::move(vec);
  EXPECT_EQ(TrackedString::alive_count, 2);
  EXPECT_GE(TrackedString::move_count, 2);
  EXPECT_EQ(vec2[0].value, "a");
}

TEST_F(SmallVectorTrackedStringTest, MoveConstructorLarge) {
  es::small_vector<2, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  vec.emplace_back("c");
  EXPECT_FALSE(vec.is_small());
  int alive_before = TrackedString::alive_count;
  auto vec2 = std::move(vec);
  EXPECT_EQ(TrackedString::alive_count, alive_before);
  EXPECT_EQ(vec2[0].value, "a");
  EXPECT_EQ(vec2[1].value, "b");
  EXPECT_EQ(vec2[2].value, "c");
}

TEST_F(SmallVectorTrackedStringTest, SVOTransitionSmallToLarge) {
  es::small_vector<2, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  EXPECT_TRUE(vec.is_small());
  TrackedString::reset();
  vec.emplace_back("c");
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec[0].value, "a");
  EXPECT_EQ(vec[1].value, "b");
  EXPECT_EQ(vec[2].value, "c");
  EXPECT_GE(TrackedString::move_count, 2);
}

TEST_F(SmallVectorTrackedStringTest, SVOTransitionLargeToSmall) {
  es::small_vector<4, TrackedString> vec;
  vec.emplace_back("a");
  vec.emplace_back("b");
  vec.emplace_back("c");
  vec.emplace_back("d");
  vec.emplace_back("e");
  EXPECT_FALSE(vec.is_small());
  vec.resize(2);
  vec.shrink_to_fit();
  EXPECT_TRUE(vec.is_small());
  EXPECT_EQ(vec[0].value, "a");
  EXPECT_EQ(vec[1].value, "b");
}

struct MultiArg {
  static int ctor_count;
  static int copy_count;
  static int move_count;
  int a;
  double b;
  std::string c;

  MultiArg(int a_, double b_, std::string c_)
      : a(a_), b(b_), c(std::move(c_)) {
    ++ctor_count;
  }
  MultiArg(const MultiArg& o) : a(o.a), b(o.b), c(o.c) { ++copy_count; }
  MultiArg(MultiArg&& o) noexcept
      : a(o.a), b(o.b), c(std::move(o.c)) {
    ++move_count;
  }
  MultiArg& operator=(const MultiArg&) = delete;
  MultiArg& operator=(MultiArg&&) = delete;
  static void reset() {
    ctor_count = 0;
    copy_count = 0;
    move_count = 0;
  }
};

int MultiArg::ctor_count = 0;
int MultiArg::copy_count = 0;
int MultiArg::move_count = 0;

class SmallVectorEmplaceBackTest : public ::testing::Test {
protected:
  void SetUp() override { MultiArg::reset(); }
};

TEST_F(SmallVectorEmplaceBackTest, EmplaceBackMultiArg) {
  es::small_vector<4, MultiArg> vec;
  vec.emplace_back(1, 2.5, "hello");
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec[0].a, 1);
  EXPECT_EQ(vec[0].b, 2.5);
  EXPECT_EQ(vec[0].c, "hello");
  EXPECT_EQ(MultiArg::ctor_count, 1);
  EXPECT_EQ(MultiArg::copy_count, 0);
  EXPECT_EQ(MultiArg::move_count, 0);
}

TEST_F(SmallVectorEmplaceBackTest, EmplaceBackNoExtraCopiesOrMoves) {
  es::small_vector<4, MultiArg> vec;
  vec.emplace_back(1, 1.0, "a");
  vec.emplace_back(2, 2.0, "b");
  vec.emplace_back(3, 3.0, "c");
  EXPECT_EQ(MultiArg::ctor_count, 3);
  EXPECT_EQ(MultiArg::copy_count, 0);
  EXPECT_EQ(MultiArg::move_count, 0);
}

TEST_F(SmallVectorEmplaceBackTest, EmplaceBackTriggersTransition) {
  es::small_vector<2, MultiArg> vec;
  vec.emplace_back(1, 1.0, "a");
  vec.emplace_back(2, 2.0, "b");
  EXPECT_TRUE(vec.is_small());
  MultiArg::reset();
  vec.emplace_back(3, 3.0, "c");
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec[0].a, 1);
  EXPECT_EQ(vec[1].a, 2);
  EXPECT_EQ(vec[2].a, 3);
  EXPECT_GE(MultiArg::move_count, 2);
}

TEST_F(SmallVectorEmplaceBackTest, EmplaceBackReturnsReference) {
  es::small_vector<4, MultiArg> vec;
  auto& ref = vec.emplace_back(42, 3.14, "world");
  EXPECT_EQ(ref.a, 42);
  EXPECT_EQ(ref.b, 3.14);
  EXPECT_EQ(ref.c, "world");
}

class SmallVectorInsertErasePositionTest : public ::testing::Test {};

TEST_F(SmallVectorInsertErasePositionTest, InsertAtBeginTriggersLarge) {
  es::small_vector<4, int> vec{2, 3, 4, 5};
  EXPECT_TRUE(vec.is_small());
  vec.insert(vec.begin(), 1);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[4], 5);
}

TEST_F(SmallVectorInsertErasePositionTest, InsertAtEndTriggersLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4};
  EXPECT_TRUE(vec.is_small());
  vec.insert(vec.end(), 5);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[4], 5);
}

TEST_F(SmallVectorInsertErasePositionTest, InsertAtMidTriggersLarge) {
  es::small_vector<4, int> vec{1, 2, 4, 5};
  EXPECT_TRUE(vec.is_small());
  vec.insert(vec.begin() + 2, 3);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[2], 3);
}

TEST_F(SmallVectorInsertErasePositionTest, EraseAtBeginLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec.erase(vec.begin());
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[0], 2);
}

TEST_F(SmallVectorInsertErasePositionTest, EraseAtLastLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  vec.erase(vec.end() - 1);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec.back(), 4);
}

TEST_F(SmallVectorInsertErasePositionTest, EraseAtMidLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  vec.erase(vec.begin() + 2);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[2], 4);
}

class SmallVectorSwapComprehensiveTest : public ::testing::Test {};

TEST_F(SmallVectorSwapComprehensiveTest, SwapBothSmallWithStrings) {
  es::small_vector<4, std::string> vec1{"a", "b"};
  es::small_vector<4, std::string> vec2{"x", "y", "z"};
  EXPECT_TRUE(vec1.is_small());
  EXPECT_TRUE(vec2.is_small());
  vec1.swap(vec2);
  EXPECT_TRUE(vec1.is_small());
  EXPECT_TRUE(vec2.is_small());
  EXPECT_EQ(vec1.size(), 3);
  EXPECT_EQ(vec2.size(), 2);
  EXPECT_EQ(vec1[0], "x");
  EXPECT_EQ(vec2[0], "a");
}

TEST_F(SmallVectorSwapComprehensiveTest, SwapBothLargeWithStrings) {
  es::small_vector<2, std::string> vec1{"a", "b", "c"};
  es::small_vector<2, std::string> vec2{"x", "y", "z", "w"};
  EXPECT_FALSE(vec1.is_small());
  EXPECT_FALSE(vec2.is_small());
  vec1.swap(vec2);
  EXPECT_EQ(vec1.size(), 4);
  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec1[0], "x");
  EXPECT_EQ(vec2[0], "a");
}

TEST_F(SmallVectorSwapComprehensiveTest, SwapSmallAndLargeWithStrings) {
  es::small_vector<4, std::string> vec1{"a"};
  es::small_vector<4, std::string> vec2{"x", "y", "z", "w", "q"};
  EXPECT_TRUE(vec1.is_small());
  EXPECT_FALSE(vec2.is_small());
  vec1.swap(vec2);
  EXPECT_FALSE(vec1.is_small());
  EXPECT_TRUE(vec2.is_small());
  EXPECT_EQ(vec1.size(), 5);
  EXPECT_EQ(vec2.size(), 1);
  EXPECT_EQ(vec1[0], "x");
  EXPECT_EQ(vec2[0], "a");
}

class SmallVectorEdgeCaseNewTest : public ::testing::Test {};

TEST_F(SmallVectorEdgeCaseNewTest, MoveFromSelfSmall) {
  es::small_vector<4, int> vec{1, 2, 3};
  vec = std::move(vec);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
}

TEST_F(SmallVectorEdgeCaseNewTest, MoveFromSelfLarge) {
  es::small_vector<4, int> vec{1, 2, 3, 4, 5};
  EXPECT_FALSE(vec.is_small());
  vec = std::move(vec);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
}

TEST_F(SmallVectorEdgeCaseNewTest, SmallVectorN1) {
  es::small_vector<1, int> vec;
  EXPECT_TRUE(vec.is_small());
  EXPECT_EQ(vec.capacity(), 1);
  vec.push_back(1);
  EXPECT_TRUE(vec.is_small());
  vec.push_back(2);
  EXPECT_FALSE(vec.is_small());
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
}
