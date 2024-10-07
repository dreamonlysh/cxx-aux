#include <cstdint>
#include <estd/__utility/bytes_of.h>
#include <gtest/gtest.h>

namespace {
struct A {
  uint16_t a;
  uint32_t b;
};
} // namespace

TEST(BytesOf, General) {
  ASSERT_EQ(es::bytes_of<bool>, 1);
  ASSERT_EQ(es::bytes_of<uint32_t>, 4);

  ASSERT_EQ(es::bytes_of<A>, 8);
}

TEST(BytesOf, Array) {
  ASSERT_EQ(es::bytes_of<bool[2]>, 2);
  ASSERT_EQ(es::bytes_of<uint32_t[4]>, 4 * 4);

  ASSERT_EQ(es::bytes_of<A[3]>, sizeof(A) * 3);
}
