#include <estd/string/string_traits.h>
#include <gtest/gtest.h>
#include <string>

TEST(StringTraits, IsCString) {
  const char* cstr = "hello";
  std::string str = "world";

  EXPECT_TRUE(es::string::is_c_string_v<const char*>);
  EXPECT_TRUE(es::string::is_c_string_v<const char*>);
  EXPECT_TRUE(es::string::is_c_string_v<char*>);
  EXPECT_FALSE(es::string::is_c_string_v<std::string>);
  EXPECT_FALSE(es::string::is_c_string_v<int>);
}

TEST(StringTraits, IsCStringWithCharT) {
  const char* cstr = "hello";
  const wchar_t* wcstr = L"hello";

  EXPECT_TRUE((es::string::is_c_string_v<const char*, char>));
  EXPECT_TRUE((es::string::is_c_string_v<const wchar_t*, wchar_t>));
  EXPECT_FALSE((es::string::is_c_string_v<const char*, wchar_t>));
  EXPECT_FALSE((es::string::is_c_string_v<std::string, char>));
}
