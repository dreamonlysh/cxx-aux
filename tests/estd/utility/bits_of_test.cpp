#include <cstdint>
#include <estd/__utility/bits_of.h>
#include <gtest/gtest.h>

namespace {
struct A {
  uint16_t a;
  uint32_t b;
};
} // namespace

TEST(BitsOf, General) {
  ASSERT_EQ(es::bits_of<bool>, 8);
  ASSERT_EQ(es::bits_of<uint32_t>, 32);

  ASSERT_EQ(es::bits_of<A>, 64);
}

TEST(BitsOf, Array) {
  ASSERT_EQ(es::bits_of<bool[2]>, 8 * 2);
  ASSERT_EQ(es::bits_of<uint32_t[4]>, 32 * 4);

  ASSERT_EQ(es::bits_of<A[3]>, sizeof(A) * 8 * 3);
}
