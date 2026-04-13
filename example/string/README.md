# String Utilities Examples

This directory contains examples demonstrating the `es::string` library for string manipulation and utilities.

## Examples

### 1. flat_string.cpp
**Fixed-capacity string (no heap allocation)**

```cpp
#include <estd/string/flat_string.h>

es::string::flat_string<32> str;
str = "Hello";
str += " World";
// Uses stack memory only, no dynamic allocation

// Note: constructor from const char* is explicit
es::string::flat_string<16> s1("Hello");        // OK
es::string::flat_string<16> s2 = "Hello";       // Error!
es::string::flat_string<16> s3 = std::string_view("Hello");  // OK (implicit)
```

**Custom OutOfRangeAssert:**
```cpp
struct LogOnOverflow {
  constexpr void operator()(bool cond, const char* msg) const {
    if (!cond) {
      std::cerr << "Warning: " << msg << "\n";
    }
  }
};

es::string::flat_string<10, LogOnOverflow> str;
str = "too long";  // Logs warning instead of silent truncation
```

**Compile:**
```bash
g++ -std=c++17 -I include example/string/flat_string.cpp -o flat_string
./flat_string
```

### 2. string_table.cpp
**String interning table**

```cpp
#include <estd/string/string_table.h>
#include <unordered_set>

es::string::StringTable<std::unordered_set> table;
std::string_view s1 = table.add("hello");
std::string_view s2 = table.add("hello");
// s1.data() == s2.data() (same pointer, one copy in memory)
```

**Compile:**
```bash
g++ -std=c++17 -I include example/string/string_table.cpp -o string_table
./string_table
```

### 3. concat_buffer.cpp
**Efficient string concatenation buffer**

```cpp
#include <estd/string/concat_buffer.h>

// Fixed buffer (returns false on overflow)
es::string::concat_buffer<128> buf;
buf.append("Name: ", "Alice", ", Age: ", 25);
std::string result = buf.to_string();

// Dynamic buffer (auto-expands)
es::string::concat_buffer<0, std::string> dynamic;
dynamic.append("Long string...");

// Reference to external container
std::string str;
es::string::concat_buffer ref(str);
ref.append("Hello");  // str now contains "Hello"
```

**Append with lazy_views:**
```cpp
es::string::concat_buffer<128> buf;
std::string name = "hello.world";

buf.append("Original: ", name, "\n");
buf.append("Upper: ", name | es::string::views::to_upper, "\n");
buf.append("Lower: ", name | es::string::views::to_lower, "\n");
buf.append("Underscore: ", name | es::string::views::dot_to_underline);
```

**Compile:**
```bash
g++ -std=c++17 -I include example/string/concat_buffer.cpp -o concat_buffer
./concat_buffer
```

## API Summary

### flat_string<N, OutOfRangeAssert>

Fixed-capacity string stored on the stack.

| Feature | Description |
|---------|-------------|
| Capacity | Template parameter N (actual capacity is N-1) |
| Allocation | Stack only, no heap |
| Constructor | Explicit from `const char*`, implicit from `string_view` |
| OutOfRangeAssert | Custom handler for overflow (default: silent truncation) |

**OutOfRangeAssert signature:**
```cpp
struct MyAssert {
  constexpr void operator()(bool cond, const char* msg) const {
    if (!cond) { /* handle error */ }
  }
};
```

### StringTable<SetT>

String interning for memory efficiency and fast comparison.

| Method | Description |
|--------|-------------|
| `add(str)` | Return unique `string_view` for string |

**Template parameters:**
- `SetT`: Set container type (`std::unordered_set` for O(1), `std::set` for ordered)

**Benefits:**
- Same string stored once in memory
- Pointer comparison instead of string comparison
- Useful for symbols, identifiers, configuration keys

### concat_buffer<N, Container>

Fixed or dynamic concatenation buffer.

| Method | Description |
|--------|-------------|
| `append(args...)` | Append multiple items (returns bool for fixed, void for dynamic) |
| `to_string()` | Get result as std::string |
| `view()` | Get result as std::string_view |
| `c_str()` | Get null-terminated C-string |
| `size()` | Current size |
| `capacity()` | Current capacity |
| `clear()` | Reset buffer |

**Buffer types:**
- `concat_buffer<N>`: Fixed buffer (stack-allocated `std::array<char, N>`)
- `concat_buffer<0, std::string>`: Dynamic buffer (auto-expands)
- `concat_buffer(container)`: Reference to external container

### lazy_views

Lazy string transformations (used with concat_buffer or ranges).

| View | Description |
|------|-------------|
| `to_upper` | Convert to uppercase |
| `to_lower` | Convert to lowercase |
| `dot_to_underline` | Replace `.` with `_` |

## Use Cases

### Building log messages
```cpp
es::string::concat_buffer<256> log;
log.append("[INFO] User ", username, " logged in from ", ip_address);
```

### String interning for symbols
```cpp
es::string::StringTable<std::unordered_set> symbols;
std::string_view sym = symbols.add("function_name");
// All uses of "function_name" return same pointer
```

### No-heap string operations
```cpp
es::string::flat_string<64> str;
str = "Hello";
str += " World";  // No heap allocation
```

### Lazy transformations with concat_buffer
```cpp
es::string::concat_buffer<128> buf;
std::string input = "hello.world";
buf.append(input | es::string::views::to_upper | es::string::views::dot_to_underline);
// buf contains "HELLO_WORLD"
```
