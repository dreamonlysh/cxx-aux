#include <estd/integer.h>
#include <gtest/gtest.h>

class IndexTag;
using Index = es::Integer<int32_t, IndexTag>;

TEST(IntegerTest, Construct) {
  Index a;
  ASSERT_EQ(a.value(), 0);
  Index b(10);
  ASSERT_EQ(b.value(), 10);
  ASSERT_EQ(Index(b).value(), 10);
  ASSERT_EQ(Index(std::move(b)).value(), 10);
}

TEST(IntegerTest, OperatorAssign) {
  Index c(11);
  Index a = c;
  ASSERT_EQ(a.value(), 11);
  Index b = std::move(c);
  ASSERT_EQ(b.value(), 11);
}

TEST(IntegerTest, Reset) {
  Index a(20);
  a.reset(8);
  ASSERT_EQ(a.value(), 8);
}

TEST(IntegerTest, Swap) {
  Index a(20);
  a.reset(8);
  ASSERT_EQ(a.value(), 8);
}

TEST(IntegerTest, Cast) {
  Index a(20);
  ASSERT_EQ(static_cast<int32_t>(a), 20);
}

TEST(IntegerTest, AssignmentOperators) {
  Index a(20);

  a += 1;
  ASSERT_EQ(a.value(), 21);
  a += Index(1);
  ASSERT_EQ(a.value(), 22);
  a -= 1;
  ASSERT_EQ(a.value(), 21);
  a -= Index(1);
  ASSERT_EQ(a.value(), 20);
  a *= 2;
  ASSERT_EQ(a.value(), 40);
  a *= Index(2);
  ASSERT_EQ(a.value(), 80);
  a /= 2;
  ASSERT_EQ(a.value(), 40);
  a /= Index(2);
  ASSERT_EQ(a.value(), 20);
  a %= 7;
  ASSERT_EQ(a.value(), 6);
  a %= Index(4);
  ASSERT_EQ(a.value(), 2);
  a &= 0xf;
  ASSERT_EQ(a.value(), 0x2);
  a &= Index(0xf);
  ASSERT_EQ(a.value(), 0x2);
  a |= 0x10;
  ASSERT_EQ(a.value(), 0x12);
  a |= Index(0x20);
  ASSERT_EQ(a.value(), 0x32);
  a ^= 0x10;
  ASSERT_EQ(a.value(), 0x22);
  a ^= Index(0x20);
  ASSERT_EQ(a.value(), 2);
  a <<= 1;
  ASSERT_EQ(a.value(), 4);
  a <<= Index(1);
  ASSERT_EQ(a.value(), 8);
  a >>= 1;
  ASSERT_EQ(a.value(), 4);
  a >>= Index(1);
  ASSERT_EQ(a.value(), 2);
}

TEST(IntegerTest, IncdecOperators) {
  Index a(20);

  ASSERT_EQ((++a).value(), 21);
  ASSERT_EQ((--a).value(), 20);
  ASSERT_EQ((a++).value(), 20);
  ASSERT_EQ(a.value(), 21);
  ASSERT_EQ((a--).value(), 21);
  ASSERT_EQ(a.value(), 20);
}

TEST(IntegerTest, ComparisonOperators) {
  Index a(20);
  Index b(16);

  ASSERT_TRUE(a == a);
  ASSERT_TRUE(a == 20);
  ASSERT_TRUE(20 == a);
  ASSERT_FALSE(a == b);
  ASSERT_FALSE(a == 16);
  ASSERT_FALSE(20 == b);

  ASSERT_FALSE(a != a);
  ASSERT_FALSE(a != 20);
  ASSERT_FALSE(20 != a);
  ASSERT_TRUE(a != b);
  ASSERT_TRUE(a != 16);
  ASSERT_TRUE(20 != b);

  ASSERT_FALSE(a > a);
  ASSERT_FALSE(a > 20);
  ASSERT_FALSE(20 > a);
  ASSERT_TRUE(a > b);
  ASSERT_TRUE(a > 16);
  ASSERT_TRUE(20 > b);
  ASSERT_FALSE(b > a);
  ASSERT_FALSE(b > 20);
  ASSERT_FALSE(16 > a);

  ASSERT_FALSE(a < a);
  ASSERT_FALSE(a < 20);
  ASSERT_FALSE(20 < a);
  ASSERT_FALSE(a < b);
  ASSERT_FALSE(a < 16);
  ASSERT_FALSE(20 < b);
  ASSERT_TRUE(b < a);
  ASSERT_TRUE(b < 20);
  ASSERT_TRUE(16 < a);

  ASSERT_TRUE(a >= a);
  ASSERT_TRUE(a >= 20);
  ASSERT_TRUE(20 >= a);
  ASSERT_TRUE(a >= b);
  ASSERT_TRUE(a >= 16);
  ASSERT_TRUE(20 >= b);
  ASSERT_FALSE(b >= a);
  ASSERT_FALSE(b >= 20);
  ASSERT_FALSE(16 >= a);

  ASSERT_TRUE(a <= a);
  ASSERT_TRUE(a <= 20);
  ASSERT_TRUE(20 <= a);
  ASSERT_FALSE(a <= b);
  ASSERT_FALSE(a <= 16);
  ASSERT_FALSE(20 <= b);
  ASSERT_TRUE(b <= a);
  ASSERT_TRUE(b <= 20);
  ASSERT_TRUE(16 <= a);
}

TEST(IntegerTest, ArithmeticOperators) {
  Index a(20);
  Index b(16);

  ASSERT_EQ((+a).value(), 20);
  ASSERT_EQ((-a).value(), -20);
  ASSERT_EQ((a + b).value(), 36);
  ASSERT_EQ((a + 16).value(), 36);
  ASSERT_EQ((20 + b).value(), 36);
  ASSERT_EQ((a - b).value(), 4);
  ASSERT_EQ((a - 16).value(), 4);
  ASSERT_EQ((20 - b).value(), 4);
  ASSERT_EQ((a * b).value(), 320);
  ASSERT_EQ((a * 16).value(), 320);
  ASSERT_EQ((20 * b).value(), 320);
  ASSERT_EQ((a / b).value(), 1);
  ASSERT_EQ((a / 16).value(), 1);
  ASSERT_EQ((20 / b).value(), 1);
  ASSERT_EQ((a % b).value(), 4);
  ASSERT_EQ((a % 16).value(), 4);
  ASSERT_EQ((20 % b).value(), 4);
  ASSERT_EQ((~a).value(), static_cast<int>(0xffff'fffeb));
  b.reset(7);
  ASSERT_EQ((a & b).value(), 4);
  ASSERT_EQ((a & 7).value(), 4);
  ASSERT_EQ((20 & b).value(), 4);
  ASSERT_EQ((a | b).value(), 23);
  ASSERT_EQ((a | 7).value(), 23);
  ASSERT_EQ((20 | b).value(), 23);
  ASSERT_EQ((a ^ b).value(), 19);
  ASSERT_EQ((a ^ 7).value(), 19);
  ASSERT_EQ((20 ^ b).value(), 19);
  b.reset(1);
  ASSERT_EQ((a << b).value(), 40);
  ASSERT_EQ((a << 1).value(), 40);
  ASSERT_EQ(20 << b, 40);
  ASSERT_EQ((a >> b).value(), 10);
  ASSERT_EQ((a >> 1).value(), 10);
  ASSERT_EQ(20 >> b, 10);
}

TEST(IntegerTest, LogicOperators) {
  Index a(20);
  Index b(16);

  ASSERT_FALSE(!a);
  ASSERT_TRUE(!Index(0));
}

TEST(IntegerTest, ExplicitConversion) {
  Index a(42);
  EXPECT_EQ(static_cast<int32_t>(a), 42);
  static_assert(!std::is_convertible_v<Index, int32_t>);
}

TEST(IntegerTest, UnitTypes) {
  using namespace es::units;
  Bit bits = 32_b;
  EXPECT_EQ(bits.value(), 32u);
  Byte bytes = 512_B;
  EXPECT_EQ(bytes.value(), 512u);
  KiloByte kb = 4_KB;
  EXPECT_EQ(kb.value(), 4u);
  EXPECT_EQ((bits + 8_b).value(), 40u);
  EXPECT_EQ((bytes + 128_B).value(), 640u);
  EXPECT_EQ((kb + 1_KB).value(), 5u);
}

TEST(IntegerTest, DifferentTags) {
  struct TagA;
  struct TagB;
  using TypeA = es::Integer<int, TagA>;
  using TypeB = es::Integer<int, TagB>;
  static_assert(!std::is_same_v<TypeA, TypeB>);
  TypeA a(10);
  TypeB b(10);
  EXPECT_EQ(a.value(), b.value());
}
