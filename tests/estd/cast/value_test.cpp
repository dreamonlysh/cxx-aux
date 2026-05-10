#include <estd/cast/type_id.h>
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

TEST(CastTest, CopyMove) {
  A a;
  ASSERT_EQ(a.v, 1);
  {
    A b = a;
    ASSERT_EQ(b.v, 1);
    ASSERT_TRUE(es::isa<A>(b));
    A c = std::move(b);
    ASSERT_EQ(c.v, 1);
    ASSERT_TRUE(es::isa<A>(c));
  }
  {
    A b(a);
    ASSERT_EQ(b.v, 1);
    ASSERT_TRUE(es::isa<A>(b));
    A c(std::move(b));
    ASSERT_EQ(c.v, 1);
    ASSERT_TRUE(es::isa<A>(c));
  }
}

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

namespace {
class X : public es::Value<void> {
public:
  static constexpr unsigned CLASSID = 10;
  X() : es::Value<void>(CLASSID) {}
  int x = 10;
};

class Y : public es::Value<void> {
public:
  static constexpr unsigned CLASSID = 11;
  Y() : es::Value<void>(CLASSID) {}
  int y = 11;
};

class Z : public es::Value<void> {
public:
  static constexpr unsigned CLASSID = 12;
  Z() : es::Value<void>(CLASSID) {}
  int z = 12;
};
} // namespace

TEST(CastTest, MultipleDerivedTypes) {
  X xv;
  Y yv;
  Z zv;
  es::Value<void>& xr = xv;
  es::Value<void>& yr = yv;
  es::Value<void>& zr = zv;

  EXPECT_TRUE(es::isa<X>(xr));
  EXPECT_FALSE(es::isa<X>(yr));
  EXPECT_FALSE(es::isa<X>(zr));

  EXPECT_FALSE(es::isa<Y>(xr));
  EXPECT_TRUE(es::isa<Y>(yr));
  EXPECT_FALSE(es::isa<Y>(zr));

  EXPECT_FALSE(es::isa<Z>(xr));
  EXPECT_FALSE(es::isa<Z>(yr));
  EXPECT_TRUE(es::isa<Z>(zr));

  EXPECT_EQ(es::cast<X>(xr).x, 10);
  EXPECT_EQ(es::cast<Y>(yr).y, 11);
  EXPECT_EQ(es::cast<Z>(zr).z, 12);

  EXPECT_NE(es::dyn_cast<X>(&xr), nullptr);
  EXPECT_EQ(es::dyn_cast<X>(&yr), nullptr);
  EXPECT_EQ(es::dyn_cast<X>(&zr), nullptr);

  EXPECT_EQ(es::dyn_cast<Y>(&xr), nullptr);
  EXPECT_NE(es::dyn_cast<Y>(&yr), nullptr);
  EXPECT_EQ(es::dyn_cast<Y>(&zr), nullptr);

  EXPECT_EQ(es::dyn_cast<Z>(&xr), nullptr);
  EXPECT_EQ(es::dyn_cast<Z>(&yr), nullptr);
  EXPECT_NE(es::dyn_cast<Z>(&zr), nullptr);
}

TEST(CastTest, TypeIdUniqueness) {
  struct Foo {};
  struct Bar {};
  struct Baz {};
  EXPECT_NE(es::type_id<Foo>(), es::type_id<Bar>());
  EXPECT_NE(es::type_id<Foo>(), es::type_id<Baz>());
  EXPECT_NE(es::type_id<Bar>(), es::type_id<Baz>());
  EXPECT_EQ(es::type_id<Foo>(), es::type_id<Foo>());
}
