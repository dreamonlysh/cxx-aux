#include <gtest/gtest.h>
#include <type_traits>

TEST(StructuredBindingDeclarationArray, Value) {
  int a[2] = {1, 2};
  auto [x, y] = a;
  ASSERT_EQ(x, 1);
  ASSERT_EQ(y, 2);
  ASSERT_NE(&x, a);
  ASSERT_NE(&y, std::next(a));
}

TEST(StructuredBindingDeclarationArray, Reference) {
  int a[2] = {1, 2};
  auto& [x, y] = a;
  ASSERT_EQ(x, 1);
  ASSERT_EQ(y, 2);
  ASSERT_EQ(&x, a);
  ASSERT_EQ(&y, std::next(a));
  ASSERT_FALSE((std::is_const_v<decltype(x)>));
  ASSERT_FALSE((std::is_const_v<decltype(y)>));
}

TEST(StructuredBindingDeclarationArray, UniversalReference) {
  int a[2] = {1, 2};
  auto&& [x, y] = a;
  ASSERT_EQ(x, 1);
  ASSERT_EQ(y, 2);
  ASSERT_EQ(&x, a);
  ASSERT_EQ(&y, std::next(a));
}

TEST(StructuredBindingDeclarationArray, Reference2Const) {
  const int a[2] = {1, 2};
  auto& [x, y] = a;
  ASSERT_EQ(x, 1);
  ASSERT_EQ(y, 2);
  ASSERT_EQ(&x, a);
  ASSERT_EQ(&y, std::next(a));
  ASSERT_TRUE((std::is_const_v<decltype(x)>));
  ASSERT_TRUE((std::is_const_v<decltype(y)>));
}

TEST(StructuredBindingDeclarationArray, ConstReference) {
  int a[2] = {1, 2};
  const auto& [x, y] = a;
  ASSERT_EQ(x, 1);
  ASSERT_EQ(y, 2);
  ASSERT_EQ(&x, a);
  ASSERT_EQ(&y, std::next(a));
  ASSERT_TRUE((std::is_const_v<decltype(x)>));
  ASSERT_TRUE((std::is_const_v<decltype(y)>));
}

class StructuredBindingDeclarationTuple : public testing::Test {
protected:
  float tx{1.0};
  int ty{2};

  std::tuple<float&, int, char&&> baseline() {
    char tz{'a'};
    return std::tuple<float&, int, char&&>(tx, ty, std::move(tz));
  }
};

TEST_F(StructuredBindingDeclarationTuple, RValue) {
  auto [x, y, z] = this->baseline();
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationTuple, UniversalRValue) {
  auto&& [x, y, z] = this->baseline();
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationTuple, Reference) {
  auto a = baseline();
  auto& [x, y, z] = a;
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationTuple, UniversalReference) {
  auto a = baseline();
  auto&& [x, y, z] = a;
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationTuple, Reference2Const) {
  const auto a = baseline();
  auto&& [x, y, z] = a;
  // const has no effect on the reference type.
  ASSERT_FALSE((std::is_const_v<decltype(x)>));
  ASSERT_TRUE((std::is_const_v<decltype(y)>));
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationTuple, ConstReference) {
  const auto&& [x, y, z] = baseline();
  ASSERT_FALSE((std::is_const_v<decltype(x)>));
  ASSERT_TRUE((std::is_const_v<decltype(y)>));
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

class StructuredBindingDeclarationStruct : public testing::Test {
protected:
  struct SBD {
    SBD(float& sx, int sy, char&& sz) : sx(sx), sy(sy), sz(std::move(sz)) {}

    float& sx;
    int sy;
    char&& sz;
  };
  float tx{1.0};
  int ty{2};

  SBD baseline() {
    char tz{'a'};
    return SBD(tx, ty, std::move(tz));
  }
};

TEST_F(StructuredBindingDeclarationStruct, RValue) {
  auto [x, y, z] = this->baseline();
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationStruct, UniversalRValue) {
  auto&& [x, y, z] = this->baseline();
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationStruct, Reference) {
  auto a = baseline();
  auto& [x, y, z] = a;
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationStruct, UniversalReference) {
  auto a = baseline();
  auto&& [x, y, z] = a;
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationStruct, Reference2Const) {
  const auto a = baseline();
  auto&& [x, y, z] = a;
  // const has no effect on the reference type.
  ASSERT_FALSE((std::is_const_v<decltype(x)>));
  ASSERT_TRUE((std::is_const_v<decltype(y)>));
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}

TEST_F(StructuredBindingDeclarationStruct, ConstReference) {
  const auto&& [x, y, z] = baseline();
  ASSERT_FALSE((std::is_const_v<decltype(x)>));
  ASSERT_TRUE((std::is_const_v<decltype(y)>));
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}
