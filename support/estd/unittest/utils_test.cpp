#include <gtest/gtest.h>

#include <estd/meta/range.h>
#include <estd/meta/fill.h>
#include <array>
#include <climits>
#include <iostream>
#include <any>

constexpr auto chars = estd::meta::partial_initializer<std::array<bool, CHAR_MAX>>(
    true,
    estd::meta::char_enumerate_slice{'_', '$', ';', '/', '|', '.', '?', '@'},
    estd::meta::char_slice_number,
    estd::meta::char_slice_lower_alphabet,
    estd::meta::char_slice_upper_alphabet);

TEST(XXX, XXX) {
  for (size_t i = 0; i < CHAR_MAX; ++i) {
    if (chars[i]) {
      std::cout << (char)i << " ";
    }
  }
  std::cout << std::endl;
  ASSERT_TRUE(chars['a']);
}