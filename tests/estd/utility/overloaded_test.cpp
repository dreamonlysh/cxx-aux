#include <estd/__utility/overloaded.h>
#include <gtest/gtest.h>
#include <string>
#include <variant>

TEST(Overloaded, BasicUsage) {
  std::variant<int, std::string, double> v = 42;

  auto visitor =
      es::overloaded{[](int i) { return "int: " + std::to_string(i); },
                     [](const std::string& s) { return "string: " + s; },
                     [](double d) { return "double: " + std::to_string(d); }};

  std::string result = std::visit(visitor, v);

  EXPECT_EQ(result, "int: 42");
}

TEST(Overloaded, VisitString) {
  std::variant<int, std::string, double> v = std::string("hello");

  auto visitor =
      es::overloaded{[](int i) { return "int: " + std::to_string(i); },
                     [](const std::string& s) { return "string: " + s; },
                     [](double d) { return "double: " + std::to_string(d); }};

  std::string result = std::visit(visitor, v);

  EXPECT_EQ(result, "string: hello");
}

TEST(Overloaded, VisitDouble) {
  std::variant<int, std::string, double> v = 3.14;

  auto visitor =
      es::overloaded{[](int i) { return "int: " + std::to_string(i); },
                     [](const std::string& s) { return "string: " + s; },
                     [](double d) -> std::string {
                       int whole = static_cast<int>(d);
                       return "double: " + std::to_string(whole) + "%";
                     }};

  std::string result = std::visit(visitor, v);

  EXPECT_EQ(result, "double: 3%");
}

TEST(Overloaded, DeductionGuide) {
  auto visitor = es::overloaded{[](int i) { return i * 2; },
                                [](double d) { return static_cast<int>(d); }};

  std::variant<int, double> v1 = 21;
  std::variant<int, double> v2 = 3.14;

  EXPECT_EQ(std::visit(visitor, v1), 42);
  EXPECT_EQ(std::visit(visitor, v2), 3);
}
