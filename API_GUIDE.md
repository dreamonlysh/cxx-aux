# cxx-aux API Guide

Commonly used lite libraries and C++ standard library extensions.

## Directory Structure

```
include/
├── estd/           # Extension of C++ standard library (header-only)
│   ├── bit.h
│   ├── concurrency.h
│   ├── flat_vector.h
│   ├── integer.h
│   ├── maybe_owned.h
│   ├── memory.h
│   ├── meta.h
│   ├── utility.h
│   ├── __container/
│   │   ├── flat_vector.h
│   │   └── small_vector.h
│   ├── cast/
│   ├── idiom/
│   └── string/
└── cxxaux/         # General utility modules
    ├── binary/
    │   ├── binary_decoder.h
    │   ├── elf_format.h
    │   ├── elf.ini
    │   └── elf.h
    ├── compiler/
    └── utility/
```

---

# estd - Standard Library Extensions

## meta.h

Meta programming utilities for type introspection and trait generation.

### META_HAS_MEMBER_* Macros

Generate type traits to check for member existence.

```cpp
// Check for member type
META_HAS_MEMBER_TYPE(iterator);
// Generates: has_member_iterator, has_member_iterator_v

// Check for member data
META_HAS_MEMBER_DATA(size);
// Generates: has_member_size, has_member_size_v

// Check for member function
META_HAS_MEMBER_FUNCTION(begin);
// Generates: has_member_begin, has_member_begin_v

// Use to check if type is iterable
template <typename T>
struct is_iterable
    : std::bool_constant<has_member_begin_v<T> && has_member_end_v<T>> {};

template <typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;
```

### Meta Logic Operations

Use `std::conjunction`, `std::disjunction` from `<type_traits>` (C++17):

```cpp
// Combine multiple conditions
template <typename T>
using is_numeric = std::conjunction<std::is_arithmetic<T>, std::bool_constant<!std::is_same_v<T, bool>>>;
```

| Standard Utility | Description |
|-----------------|-------------|
| `std::conjunction<Ts...>` | Logical AND of type traits (short-circuit) |
| `std::conjunction_v<Ts...>` | Result of conjunction |
| `std::disjunction<Ts...>` | Logical OR of type traits (short-circuit) |
| `std::disjunction_v<Ts...>` | Result of disjunction |
| `std::bool_constant<!T::value>` | Logical NOT of type trait |

### System Detection

| Trait | Description |
|-------|-------------|
| `is_win_v` | Windows platform |
| `is_win64_v` | 64-bit Windows |
| `is_win32_v` | 32-bit Windows |
| `is_cygwin_v` | Cygwin environment |
| `is_unix_v` | Unix-like platform |
| `is_linux_v` | Linux platform |
| `is_free_bsd_v` | FreeBSD platform |
| `is_macos_v` | macOS platform |
| `is_android_v` | Android platform |

```cpp
#if es::is_linux_v
// Linux-specific code
#endif
```

### Type Traits

| Utility | Description |
|---------|-------------|
| `add_const_as<T, U>` | Add const to T as U's const-ness |
| `add_const_as_t<T, U>` | Result type |
| `remove_cvref<T>` | Remove cv-qualifiers and reference |
| `remove_cvref_t<T>` | Result type |
| `is_iterable_v<T>` | Check if type has begin() and end() |

### Array Traits

| Utility | Description |
|---------|-------------|
| `is_c_array_v<T>` | Check if T is C-style array |
| `is_std_array_v<T>` | Check if T is std::array |
| `array_traits<T>` | Get array properties (size, element type) |
| `to_std_array_t<T>` | Convert C-array to std::array |

```cpp
int arr[5];
static_assert(is_c_array_v<decltype(arr)>);

auto std_arr = to_std_array_t<decltype(arr)>{};
// std_arr is std::array<int, 5>
```

---

## bit.h

Bit manipulation operations, originated from **Hacker's Delight**.

### Counting Functions

| Function | Description |
|----------|-------------|
| `count_bit1(v)` | Count bits set to 1 |
| `count_bit0(v)` | Count bits set to 0 |
| `countl_bit0(v)` | Count leading zero bits |
| `countl_bit1(v)` | Count leading one bits |
| `countr_bit0(v)` | Count trailing zero bits |
| `countr_bit1(v)` | Count trailing one bits |

```cpp
unsigned value = 0b10101000;
count_bit1(value);     // 3
countr_bit0(value);    // 3 (trailing zeros)
```

### Highlight/Dim Functions

| Function | Description | Example |
|----------|-------------|---------|
| `hl_first_bit1(v)` | Isolate rightmost 1 | `10101000 → 00001000` |
| `hl_first_bit0(v)` | Isolate rightmost 0 | `10100111 → 00001000` |
| `dim_first_bit1(v)` | Clear rightmost 1 | `10101000 → 11110111` |
| `hlr_bit0(v)` | Highlight trailing zeros | `01011000 → 00000111` |
| `hlr_bit0_inc(v)` | Highlight trailing zeros + first 1 | `01011000 → 00001111` |
| `dimr_bit1(v)` | Dim trailing ones | `10100111 → 11111000` |
| `hlr_bit1_inc(v)` | Highlight trailing ones + first 0 | `01010111 → 00001111` |

### Power of 2 Functions

| Function | Description |
|----------|-------------|
| `is_pow2(v)` | Check if v is power of 2 |
| `is_pow2_sub1(v)` | Check if v = 2^n - 1 |
| `is_pow2_sub_pow2(v)` | Check if v = 2^n - 2^m |
| `floor_to_pow2(v)` | Round down to power of 2 |
| `ceil_to_pow2(v)` | Round up to power of 2 |

```cpp
is_pow2(8u);           // true
is_pow2_sub1(7u);      // true (7 = 2^3 - 1)
is_pow2_sub_pow2(12u); // true (12 = 2^4 - 2^2)
floor_to_pow2(100u);   // 64
ceil_to_pow2(100u);    // 128
```

### Get Functions

| Function | Description |
|----------|-------------|
| `get_bit(v, pos)` | Get bit at position |
| `get_bits(v, pos, n)` | Get n bits starting at position |
| `get_nth_bit(v, n)` | Get bitmask at nth set bit position |

```cpp
unsigned value = 0b11010110;
get_bit(value, 0);     // 0
get_bit(value, 1);     // 1
get_bits(value, 2, 3); // 0b101 (bits 2-4)

// get_nth_bit: extract position of nth set bit as bitmask
unsigned v = 0b10101010;  // set bits at positions 1, 3, 5, 7
get_nth_bit(v, 0);    // 0b00000010 (1st set bit at pos 1)
get_nth_bit(v, 1);    // 0b00001000 (2nd set bit at pos 3)
get_nth_bit(v, 4);    // 0 (no 5th set bit)
```

### Set/Reset Functions

| Function | Description |
|----------|-------------|
| `set_bit(v, pos)` | Set bit at position |
| `set_bits(v, pos, n)` | Set n bits starting at position |
| `reset_first(v)` | Clear rightmost 1 |
| `set_first(v)` | Set rightmost 0 |
| `resetr(v)` | Clear trailing 1s |
| `setr(v)` | Set trailing 0s |
| `reset_first_adjacent(v)` | Clear rightmost contiguous 1s |

```cpp
unsigned value = 0b01011100;
reset_first(value);          // 0b01011000
set_first(value);            // 0b01011101
resetr(value);               // 0b01010000
setr(value);                 // 0b01011111
reset_first_adjacent(value); // 0b01000000
```

---

## integer.h

Type-safe integer wrappers to prevent mixing different integer types.

```cpp
// Define distinct integer types
class __BitTag;
using Bit = Integer<uint32_t, __BitTag>;
constexpr Bit operator""_b(unsigned long long int v) {
  return Bit{static_cast<uint32_t>(v)};
}

class __ByteTag;
using Byte = Integer<uint32_t, __ByteTag>;
constexpr Byte operator""_B(unsigned long long int v) {
  return Byte{static_cast<uint32_t>(v)};
}

// Usage - types cannot be mixed
Bit bits = 32_b;
Byte bytes = 4_B;
// bits + bytes;  // Compile error!
```

---

## memory.h

Memory pool and allocator utilities.

### layout_bit_mapping<MemoryBytes, MemberBytes>

Bitmap-based allocator for small objects (< sizeof(void*)).

```cpp
char memory[4096];
layout_bit_mapping<4096, 64> allocator(memory);
allocator.reset();

void* member = allocator.acquire();
allocator.release(member);
```

**Memory Layout:**
```
┌──────────┬────────────────────────────────────┬──────┐
│  Header  │        Data Area (members)         │ Tail │
│  bitmap  │  member_capacity × MemberBytes     │unused│
└──────────┴────────────────────────────────────┴──────┘
```

### layout_stack

Stack-based memory segment manager using in-band pointers.

```cpp
layout_stack stack;
char mem[1024];
stack.push(mem, sizeof(mem));
char* top = stack.pop();
```

**Memory Layout:**
```
┌─────────────────────────────────────────────────────────────────┐
│  Segment 1    │  Segment 2    │  Segment 3    │  ...           │
│  ┌─────┬────┐ │  ┌─────┬────┐ │  ┌─────┬────┐ │                │
│  │prev │data│ │  │prev │data│ │  │prev │data│ │                │
│  │null │    │ │  │ &S1 │    │ │  │ &S2 │    │ │                │
│  └─────┴────┘ │  └─────┴────┘ │  └─────┴────┘ │                │
└─────────────────────────────────────────────────────────────────┘
```

### MemoryPoolUniform<Size>

Fixed-size memory pool.

```cpp
MemoryPoolUniform<64> pool;
void* mem = pool.allocate();
pool.deallocate(mem);
```

### ObjectPool<T>

Type-safe object pool.

```cpp
ObjectPool<Node> pool;

// Allocate and construct
Node* n = pool.operator_new(value);

// Destroy and return to pool
pool.operator_delete(n);

// RAII with make_unique
auto node = es::memory::make_unique(pool, arg1, arg2);
```

### SubObjectPool<T>

Thread-local object pool that merges to parent on destruction.

```cpp
ObjectPool<Task> global_pool;
std::mutex mtx;

void thread_func() {
    SubObjectPool<Task, alignof(Task), 4096, std::mutex>
        local_pool(global_pool, mtx);
    // Fast local allocation
    Task* t = local_pool.operator_new();
    // Memory merges to global_pool on thread exit
}
```

---

## flat_vector.h

Fixed-capacity vector with stack-allocated storage.

```cpp
flat_vector<100, int> vec;
vec.push_back(1);
vec.push_back(2);
// vec.size() == 2, vec.capacity() == 100
// No heap allocation
```

## small_vector.h

Vector with small vector optimization (SVO).

```cpp
// Uses flat_vector<N, T> for small vectors (stack)
// Switches to std::vector<T> when size exceeds N (heap)

small_vector<16, int> vec;
vec.push_back(1);        // Uses flat_vector<16, int> (stack)
vec.push_back(2);        // Still uses flat_vector<16, int>
for (int i = 0; i < 20; ++i) vec.push_back(i);  // Switches to std::vector<int> (heap)

// Check storage type
if (vec.is_small()) {
    // Stack allocated
}
```

**Key features:**
- Stack allocation for small vectors (size <= N)
- Automatic transition to heap when needed
- Standard vector interface
- `is_small()` to check storage type
- Uses union-based storage (no `std::variant` overhead)
- Conditional `noexcept` on move operations

---

## cast/

RTTI-free cast operations for class hierarchies.

### Value<T>

Base class for cast-enabled types.

```cpp
class Shape : public es::Value<void> {
public:
    static constexpr unsigned CLASSID = 1;
    Shape() : es::Value<void>(CLASSID) {}
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    static constexpr unsigned CLASSID = 2;
    Circle() : Shape() {}
    double radius;
};
```

### isa<T>(obj)

Check if object is of type T.

```cpp
Shape* s = new Circle();
if (es::isa<Circle>(s)) {
    // s is a Circle
}
```

### cast<T>(obj)

Cast object to type T (undefined if not T).

```cpp
Circle* c = es::cast<Circle>(s);
```

### dyn_cast<T>(obj)

Dynamic cast returning nullptr if not T.

```cpp
if (Circle* c = es::dyn_cast<Circle>(s)) {
    // Safe to use c
}
```

### type_id<T>()

Get unique runtime type identifier.

```cpp
size_t id = es::type_id<MyClass>();
// Same type returns same ID
assert(es::type_id<MyClass>() == id);
```

---

## string/

String utilities and containers.

### flat_string<N, OutOfRangeAssert>

Fixed-capacity string with stack storage.

```cpp
// Basic usage
flat_string<64> str;
str = "Hello";
str += " World";

// Custom overflow handler
struct LogOnOverflow {
    constexpr void operator()(bool cond, const char* msg) const {
        if (!cond) std::cerr << "Warning: " << msg << "\n";
    }
};

flat_string<32, LogOnOverflow> safe_str;
```

### small_string<N>

String with small string optimization (SSO).

```cpp
// Uses flat_string<N> for small strings (stack)
// Switches to std::string when size exceeds N (heap)

small_string<32> str;
str = "Hello";           // Uses flat_string<32> (stack)
str += " World";         // Still uses flat_string<32>
str += " very long...";  // Switches to std::string (heap)

// Check storage type
if (str.is_small()) {
    // Stack allocated
}
```

**Key features:**
- Stack allocation for small strings (size <= N-1)
- Automatic transition to heap when needed
- Standard string interface
- `is_small()` to check storage type

### StringTable<SetT>

String interning table for deduplication.

```cpp
#include <unordered_set>

StringTable<std::unordered_set> table;

std::string_view s1 = table.add("hello");
std::string_view s2 = table.add("hello");
// s1.data() == s2.data() - same pointer!

// Fast comparison by pointer
if (s1.data() == s2.data()) { /* same string */ }
```

### concat_buffer<N, Container>

Efficient string concatenation buffer.

```cpp
// Fixed buffer
concat_buffer<128> buf;
buf.append("Name: ", name, ", Age: ", age);
std::string result = buf.to_string();

// Dynamic buffer (auto-expands)
concat_buffer<0, std::string> dynamic;
dynamic.append("Very long string...");

// With lazy views
buf.append(str | es::string::views::to_upper);
```

### append(s, args...)

Append various types to string-like containers.

```cpp
std::string str;
es::string::append(str, "Value: ", 42, ", Status: ", "OK");
// str = "Value: 42, Status: OK"

// Append multiple at once
es::string::append_n(str, "Name: ", name, ", Age: ", age);
```

### Lazy Views

| View | Description |
|------|-------------|
| `to_upper` | Convert to uppercase |
| `to_lower` | Convert to lowercase |
| `dot_to_underline` | Replace `.` with `_` |

```cpp
std::string s = "hello.world";
auto upper = s | es::string::views::to_upper;  // "HELLO.WORLD"
auto under = s | es::string::views::dot_to_underline;  // "hello_world"
```

### String Traits

| Trait | Description |
|-------|-------------|
| `is_c_string_v<T>` | True if T converts to `const char*` |
| `is_string_like_v<T>` | True if T has string interface |

---

## utility.h

General utility functions and classes.

| Utility | Description |
|---------|-------------|
| `type_name<T>()` | Get type name as string_view |
| `NumericType` | Numeric type classification |
| `disabled_copy` | Delete copy operations |
| `disabled_move` | Delete move operations |
| `disabled_copy_move` | Delete both copy and move |
| `overloaded<Ts...>` | Combine multiple lambdas |
| `bytes_of(T)` | Get byte representation |
| `bits_of(T)` | Get bit representation |

```cpp
// Type name
std::cout << es::type_name<int>();  // "int"

// Overloaded pattern matching
auto visitor = es::overloaded{
    [](int i) { return "int"; },
    [](double d) { return "double"; },
    [](auto x) { return "other"; }
};
```

---

## concurrency.h

Thread safety utilities.

### is_mutex_like_v<T>

Check if type satisfies mutex interface.

```cpp
static_assert(es::is_mutex_like_v<std::mutex>);
```

### mutex_silent

No-op mutex for single-threaded scenarios.

```cpp
mutex_silent mtx;
mtx.lock();    // Does nothing
mtx.unlock();  // Does nothing
```

### ThreadSafeObjectWrapper

Wrap object with mutex for thread-safe access.

```cpp
std::mutex mtx;
Data data;

ThreadSafeObjectWrapper<std::mutex, Data> wrapper(data, mtx);
wrapper.invoke(&Data::method, arg1, arg2);
```

---

## idiom/

C++ idioms and patterns.

### pimpl<T, StorageSize>

Pimpl idiom with optional inline storage.

```cpp
// header
class Widget : public es::idiom::pimpl<Widget, 64> {
public:
    Widget(int value);
    ~Widget() noexcept;
    void doSomething();
};

// implementation
class WidgetImpl {
public:
    WidgetImpl(int v) : value(v) {}
    void doSomething() { /* ... */ }
private:
    int value;
};

template<>
struct es::idiom::pimpl_traits<Widget> {
    using impl_type = WidgetImpl;
};

Widget::Widget(int v) : pimpl(v) {}
Widget::~Widget() noexcept = default;
void Widget::doSomething() { pimpl_cast(this)->doSomething(); }
```

---

# cxxaux - Utility Modules

## binary/binary_decoder.h

Stream-like binary data decoder.

```cpp
cxxaux::BinaryDecoder decoder(data, size);

// Position control
decoder.seekg(0);
uint32_t pos = decoder.tellg();

// Read values
uint32_t value = decoder.get<uint32_t>();
std::string_view str = decoder.peekn(10);

// Skip until pattern
decoder.ignore("\n");
```

### Key Methods

| Method | Description |
|--------|-------------|
| `seekg(pos)` | Set cursor position |
| `tellg()` | Get cursor position |
| `get<T>()` | Read and extract value |
| `peek<T>()` | Read without extracting |
| `peekn(n)` | Read n chars without extracting |
| `ignore(s)` | Skip until pattern found |

---

## binary/elf

Header-only ELF format parser with standalone format definitions.

### File Structure

| File | Description |
|------|-------------|
| `elf_format.h` | ELF format definitions (types, structs, enums) — no dependencies |
| `elf.ini` | Internal implementation details (`detail` namespace) |
| `elf.h` | Public API — include this to use the parser |

### Usage

```cpp
#include <cxxaux/binary/elf.h>

// Parse ELF binary from raw data
auto elf = cxxaux::elf::parseElf(data, size);
if (!elf) { /* not a valid little-endian ELF */ }

// Or parse directly by architecture:
// auto elf32 = cxxaux::elf::parseElf32(data, size);
// auto elf64 = cxxaux::elf::parseElf64(data, size);

// Header metadata
auto bits = elf->archBits();   // EIClass::ELFCLASS32 or ELFCLASS64
auto enc  = elf->encoding();   // EIData::ELFDATA2LSB
auto type = elf->type();       // EType::ET_EXEC, ET_DYN, etc.
auto mach = elf->machine();    // EMachine::EM_X86_64, etc.

// Access sections
for (size_t i = 0; i < elf->s_size(); ++i) {
    auto sec = elf->s_at(i);
    std::cout << sec->name() << " type=" << static_cast<uint32_t>(sec->type())
              << " size=" << sec->content().second << "\n";
}

// Access program headers
for (size_t i = 0; i < elf->p_size(); ++i) {
    auto prog = elf->p_at(i);
    std::cout << "segment vaddr=0x" << std::hex << prog->vaddr() << "\n";
}

// Dump function symbols to any output stream
elf->dump(std::cout);
```

### Key Classes

| Class | Description |
|-------|-------------|
| `Elf` | Abstract interface for a parsed ELF file |
| `Section` | Parsed section with name, type, flags, content |
| `Program` | Parsed program header with type, flags, layout |
| `ElfImpl<BitNArch>` | Concrete implementation (32/64-bit) |

### Key Functions

| Function | Description |
|----------|-------------|
| `parseElf(data, size)` | Factory: auto-detect class, return `unique_ptr<Elf>` |
| `parseElf32(data, size)` | Parse as ELF32, return `unique_ptr<Elf>` |
| `parseElf64(data, size)` | Parse as ELF64, return `unique_ptr<Elf>` |
| `isElfFile(data, size)` | Check if data starts with ELF magic |

### Format Definitions (`elf_format.h`)

All ELF format types and constants are defined independently (no LLVM dependency):

- **Basic types**: `Elf32_Addr`, `Elf64_Off`, etc.
- **Structs**: `Elf32_Ehdr`/`Elf64_Ehdr`, `Elf32_Phdr`/`Elf64_Phdr`, `Elf32_Shdr`/`Elf64_Shdr`, `Elf32_Sym`/`Elf64_Sym`, `Elf32_Rel`/`Elf64_Rel`, `Elf32_Rela`/`Elf64_Rela`, `Elf32_Dyn`/`Elf64_Dyn`, `Elf32_Nhdr`/`Elf64_Nhdr`
- **Enums**: `EIClass`, `EIData`, `EIVersion`, `EIOSABI`, `EType`, `EMachine`, `EVersion`, `PType`, `PFlags`, `SHType`, `SHFlags`, `STType`, `STBind`, `STVis`, `DTag`, `NTCore`, `NTGNU`, `NTDefault`
- **Architecture aliases**: `Bit32Arch`, `Bit64Arch`

---

## utility/logger_wrapper

Unified logger interface.

```cpp
// Use with spdlog
#include <cxxaux/utility/logger_wrapper_spdlog.h>
using Logger = cxxaux::LoggerWrapperSpdlog;

// Use with std::ostream
#include <cxxaux/utility/logger_wrapper_simple.h>
using Logger = cxxaux::LoggerWrapperSimple;

// Unified API
Logger::info("Message: {}", value);
Logger::error("Error occurred");
Logger::debug("Debug info");
```

---

## utility/memory_pool

Simple memory pool for general use.

```cpp
cxxaux::MemoryPool pool(1024);  // 1KB pool

void* mem = pool.allocate(64);
pool.deallocate(mem);
```

---

## utility/guard.h

RAII guard utilities.

```cpp
// Execute on scope exit
auto guard = cxxaux::make_guard([&]() {
    cleanup();
});

// Release guard early
guard.release();
```

---

## utility/version.h

Version management with efficient bit-based identifiers.

```cpp
#include <cxxaux/utility/version.h>

using VID = cxxaux::VersionID<uint32_t>;
using VInfo = cxxaux::VersionInfo<VID>;
using VMgr = cxxaux::VersionManager<VInfo>;

// Create version identifiers from offsets
VID v1(0);   // Version at offset 0
VID v2(5);   // Version at offset 5

// VersionIDSet for efficient set operations
cxxaux::VersionIDSet<VID> set(v1, v2);
set.has(v1);      // true
set.size();       // 2
set.foreach([](VID id) { /* iterate */ });

// VersionManager for registration and lookup
VMgr mgr;
auto [ptr, inserted] = mgr.emplace(VID(0), VID(0), "v1.0");
if (inserted) {
    // Successfully added
}

if (auto* info = mgr.find(VID(0))) {
    std::cout << info->name << "\n";  // "v1.0"
}
```

### Key Classes

| Class | Description |
|-------|-------------|
| `VersionID<BitSetT>` | Single version identifier (bit position) |
| `VersionIDSet<VersionID>` | Set of version identifiers with set operations |
| `VersionInfo<VersionID>` | Base struct for version information |
| `VersionManager<VersionInfo>` | Manages registration and lookup |

### VersionID Methods

| Method | Description |
|--------|-------------|
| `VersionID(offset)` | Construct from bit offset |
| `valid()` | Check if valid (non-zero) |
| `offset()` | Get bit offset |
| `value()` | Get underlying bit value |

### VersionIDSet Methods

| Method | Description |
|--------|-------------|
| `add(id)` / `remove(id)` | Modify the set |
| `has(id)` / `has(set)` | Check membership |
| `size()` / `empty()` | Query size |
| `at(index)` / `first()` | Access elements |
| `\|` / `&` | Set union/intersection |
| `foreach(func)` | Iterate over elements |

### VersionManager Methods

| Method | Description |
|--------|-------------|
| `emplace(id, args...)` | Construct in-place, returns `{ptr, inserted}` |
| `add(id, info)` | Add by copy/move, returns `{ptr, inserted}` |
| `find(id)` | Find by VersionID |
| `has(id)` | Check if registered |
| `foreach(func)` | Iterate over all registered |
| `size()` / `capacity()` | Query size |
