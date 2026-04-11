#include <estd/__meta/has_member.h>
#include <gtest/gtest.h>

// ==================== Test types for member type detection
// ====================
struct TypeWithIterator {
  using iterator = int*;
};

struct TypeWithoutIterator {};

struct TypeWithValueType {
  using value_type = int;
};

struct TypeWithoutValueType {};

// ==================== Test types for member data detection
// ====================
struct TypeWithDataMember {
  int data;
};

struct TypeWithoutDataMember {};

// ==================== Test types for member function detection
// ====================
struct TypeWithSizeMethod {
  size_t size() const { return 0; }
};

struct TypeWithoutSizeMethod {};

struct TypeWithFooMethod {
  void foo(int) {}
  int bar(double, float) const { return 0; }
};

struct TypeWithoutFooMethod {};

// ==================== Test types for inheritance - base class has members
// ====================
struct BaseWithIterator {
  using iterator = int*;
};

struct BaseWithDataMember {
  int data;
};

struct BaseWithSizeMethod {
  size_t size() const { return 0; }
};

struct BaseWithFooMethod {
  void foo(int) {}
  int bar(double, float) const { return 0; }
};

struct DerivedFromBaseWithIterator : public BaseWithIterator {};
struct DerivedFromBaseWithDataMember : public BaseWithDataMember {};
struct DerivedFromBaseWithSizeMethod : public BaseWithSizeMethod {};
struct DerivedFromBaseWithFoo : public BaseWithFooMethod {};

// Define meta functions in es namespace
namespace es {

META_HAS_MEMBER_TYPE(value_type);
META_HAS_MEMBER_DATA(data);
META_HAS_MEMBER_FUNCTION(size); // No args for size()
META_HAS_MEMBER_FUNCTION(foo, int(0));
META_HAS_MEMBER_FUNCTION(bar, double(0), float(0));

} // namespace es

// ==================== Tests for META_HAS_MEMBER_TYPE ====================

TEST(HasMemberTest, hasMemberType_Basic) {
  // Type with iterator (pre-defined in has_member.h)
  ASSERT_TRUE(es::has_member_iterator<TypeWithIterator>::value);
  ASSERT_TRUE(es::has_member_iterator_v<TypeWithIterator>);

  // Type without iterator
  ASSERT_FALSE(es::has_member_iterator<TypeWithoutIterator>::value);
  ASSERT_FALSE(es::has_member_iterator_v<TypeWithoutIterator>);
}

TEST(HasMemberTest, hasMemberType_ValueType) {
  // Type with value_type
  ASSERT_TRUE(es::has_member_value_type<TypeWithValueType>::value);
  ASSERT_TRUE(es::has_member_value_type_v<TypeWithValueType>);

  // Type without value_type
  ASSERT_FALSE(es::has_member_value_type<TypeWithoutValueType>::value);
  ASSERT_FALSE(es::has_member_value_type_v<TypeWithoutValueType>);
}

TEST(HasMemberTest, hasMemberType_Inheritance) {
  // Derived class inherits type from base - should be detected
  ASSERT_TRUE(es::has_member_iterator<DerivedFromBaseWithIterator>::value);
  ASSERT_TRUE(es::has_member_iterator_v<DerivedFromBaseWithIterator>);
}

// ==================== Tests for META_HAS_MEMBER_DATA ====================

TEST(HasMemberTest, hasMemberData_Basic) {
  // Type with data member
  ASSERT_TRUE(es::has_member_data<TypeWithDataMember>::value);
  ASSERT_TRUE(es::has_member_data_v<TypeWithDataMember>);

  // Type without data member
  ASSERT_FALSE(es::has_member_data<TypeWithoutDataMember>::value);
  ASSERT_FALSE(es::has_member_data_v<TypeWithoutDataMember>);
}

TEST(HasMemberTest, hasMemberData_Inheritance) {
  // Derived class inherits data member from base - should be detected
  ASSERT_TRUE(es::has_member_data<DerivedFromBaseWithDataMember>::value);
  ASSERT_TRUE(es::has_member_data_v<DerivedFromBaseWithDataMember>);
}

// ==================== Tests for META_HAS_MEMBER_FUNCTION ====================

TEST(HasMemberTest, hasMemberFunction_Size) {
  // Type with size() method
  ASSERT_TRUE(es::has_member_size<TypeWithSizeMethod>::value);
  ASSERT_TRUE(es::has_member_size_v<TypeWithSizeMethod>);

  // Type without size() method
  ASSERT_FALSE(es::has_member_size<TypeWithoutSizeMethod>::value);
  ASSERT_FALSE(es::has_member_size_v<TypeWithoutSizeMethod>);
}

TEST(HasMemberTest, hasMemberFunction_Foo) {
  // Type with foo(int) method
  ASSERT_TRUE(es::has_member_foo<TypeWithFooMethod>::value);
  ASSERT_TRUE(es::has_member_foo_v<TypeWithFooMethod>);

  // Type without foo method
  ASSERT_FALSE(es::has_member_foo<TypeWithoutFooMethod>::value);
  ASSERT_FALSE(es::has_member_foo_v<TypeWithoutFooMethod>);
}

TEST(HasMemberTest, hasMemberFunction_Bar) {
  // Type with bar(double, float) const method
  ASSERT_TRUE(es::has_member_bar<TypeWithFooMethod>::value);
  ASSERT_TRUE(es::has_member_bar_v<TypeWithFooMethod>);

  // Type without bar method
  ASSERT_FALSE(es::has_member_bar<TypeWithoutFooMethod>::value);
  ASSERT_FALSE(es::has_member_bar_v<TypeWithoutFooMethod>);
}

TEST(HasMemberTest, hasMemberFunction_Inheritance) {
  // Derived class inherits size() method from base - should be detected
  ASSERT_TRUE(es::has_member_size<DerivedFromBaseWithSizeMethod>::value);
  ASSERT_TRUE(es::has_member_size_v<DerivedFromBaseWithSizeMethod>);

  // Derived class inherits foo() method from base - should be detected
  ASSERT_TRUE(es::has_member_foo<DerivedFromBaseWithFoo>::value);
  ASSERT_TRUE(es::has_member_foo_v<DerivedFromBaseWithFoo>);

  // Derived class inherits bar() method from base - should be detected
  ASSERT_TRUE(es::has_member_bar<DerivedFromBaseWithFoo>::value);
  ASSERT_TRUE(es::has_member_bar_v<DerivedFromBaseWithFoo>);
}

// ==================== Edge Cases ====================

TEST(HasMemberTest, hasMemberType_CVQualified) {
  // cv-qualified types should work the same for type detection
  ASSERT_TRUE(es::has_member_iterator_v<const TypeWithIterator>);
  ASSERT_TRUE(es::has_member_iterator_v<volatile TypeWithIterator>);
  ASSERT_TRUE(es::has_member_iterator_v<const volatile TypeWithIterator>);
}

TEST(HasMemberTest, hasMemberData_CVQualified) {
  // cv-qualified types should work the same for data member detection
  ASSERT_TRUE(es::has_member_data_v<const TypeWithDataMember>);
  ASSERT_TRUE(es::has_member_data_v<volatile TypeWithDataMember>);
  ASSERT_TRUE(es::has_member_data_v<const volatile TypeWithDataMember>);
}

TEST(HasMemberTest, hasMemberFunction_CVQualified) {
  // cv-qualified types should work for const methods
  // Note: non-const methods on const objects won't work, but const methods will
  ASSERT_TRUE(es::has_member_size_v<const TypeWithSizeMethod>);
  ASSERT_TRUE(es::has_member_bar_v<const TypeWithFooMethod>);
}
