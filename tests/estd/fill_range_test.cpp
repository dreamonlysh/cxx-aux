#include <gtest/gtest.h>

#include <estd/meta/range.h>
#include <estd/meta/fill.h>
#include <array>
#include <climits>
#include <iostream>
#include <any>

using namespace es::meta;
constexpr auto chars = partial_initializer<std::array<bool, CHAR_MAX>>(
  true,
  es::meta::char_enumerate_slice{'_', '$', ';', '/', '|', '.', '?', '@'},
  es::meta::char_slice_number,
  es::meta::char_slice_lower_alphabet,
  es::meta::char_slice_upper_alphabet);

TEST(fill_range, partial_initializer) {
  std::string rst;
  for (size_t i = 0; i < CHAR_MAX; ++i) {
    if (chars[i]) rst.push_back(i);
  }
  ASSERT_EQ(rst, "$./"
                 "0123456789"
                 ";?@"
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "_"
                 "abcdefghijklmnopqrstuvwxyz"
                 "|");
}