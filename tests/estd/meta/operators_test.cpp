#include <estd/meta.h>
#include <gtest/gtest.h>

TEST(MetaTest, conjunction) {
  bool v = std::conjunction_v<std::true_type>;
  ASSERT_TRUE(v);
  v = std::conjunction_v<std::false_type>;
  ASSERT_FALSE(v);
  v = std::conjunction_v<std::true_type, std::true_type>;
  ASSERT_TRUE(v);
  v = std::conjunction_v<std::false_type, std::true_type>;
  ASSERT_FALSE(v);
  v = std::conjunction_v<std::true_type, std::false_type>;
  ASSERT_FALSE(v);
  v = std::conjunction_v<std::false_type, std::false_type>;
  ASSERT_FALSE(v);
  v = std::conjunction_v<std::true_type, std::true_type, std::true_type>;
  ASSERT_TRUE(v);
}

TEST(MetaTest, disjunction) {
  bool v = std::disjunction_v<std::true_type>;
  ASSERT_TRUE(v);
  v = std::disjunction_v<std::false_type>;
  ASSERT_FALSE(v);
  v = std::disjunction_v<std::true_type, std::true_type>;
  ASSERT_TRUE(v);
  v = std::disjunction_v<std::false_type, std::true_type>;
  ASSERT_TRUE(v);
  v = std::disjunction_v<std::true_type, std::false_type>;
  ASSERT_TRUE(v);
  v = std::disjunction_v<std::false_type, std::false_type>;
  ASSERT_FALSE(v);
  v = std::disjunction_v<std::false_type, std::false_type, std::false_type>;
  ASSERT_FALSE(v);
}

TEST(MetaTest, bool_constant_negation) {
  bool v = std::bool_constant<!std::true_type::value>::value;
  ASSERT_FALSE(v);
  v = std::bool_constant<!std::false_type::value>::value;
  ASSERT_TRUE(v);
}
