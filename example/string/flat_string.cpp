// flat_string.h example: fixed-capacity string

#include <cassert>
#include <estd/string/flat_string.h>
#include <iostream>

struct AssertOnOverflow {
  constexpr void operator()(bool cond, const char* msg) const {
    if (!cond) {
      std::cerr << "flat_string overflow: " << msg << "\n";
      assert(false);
    }
  }
};

struct LogOnOverflow {
  constexpr void operator()(bool cond, const char* msg) const {
    if (!cond) {
      std::cerr << "Warning: " << msg << "\n";
    }
  }
};

int main() {
  std::cout << "=== flat_string Example ===\n\n";

  std::cout << "1. Basic usage:\n";
  {
    es::string::flat_string<32> str;
    str = "Hello";
    str += " World";

    std::cout << "  str: \"" << str << "\"\n";
    std::cout << "  size: " << str.size() << "\n";
    std::cout << "  capacity: " << str.capacity() << "\n\n";
  }

  std::cout << "2. Construction methods:\n";
  {
    es::string::flat_string<16> s1;
    es::string::flat_string<16> s2("Hello");
    es::string::flat_string<16> s3(5, 'x');
    es::string::flat_string<16> s4(s2);
    es::string::flat_string<16> s5(std::move(s2));

    std::cout << "  Default: \"" << s1 << "\"\n";
    std::cout << "  From C-string: \"" << s2 << "\"\n";
    std::cout << "  From count+char: \"" << s3 << "\"\n";
    std::cout << "  Copy: \"" << s4 << "\"\n";
    std::cout << "  Move: \"" << s5 << "\"\n\n";
  }

  std::cout << "3. Construction from string_view (implicit):\n";
  {
    std::string_view sv = "Hello";
    es::string::flat_string<16> str = sv;
    std::cout << "  From string_view: \"" << str << "\"\n\n";
  }

  std::cout << "4. Default behavior (silent truncation):\n";
  {
    es::string::flat_string<10> str;
    str = "This is a very long string";

    std::cout << "  Assigned 25 chars to capacity 10\n";
    std::cout << "  Result: \"" << str << "\"\n";
    std::cout << "  size: " << str.size() << " (truncated silently)\n\n";
  }

  std::cout << "5. Custom OutOfRangeAssert - LogOnOverflow:\n";
  {
    es::string::flat_string<10, LogOnOverflow> str;
    str = "This is a very long string";

    std::cout << "  Result: \"" << str << "\"\n";
    std::cout << "  size: " << str.size() << "\n\n";
  }

  std::cout << "6. Custom OutOfRangeAssert - AssertOnOverflow:\n";
  {
    std::cout << "  Using AssertOnOverflow will assert on overflow:\n";
    std::cout << "  es::string::flat_string<10, AssertOnOverflow> str;\n";
    std::cout << "  str = \"too long\"; // Would trigger assert\n\n";
  }

  std::cout << "7. String operations:\n";
  {
    es::string::flat_string<32> str("Hello World");

    std::cout << "  Original: \"" << str << "\"\n";
    std::cout << "  substr(0, 5): \"" << str.substr(0, 5) << "\"\n";
    std::cout << "  find(\"World\"): " << str.find("World") << "\n";
    std::cout << "  find(\"xyz\"): " << str.find("xyz") << "\n\n";
  }

  std::cout << "8. Comparison:\n";
  {
    es::string::flat_string<16> s1("hello");
    es::string::flat_string<16> s2("world");
    es::string::flat_string<16> s3("hello");

    std::cout << "  s1 == s3: " << std::boolalpha << (s1 == s3) << "\n";
    std::cout << "  s1 < s2: " << (s1 < s2) << "\n";
    std::cout << "  s1 == \"hello\": " << (s1 == "hello") << "\n\n";
  }

  std::cout << "9. Use case: embedded systems / no heap:\n";
  {
    std::cout << "  flat_string uses stack memory only\n";
    std::cout << "  No dynamic allocation, suitable for:\n";
    std::cout << "    - Embedded systems\n";
    std::cout << "    - Real-time systems\n";
    std::cout << "    - Deterministic memory usage\n";
  }

  return 0;
}
