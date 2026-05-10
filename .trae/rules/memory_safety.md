# C++ Memory Safety Checklist

Specialized checklist for reviewing memory management code in cxx-aux. This project implements custom containers, memory pools, and allocators — memory safety is the #1 priority.

## When to Use

When reviewing or modifying code in `__container/`, `__memory/`, `string/`, or any code that uses placement new, raw storage, or manual lifetime management.

## Container Safety Checklist

### Construction / Destruction

- [ ] Every `placement new` has a corresponding `std::destroy_at` (or manual destructor call)
- [ ] Destructor calls `std::destroy(begin(), end())` for non-trivially-destructible types
- [ ] `pop_back()` destroys the removed element before decrementing size
- [ ] `erase()` destroys erased elements
- [ ] `clear()` destroys all elements
- [ ] `resize()` destroys elements when shrinking, constructs when growing
- [ ] `swap()` properly handles element destruction in both containers
- [ ] Move constructor/assignment leaves source in valid destructible state

### Storage Management

- [ ] `std::byte`/`char` storage buffers are properly aligned for `T` (`alignas(T)`)
- [ ] `std::launder` is used when accessing objects through `byte` storage pointer
- [ ] No UB from accessing uninitialized storage through typed pointer
- [ ] `capacity()` never exceeds the actual storage size
- [ ] `size()` never exceeds `capacity()`

### Copy / Move Semantics

- [ ] Copy constructor deep-copies all elements
- [ ] Move constructor transfers ownership and nullifies source
- [ ] Copy assignment releases old resources before copying
- [ ] Move assignment releases old resources before transferring
- [ ] Self-assignment is handled correctly (copy-and-swap or explicit check)
- [ ] `noexcept` on move operations where possible

### Iterator Safety

- [ ] Iterators remain valid only as documented (especially after insert/erase)
- [ ] End iterators are not dereferenceable
- [ ] `operator[]` and `at()` have correct bounds (0 <= index < size())

## Memory Pool Safety Checklist

### Allocation / Deallocation

- [ ] `allocate()` returns properly aligned memory
- [ ] `deallocate()` doesn't double-free
- [ ] Pool doesn't return memory from freed blocks
- [ ] Thread-safe pools use proper synchronization (or document single-threaded requirement)
- [ ] `layout_bit_mapping` bit operations don't overflow or underflow
- [ ] Object pool `acquire()` constructs the object, `release()` destroys it

### Lifetime Management

- [ ] Pool outlives all objects allocated from it
- [ ] No dangling references to pool-allocated objects after pool destruction
- [ ] `SubObjectPool` correctly merges thread-local pools to global pool
- [ ] Pool growth strategy doesn't invalidate existing allocations

## String Safety Checklist

### Null Termination

- [ ] `data()` and `c_str()` always return null-terminated strings
- [ ] Null terminator is maintained after every mutation, not lazily in `data() const`
- [ ] `const` methods don't modify the string buffer (no lazy null termination)
- [ ] `operator[]` at `size()` returns `'\0'` (as required by C++11+)

### String Operations

- [ ] `find_last_not_of(c)` searches for characters NOT equal to `c` (not `rfind(c)`)
- [ ] `compare()` doesn't read past the end of either string
- [ ] `substr()` handles `npos` position correctly
- [ ] `append()`/`insert()` check capacity before writing
- [ ] `traits_type::length()` is not called redundantly on the same C-string

## Concurrency Safety Checklist

- [ ] `ThreadSafeObjectWrapper` lock scope is minimal but correct
- [ ] No data races on internal state (especially `mutable` members)
- [ ] `mutex_silent` doesn't hide deadlock-prone lock patterns
- [ ] Lock ordering is consistent across the codebase
- [ ] No `const` methods that modify state visible to other threads

## Common Patterns in cxx-aux to Watch For

| Pattern | Risk | Check |
|---------|------|-------|
| `std::byte storage_[N]` | Elements not destroyed | Verify destructor calls `std::destroy` |
| `placement new` into buffer | Missing `std::launder` | Verify access through laundered pointer |
| `cursor_` pointer tracking | Off-by-one in size calculation | Verify `cursor_ - data() == size()` |
| `layout_bit_mapping` | Bit manipulation errors | Verify with edge cases (0, 1, max bits) |
| `#include <immintrin.h>` | Non-portable | Verify `#if defined(__BMI2__)` guard |
| `size_t sz = SIZE_MAX` | Default param semantics | Verify intent is "skip size check" |
