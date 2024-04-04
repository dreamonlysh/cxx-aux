#include <estd/bit.h>
#include <gtest/gtest.h>

TEST(BitTest, set_bit) {
  ASSERT_EQ(es::set_bit(0u, 0), 1);
  ASSERT_EQ(es::set_bit(0u, 1), 0b10);
  ASSERT_EQ(es::set_bit(0u, 31), 0x8000'0000);

  ASSERT_EQ(es::set_bit(0xf0f0'f0f0u, 0), 0xf0f0'f0f1);
  ASSERT_EQ(es::set_bit(0xf0f0'f0f0u, 1), 0xf0f0'f0f2);
  ASSERT_EQ(es::set_bit(0xf0f0'f0f0u, 31), 0xf0f0'f0f0);
}

TEST(BitTest, set_bits) {
  ASSERT_EQ(es::set_bits(0u, 0, 1), 0b1);
  ASSERT_EQ(es::set_bits(0u, 0, 3), 0b111);
  ASSERT_EQ(es::set_bits(0u, 1, 3), 0b1110);
  ASSERT_EQ(es::set_bits(0u, 31, 1), 0x8000'0000);
  ASSERT_EQ(es::set_bits(0u, 1, 31), UINT32_MAX - 1);
  ASSERT_EQ(es::set_bits(0u, 0, 32), UINT32_MAX);

  ASSERT_EQ(es::set_bits(0xf0f0'f0f0u, 0, 1), 0xf0f0'f0f1);
  ASSERT_EQ(es::set_bits(0xf0f0'f0f0u, 0, 3), 0xf0f0'f0f7);
  ASSERT_EQ(es::set_bits(0xf0f0'f0f0u, 1, 3), 0xf0f0'f0fe);
  ASSERT_EQ(es::set_bits(0xf0f0'f0f0u, 31, 1), 0xf0f0'f0f0);
  ASSERT_EQ(es::set_bits(0xf0f0'f0f0u, 1, 31), UINT32_MAX - 1);
  ASSERT_EQ(es::set_bits(0xf0f0'f0f0u, 0, 32), UINT32_MAX);
}

TEST(BitTest, reset_first) {
  ASSERT_EQ(es::reset_first(0u), 0);
  ASSERT_EQ(es::reset_first(1u), 0);
  ASSERT_EQ(es::reset_first(0x8000'0000u), 0);
  ASSERT_EQ(es::reset_first(0b01011000u), 0b01010000);
}

TEST(BitTest, set_first) {
  ASSERT_EQ(es::set_first(0u), 1);
  ASSERT_EQ(es::set_first(1u), 3);
  ASSERT_EQ(es::set_first(0x8000'0000u), 0x8000'0001);
  ASSERT_EQ(es::set_first(0b10100111u), 0b10101111);
}

TEST(BitTest, resetr) {
  ASSERT_EQ(es::resetr(0u), 0);
  ASSERT_EQ(es::resetr(1u), 0);
  ASSERT_EQ(es::resetr(0b10100111u), 0b10100000);
  ASSERT_EQ(es::resetr(0b101001110u), 0b101001110);
  ASSERT_EQ(es::resetr(0xffff'ffffu), 0);
}

TEST(BitTest, setr) {
  ASSERT_EQ(es::setr(0u), UINT32_MAX);
  ASSERT_EQ(es::setr(1u), 1);
  ASSERT_EQ(es::setr(0b10101000u), 0b10101111);
  ASSERT_EQ(es::setr(0b101001111u), 0b101001111);
  ASSERT_EQ(es::setr(0xffff'ffffu), UINT32_MAX);
}

TEST(BitTest, reset_first_adjacent) {
  ASSERT_EQ(es::reset_first_adjacent(0u), 0);
  ASSERT_EQ(es::reset_first_adjacent(1u), 0);
  ASSERT_EQ(es::reset_first_adjacent(0b01011100u), 0b01000000);
  ASSERT_EQ(es::reset_first_adjacent(0b01000100u), 0b01000000);
  ASSERT_EQ(es::reset_first_adjacent(0b101001111u), 0b101000000);
  ASSERT_EQ(es::reset_first_adjacent(0xffff'ffffu), 0);
}