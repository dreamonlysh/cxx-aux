#include <gtest/gtest.h>
#include "estd/dl/dynamic_library.h"

using namespace estd;

TEST(dl, loadTest) {
#ifdef __WIN32
  std::string file = "../lib/libdl_sum.dll";
#else
  std::string file = "../lib/libdl_sum.so";
#endif
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
  ASSERT_TRUE(rst.has_value());
  ASSERT_EQ(rst.value(), 3);
}
