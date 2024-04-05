#include <estd/meta.h>
#include <gtest/gtest.h>

TEST(MetaTest, meta_and) {
  bool v = es::meta_and_v<std::true_type>;
  ASSERT_TRUE(v);
  v = es::meta_and_v<std::false_type>;
  ASSERT_FALSE(v);
  v = es::meta_and_v<std::true_type, std::true_type>;
  ASSERT_TRUE(v);
  v = es::meta_and_v<std::false_type, std::true_type>;
  ASSERT_FALSE(v);
  v = es::meta_and_v<std::true_type, std::false_type>;
  ASSERT_FALSE(v);
  v = es::meta_and_v<std::false_type, std::false_type>;
  ASSERT_FALSE(v);
  v = es::meta_and_v<std::true_type, std::true_type, std::true_type>;
  ASSERT_TRUE(v);
}

TEST(MetaTest, meta_or) {
  bool v = es::meta_or_v<std::true_type>;
  ASSERT_TRUE(v);
  v = es::meta_or_v<std::false_type>;
  ASSERT_FALSE(v);
  v = es::meta_or_v<std::true_type, std::true_type>;
  ASSERT_TRUE(v);
  v = es::meta_or_v<std::false_type, std::true_type>;
  ASSERT_TRUE(v);
  v = es::meta_or_v<std::true_type, std::false_type>;
  ASSERT_TRUE(v);
  v = es::meta_or_v<std::false_type, std::false_type>;
  ASSERT_FALSE(v);
  v = es::meta_or_v<std::false_type, std::false_type, std::false_type>;
  ASSERT_FALSE(v);
}

TEST(MetaTest, meta_not) {
  bool v = es::meta_not_v<std::true_type>;
  ASSERT_FALSE(v);
  v = es::meta_not_v<std::false_type>;
  ASSERT_TRUE(v);
}
