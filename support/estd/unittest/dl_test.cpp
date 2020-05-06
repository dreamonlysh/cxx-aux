#include <gtest/gtest.h>
#include "estd/dl/dynamic_library.h"

TEST(dl, loadTest) {
  using namespace estd;
  std::string file = "../lib/libdl_test.so";
  std::optional<DynamicLibrary> optDL = loadDynamicLibrary(file.c_str());
  ASSERT_TRUE(optDL.has_value());
  DynamicLibrary& dl = optDL.value();

  std::optional<int> rst = dl.invoke<int>("sum", 1, 2);
  ASSERT_TRUE(rst.has_value());
  ASSERT_EQ(rst.value(), 3);
}
