#include <gtest/gtest.h>
#include <estd/dl/dynamic_library.h>
#include <estd/meta/conditional.h>
#include <estd/meta/system.h>

using namespace estd;

TEST(dl, loadTest) {
  std::string file = "../lib/";
  file += conditional_v<is_win_v, is_cygwin_v>(
    "libdl_sum.dll", "cygdl_sum.dll", "libdl_sum.so");
  std::optional<DynamicLibrary> optDL = loadDynamicLibrary(file.c_str());
  ASSERT_TRUE(optDL.has_value());
  DynamicLibrary& dl = optDL.value();

  std::optional<int> rst = dl.invoke<int>("dl_sum", 1, 2);
  ASSERT_TRUE(rst.has_value());
  ASSERT_EQ(rst.value(), 3);
}

TEST(al, loadTest) {
  std::optional<DynamicLibrary> optDL = loadDynamicLibrary(nullptr);
  ASSERT_TRUE(optDL.has_value());
  DynamicLibrary& dl = optDL.value();

  std::optional<int> rst = dl.invoke<int>("al_sum", 1, 2);
  // Due the ld will remove the unused symbols, so al_sum will not been found.
  ASSERT_FALSE(rst.has_value());
}
