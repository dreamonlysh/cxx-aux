#include <estd/__utility/disabled_copy_move.h>
#include <gtest/gtest.h>

class NonCopyable : public es::disabled_copy {
public:
  int value = 0;
};

class NonMovable : public es::disabled_move {
public:
  int value = 0;
};

class NonCopyMove : public es::disabled_copy_move {
public:
  int value = 0;
};

TEST(DisabledCopy, CannotCopy) {
  NonCopyable obj1;
  obj1.value = 42;

  EXPECT_FALSE(std::is_copy_constructible_v<NonCopyable>);
  EXPECT_FALSE(std::is_copy_assignable_v<NonCopyable>);
}

TEST(DisabledMove, CanConstruct) {
  NonMovable obj1;
  obj1.value = 42;

  EXPECT_EQ(obj1.value, 42);
}

TEST(DisabledCopyMove, CannotCopyOrMove) {
  NonCopyMove obj1;
  obj1.value = 42;

  EXPECT_FALSE(std::is_copy_constructible_v<NonCopyMove>);
  EXPECT_FALSE(std::is_copy_assignable_v<NonCopyMove>);
  EXPECT_FALSE(std::is_move_constructible_v<NonCopyMove>);
  EXPECT_FALSE(std::is_move_assignable_v<NonCopyMove>);
}

TEST(DisabledCopyMove, CanConstruct) {
  NonCopyMove obj;
  obj.value = 100;

  EXPECT_EQ(obj.value, 100);
}
