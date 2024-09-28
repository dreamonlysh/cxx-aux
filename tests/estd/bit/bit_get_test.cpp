#include <estd/bit.h>
#include <gtest/gtest.h>

TEST(BitTest, get_bit) {
  ASSERT_EQ(es::get_bit(0xf0f0'f0f0u, 0), 0);
  ASSERT_EQ(es::get_bit(0xf0f0'f0f0u, 4), 1);
  ASSERT_EQ(es::get_bit(0xf0f0'f0f0u, 31), 1);

  ASSERT_EQ(es::get_bit(0x0f0f'0f0fu, 0), 1);
  ASSERT_EQ(es::get_bit(0x0f0f'0f0fu, 4), 0);
  ASSERT_EQ(es::get_bit(0x0f0f'0f0fu, 31), 0);
}

TEST(BitTest, get_bits) {
  ASSERT_EQ(es::get_bits(0xf0f0'f0f0u, 0, 1), 0);
  ASSERT_EQ(es::get_bits(0xf0f0'f0f0u, 3, 3), 0b110);
  ASSERT_EQ(es::get_bits(0xf0f0'f0f0u, 31, 1), 1);
  ASSERT_EQ(es::get_bits(0xf0f0'f0f0u, 26, 3), 0b100);
  ASSERT_EQ(es::get_bits(0xf0f0'f0f0u, 0, 32), 0xf0f0'f0f0u);

  ASSERT_EQ(es::get_bits(0x0f0f'0f0fu, 0, 1), 1);
  ASSERT_EQ(es::get_bits(0x0f0f'0f0fu, 3, 3), 0b001);
  ASSERT_EQ(es::get_bits(0x0f0f'0f0fu, 31, 1), 0);
  ASSERT_EQ(es::get_bits(0x0f0f'0f0fu, 26, 3), 0b011);
  ASSERT_EQ(es::get_bits(0x0f0f'0f0fu, 0, 32), 0x0f0f'0f0fu);
}
