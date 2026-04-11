#include "estd/__container/maybe_owned.h"
#include <gtest/gtest.h>
#include <string>
#include <utility>

TEST(MaybeOwnedTest, DefaultConstruct) {
  es::maybe_owned<std::string> mo;
  EXPECT_FALSE(mo.has_value());
  EXPECT_FALSE(mo.is_owned());
  EXPECT_FALSE(mo.is_reference());
}

TEST(MaybeOwnedTest, RvalueConstruct) {
  es::maybe_owned<std::string> mo("hello");
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "hello");
}

TEST(MaybeOwnedTest, ConstLvalueConstruct) {
  const std::string s = "world";
  es::maybe_owned<std::string> mo(s);
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "world");
}

TEST(MaybeOwnedTest, LvalueConstruct) {
  std::string s = "test";
  es::maybe_owned<std::string> mo(s);
  EXPECT_TRUE(mo.has_value());
  EXPECT_FALSE(mo.is_owned());
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ(*mo, "test");
}

TEST(MaybeOwnedTest, CopyConstruct) {
  es::maybe_owned<std::string> original("hello");
  es::maybe_owned<std::string> copy = original;
  EXPECT_TRUE(copy.has_value());
  EXPECT_TRUE(copy.is_owned());
  EXPECT_EQ(*copy, "hello");
}

TEST(MaybeOwnedTest, MoveConstruct) {
  es::maybe_owned<std::string> original("hello");
  es::maybe_owned<std::string> moved = std::move(original);
  EXPECT_TRUE(moved.has_value());
  EXPECT_TRUE(moved.is_owned());
  EXPECT_EQ(*moved, "hello");
}

TEST(MaybeOwnedTest, CopyAssign) {
  es::maybe_owned<std::string> mo;
  es::maybe_owned<std::string> original("hello");
  mo = original;
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "hello");
}

TEST(MaybeOwnedTest, MoveAssign) {
  es::maybe_owned<std::string> mo;
  es::maybe_owned<std::string> original("hello");
  mo = std::move(original);
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "hello");
}

TEST(MaybeOwnedTest, AssignRvalue) {
  es::maybe_owned<std::string> mo;
  mo = std::string("hello");
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "hello");
}

TEST(MaybeOwnedTest, AssignConstLvalue) {
  es::maybe_owned<std::string> mo;
  const std::string s = "hello";
  mo = s;
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "hello");
}

TEST(MaybeOwnedTest, AssignLvalue) {
  es::maybe_owned<std::string> mo;
  std::string s = "hello";
  mo = s;
  EXPECT_TRUE(mo.has_value());
  EXPECT_FALSE(mo.is_owned());
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ(*mo, "hello");
}

TEST(MaybeOwnedTest, Get) {
  std::string s = "test";
  es::maybe_owned<std::string> mo(s);
  EXPECT_EQ(mo.get(), "test");
  mo.get() = "modified";
  EXPECT_EQ(s, "modified");
}

TEST(MaybeOwnedTest, GetConst) {
  const std::string s = "test";
  es::maybe_owned<std::string> mo(s);
  EXPECT_EQ(mo.get(), "test");
}

TEST(MaybeOwnedTest, OperatorArrow) {
  es::maybe_owned<std::string> mo("hello");
  EXPECT_EQ(mo->size(), 5u);
}

TEST(MaybeOwnedTest, ModifyThroughReference) {
  std::string s = "original";
  es::maybe_owned<std::string> mo(s);
  mo->append("_modified");
  EXPECT_EQ(s, "original_modified");
}

TEST(MaybeOwnedTest, Swap) {
  es::maybe_owned<std::string> a("hello");
  std::string b_str = "world";
  es::maybe_owned<std::string> b(b_str);
  std::swap(a, b);
  EXPECT_TRUE(a.has_value());
  EXPECT_TRUE(a.is_reference());
  EXPECT_EQ(*a, "world");
  EXPECT_TRUE(b.has_value());
  EXPECT_TRUE(b.is_owned());
  EXPECT_EQ(*b, "hello");
}

TEST(MaybeOwnedTest, OperatorBool) {
  es::maybe_owned<std::string> mo;
  EXPECT_FALSE(static_cast<bool>(mo));
  es::maybe_owned<std::string> owned("test");
  EXPECT_TRUE(static_cast<bool>(owned));
}
