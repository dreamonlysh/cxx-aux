#include <functional>
#include <gtest/gtest.h>

int sum(int lhs, int rhs) { return lhs + rhs; }

class SumObj {
public:
  int sum(int lhs, int rhs) { return (lhs + rhs) * scale; }

private:
  int scale = 100;
};

TEST(Invoke, GeneralFunction) { ASSERT_EQ(std::invoke(sum, 1, 1), 2); }

TEST(Invoke, GeneralFunctionPointer) { ASSERT_EQ(std::invoke(&sum, 1, 1), 2); }

template <typename T> using binary_method = int (T::*)(int, int);

TEST(Invoke, ClassMethod) {
  SumObj obj;
  ASSERT_EQ(std::invoke(&SumObj::sum, obj, 1, 1), 200);
}
