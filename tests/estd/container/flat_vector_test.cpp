#include "estd/__container/flat_vector.h"
#include <gtest/gtest.h>
#include <string>

namespace {

struct TrackedObj {
  static int alive_count;
  static int dtor_count;

  int value;

  explicit TrackedObj(int v = 0) : value(v) { ++alive_count; }

  TrackedObj(const TrackedObj& other) : value(other.value) { ++alive_count; }

  TrackedObj(TrackedObj&& other) noexcept : value(other.value) {
    ++alive_count;
  }

  TrackedObj& operator=(const TrackedObj& other) {
    if (this != &other) {
      ++dtor_count;
      --alive_count;
      value = other.value;
      ++alive_count;
    }
    return *this;
  }

  TrackedObj& operator=(TrackedObj&& other) noexcept {
    if (this != &other) {
      ++dtor_count;
      --alive_count;
      value = other.value;
      ++alive_count;
    }
    return *this;
  }

  bool operator==(const TrackedObj& other) const {
    return value == other.value;
  }

  ~TrackedObj() {
    --alive_count;
    ++dtor_count;
  }

  static void reset() {
    alive_count = 0;
    dtor_count = 0;
  }
};

int TrackedObj::alive_count = 0;
int TrackedObj::dtor_count = 0;

} // namespace

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

// --- Destruction tracking tests for non-trivially-destructible types ---

TEST(FlatVectorDestructionTest, DestructorDestroysAllElements) {
  TrackedObj::reset();
  {
    es::flat_vector<10, TrackedObj> vec;
    vec.push_back(TrackedObj(1));
    vec.push_back(TrackedObj(2));
    vec.push_back(TrackedObj(3));
    ASSERT_EQ(TrackedObj::alive_count, 3);
  }
  ASSERT_EQ(TrackedObj::alive_count, 0);
}

TEST(FlatVectorDestructionTest, DestructorEmptyVector) {
  TrackedObj::reset();
  {
    es::flat_vector<10, TrackedObj> vec;
    ASSERT_EQ(TrackedObj::alive_count, 0);
  }
  ASSERT_EQ(TrackedObj::alive_count, 0);
}

TEST(FlatVectorDestructionTest, PopBackDestroysElement) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec.pop_back();
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec.size(), 2u);

  vec.pop_back();
  ASSERT_EQ(TrackedObj::alive_count, 1);

  vec.pop_back();
  ASSERT_EQ(TrackedObj::alive_count, 0);
}

TEST(FlatVectorDestructionTest, EraseSingleDestroysElement) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec.erase(vec.begin() + 1);
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec.size(), 2u);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 3);
}

TEST(FlatVectorDestructionTest, EraseRangeDestroysElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  vec.push_back(TrackedObj(4));
  vec.push_back(TrackedObj(5));
  ASSERT_EQ(TrackedObj::alive_count, 5);

  vec.erase(vec.begin() + 1, vec.begin() + 3);
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 4);
  ASSERT_EQ(vec[2].value, 5);
}

TEST(FlatVectorDestructionTest, ClearDestroysAllElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec.clear();
  ASSERT_EQ(TrackedObj::alive_count, 0);
  ASSERT_TRUE(vec.empty());
}

TEST(FlatVectorDestructionTest, ClearEmptyVector) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.clear();
  ASSERT_EQ(TrackedObj::alive_count, 0);
}

TEST(FlatVectorDestructionTest, ResizeShrinkDestroysElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  vec.push_back(TrackedObj(4));
  vec.push_back(TrackedObj(5));
  ASSERT_EQ(TrackedObj::alive_count, 5);

  vec.resize(2);
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec.size(), 2u);
}

TEST(FlatVectorDestructionTest, ResizeGrowKeepsExisting) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  ASSERT_EQ(TrackedObj::alive_count, 2);

  vec.resize(4, TrackedObj(99));
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 99);
  ASSERT_EQ(vec[3].value, 99);
}

TEST(FlatVectorDestructionTest, ResizeSameSizeNoChange) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  ASSERT_EQ(TrackedObj::alive_count, 2);

  vec.resize(2);
  ASSERT_EQ(TrackedObj::alive_count, 2);
}

TEST(FlatVectorDestructionTest, CopyAssignmentDestroysOldElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec1;
  vec1.push_back(TrackedObj(10));
  vec1.push_back(TrackedObj(20));

  es::flat_vector<10, TrackedObj> vec2;
  vec2.push_back(TrackedObj(1));
  vec2.push_back(TrackedObj(2));
  vec2.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 5);

  vec2 = vec1;
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(vec2.size(), 2u);
  ASSERT_EQ(vec2[0].value, 10);
  ASSERT_EQ(vec2[1].value, 20);
}

TEST(FlatVectorDestructionTest, MoveAssignmentDestroysOldElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec1;
  vec1.push_back(TrackedObj(10));
  vec1.push_back(TrackedObj(20));

  es::flat_vector<10, TrackedObj> vec2;
  vec2.push_back(TrackedObj(1));
  vec2.push_back(TrackedObj(2));
  vec2.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 5);

  vec2 = std::move(vec1);
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(vec2.size(), 2u);
  ASSERT_EQ(vec2[0].value, 10);
  ASSERT_EQ(vec2[1].value, 20);
}

TEST(FlatVectorDestructionTest, AssignDestroysOldElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec.assign(2, TrackedObj(99));
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec.size(), 2u);
  ASSERT_EQ(vec[0].value, 99);
  ASSERT_EQ(vec[1].value, 99);
}

TEST(FlatVectorDestructionTest, AssignInitializerListDestroysOldElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec.assign({TrackedObj(10), TrackedObj(20)});
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec.size(), 2u);
  ASSERT_EQ(vec[0].value, 10);
  ASSERT_EQ(vec[1].value, 20);
}

TEST(FlatVectorDestructionTest, SwapSameSize) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec1;
  vec1.push_back(TrackedObj(1));
  vec1.push_back(TrackedObj(2));

  es::flat_vector<10, TrackedObj> vec2;
  vec2.push_back(TrackedObj(10));
  vec2.push_back(TrackedObj(20));

  ASSERT_EQ(TrackedObj::alive_count, 4);
  vec1.swap(vec2);
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(vec1[0].value, 10);
  ASSERT_EQ(vec1[1].value, 20);
  ASSERT_EQ(vec2[0].value, 1);
  ASSERT_EQ(vec2[1].value, 2);
}

TEST(FlatVectorDestructionTest, SwapDifferentSize) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec1;
  vec1.push_back(TrackedObj(1));
  vec1.push_back(TrackedObj(2));
  vec1.push_back(TrackedObj(3));

  es::flat_vector<10, TrackedObj> vec2;
  vec2.push_back(TrackedObj(10));

  ASSERT_EQ(TrackedObj::alive_count, 4);
  vec1.swap(vec2);
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(vec1.size(), 1u);
  ASSERT_EQ(vec1[0].value, 10);
  ASSERT_EQ(vec2.size(), 3u);
  ASSERT_EQ(vec2[0].value, 1);
  ASSERT_EQ(vec2[1].value, 2);
  ASSERT_EQ(vec2[2].value, 3);
}

TEST(FlatVectorDestructionTest, SwapWithEmpty) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec1;
  vec1.push_back(TrackedObj(1));
  vec1.push_back(TrackedObj(2));

  es::flat_vector<10, TrackedObj> vec2;

  ASSERT_EQ(TrackedObj::alive_count, 2);
  vec1.swap(vec2);
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_TRUE(vec1.empty());
  ASSERT_EQ(vec2.size(), 2u);
  ASSERT_EQ(vec2[0].value, 1);
  ASSERT_EQ(vec2[1].value, 2);
}

TEST(FlatVectorDestructionTest, StdEraseDestroysElement) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  std::erase(vec, TrackedObj(2));
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec.size(), 2u);
}

TEST(FlatVectorDestructionTest, StdEraseIfDestroysElements) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  vec.push_back(TrackedObj(4));
  ASSERT_EQ(TrackedObj::alive_count, 4);

  std::erase_if(vec, [](const TrackedObj& obj) { return obj.value % 2 == 0; });
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec.size(), 2u);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 3);
}

TEST(FlatVectorDestructionTest, SelfAssignmentNoLeak) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec = vec;
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec.size(), 3u);
}

TEST(FlatVectorDestructionTest, SequentialOperationsNoLeak) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;

  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec.pop_back();
  ASSERT_EQ(TrackedObj::alive_count, 2);

  vec.erase(vec.begin());
  ASSERT_EQ(TrackedObj::alive_count, 1);

  vec.push_back(TrackedObj(10));
  vec.push_back(TrackedObj(20));
  ASSERT_EQ(TrackedObj::alive_count, 3);

  vec.resize(1);
  ASSERT_EQ(TrackedObj::alive_count, 1);

  vec.clear();
  ASSERT_EQ(TrackedObj::alive_count, 0);

  vec.push_back(TrackedObj(100));
  vec.push_back(TrackedObj(200));
  ASSERT_EQ(TrackedObj::alive_count, 2);

  vec.assign(1, TrackedObj(99));
  ASSERT_EQ(TrackedObj::alive_count, 1);
}

// --- TrackedObj constructor tests ---

TEST(FlatVectorTrackedTest, FillConstructor) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec(3, TrackedObj(42));
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0].value, 42);
  ASSERT_EQ(vec[1].value, 42);
  ASSERT_EQ(vec[2].value, 42);
}

TEST(FlatVectorTrackedTest, DefaultFillConstructor) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec(4);
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(vec.size(), 4u);
}

TEST(FlatVectorTrackedTest, InitializerListConstructor) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec = {TrackedObj(1), TrackedObj(2),
                                         TrackedObj(3)};
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, CopyConstructor) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec1;
  vec1.push_back(TrackedObj(10));
  vec1.push_back(TrackedObj(20));

  auto copy = vec1;
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(copy[0].value, 10);
  ASSERT_EQ(copy[1].value, 20);
}

TEST(FlatVectorTrackedTest, MoveConstructor) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec1;
  vec1.push_back(TrackedObj(10));
  vec1.push_back(TrackedObj(20));

  auto moved = std::move(vec1);
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(moved[0].value, 10);
  ASSERT_EQ(moved[1].value, 20);
}

// --- Insert with TrackedObj ---

TEST(FlatVectorTrackedTest, InsertLValue) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(3));

  TrackedObj val(2);
  vec.insert(vec.begin() + 1, val);
  ASSERT_EQ(TrackedObj::alive_count, 4);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, InsertRValue) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(3));

  vec.insert(vec.begin() + 1, TrackedObj(2));
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, InsertAtBegin) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));

  vec.insert(vec.begin(), TrackedObj(1));
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, InsertAtEnd) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));

  vec.insert(vec.end(), TrackedObj(3));
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, InsertCount) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(5));

  vec.insert(vec.begin() + 1, 3, TrackedObj(9));
  ASSERT_EQ(TrackedObj::alive_count, 5);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 9);
  ASSERT_EQ(vec[2].value, 9);
  ASSERT_EQ(vec[3].value, 9);
  ASSERT_EQ(vec[4].value, 5);
}

TEST(FlatVectorTrackedTest, InsertRange) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(5));

  es::flat_vector<10, TrackedObj> src;
  src.push_back(TrackedObj(2));
  src.push_back(TrackedObj(3));
  src.push_back(TrackedObj(4));

  vec.insert(vec.begin() + 1, src.begin(), src.end());
  ASSERT_EQ(TrackedObj::alive_count, 8);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
  ASSERT_EQ(vec[3].value, 4);
  ASSERT_EQ(vec[4].value, 5);
}

// --- Emplace with TrackedObj ---

TEST(FlatVectorTrackedTest, EmplaceAtPosition) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(3));

  vec.emplace(vec.begin() + 1, 2);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, EmplaceAtBegin) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));

  vec.emplace(vec.begin(), 1);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, EmplaceAtEnd) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));

  vec.emplace(vec.end(), 3);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

TEST(FlatVectorTrackedTest, EmplaceBack) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.emplace_back(1);
  vec.emplace_back(2);
  vec.emplace_back(3);
  ASSERT_EQ(TrackedObj::alive_count, 3);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
  ASSERT_EQ(vec[2].value, 3);
}

// --- Erase edge cases with TrackedObj ---

TEST(FlatVectorTrackedTest, EraseFirstElement) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));

  vec.erase(vec.begin());
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec[0].value, 2);
  ASSERT_EQ(vec[1].value, 3);
}

TEST(FlatVectorTrackedTest, EraseLastElement) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));

  vec.erase(vec.end() - 1);
  ASSERT_EQ(TrackedObj::alive_count, 2);
  ASSERT_EQ(vec[0].value, 1);
  ASSERT_EQ(vec[1].value, 2);
}

TEST(FlatVectorTrackedTest, EraseAllByRange) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(1));
  vec.push_back(TrackedObj(2));
  vec.push_back(TrackedObj(3));

  vec.erase(vec.begin(), vec.end());
  ASSERT_EQ(TrackedObj::alive_count, 0);
  ASSERT_TRUE(vec.empty());
}

TEST(FlatVectorTrackedTest, EraseSingleElementVector) {
  TrackedObj::reset();
  es::flat_vector<10, TrackedObj> vec;
  vec.push_back(TrackedObj(42));

  vec.erase(vec.begin());
  ASSERT_EQ(TrackedObj::alive_count, 0);
  ASSERT_TRUE(vec.empty());
}

// --- Edge cases ---

TEST(FlatVectorEdgeTest, FullCapacity) {
  es::flat_vector<3, int> vec = {1, 2, 3};
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec.capacity(), 3u);
  ASSERT_THROW(vec.push_back(4), std::out_of_range);
  ASSERT_THROW(vec.emplace_back(4), std::out_of_range);
  ASSERT_THROW(vec.insert(vec.begin(), 0), std::out_of_range);
}

TEST(FlatVectorEdgeTest, SingleElementCapacity) {
  es::flat_vector<1, int> vec;
  vec.push_back(42);
  ASSERT_EQ(vec.size(), 1u);
  ASSERT_EQ(vec[0], 42);
  ASSERT_THROW(vec.push_back(99), std::out_of_range);
}

TEST(FlatVectorEdgeTest, PopBackEmptyThrows) {
  es::flat_vector<10, int> vec;
  ASSERT_THROW(vec.pop_back(), std::out_of_range);
}

TEST(FlatVectorEdgeTest, AtOutOfRangeThrows) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  ASSERT_THROW(vec.at(3), std::out_of_range);
  ASSERT_THROW(vec.at(100), std::out_of_range);
}

TEST(FlatVectorEdgeTest, ResizeBeyondCapacityNoThrow) {
  es::flat_vector<5, int> vec = {1, 2, 3};
  ASSERT_NO_THROW(vec.resize(6));
}

TEST(FlatVectorEdgeTest, ResizeToZero) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec.resize(0);
  ASSERT_TRUE(vec.empty());
  ASSERT_EQ(vec.size(), 0u);
}

TEST(FlatVectorEdgeTest, ResizeGrowDefault) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec.resize(5);
  ASSERT_EQ(vec.size(), 5u);
  ASSERT_EQ(vec[3], 0);
  ASSERT_EQ(vec[4], 0);
}

TEST(FlatVectorEdgeTest, ResizeAndOverwrite) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec.resize_and_overwrite(5, [](int* p, size_t cap) {
    p[0] = 10;
    p[1] = 20;
    p[2] = 30;
    return 3;
  });
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0], 10);
  ASSERT_EQ(vec[1], 20);
  ASSERT_EQ(vec[2], 30);
}

TEST(FlatVectorEdgeTest, AssignRangeMethod) {
  std::vector<int> src = {10, 20, 30};
  es::flat_vector<10, int> vec = {1, 2, 3, 4, 5};
  vec.assign_range(src);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0], 10);
  ASSERT_EQ(vec[1], 20);
  ASSERT_EQ(vec[2], 30);
}

TEST(FlatVectorEdgeTest, AppendRange) {
  es::flat_vector<10, int> vec = {1, 2};
  std::vector<int> src = {3, 4, 5};
  vec.append_range(src);
  ASSERT_EQ(vec.size(), 5u);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(FlatVectorEdgeTest, InsertInitializerList) {
  es::flat_vector<10, int> vec = {1, 5};
  vec.insert(vec.begin() + 1, {2, 3, 4});
  ASSERT_EQ(vec.size(), 5u);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(FlatVectorEdgeTest, InsertRangeMethod) {
  es::flat_vector<10, int> vec = {1, 5};
  std::vector<int> src = {2, 3, 4};
  vec.insert_range(vec.begin() + 1, src);
  ASSERT_EQ(vec.size(), 5u);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

// --- Iterator tests ---

TEST(FlatVectorIteratorTest, ForwardIteration) {
  es::flat_vector<10, int> vec = {10, 20, 30};
  int sum = 0;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    sum += *it;
  }
  ASSERT_EQ(sum, 60);
}

TEST(FlatVectorIteratorTest, ReverseIteration) {
  es::flat_vector<10, int> vec = {10, 20, 30};
  auto it = vec.rbegin();
  ASSERT_EQ(*it, 30);
  ++it;
  ASSERT_EQ(*it, 20);
  ++it;
  ASSERT_EQ(*it, 10);
}

TEST(FlatVectorIteratorTest, ConstIteratorAccess) {
  const es::flat_vector<10, int> vec = {1, 2, 3};
  ASSERT_EQ(*vec.cbegin(), 1);
  ASSERT_EQ(*(vec.cend() - 1), 3);
}

TEST(FlatVectorIteratorTest, ConstReverseIterator) {
  const es::flat_vector<10, int> vec = {1, 2, 3};
  ASSERT_EQ(*vec.crbegin(), 3);
  ASSERT_EQ(*(vec.crend() - 1), 1);
}

TEST(FlatVectorIteratorTest, RangeForLoop) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  int sum = 0;
  for (const auto& v : vec) {
    sum += v;
  }
  ASSERT_EQ(sum, 6);
}

// --- Comparison operator edge cases ---

TEST(FlatVectorComparisonTest, EmptyVectorsEqual) {
  es::flat_vector<10, int> vec1, vec2;
  ASSERT_TRUE(vec1 == vec2);
  ASSERT_FALSE(vec1 != vec2);
}

TEST(FlatVectorComparisonTest, DifferentSizeNotEqual) {
  es::flat_vector<10, int> vec1 = {1, 2};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec1 != vec2);
  ASSERT_TRUE(vec1 < vec2);
}

TEST(FlatVectorComparisonTest, LexicographicCompare) {
  es::flat_vector<10, int> vec1 = {1, 2, 4};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec2 < vec1);
  ASSERT_TRUE(vec1 > vec2);
}

// --- TrackedObj full lifecycle stress test ---

TEST(FlatVectorTrackedTest, FullLifecycleStress) {
  TrackedObj::reset();
  {
    es::flat_vector<8, TrackedObj> vec;

    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    ASSERT_EQ(vec.size(), 3u);

    vec.insert(vec.begin() + 1, TrackedObj(10));
    ASSERT_EQ(vec.size(), 4u);
    ASSERT_EQ(vec[1].value, 10);

    vec.emplace(vec.begin(), 0);
    ASSERT_EQ(vec.size(), 5u);
    ASSERT_EQ(vec[0].value, 0);

    vec.erase(vec.begin() + 2);
    ASSERT_EQ(vec.size(), 4u);

    vec.pop_back();
    ASSERT_EQ(vec.size(), 3u);

    auto copy = vec;
    ASSERT_EQ(copy.size(), 3u);

    vec = std::move(copy);
    ASSERT_EQ(vec.size(), 3u);

    vec.resize(5, TrackedObj(99));
    ASSERT_EQ(vec.size(), 5u);
    ASSERT_EQ(vec[3].value, 99);

    vec.resize(2);
    ASSERT_EQ(vec.size(), 2u);

    vec.assign({TrackedObj(100), TrackedObj(200), TrackedObj(300)});
    ASSERT_EQ(vec.size(), 3u);
    ASSERT_EQ(vec[0].value, 100);
    ASSERT_EQ(vec[1].value, 200);
    ASSERT_EQ(vec[2].value, 300);

    es::flat_vector<8, TrackedObj> other;
    other.push_back(TrackedObj(50));
    vec.swap(other);
    ASSERT_EQ(vec.size(), 1u);
    ASSERT_EQ(vec[0].value, 50);
    ASSERT_EQ(other.size(), 3u);
  }
}

namespace {
struct Point {
  int x, y;
  Point(int x_, int y_) : x(x_), y(y_) {}
  bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};
} // namespace

TEST(FlatVectorStringTest, DestructorNoLeak) {
  es::flat_vector<10, std::string> vec;
  vec.push_back("hello");
  vec.push_back("world");
  vec.push_back("test");
}

TEST(FlatVectorStringTest, ClearDestroysAll) {
  es::flat_vector<10, std::string> vec;
  vec.push_back("hello");
  vec.push_back("world");
  vec.clear();
  ASSERT_TRUE(vec.empty());
}

TEST(FlatVectorStringTest, EraseDestroysElement) {
  es::flat_vector<10, std::string> vec;
  vec.push_back("a");
  vec.push_back("b");
  vec.push_back("c");
  vec.erase(vec.begin() + 1);
  ASSERT_EQ(vec.size(), 2u);
  ASSERT_EQ(vec[0], "a");
  ASSERT_EQ(vec[1], "c");
}

TEST(FlatVectorStringTest, PopBackDestroysElement) {
  es::flat_vector<10, std::string> vec;
  vec.push_back("a");
  vec.push_back("b");
  vec.pop_back();
  ASSERT_EQ(vec.size(), 1u);
  ASSERT_EQ(vec.back(), "a");
}

TEST(FlatVectorStringTest, ResizeShrinkDestroys) {
  es::flat_vector<10, std::string> vec;
  vec.push_back("a");
  vec.push_back("b");
  vec.push_back("c");
  vec.resize(1);
  ASSERT_EQ(vec.size(), 1u);
  ASSERT_EQ(vec[0], "a");
}

TEST(FlatVectorStringTest, CopyConstructor) {
  es::flat_vector<10, std::string> vec;
  vec.push_back("hello");
  vec.push_back("world");
  auto copy = vec;
  ASSERT_EQ(copy.size(), 2u);
  ASSERT_EQ(copy[0], "hello");
  ASSERT_EQ(copy[1], "world");
}

TEST(FlatVectorStringTest, MoveConstructor) {
  es::flat_vector<10, std::string> vec;
  vec.push_back("hello");
  vec.push_back("world");
  auto moved = std::move(vec);
  ASSERT_EQ(moved.size(), 2u);
  ASSERT_EQ(moved[0], "hello");
  ASSERT_EQ(moved[1], "world");
}

TEST(FlatVectorStringTest, CopyAssignment) {
  es::flat_vector<10, std::string> vec1;
  vec1.push_back("hello");
  vec1.push_back("world");
  es::flat_vector<10, std::string> vec2;
  vec2.push_back("old");
  vec2 = vec1;
  ASSERT_EQ(vec2.size(), 2u);
  ASSERT_EQ(vec2[0], "hello");
  ASSERT_EQ(vec2[1], "world");
}

TEST(FlatVectorStringTest, MoveAssignment) {
  es::flat_vector<10, std::string> vec1;
  vec1.push_back("hello");
  vec1.push_back("world");
  es::flat_vector<10, std::string> vec2;
  vec2.push_back("old");
  vec2 = std::move(vec1);
  ASSERT_EQ(vec2.size(), 2u);
  ASSERT_EQ(vec2[0], "hello");
  ASSERT_EQ(vec2[1], "world");
}

TEST(FlatVectorResizeAndOverwriteTest, ReturnSizeLessThanCount) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec.resize_and_overwrite(8, [](int* p, size_t) {
    p[0] = 10;
    p[1] = 20;
    return 2;
  });
  ASSERT_EQ(vec.size(), 2u);
  ASSERT_EQ(vec[0], 10);
  ASSERT_EQ(vec[1], 20);
}

TEST(FlatVectorResizeAndOverwriteTest, ReturnSizeEqualToCount) {
  es::flat_vector<10, int> vec;
  vec.resize_and_overwrite(5, [](int* p, size_t) {
    for (int i = 0; i < 5; ++i)
      p[i] = i * 10;
    return 5;
  });
  ASSERT_EQ(vec.size(), 5u);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vec[i], i * 10);
  }
}

TEST(FlatVectorAssignRangeTest, FromVector) {
  std::vector<int> src = {10, 20, 30, 40};
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec.assign_range(src);
  ASSERT_EQ(vec.size(), 4u);
  ASSERT_EQ(vec[0], 10);
  ASSERT_EQ(vec[3], 40);
}

TEST(FlatVectorInsertRangeTest, AtBegin) {
  es::flat_vector<10, int> vec = {4, 5};
  std::vector<int> src = {1, 2, 3};
  vec.insert_range(vec.begin(), src);
  ASSERT_EQ(vec.size(), 5u);
  for (int i = 0; i < 5; ++i)
    ASSERT_EQ(vec[i], i + 1);
}

TEST(FlatVectorInsertRangeTest, AtMiddle) {
  es::flat_vector<10, int> vec = {1, 4, 5};
  std::vector<int> src = {2, 3};
  vec.insert_range(vec.begin() + 1, src);
  ASSERT_EQ(vec.size(), 5u);
  for (int i = 0; i < 5; ++i)
    ASSERT_EQ(vec[i], i + 1);
}

TEST(FlatVectorInsertRangeTest, AtEnd) {
  es::flat_vector<10, int> vec = {1, 2};
  std::vector<int> src = {3, 4, 5};
  vec.insert_range(vec.end(), src);
  ASSERT_EQ(vec.size(), 5u);
  for (int i = 0; i < 5; ++i)
    ASSERT_EQ(vec[i], i + 1);
}

TEST(FlatVectorEmplaceTest, MultiArgConstructor) {
  es::flat_vector<10, Point> vec;
  vec.emplace_back(1, 2);
  vec.emplace_back(3, 4);
  vec.emplace(vec.begin() + 1, 5, 6);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0], Point(1, 2));
  ASSERT_EQ(vec[1], Point(5, 6));
  ASSERT_EQ(vec[2], Point(3, 4));
}

TEST(FlatVectorAtTest, ConstAtThrowsOutOfRange) {
  const es::flat_vector<10, int> vec = {1, 2, 3};
  ASSERT_EQ(vec.at(1), 2);
  ASSERT_THROW(vec.at(3), std::out_of_range);
  ASSERT_THROW(vec.at(100), std::out_of_range);
}

TEST(FlatVectorAtTest, EmptyVectorAtThrows) {
  const es::flat_vector<10, int> vec;
  ASSERT_THROW(vec.at(0), std::out_of_range);
}

TEST(FlatVectorStdAlgorithmsTest, StdSwapDifferentSizes) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {4, 5};
  std::swap(vec1, vec2);
  ASSERT_EQ(vec1.size(), 2u);
  ASSERT_EQ(vec2.size(), 3u);
  ASSERT_EQ(vec1[0], 4);
  ASSERT_EQ(vec2[0], 1);
}

TEST(FlatVectorStdAlgorithmsTest, StdEraseMultipleOccurrences) {
  es::flat_vector<10, int> vec = {1, 2, 3, 2, 4, 2};
  auto count = std::erase(vec, 2);
  ASSERT_EQ(count, 3u);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 3);
  ASSERT_EQ(vec[2], 4);
}

TEST(FlatVectorStdAlgorithmsTest, StdEraseNotFound) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  auto count = std::erase(vec, 99);
  ASSERT_EQ(count, 0u);
  ASSERT_EQ(vec.size(), 3u);
}

TEST(FlatVectorStdAlgorithmsTest, StdEraseIfAll) {
  es::flat_vector<10, int> vec = {2, 4, 6, 8};
  auto count = std::erase_if(vec, [](int v) { return v % 2 == 0; });
  ASSERT_EQ(count, 4u);
  ASSERT_TRUE(vec.empty());
}

TEST(FlatVectorComparisonTest, LessThanOrEqualDifferentSizes) {
  es::flat_vector<10, int> vec1 = {1, 2};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec1 <= vec2);
}

TEST(FlatVectorComparisonTest, GreaterThanOrEqualDifferentSizes) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {1, 2};
  ASSERT_TRUE(vec1 >= vec2);
}

TEST(FlatVectorComparisonTest, EqualSameContent) {
  es::flat_vector<10, int> vec1 = {1, 2, 3};
  es::flat_vector<10, int> vec2 = {1, 2, 3};
  ASSERT_TRUE(vec1 == vec2);
  ASSERT_FALSE(vec1 != vec2);
  ASSERT_TRUE(vec1 <= vec2);
  ASSERT_TRUE(vec1 >= vec2);
}

TEST(FlatVectorEdgeCaseTest, ZeroCapacityPushBackThrows) {
  es::flat_vector<0, int> vec;
  ASSERT_TRUE(vec.empty());
  ASSERT_EQ(vec.size(), 0u);
  ASSERT_EQ(vec.capacity(), 0u);
  ASSERT_THROW(vec.push_back(1), std::out_of_range);
}

TEST(FlatVectorEdgeCaseTest, SelfAssignment) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec = vec;
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 2);
  ASSERT_EQ(vec[2], 3);
}

TEST(FlatVectorEdgeCaseTest, MoveFromSelf) {
  es::flat_vector<10, int> vec = {1, 2, 3};
  vec = std::move(vec);
  ASSERT_EQ(vec.size(), 3u);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 2);
  ASSERT_EQ(vec[2], 3);
}
