#include <estd/bit.h>
#include <gtest/gtest.h>

TEST(BitTest, hl_first_bit0) {
  ASSERT_EQ(es::hl_first_bit0(0u), 1);
  ASSERT_EQ(es::hl_first_bit0(0x0fu), 0x10);
  ASSERT_EQ(es::hl_first_bit0(UINT32_MAX), 0);
}

TEST(BitTest, hl_first_bit1) {
  ASSERT_EQ(es::hl_first_bit1(0u), 0);
  ASSERT_EQ(es::hl_first_bit1(1u), 1);
  ASSERT_EQ(es::hl_first_bit1(0xf0u), 0x10);
  ASSERT_EQ(es::hl_first_bit1(UINT32_MAX), 1);
}

TEST(BitTest, dim_first_bit1) {
  ASSERT_EQ(es::dim_first_bit1(0u), UINT32_MAX);
  ASSERT_EQ(es::dim_first_bit1(1u), 0xffff'fffe);
  ASSERT_EQ(es::dim_first_bit1(0xf0u), 0xffff'ffef);
  ASSERT_EQ(es::dim_first_bit1(UINT32_MAX), 0xffff'fffe);
}

TEST(BitTest, hlr_bit0) {
  ASSERT_EQ(es::hlr_bit0(0u), UINT32_MAX);
  ASSERT_EQ(es::hlr_bit0(1u), 0);
  ASSERT_EQ(es::hlr_bit0(0xf0u), 0xf);
  ASSERT_EQ(es::hlr_bit0(UINT32_MAX), 0);
}

TEST(BitTest, hlr_bit0_inc) {
  ASSERT_EQ(es::hlr_bit0_inc(0u), UINT32_MAX);
  ASSERT_EQ(es::hlr_bit0_inc(1u), 1);
  ASSERT_EQ(es::hlr_bit0_inc(0xf0u), 0x1f);
  ASSERT_EQ(es::hlr_bit0_inc(UINT32_MAX), 1);
}

TEST(BitTest, dimr_bit1) {
  ASSERT_EQ(es::dimr_bit1(0u), UINT32_MAX);
  ASSERT_EQ(es::dimr_bit1(1u), 0xffff'fffe);
  ASSERT_EQ(es::dimr_bit1(0xf0u), UINT32_MAX);
  ASSERT_EQ(es::dimr_bit1(UINT32_MAX), 0);
}

TEST(BitTest, hlr_bit1_inc) {
  ASSERT_EQ(es::hlr_bit1_inc(0u), 1);
  ASSERT_EQ(es::hlr_bit1_inc(1u), 3);
  ASSERT_EQ(es::hlr_bit1_inc(0xf0u), 1);
  ASSERT_EQ(es::hlr_bit1_inc(UINT32_MAX), UINT32_MAX);
}
