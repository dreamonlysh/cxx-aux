#include <gtest/gtest.h>

#include <estd/meta/range.h>
#include <estd/meta/fill.h>
#include <array>
#include <climits>
#include <iostream>
#include <any>

constexpr auto chars = es::meta::partial_initializer<std::array<bool, CHAR_MAX>>(
  true,
  es::meta::char_enumerate_slice{'_', '$', ';', '/', '|', '.', '?', '@'},
  es::meta::char_slice_number,
  es::meta::char_slice_lower_alphabet,
  es::meta::char_slice_upper_alphabet);

TEST(XXX, XXX) {
  for (size_t i = 0; i < CHAR_MAX; ++i) {
    if (chars[i]) {
      std::cout << (char)i << " ";
    }
  }
  std::cout << std::endl;
  ASSERT_TRUE(chars['a']);
}