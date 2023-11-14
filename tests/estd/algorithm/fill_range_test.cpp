#include <gtest/gtest.h>

#include <array>
#include <climits>
#include <estd/algorithm/fill.h>

using namespace es;
constexpr auto chars = partial_initializer<std::array<bool, CHAR_MAX>>(
    true, char_enumerate_slice{'_', '$', ';', '/', '|', '.', '?', '@'},
    char_slice_number, char_slice_lower_alphabet, char_slice_upper_alphabet);

TEST(fill_range, partial_initializer) {
  std::string rst;
  for (size_t i = 0; i < CHAR_MAX; ++i) {
    if (chars[i])
      rst.push_back(i);
  }
  ASSERT_EQ(rst, "$./"
                 "0123456789"
                 ";?@"
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "_"
                 "abcdefghijklmnopqrstuvwxyz"
                 "|");
}