#include <estd/cast/type_id.h>
#include <gtest/gtest.h>

struct TypeA {};
struct TypeB {};
class TypeC {};

TEST(TypeId, DifferentTypesHaveDifferentIds) {
  size_t id_a = es::type_id<TypeA>();
  size_t id_b = es::type_id<TypeB>();

  EXPECT_NE(id_a, id_b);
}

TEST(TypeId, SameTypeHasSameId) {
  size_t id_a1 = es::type_id<TypeA>();
  size_t id_a2 = es::type_id<TypeA>();

  EXPECT_EQ(id_a1, id_a2);
}

TEST(TypeId, IdIsPointerBased) {
  size_t id_a = es::type_id<TypeA>();
  size_t id_b = es::type_id<TypeB>();

  EXPECT_GT(id_a, 0);
  EXPECT_GT(id_b, 0);
}

TEST(TypeId, WorksWithClassType) {
  size_t id = es::type_id<TypeC>();

  EXPECT_GT(id, 0);
}

TEST(TypeId, WorksWithConstRef) {
  size_t id_a = es::type_id<TypeA>();
  const TypeA a;
  size_t id_from_ref = es::type_id(a);

  EXPECT_EQ(id_a, id_from_ref);
}

TEST(TypeId, WorksWithRvalueRef) {
  size_t id_a = es::type_id<TypeA>();
  size_t id_from_rvalue = es::type_id(TypeA{});

  EXPECT_EQ(id_a, id_from_rvalue);
}
