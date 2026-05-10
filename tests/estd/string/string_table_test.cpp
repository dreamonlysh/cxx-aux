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

TEST(StringTable, Deduplication) {
  std::string_view s1 = StringTable::add("dedup_key");
  std::string_view s2 = StringTable::add("dedup_key");
  EXPECT_EQ(s1.data(), s2.data());
  EXPECT_EQ(s1, "dedup_key");
}

TEST(StringTable, DifferentStrings) {
  std::string_view s1 = StringTable::add("alpha_unique");
  std::string_view s2 = StringTable::add("beta_unique");
  EXPECT_NE(s1.data(), s2.data());
  EXPECT_EQ(s1, "alpha_unique");
  EXPECT_EQ(s2, "beta_unique");
}

TEST(StringTable, EmptyString) {
  std::string_view s = StringTable::add("");
  EXPECT_EQ(s, "");
  std::string_view s2 = StringTable::add("");
  EXPECT_EQ(s.data(), s2.data());
}

TEST(StringTable, IteratorStability) {
  es::string::StringTable<std::unordered_set> local_tbl;
  std::string_view sv1 = local_tbl.add("stable_first");
  local_tbl.add("stable_second_longer_string_to_force_allocation");
  local_tbl.add("stable_third");
  EXPECT_EQ(sv1, "stable_first");
}
