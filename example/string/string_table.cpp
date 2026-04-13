// string_table.h example: string interning table

#include <estd/string/string_table.h>
#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  std::cout << "=== string_table Example ===\n\n";

  std::cout << "1. Basic usage - string interning:\n";
  {
    es::string::StringTable<std::unordered_set> table;

    std::string_view s1 = table.add("hello");
    std::string_view s2 = table.add("world");
    std::string_view s3 = table.add("hello");

    std::cout << "  Interned \"hello\": " << static_cast<const void*>(s1.data())
              << "\n";
    std::cout << "  Interned \"world\": " << static_cast<const void*>(s2.data())
              << "\n";
    std::cout << "  Interned \"hello\" again: "
              << static_cast<const void*>(s3.data()) << "\n";
    std::cout << "  Same pointer for \"hello\": " << std::boolalpha
              << (s1.data() == s3.data()) << "\n\n";
  }

  std::cout << "2. Memory efficiency:\n";
  {
    es::string::StringTable<std::unordered_set> table;

    std::cout << "  Interning same string 1000 times...\n";
    const char* first = nullptr;
    for (int i = 0; i < 1000; ++i) {
      std::string_view s = table.add("configuration_key");
      if (i == 0)
        first = s.data();
    }

    std::string_view again = table.add("configuration_key");
    std::cout << "  All 1000 calls return same pointer: "
              << (first == again.data()) << "\n";
    std::cout << "  Only one copy stored in memory\n\n";
  }

  std::cout << "3. Fast string comparison:\n";
  {
    es::string::StringTable<std::unordered_set> table;

    std::string_view a1 = table.add("apple");
    std::string_view a2 = table.add("apple");
    std::string_view b = table.add("banana");

    std::cout << "  Comparing interned strings by pointer:\n";
    std::cout << "    a1.data() == a2.data(): " << (a1.data() == a2.data())
              << " (same string, same pointer)\n";
    std::cout << "    a1.data() == b.data(): " << (a1.data() == b.data())
              << " (different strings)\n";
    std::cout << "  No strcmp needed - just pointer comparison!\n\n";
  }

  std::cout << "4. Use case: symbol tables:\n";
  {
    es::string::StringTable<std::unordered_set> table;

    auto add_symbol = [&](const std::string& name) { return table.add(name); };

    std::string_view sym1 = add_symbol("function_name");
    std::string_view sym2 = add_symbol("variable_name");
    std::string_view sym3 = add_symbol("function_name");

    std::cout << "  Symbol \"function_name\" -> "
              << static_cast<const void*>(sym1.data()) << "\n";
    std::cout << "  Symbol \"variable_name\" -> "
              << static_cast<const void*>(sym2.data()) << "\n";
    std::cout << "  Symbol \"function_name\" -> "
              << static_cast<const void*>(sym3.data()) << "\n";
    std::cout << "  Same symbol returns same pointer: "
              << (sym1.data() == sym3.data()) << "\n\n";
  }

  std::cout << "5. Use case: configuration keys:\n";
  {
    es::string::StringTable<std::unordered_set> table;

    std::string_view key1 = table.add("database.host");
    std::string_view key2 = table.add("database.port");
    std::string_view key3 = table.add("database.host");

    std::cout << "  Configuration keys are interned once\n";
    std::cout << "  Fast lookup: just compare pointers\n";
    std::cout << "  \"database.host\" instances: "
              << (key1.data() == key3.data() ? "same" : "different") << "\n";
  }

  return 0;
}
