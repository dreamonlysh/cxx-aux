# cxx-aux

Commonly used lite libraries and C++ standard library extensions.

## Features

- **Header-only** - Most components are header-only, easy to integrate
- **C++17** - Requires C++17 or later
- **No external dependencies** - Only depends on C++ standard library

## Quick Start

### As Header-Only Library

Just copy the `include` directory to your project:

```cmake
# CMakeLists.txt
target_include_directories(your_target PRIVATE /path/to/cxx-aux/include)
```

Or use CMake FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(
  cxxaux
  GIT_REPOSITORY https://github.com/guyuemeng/cxx-aux.git
  GIT_TAG main
)
FetchContent_MakeAvailable(cxxaux)

target_link_libraries(your_target PRIVATE cxxaux::cxxaux)
```

### Usage Example

```cpp
#include <estd/bit.h>
#include <estd/memory.h>
#include <estd/string/flat_string.h>
#include <iostream>

int main() {
    // Bit operations
    unsigned value = 0b10101010;
    std::cout << "Bit count: " << es::count_bit1(value) << "\n";
    
    // Fixed-capacity string (no heap allocation)
    es::string::flat_string<64> str;
    str = "Hello";
    str += " World";
    std::cout << str << "\n";
    
    return 0;
}
```

## Modules

### estd - Standard Library Extensions

| Module | Description |
|--------|-------------|
| `meta.h` | Meta programming utilities |
| `bit.h` | Bit manipulation operations |
| `integer.h` | Type-safe integer wrappers |
| `memory.h` | Memory pools and allocators |
| `flat_vector` | Fixed-capacity vector |
| `cast/` | RTTI-free cast operations (`isa`, `cast`, `dyn_cast`) |
| `string/` | String utilities (`flat_string`, `string_table`, `concat_buffer`) |
| `idiom/` | C++ idioms (`pimpl`) |

### cxxaux - Utility Modules

| Module | Description |
|--------|-------------|
| `binary/binary_decoder` | Binary data decoder |
| `binary/elf/` | ELF format parser |
| `utility/logger_wrapper` | Unified logger interface |
| `utility/memory_pool` | Simple memory pool |

## Build & Test

```bash
# Build
cmake -B build -DCXXAUX_ENABLE_TESTS=ON
cmake --build build

# Run tests
cd build && ctest
```

## Examples

See the [example/](example/) directory for detailed examples:

- [bit/](example/bit/) - Bit manipulation examples
- [cast/](example/cast/) - Cast operation examples
- [memory/](example/memory/) - Memory pool examples
- [string/](example/string/) - String utility examples
- [idiom/](example/idiom/) - Pimpl idiom examples

## Documentation

See [API_GUIDE.md](API_GUIDE.md) for detailed API documentation.

## License

Licensed under [Mulan Permissive Software License, Version 2 (Mulan PSL v2)](LICENSE).

```
Copyright (c) 2024 guyuemeng
cxx-aux is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
```
