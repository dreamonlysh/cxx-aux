#include <estd/dl/dl_load.h>
#include <estd/meta.h>
#include <gtest/gtest.h>

using namespace es;

TEST(dl, load) {
  const char* file;
  if constexpr (is_win_v)
    file = "./dl/libdll0.dll";
  else if constexpr (is_cygwin_v)
    file = "./dl/cygdll0.dll";
  else
    file = "./dl/libdll0.so";
  std::optional<DynamicLinkingLibrary> optDL = dlLoad(file);
  ASSERT_TRUE(optDL.has_value());
  DynamicLinkingLibrary& dl = optDL.value();

  std::optional<int> rst = dl.invoke<int>("sum", 1, 2);
  ASSERT_TRUE(rst.has_value());
  ASSERT_EQ(rst.value(), 3);
}

extern "C" int sum_dumplicate(int lhs, int rhs) { return lhs + rhs; }

TEST(dl, loadNoOverride) {
  const char* file;
  if constexpr (is_win_v)
    file = "./dl/libdll1.dll";
  else if constexpr (is_cygwin_v)
    file = "./dl/cygdll1.dll";
  else
    file = "./dl/libdll1.so";
  std::optional<DynamicLinkingLibrary> optDL = dlLoad(file);
  ASSERT_TRUE(optDL.has_value());
  DynamicLinkingLibrary& dl = optDL.value();

  std::optional<int> rst = dl.invoke<int>("sum_dumplicate", 1, 2);
  ASSERT_TRUE(rst.has_value());
  ASSERT_EQ(rst.value(), 13);
}

#ifdef LM_ID_NEWLM
TEST(dl, loadNewLM) {
  const char* file;
  if constexpr (is_win_v)
    file = "./dl/libdll1.dll";
  else if constexpr (is_cygwin_v)
    file = "./dl/cygdll1.dll";
  else
    file = "./dl/libdll1.so";
  std::optional<DynamicLinkingLibrary> optDL = dlLoadNewLM(file);
  ASSERT_TRUE(optDL.has_value());
  DynamicLinkingLibrary& dl = optDL.value();

  std::optional<int> rst = dl.invoke<int>("sum_dumplicate", 1, 2);
  ASSERT_TRUE(rst.has_value());
  ASSERT_EQ(rst.value(), 13);
}
#endif
