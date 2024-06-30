#include <estd/cast/dyn_cast_variant.h>
#include <gtest/gtest.h>

namespace {
struct A {
  int v;
};
struct B {
  char v;
};
} // namespace

TEST(CastTest, ISAVariant) {
  std::variant<A, B, int, float> v;
  v = A{1};
  ASSERT_TRUE(es::isa<A>(v));
  ASSERT_FALSE(es::isa<B>(v));
  ASSERT_FALSE(es::isa<int>(v));
  ASSERT_FALSE(es::isa<float>(v));

  v = 1.1f;
  ASSERT_FALSE(es::isa<A>(v));
  ASSERT_FALSE(es::isa<B>(v));
  ASSERT_FALSE(es::isa<int>(v));
  ASSERT_TRUE(es::isa<float>(v));
}

TEST(CastTest, CastVariant) {
  std::variant<A, B, int, float> v;
  v = A{1};
  ASSERT_EQ(es::cast<A>(v).v, 1);

  v = 1.1f;
  ASSERT_FLOAT_EQ(es::cast<float>(v), 1.1f);
}

TEST(CastTest, DynCastVariant) {
  std::variant<A, B, int, float> v;
  v = A{1};
  auto* a = es::dyn_cast<A>(&v);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(a->v, 1);
  ASSERT_NO_THROW(es::dyn_cast<A>(v));
  ASSERT_ANY_THROW(es::dyn_cast<B>(v));
  ASSERT_EQ(es::dyn_cast<A>(v).v, 1);
  es::dyn_cast<A>(v).v = 2;
  ASSERT_EQ(es::dyn_cast<A>(v).v, 2);

  v = B{10};
  auto* b = es::dyn_cast<B>(&v);
  ASSERT_NE(b, nullptr);
  ASSERT_EQ(b->v, 10);
  ASSERT_ANY_THROW(es::dyn_cast<A>(v));
  ASSERT_NO_THROW(es::dyn_cast<B>(v));
  ASSERT_EQ(es::dyn_cast<B>(v).v, 10);
  es::dyn_cast<B>(v).v = 20;
  ASSERT_EQ(es::dyn_cast<B>(v).v, 20);

  ASSERT_EQ(es::dyn_cast<A>((std::variant<A, B, int, float>*)nullptr), nullptr);
}
