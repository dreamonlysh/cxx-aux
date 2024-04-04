#include <estd/bit.h>
#include <gtest/gtest.h>

TEST(BitTest, count_bit1) {
  ASSERT_EQ(es::count_bit1<uint8_t>(0), 0);
  ASSERT_EQ(es::count_bit1<uint8_t>(1), 1);
  ASSERT_EQ(es::count_bit1<uint8_t>(0x70), 3);
  ASSERT_EQ(es::count_bit1<uint8_t>(UINT8_MAX), 8);

  ASSERT_EQ(es::count_bit1<uint16_t>(0), 0);
  ASSERT_EQ(es::count_bit1<uint16_t>(1), 1);
  ASSERT_EQ(es::count_bit1<uint16_t>(0x70), 3);
  ASSERT_EQ(es::count_bit1<uint16_t>(UINT16_MAX), 16);

  ASSERT_EQ(es::count_bit1(0u), 0);
  ASSERT_EQ(es::count_bit1(1u), 1);
  ASSERT_EQ(es::count_bit1(0x70u), 3);
  ASSERT_EQ(es::count_bit1(UINT32_MAX), 32);

  ASSERT_EQ(es::count_bit1(0ull), 0);
  ASSERT_EQ(es::count_bit1(1ull), 1);
  ASSERT_EQ(es::count_bit1(0x70ull), 3);
  ASSERT_EQ(es::count_bit1(UINT64_MAX), 64);
}

TEST(BitTest, count_bit0) {
  ASSERT_EQ(es::count_bit0<uint8_t>(0), 8);
  ASSERT_EQ(es::count_bit0<uint8_t>(1), 7);
  ASSERT_EQ(es::count_bit0<uint8_t>(0x70), 5);
  ASSERT_EQ(es::count_bit0<uint8_t>(UINT8_MAX), 0);

  ASSERT_EQ(es::count_bit0<uint16_t>(0), 16);
  ASSERT_EQ(es::count_bit0<uint16_t>(1), 15);
  ASSERT_EQ(es::count_bit0<uint16_t>(0x70), 13);
  ASSERT_EQ(es::count_bit0<uint16_t>(UINT16_MAX), 0);

  ASSERT_EQ(es::count_bit0(0u), 32);
  ASSERT_EQ(es::count_bit0(1u), 31);
  ASSERT_EQ(es::count_bit0(0x70u), 29);
  ASSERT_EQ(es::count_bit0(UINT32_MAX), 0);

  ASSERT_EQ(es::count_bit0(0ull), 64);
  ASSERT_EQ(es::count_bit0(1ull), 63);
  ASSERT_EQ(es::count_bit0(0x70ull), 61);
  ASSERT_EQ(es::count_bit0(UINT64_MAX), 0);
}

TEST(BitTest, countl_bit0) {
  ASSERT_EQ(es::countl_bit0<uint8_t>(0), 8);
  ASSERT_EQ(es::countl_bit0<uint8_t>(1), 7);
  ASSERT_EQ(es::countl_bit0<uint8_t>(0x70), 1);
  ASSERT_EQ(es::countl_bit0<uint8_t>(UINT8_MAX), 0);

  ASSERT_EQ(es::countl_bit0<uint16_t>(0), 16);
  ASSERT_EQ(es::countl_bit0<uint16_t>(1), 15);
  ASSERT_EQ(es::countl_bit0<uint16_t>(0x70), 9);
  ASSERT_EQ(es::countl_bit0<uint16_t>(UINT16_MAX), 0);

  ASSERT_EQ(es::countl_bit0(0u), 32);
  ASSERT_EQ(es::countl_bit0(1u), 31);
  ASSERT_EQ(es::countl_bit0(0x70u), 25);
  ASSERT_EQ(es::countl_bit0(UINT32_MAX), 0);

  ASSERT_EQ(es::countl_bit0(0ull), 64);
  ASSERT_EQ(es::countl_bit0(1ull), 63);
  ASSERT_EQ(es::countl_bit0(0x70ull), 57);
  ASSERT_EQ(es::countl_bit0(UINT64_MAX), 0);
}

TEST(BitTest, countl_bit1) {
  ASSERT_EQ(es::countl_bit1<uint8_t>(0), 0);
  ASSERT_EQ(es::countl_bit1<uint8_t>(0x80), 1);
  ASSERT_EQ(es::countl_bit1<uint8_t>(0xf0), 4);
  ASSERT_EQ(es::countl_bit1<uint8_t>(0x70), 0);
  ASSERT_EQ(es::countl_bit1<uint8_t>(UINT8_MAX), 8);

  ASSERT_EQ(es::countl_bit1<uint16_t>(0), 0);
  ASSERT_EQ(es::countl_bit1<uint16_t>(0x8000u), 1);
  ASSERT_EQ(es::countl_bit1<uint16_t>(0xf000u), 4);
  ASSERT_EQ(es::countl_bit1<uint16_t>(0x7000u), 0);
  ASSERT_EQ(es::countl_bit1<uint16_t>(UINT16_MAX), 16);

  ASSERT_EQ(es::countl_bit1(0u), 0);
  ASSERT_EQ(es::countl_bit1(0x8000'0000u), 1);
  ASSERT_EQ(es::countl_bit1(0xf000'0000u), 4);
  ASSERT_EQ(es::countl_bit1(0x7000'0000u), 0);
  ASSERT_EQ(es::countl_bit1(UINT32_MAX), 32);

  ASSERT_EQ(es::countl_bit1(0ull), 0);
  ASSERT_EQ(es::countl_bit1(0x8000'0000'0000'0000ull), 1);
  ASSERT_EQ(es::countl_bit1(0xf000'0000'0000'0000ull), 4);
  ASSERT_EQ(es::countl_bit1(0x7000'0000'0000'0000ull), 0);
  ASSERT_EQ(es::countl_bit1(UINT64_MAX), 64);
}

TEST(BitTest, countr_bit0) {
  ASSERT_EQ(es::countr_bit0<uint8_t>(0), 8);
  ASSERT_EQ(es::countr_bit0<uint8_t>(0x80), 7);
  ASSERT_EQ(es::countr_bit0<uint8_t>(0x0E), 1);
  ASSERT_EQ(es::countr_bit0<uint8_t>(UINT8_MAX), 0);

  ASSERT_EQ(es::countr_bit0<uint16_t>(0), 16);
  ASSERT_EQ(es::countr_bit0<uint16_t>(0x8000), 15);
  ASSERT_EQ(es::countr_bit0<uint16_t>(0x000E), 1);
  ASSERT_EQ(es::countr_bit0<uint16_t>(UINT16_MAX), 0);

  ASSERT_EQ(es::countr_bit0(0u), 32);
  ASSERT_EQ(es::countr_bit0(0x8000'0000u), 31);
  ASSERT_EQ(es::countr_bit0(0x0000'000Eu), 1);
  ASSERT_EQ(es::countr_bit0(UINT32_MAX), 0);

  ASSERT_EQ(es::countr_bit0(0ull), 64);
  ASSERT_EQ(es::countr_bit0(0x8000'0000'0000'0000ull), 63);
  ASSERT_EQ(es::countr_bit0(0x0000'0000'0000'000Eull), 1);
  ASSERT_EQ(es::countr_bit0(UINT64_MAX), 0);
}

TEST(BitTest, countr_bit1) {
  ASSERT_EQ(es::countr_bit1<uint8_t>(0), 0);
  ASSERT_EQ(es::countr_bit1<uint8_t>(0x1), 1);
  ASSERT_EQ(es::countr_bit1<uint8_t>(0xf), 4);
  ASSERT_EQ(es::countr_bit1<uint8_t>(0xe), 0);
  ASSERT_EQ(es::countr_bit1<uint8_t>(UINT8_MAX), 8);

  ASSERT_EQ(es::countr_bit1<uint16_t>(0), 0);
  ASSERT_EQ(es::countr_bit1<uint16_t>(0x1u), 1);
  ASSERT_EQ(es::countr_bit1<uint16_t>(0xfu), 4);
  ASSERT_EQ(es::countr_bit1<uint16_t>(0xeu), 0);
  ASSERT_EQ(es::countr_bit1<uint16_t>(UINT16_MAX), 16);

  ASSERT_EQ(es::countr_bit1(0u), 0);
  ASSERT_EQ(es::countr_bit1(0x1u), 1);
  ASSERT_EQ(es::countr_bit1(0xfu), 4);
  ASSERT_EQ(es::countr_bit1(0xEu), 0);
  ASSERT_EQ(es::countr_bit1(UINT32_MAX), 32);

  ASSERT_EQ(es::countr_bit1(0ull), 0);
  ASSERT_EQ(es::countr_bit1(0x1ull), 1);
  ASSERT_EQ(es::countr_bit1(0xfull), 4);
  ASSERT_EQ(es::countr_bit1(0xeull), 0);
  ASSERT_EQ(es::countr_bit1(UINT64_MAX), 64);
}
