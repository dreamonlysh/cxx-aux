#include <gtest/gtest.h>
#include <estd/dl/dynamic_library.h>
#include <estd/meta/system.h>

using namespace es;

TEST(dl, loadTest) {
  auto file = []() {
    if constexpr (is_win_v) return "../lib/" "libdl_sum.dll";
    else if (is_cygwin_v)   return "../lib/" "cygdl_sum.dll";
    else                    return "../lib/" "libdl_sum.so";
  };
  std::optional<DynamicLibrary> optDL = loadDynamicLibrary(file());
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
