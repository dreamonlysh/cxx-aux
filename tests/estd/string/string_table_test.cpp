#include <estd/string/string_table.h>
#include <gtest/gtest.h>
#include <unordered_set>

static es::string::StringTable<std::unordered_set> g_strTbl;
struct StringTable {
  static std::string_view add(std::string_view s) { return g_strTbl.add(s); }
};

TEST(StringTable, AddSmall) {
  std::string_view hello = StringTable::add("hello");
  std::string_view world = StringTable::add("world");
  ASSERT_EQ(hello, "hello");
  ASSERT_EQ(world, "world");
  std::string_view hello2 = StringTable::add("hello");
  ASSERT_EQ(hello.data(), hello2.data());
}

TEST(StringTable, AddLarge) {
  std::string_view hello = StringTable::add("1234567890abcdef_hello");
  std::string_view world = StringTable::add("1234567890abcdef_world");
  ASSERT_EQ(hello, "1234567890abcdef_hello");
  ASSERT_EQ(world, "1234567890abcdef_world");
  std::string_view hello2 = StringTable::add("1234567890abcdef_hello");
  ASSERT_EQ(hello.data(), hello2.data());
}
