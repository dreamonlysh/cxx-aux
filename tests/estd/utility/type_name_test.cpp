#include <estd/__utility/type_name.h>
#include <gtest/gtest.h>

TEST(TypeName, Numeric) {
  ASSERT_EQ(es::type_name<int>(), "int");
  ASSERT_EQ(es::type_name<double>(), "double");
#if defined(__clang__) || defined(_MSC_VER)
  ASSERT_EQ(es::type_name<uint16_t>(), "unsigned short");
#elif defined(__GNUC__)
  ASSERT_EQ(es::type_name<uint16_t>(), "short unsigned int");
#endif
  ASSERT_EQ(es::type_name(0), "int");
  ASSERT_EQ(es::type_name(0u), "unsigned int");
}

struct A;

TEST(TypeName, Struct) { ASSERT_EQ(es::type_name<A>(), "A"); }

template <typename>
struct B;

TEST(TypeName, Template) { ASSERT_EQ(es::type_name<B<int>>(), "B<int>"); }
