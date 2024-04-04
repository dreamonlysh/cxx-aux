#include <estd/bit.h>
#include <gtest/gtest.h>

TEST(BitTest, is_pow2) {
  ASSERT_TRUE(es::is_pow2(0u));
  ASSERT_TRUE(es::is_pow2(1u));
  ASSERT_TRUE(es::is_pow2(0x8000'0000u));
  ASSERT_FALSE(es::is_pow2(0x8000'0001u));
  ASSERT_FALSE(es::is_pow2(UINT32_MAX));
}

TEST(BitTest, is_pow2_sub1) {
  ASSERT_TRUE(es::is_pow2_sub1(0u));
  ASSERT_TRUE(es::is_pow2_sub1(1u));
  ASSERT_TRUE(es::is_pow2_sub1(0xffu));
  ASSERT_FALSE(es::is_pow2_sub1(0xff0u));
  ASSERT_FALSE(es::is_pow2_sub1(0x8000'0000u));
  ASSERT_FALSE(es::is_pow2_sub1(0x8000'0001u));
  ASSERT_TRUE(es::is_pow2_sub1(UINT32_MAX));
}

TEST(BitTest, is_pow2_sub_pow2) {
  ASSERT_TRUE(es::is_pow2_sub_pow2(0u));
  ASSERT_TRUE(es::is_pow2_sub_pow2(1u));
  ASSERT_TRUE(es::is_pow2_sub_pow2(0xffu));
  ASSERT_TRUE(es::is_pow2_sub_pow2(0xff0u));
  ASSERT_TRUE(es::is_pow2_sub_pow2(0x8000'0000u));
  ASSERT_FALSE(es::is_pow2_sub_pow2(0x8000'0001u));
  ASSERT_TRUE(es::is_pow2_sub_pow2(UINT32_MAX));
}

TEST(BitTest, floor_to_pow2) {
  ASSERT_EQ(es::floor_to_pow2(0u), 0);
  ASSERT_EQ(es::floor_to_pow2(1u), 1);
  ASSERT_EQ(es::floor_to_pow2(0xffu), 0x80);
  ASSERT_EQ(es::floor_to_pow2(0xff0u), 0x800);
  ASSERT_EQ(es::floor_to_pow2(0x8000'0000u), 0x8000'0000);
  ASSERT_EQ(es::floor_to_pow2(0x8000'0001u), 0x8000'0000);
  ASSERT_EQ(es::floor_to_pow2(UINT32_MAX), 0x8000'0000);
}

TEST(BitTest, ceil_to_pow2) {
  ASSERT_EQ(es::ceil_to_pow2(0u), 0);
  ASSERT_EQ(es::ceil_to_pow2(1u), 1);
  ASSERT_EQ(es::ceil_to_pow2(0xffu), 0x100);
  ASSERT_EQ(es::ceil_to_pow2(0xff0u), 0x1000);
  ASSERT_EQ(es::ceil_to_pow2(0x8000'0000u), 0x8000'0000);
  ASSERT_EQ(es::ceil_to_pow2(0x8000'0001u), 0);
  ASSERT_EQ(es::ceil_to_pow2(UINT32_MAX), 0);
  ASSERT_EQ(es::ceil_to_pow2(UINT64_MAX), 0);
  ASSERT_EQ(es::ceil_to_pow2<uint16_t>(UINT16_MAX), 0);
}
