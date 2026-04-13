// concat_buffer.h example: efficient string concatenation buffer

#include <estd/string/concat_buffer.h>
#include <iostream>

int main() {
  std::cout << "=== concat_buffer Example ===\n\n";

  std::cout << "1. Basic usage:\n";
  {
    es::string::concat_buffer<64> buf;
    buf.append("Hello", " ", "World");
    std::cout << "  Result: \"" << buf.to_string() << "\"\n";
    std::cout << "  Size: " << buf.size() << "\n\n";
  }

  std::cout << "2. Append multiple items at once:\n";
  {
    es::string::concat_buffer<128> buf;
    buf.append("Name: ", "Alice", ", Age: ", 25, ", Score: ", 98);
    std::cout << "  Result: \"" << buf.to_string() << "\"\n\n";
  }

  std::cout << "3. Overflow handling (fixed buffer):\n";
  {
    es::string::concat_buffer<10> buf;

    bool ok = buf.append("Hello", " ", "World");
    std::cout << "  Append \"Hello World\" to capacity 10\n";
    std::cout << "  Success: " << ok << "\n";
    std::cout << "  Result: \"" << buf.to_string() << "\"\n";
    std::cout << "  Size: " << buf.size() << "\n\n";
  }

  std::cout << "4. Dynamic buffer (auto-expands):\n";
  {
    es::string::concat_buffer<0, std::string> buf;

    buf.append("Hello", " World",
               " - this is a long string that exceeds initial capacity");

    std::cout << "  Result: \"" << buf.to_string() << "\"\n";
    std::cout << "  Size: " << buf.size() << "\n";
    std::cout << "  Capacity: " << buf.capacity() << "\n\n";
  }

  std::cout << "5. Append with lazy_views:\n";
  {
    es::string::concat_buffer<128> buf;
    std::string name = "hello.world";

    buf.append("Original: ", name, "\n");
    buf.append("Upper: ", name | es::string::views::to_upper, "\n");
    buf.append("Lower: ", name | es::string::views::to_lower, "\n");
    buf.append("Underscore: ", name | es::string::views::dot_to_underline);

    std::cout << buf.to_string() << "\n";
  }

  std::cout << "6. Building formatted output:\n";
  {
    es::string::concat_buffer<256> buf;
    buf.append("=== Report ===\n", "ID: ", 12345, "\n", "Status: ", "OK", "\n",
               "Count: ", 42);

    std::cout << buf.to_string() << "\n";
  }

  std::cout << "7. Use case: log message building:\n";
  {
    es::string::concat_buffer<128> log;
    log.append("[INFO] User ", "alice", " logged in from ", "192.168.1.1");
    std::cout << "  " << log.to_string() << "\n\n";
  }

  std::cout << "8. Reference to external container:\n";
  {
    std::string str;
    es::string::concat_buffer buf(str);
    buf.append("Hello", " ", "World");

    std::cout << "  Buffer: \"" << buf.to_string() << "\"\n";
    std::cout << "  Original string: \"" << str << "\"\n";
  }

  return 0;
}
