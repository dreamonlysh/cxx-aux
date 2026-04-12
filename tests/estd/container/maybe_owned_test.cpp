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

TEST(MaybeOwnedTest, LvalueConstruct) {
  std::string s = "test";
  es::maybe_owned<std::string> mo(s);
  EXPECT_TRUE(mo.has_value());
  EXPECT_FALSE(mo.is_owned());
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ(*mo, "test");
}

TEST(MaybeOwnedTest, ConstLvalueConstruct) {
  const std::string s = "world";
  es::maybe_owned<std::string> mo(std::in_place_type<std::string>, s);
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "world");
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

TEST(MaybeOwnedTest, AssignLvalue) {
  es::maybe_owned<std::string> mo;
  std::string s = "hello";
  mo = s;
  EXPECT_TRUE(mo.has_value());
  EXPECT_FALSE(mo.is_owned());
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ(*mo, "hello");
}

TEST(MaybeOwnedTest, AssignConstLvalue) {
  es::maybe_owned<std::string> mo;
  const std::string s = "hello";
  std::string copy = s;
  mo = std::move(copy);
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
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
  std::string s = "test";
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

TEST(MaybeOwnedTest, InPlaceTypeConstruct) {
  es::maybe_owned<std::string> mo(std::in_place_type<std::string>, "hello", 3);
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "hel");
}

TEST(MaybeOwnedTest, InPlaceTypeConstructWithMultipleArgs) {
  struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
  };
  es::maybe_owned<Point> mo(std::in_place_type<Point>, 10, 20);
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(mo->x, 10);
  EXPECT_EQ(mo->y, 20);
}

TEST(MaybeOwnedTest, Reset) {
  es::maybe_owned<std::string> mo("hello");
  EXPECT_TRUE(mo.has_value());
  mo.reset();
  EXPECT_FALSE(mo.has_value());
  EXPECT_FALSE(mo.is_owned());
  EXPECT_FALSE(mo.is_reference());
}

TEST(MaybeOwnedTest, ResetOnEmpty) {
  es::maybe_owned<std::string> mo;
  EXPECT_FALSE(mo.has_value());
  mo.reset();
  EXPECT_FALSE(mo.has_value());
}

TEST(MaybeOwnedTest, ArrayOwnedConstruct) {
  es::maybe_owned<int[5]> mo(std::in_place_type<int[5]>,
                             std::array<int, 5>{1, 2, 3, 4, 5});
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ((*mo)[0], 1);
  EXPECT_EQ((*mo)[4], 5);
}

TEST(MaybeOwnedTest, ArrayReferenceConstruct) {
  int arr[3] = {10, 20, 30};
  es::maybe_owned<int[3]> mo(arr);
  EXPECT_TRUE(mo.has_value());
  EXPECT_FALSE(mo.is_owned());
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ((*mo)[0], 10);
  EXPECT_EQ((*mo)[1], 20);
  EXPECT_EQ((*mo)[2], 30);
}

TEST(MaybeOwnedTest, ArrayModifyThroughReference) {
  int arr[3] = {1, 2, 3};
  es::maybe_owned<int[3]> mo(arr);
  (*mo)[0] = 100;
  EXPECT_EQ(arr[0], 100);
}

TEST(MaybeOwnedTest, ArrayReset) {
  int arr[2] = {1, 2};
  es::maybe_owned<int[2]> mo(arr);
  EXPECT_TRUE(mo.has_value());
  mo.reset();
  EXPECT_FALSE(mo.has_value());
}

TEST(MaybeOwnedTest, ArrayCopyConstruct) {
  es::maybe_owned<int[3]> original(std::in_place_type<int[3]>,
                                   std::array<int, 3>{1, 2, 3});
  es::maybe_owned<int[3]> copy = original;
  EXPECT_TRUE(copy.has_value());
  EXPECT_TRUE(copy.is_owned());
  EXPECT_EQ((*copy)[0], 1);
}

TEST(MaybeOwnedTest, ArrayMoveConstruct) {
  es::maybe_owned<int[3]> original(std::in_place_type<int[3]>,
                                   std::array<int, 3>{1, 2, 3});
  es::maybe_owned<int[3]> moved = std::move(original);
  EXPECT_TRUE(moved.has_value());
  EXPECT_TRUE(moved.is_owned());
  EXPECT_EQ((*moved)[0], 1);
}

TEST(MaybeOwnedTest, ArraySwap) {
  es::maybe_owned<int[2]> a(std::in_place_type<int[2]>,
                            std::array<int, 2>{1, 2});
  int arr[2] = {3, 4};
  es::maybe_owned<int[2]> b(arr);

  std::swap(a, b);

  EXPECT_TRUE(a.is_reference());
  EXPECT_EQ((*a)[0], 3);
  EXPECT_TRUE(b.is_owned());
  EXPECT_EQ((*b)[0], 1);
}

TEST(MaybeOwnedTest, NoexceptGuarantees) {
  static_assert(noexcept(es::maybe_owned<std::string>()));

  std::string s = "test";
  static_assert(noexcept(es::maybe_owned<std::string>(s)));

  es::maybe_owned<std::string> mo1;
  es::maybe_owned<std::string> mo2("hello");
  static_assert(noexcept(mo1.swap(mo2)));
  static_assert(noexcept(mo1.has_value()));
  static_assert(noexcept(mo1.is_owned()));
  static_assert(noexcept(mo1.is_reference()));
}

TEST(MaybeOwnedTest, MemberSwap) {
  es::maybe_owned<std::string> a("hello");
  std::string b_str = "world";
  es::maybe_owned<std::string> b(b_str);

  a.swap(b);

  EXPECT_TRUE(a.is_reference());
  EXPECT_EQ(*a, "world");
  EXPECT_TRUE(b.is_owned());
  EXPECT_EQ(*b, "hello");
}

TEST(MaybeOwnedTest, SelfAssign) {
  es::maybe_owned<std::string> mo("test");
  mo = mo;
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "test");
}

TEST(MaybeOwnedTest, SelfMove) {
  es::maybe_owned<std::string> mo("test");
  mo = std::move(mo);
  EXPECT_TRUE(mo.has_value());
}

TEST(MaybeOwnedTest, MultipleAssignments) {
  es::maybe_owned<std::string> mo;

  mo = std::string("first");
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "first");

  std::string s = "second";
  mo = s;
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ(*mo, "second");

  mo = std::string("third");
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "third");
}

TEST(MaybeOwnedTest, EmptyStateOperations) {
  es::maybe_owned<std::string> mo;

  EXPECT_FALSE(mo.has_value());
  EXPECT_FALSE(static_cast<bool>(mo));
  EXPECT_FALSE(mo.is_owned());
  EXPECT_FALSE(mo.is_reference());

  mo.reset();
  EXPECT_FALSE(mo.has_value());
}

TEST(MaybeOwnedTest, DifferentTypes) {
  es::maybe_owned<int> int_mo(42);
  EXPECT_EQ(*int_mo, 42);

  es::maybe_owned<double> double_mo(3.14);
  EXPECT_EQ(*double_mo, 3.14);

  es::maybe_owned<char> char_mo('a');
  EXPECT_EQ(*char_mo, 'a');
}

TEST(MaybeOwnedTest, MoveFromReference) {
  std::string s = "test";
  es::maybe_owned<std::string> mo(s);

  es::maybe_owned<std::string> moved = std::move(mo);
  EXPECT_TRUE(moved.is_reference());
  EXPECT_EQ(*moved, "test");
  EXPECT_EQ(s, "test");
}

TEST(MaybeOwnedTest, ReferenceAssignmentOverwrite) {
  std::string s1 = "first";
  std::string s2 = "second";

  es::maybe_owned<std::string> mo(s1);
  EXPECT_EQ(*mo, "first");

  mo = s2;
  EXPECT_EQ(*mo, "second");
  EXPECT_TRUE(mo.is_reference());
}

TEST(MaybeOwnedTest, OwnedToReferenceTransition) {
  es::maybe_owned<std::string> mo("owned");
  EXPECT_TRUE(mo.is_owned());

  std::string s = "reference";
  mo = s;
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ(*mo, "reference");
}

TEST(MaybeOwnedTest, ReferenceToOwnedTransition) {
  std::string s = "reference";
  es::maybe_owned<std::string> mo(s);
  EXPECT_TRUE(mo.is_reference());

  mo = std::string("owned");
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "owned");
}

TEST(MaybeOwnedTest, OperatorStarOnOwned) {
  es::maybe_owned<std::string> mo("test");
  std::string& ref = *mo;
  EXPECT_EQ(ref, "test");
  ref = "modified";
  EXPECT_EQ(*mo, "modified");
}

TEST(MaybeOwnedTest, OperatorStarOnReference) {
  std::string s = "test";
  es::maybe_owned<std::string> mo(s);
  std::string& ref = *mo;
  EXPECT_EQ(ref, "test");
  ref = "modified";
  EXPECT_EQ(s, "modified");
}

TEST(MaybeOwnedTest, ConstCorrectness) {
  const es::maybe_owned<std::string> mo("test");
  const std::string& ref = *mo;
  EXPECT_EQ(ref, "test");
}

TEST(MaybeOwnedTest, ArrayWithDifferentSizes) {
  es::maybe_owned<int[1]> mo1(std::in_place_type<int[1]>,
                              std::array<int, 1>{42});
  EXPECT_EQ((*mo1)[0], 42);

  es::maybe_owned<int[100]> mo100(std::in_place_type<int[100]>,
                                  std::array<int, 100>{});
  EXPECT_EQ((*mo100)[0], 0);
}

TEST(MaybeOwnedTest, ArrayAssignmentFromReference) {
  int arr1[3] = {1, 2, 3};
  int arr2[3] = {4, 5, 6};

  es::maybe_owned<int[3]> mo(arr1);
  EXPECT_EQ((*mo)[0], 1);

  mo = arr2;
  EXPECT_TRUE(mo.is_reference());
  EXPECT_EQ((*mo)[0], 4);
}

TEST(MaybeOwnedTest, ArrayOwnedToOwnedCopy) {
  es::maybe_owned<int[3]> mo1(std::in_place_type<int[3]>,
                              std::array<int, 3>{1, 2, 3});
  es::maybe_owned<int[3]> mo2(std::in_place_type<int[3]>,
                              std::array<int, 3>{4, 5, 6});

  mo1 = mo2;
  EXPECT_TRUE(mo1.is_owned());
  EXPECT_EQ((*mo1)[0], 4);
}

TEST(MaybeOwnedTest, ArrayReferenceToOwnedMove) {
  int arr[3] = {1, 2, 3};
  es::maybe_owned<int[3]> mo1(arr);
  es::maybe_owned<int[3]> mo2(std::in_place_type<int[3]>,
                              std::array<int, 3>{4, 5, 6});

  mo1 = std::move(mo2);
  EXPECT_TRUE(mo1.is_owned());
  EXPECT_EQ((*mo1)[0], 4);
}

TEST(MaybeOwnedTest, EmptyToValueAssignment) {
  es::maybe_owned<std::string> mo1;
  es::maybe_owned<std::string> mo2("test");

  mo1 = mo2;
  EXPECT_TRUE(mo1.has_value());
  EXPECT_EQ(*mo1, "test");
}

TEST(MaybeOwnedTest, ValueToEmptyAssignment) {
  es::maybe_owned<std::string> mo1("test");
  es::maybe_owned<std::string> mo2;

  mo1 = mo2;
  EXPECT_FALSE(mo1.has_value());
}

TEST(MaybeOwnedTest, ResetThenReassign) {
  es::maybe_owned<std::string> mo("test");
  EXPECT_TRUE(mo.has_value());

  mo.reset();
  EXPECT_FALSE(mo.has_value());

  mo = std::string("new");
  EXPECT_TRUE(mo.has_value());
  EXPECT_TRUE(mo.is_owned());
  EXPECT_EQ(*mo, "new");
}

TEST(MaybeOwnedTest, SwapEmpty) {
  es::maybe_owned<std::string> a;
  es::maybe_owned<std::string> b("test");

  std::swap(a, b);

  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, "test");
  EXPECT_FALSE(b.has_value());
}

TEST(MaybeOwnedTest, SwapBothEmpty) {
  es::maybe_owned<std::string> a;
  es::maybe_owned<std::string> b;

  std::swap(a, b);

  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(b.has_value());
}

TEST(MaybeOwnedTest, ComplexType) {
  struct Complex {
    int x;
    std::string s;
    Complex(int x, std::string s) : x(x), s(std::move(s)) {}
  };

  es::maybe_owned<Complex> mo(std::in_place_type<Complex>, 42, "test");
  EXPECT_TRUE(mo.has_value());
  EXPECT_EQ(mo->x, 42);
  EXPECT_EQ(mo->s, "test");
}

TEST(MaybeOwnedTest, VectorType) {
  es::maybe_owned<std::vector<int>> mo(std::in_place_type<std::vector<int>>,
                                       {1, 2, 3, 4, 5});
  EXPECT_TRUE(mo.has_value());
  EXPECT_EQ(mo->size(), 5u);
  EXPECT_EQ((*mo)[0], 1);
}

TEST(MaybeOwnedTest, MoveOnlyType) {
  struct MoveOnly {
    int value;
    MoveOnly(int v) : value(v) {}
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
  };

  es::maybe_owned<MoveOnly> mo(std::in_place_type<MoveOnly>, 42);
  EXPECT_TRUE(mo.has_value());
  EXPECT_EQ(mo->value, 42);

  es::maybe_owned<MoveOnly> mo2 = std::move(mo);
  EXPECT_TRUE(mo2.has_value());
  EXPECT_EQ(mo2->value, 42);
}
