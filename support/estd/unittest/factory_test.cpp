#include <gtest/gtest.h>
#include <estd/factory.h>

class OpCluster : public es::MemberFunctionFactory<OpCluster> {
  using factory_type = FactoryType<char, int, int, int>;
public:
  OpCluster() {
    initialize_once<factory_type>({
      {'+', &OpCluster::plus},
      {'-', &OpCluster::minus},
      {'*', &OpCluster::star},
      {'/', &OpCluster::div}
    });
  }
  int plus(int lhs, int rhs) {
    return (lhs + rhs) * scale;
  }

  int minus(int lhs, int rhs) {
    return (lhs - rhs) * scale;
  }

  int star(int lhs, int rhs) {
    return (lhs * rhs) * scale;
  }

  int div(int lhs, int rhs) {
    return (lhs / rhs) * scale;
  }

  std::optional<int> calc(char c, int lhs, int rhs) {
    return invoke<factory_type>(c, lhs, rhs);
  }

private:
  int scale = 10;
};

TEST(OpCluster, Plus) {
  ASSERT_EQ(OpCluster().calc('+', 1, 2).value_or(-1), 30);
}

TEST(OpCluster, Minus) {
  ASSERT_EQ(OpCluster().calc('-', 1, 2).value_or(-1), -10);
}

TEST(OpCluster, Star) {
  ASSERT_EQ(OpCluster().calc('*', 1, 2).value_or(-1), 20);
}

TEST(OpCluster, Div) {
  ASSERT_EQ(OpCluster().calc('/', 1, 2).value_or(-1), 0);
}

class VoidOpCluster : public es::MemberFunctionFactory<VoidOpCluster> {
  using factory_type = FactoryType<char, void, int, int>;
public:
  VoidOpCluster() {
    initialize_once<factory_type>({
      {'+', &VoidOpCluster::plus},
      {'-', &VoidOpCluster::minus},
      {'*', &VoidOpCluster::star},
      {'/', &VoidOpCluster::div}
    });
  }
  void plus(int lhs, int rhs) {
     rst = (lhs + rhs) * scale;
  }

  void minus(int lhs, int rhs) {
     rst = (lhs - rhs) * scale;
  }

  void star(int lhs, int rhs) {
     rst = (lhs * rhs) * scale;
  }

  void div(int lhs, int rhs) {
     rst = (lhs / rhs) * scale;
  }

  void calc(char c, int lhs, int rhs) {
    invoke<factory_type>(c, lhs, rhs);
  }

  int result() const {
    return rst;
  }

private:
  int rst = -1;
  int scale = 10;
};

TEST(VoidOpCluster, Plus) {
  VoidOpCluster v;
  v.calc('+', 1, 2);
  ASSERT_EQ(v.result(), 30);
}

TEST(VoidOpCluster, Minus) {
  VoidOpCluster v;
  v.calc('-', 1, 2);
  ASSERT_EQ(v.result(), -10);
}

TEST(VoidOpCluster, Star) {
  VoidOpCluster v;
  v.calc('*', 1, 2);
  ASSERT_EQ(v.result(), 20);
}

TEST(VoidOpCluster, Div) {
  VoidOpCluster v;
  v.calc('/', 1, 2);
  ASSERT_EQ(v.result(), 0);
}
