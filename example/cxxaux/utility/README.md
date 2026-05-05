# Version Management Examples

This directory contains examples demonstrating the `cxxaux::utility::version` library for version management.

## Examples

### version.cpp
**Version management with bit-based identifiers**

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

**Compile:**
```bash
g++ -std=c++17 -I include example/cxxaux/utility/version.cpp -o version
./version
```

## API Summary

### VersionID

| Method | Description |
|--------|-------------|
| `VersionID()` | Default constructor (invalid) |
| `VersionID(offset)` | Construct from bit offset |
| `set(offset)` | Set the bit offset |
| `valid()` | Check if valid (non-zero) |
| `offset()` | Get bit offset |
| `value()` | Get underlying bit value |

### VersionIDSet

| Method | Description |
|--------|-------------|
| `VersionIDSet()` | Default constructor (empty set) |
| `VersionIDSet(id...)` | Construct from VersionIDs |
| `add(id)` | Add a VersionID to the set |
| `remove(id)` | Remove a VersionID from the set |
| `has(id)` | Check if set contains VersionID |
| `has(set)` | Check if set contains all VersionIDs |
| `has_any(set)` | Check if set contains any VersionIDs |
| `size()` | Number of VersionIDs in set |
| `empty()` | Check if set is empty |
| `at(index)` | Get VersionID at index |
| `first()` | Get first (lowest offset) VersionID |
| `operator\|` | Set union |
| `operator&` | Set intersection |
| `foreach(func)` | Iterate over all VersionIDs |

### VersionInfo

| Field/Method | Description |
|--------------|-------------|
| `id` | The version identifier |
| `name` | Optional version name |
| `as<Derived>()` | Cast to derived type |

### VersionManager

| Method | Description |
|--------|-------------|
| `capacity()` | Maximum number of versions (32 for uint32_t) |
| `size()` | Number of registered versions |
| `at(index)` | Access VersionInfo at offset |
| `emplace(id, args...)` | Construct in-place, returns `{ptr, inserted}` |
| `add(id, info)` | Add by copy/move, returns `{ptr, inserted}` |
| `find(id)` | Find by VersionID, returns pointer or nullptr |
| `has(id)` | Check if VersionID is registered |
| `has(set)` | Check if all VersionIDs are registered |
| `has_any(set)` | Check if any VersionIDs are registered |
| `foreach(func)` | Iterate over all registered VersionInfo |

## Common Patterns

### Register multiple versions
```cpp
VMgr mgr;
mgr.emplace(VID(0), VID(0), "v1.0");
mgr.emplace(VID(1), VID(1), "v1.1");
mgr.emplace(VID(2), VID(2), "v2.0");
```

### Check version support
```cpp
VIDSet supported(VID(0), VID(1), VID(2));
VIDSet required(VID(1), VID(2));

if (supported.has(required)) {
    // All required versions are supported
}
```

### Iterate over versions
```cpp
mgr.foreach([](VInfo& info) {
    std::cout << info.name << "\n";
});
```

### Extend VersionInfo
```cpp
struct MyVersionInfo : cxxaux::VersionInfo<VID> {
    int major, minor, patch;
    
    MyVersionInfo(VID id, const char* name, int maj, int min, int pat)
        : VersionInfo(id, name), major(maj), minor(min), patch(pat) {}
};

cxxaux::VersionManager<MyVersionInfo> mgr;
mgr.emplace(VID(0), VID(0), "v1.0.0", 1, 0, 0);
```

## Notes

- `VersionID` uses bit positions, enabling efficient set operations
- `VersionManager` manages object lifetime using placement new
- `emplace()` and `add()` return `{pointer, inserted}` like `std::map::insert`
- Capacity is determined by the bitset type (32 for `uint32_t`, 64 for `uint64_t`)
