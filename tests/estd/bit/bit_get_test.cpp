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

TEST(BitTest, get_nth_bit) {
  // 0b10101010 has set bits at positions 1, 3, 5, 7
  ASSERT_EQ(es::get_nth_bit(0b10101010u, 0), 0b00000010u);
  ASSERT_EQ(es::get_nth_bit(0b10101010u, 1), 0b00001000u);
  ASSERT_EQ(es::get_nth_bit(0b10101010u, 2), 0b00100000u);
  ASSERT_EQ(es::get_nth_bit(0b10101010u, 3), 0b10000000u);
  ASSERT_EQ(es::get_nth_bit(0b10101010u, 4), 0u);

  // Single bit set
  ASSERT_EQ(es::get_nth_bit(0b00010000u, 0), 0b00010000u);
  ASSERT_EQ(es::get_nth_bit(0b00010000u, 1), 0u);

  // All bits set
  ASSERT_EQ(es::get_nth_bit(0xFFu, 0), 0b00000001u);
  ASSERT_EQ(es::get_nth_bit(0xFFu, 3), 0b00001000u);
  ASSERT_EQ(es::get_nth_bit(0xFFu, 7), 0b10000000u);
  ASSERT_EQ(es::get_nth_bit(0xFFu, 8), 0u);

  // No bits set
  ASSERT_EQ(es::get_nth_bit(0u, 0), 0u);

  // 64-bit values
  ASSERT_EQ(es::get_nth_bit(0xAAAAAAAAAAAAAAAAull, 0), 0b10ull);
  ASSERT_EQ(es::get_nth_bit(0xAAAAAAAAAAAAAAAAull, 31), 1ull << 63);
  ASSERT_EQ(es::get_nth_bit(0xAAAAAAAAAAAAAAAAull, 32), 0ull);
}

TEST(BitTest, get_bit_Position0AndMax) {
  ASSERT_EQ(es::get_bit(1u, 0), 1);
  ASSERT_EQ(es::get_bit(0xFFFFFFFEu, 0), 0);
  ASSERT_EQ(es::get_bit(0x80000000u, 31), 1);
  ASSERT_EQ(es::get_bit(0x7FFFFFFFu, 31), 0);
}

TEST(BitTest, get_bits_VariousRanges) {
  ASSERT_EQ(es::get_bits(0xA5A5A5A5u, 0, 8), 0xA5u);
  ASSERT_EQ(es::get_bits(0xA5A5A5A5u, 8, 8), 0xA5u);
  ASSERT_EQ(es::get_bits(0xA5A5A5A5u, 4, 4), 0xAu);
  ASSERT_EQ(es::get_bits(0x12345678u, 0, 4), 0x8u);
  ASSERT_EQ(es::get_bits(0x12345678u, 4, 4), 0x7u);
  ASSERT_EQ(es::get_bits(0x12345678u, 28, 4), 0x1u);
}

TEST(BitTest, get_nth_bit_EdgeCases) {
  ASSERT_EQ(es::get_nth_bit(1u, 0), 1u);
  ASSERT_EQ(es::get_nth_bit(1u, 1), 0u);
  ASSERT_EQ(es::get_nth_bit(0u, 0), 0u);
  ASSERT_EQ(es::get_nth_bit(UINT32_MAX, 0), 1u);
  ASSERT_EQ(es::get_nth_bit(UINT32_MAX, 31), 0x80000000u);
  ASSERT_EQ(es::get_nth_bit(UINT32_MAX, 32), 0u);
}
