#include <estd/cast/value.h>
#include <gtest/gtest.h>

namespace {
class A : public es::Value<void> {
public:
  static constexpr unsigned CLASSID = 1;

  A() : es::Value<void>(CLASSID) {}

  int v = 1;
};

class B : public es::Value<void> {
public:
  static constexpr unsigned CLASSID = 2;

  B() : es::Value<void>(CLASSID) {}

  int v = 2;
};
} // namespace

TEST(CastTest, ISAValue) {
  A a;
  es::Value<void>& ar = a;
  ASSERT_TRUE(es::isa<A>(ar));
  ASSERT_FALSE(es::isa<B>(ar));

  B b;
  es::Value<void>& br = b;
  ASSERT_FALSE(es::isa<A>(br));
  ASSERT_TRUE(es::isa<B>(br));
}

TEST(CastTest, CastValue) {
  A a;
  es::Value<void>& ar = a;
  ASSERT_EQ(es::cast<A>(ar).v, 1);

  B b;
  es::Value<void>& br = b;
  ASSERT_EQ(es::cast<B>(br).v, 2);
}

TEST(CastTest, DynCastValue) {
  A av;
  es::Value<void>& ar = av;
  auto* a = es::dyn_cast<A>(&ar);
  ASSERT_NE(a, nullptr);
  ASSERT_EQ(a->v, 1);
  ASSERT_NO_THROW(es::dyn_cast<A>(ar));
  ASSERT_ANY_THROW(es::dyn_cast<B>(ar));
  ASSERT_EQ(es::dyn_cast<A>(ar).v, 1);
  es::dyn_cast<A>(ar).v = 20;
  ASSERT_EQ(es::dyn_cast<A>(ar).v, 20);

  B bv;
  es::Value<void>& br = bv;
  auto* b = es::dyn_cast<B>(&br);
  ASSERT_NE(b, nullptr);
  ASSERT_EQ(b->v, 2);
  ASSERT_ANY_THROW(es::dyn_cast<A>(br));
  ASSERT_NO_THROW(es::dyn_cast<B>(br));
  ASSERT_EQ(es::dyn_cast<B>(br).v, 2);
  es::dyn_cast<B>(br).v = 30;
  ASSERT_EQ(es::dyn_cast<B>(br).v, 30);

  ASSERT_EQ(es::dyn_cast<A>((es::Value<void>*)nullptr), nullptr);
}
