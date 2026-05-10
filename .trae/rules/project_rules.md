# cxx-aux Project Rules

## Project Overview

cxx-aux is a C++17 header-only auxiliary library providing lightweight containers, string utilities, memory pools, RTTI-free type casting, and standard library extensions. Licensed under Mulan PSL v2.

## Build Commands

```bash
# Configure (Release)
cmake -B build_release -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCXXAUX_ENABLE_TESTS=ON

# Configure (Debug)
cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCXXAUX_ENABLE_TESTS=ON

# Build
cmake --build build_release --parallel

# Run tests
cd build_release && ctest --output-on-failure -V

# Configure with sanitizers
cmake -B build_asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCMAKE_C_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCXXAUX_ENABLE_TESTS=ON

# Run static analysis
clang-tidy -p build_debug <source-file>
```

## Code Style

- Based on LLVM style with left-aligned pointers (`int* ptr`, not `int *ptr`)
- Always break template declarations
- Compact namespaces
- Run `clang-format` before committing
- No comments unless explicitly requested

## C++ Standard & Constraints

- **C++17** required, no C++20/23 features
- **Header-only** for `estd/` and most `cxxaux/` components
- **No external dependencies** — only C++ standard library
- **No RTTI** — the project provides its own type casting system (`isa`, `cast`, `dyn_cast`)
- **Cross-platform** — must compile on macOS, Linux; avoid x86-only intrinsics without guards

## Project Structure

```
include/
  estd/           # Standard library extensions (header-only)
    __bit/        # Bit manipulation (Hacker's Delight algorithms)
    __container/  # flat_vector, small_vector, maybe_owned, Integer
    __memory/     # ObjectPool, MemoryPoolUniform, layout_bit_mapping
    __meta/       # Type traits, has_member, system detection
    __ranges/     # C++20-like ranges views (filter, transform, take, etc.)
    __utility/    # bits_of, bytes_of, type_name, numeric_type
    __concurrency/# ThreadSafeObjectWrapper, mutex_silent
    cast/         # RTTI-free isa/cast/dyn_cast/type_id
    idiom/        # pimpl
    string/       # flat_string, small_string, concat_buffer, StringTable
  cxxaux/         # General utilities
    binary/       # ELF parser (elf_format.h, elf.ini, elf.h), binary_decoder
    compiler/     # Type system for compiler development
    utility/      # logger_wrapper, memory_pool, guard, version
  experiment/     # Experimental features (dl_load, factory, range)
src/              # Non-header-only implementations (memory_pool)
tests/            # GoogleTest-based tests
example/          # Usage examples
third_party/      # LLVM ELF headers, OpenArkCompiler utilities
```

## Naming Conventions

- Public headers: `estd/flat_vector.h`, `estd/string/flat_string.h`
- Internal headers: `estd/__container/flat_vector.h` (double underscore prefix = internal)
- Test files: `<module>_test.cpp` (e.g., `flat_vector_test.cpp`)
- Namespace: `estd` for standard library extensions, `cxxaux` for general utilities

## Key Design Patterns

- **SVO/SSO**: `small_vector<N, T>` and `small_string<N>` use inline storage for N elements
- **Fixed-capacity**: `flat_vector<N, T>` and `flat_string<N>` never heap-allocate
- **Object pools**: `ObjectPool<T>` for type-safe pooled allocation
- **RTTI-free casting**: `isa<T>()`, `cast<T>()`, `dyn_cast<T>()` based on `type_id<T>()`
- **String interning**: `StringTable` for pointer-compare string deduplication
- **RAII guards**: `guard.h` for scope-based resource management

## Design Principles: KISS · DRY · SOLID · YAGNI

Apply in priority order when they conflict:

| Priority | Principle | C++ Meaning |
|----------|-----------|-------------|
| 1 | **YAGNI** | No template params for hypothetical types, no virtual methods with one override |
| 2 | **KISS** | No clever metaprogramming when a plain function suffices |
| 3 | **DRY** | Factor repeated logic into shared helpers, but small clear duplication > clever abstraction |
| 4 | **SOLID** | Single responsibility, extend by addition, minimal interfaces |

## Known Issues (from optimization scan)

All previously identified issues have been resolved:

1. ~~`flat_vector` does not call destructors on elements~~ — **Fixed**: `std::destroy` used in destructor, `clear`, `erase`, `pop_back`, `resize`
2. ~~`flat_string::data() const` writes null terminator~~ — **Fixed**: Eager null termination in all modifying operations, `data() const` is pure reader
3. ~~`flat_string::find_last_not_of(value_type c)` calls `rfind(c)`~~ — **Fixed**: Proper backward search for char NOT equal to c
4. ~~`bit_get.h` unconditionally includes `<immintrin.h>`~~ — **Fixed**: Guarded with `#if defined(__BMI2__)` and platform check
5. ~~`small_vector` uses `std::variant` with `std::visit` overhead~~ — **Fixed**: Replaced with union+bool, direct branching
