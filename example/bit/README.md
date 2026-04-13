# Bit Manipulation Examples

This directory contains examples demonstrating the `es::bit` library for bit manipulation.

## Examples

### bit.cpp
**Comprehensive bit manipulation utilities**

```cpp
#include <estd/bit.h>

// Counting bits
unsigned value = 0b10101010;
es::count_bit1(value);    // Count set bits
es::count_bit0(value);    // Count clear bits
es::countl_bit0(value);   // Count leading zeros
es::countr_bit0(value);   // Count trailing zeros

// Highlight/Dim operations
es::hl_first_bit1(value);  // Isolate rightmost 1
es::hl_first_bit0(value);  // Find rightmost 0
es::dim_first_bit1(value); // Clear rightmost 1

// Power of 2 operations
es::is_pow2(8u);           // true
es::floor_to_pow2(100u);   // 64
es::ceil_to_pow2(100u);    // 128

// Get/Set bits
es::get_bit(value, 3);     // Get bit at position 3
es::get_bits(value, 2, 4); // Get 4 bits starting at position 2
es::set_bit(0u, 5);        // Create mask with bit 5 set
es::set_bits(0u, 2, 4);    // Create mask with 4 bits set starting at 2
```

**Compile:**
```bash
g++ -std=c++17 -I include example/bit/bit.cpp -o bit
./bit
```

## API Summary

### Counting Functions

| Function | Description |
|----------|-------------|
| `count_bit1(v)` | Count bits set to 1 |
| `count_bit0(v)` | Count bits set to 0 |
| `countl_bit0(v)` | Count leading zero bits |
| `countl_bit1(v)` | Count leading one bits |
| `countr_bit0(v)` | Count trailing zero bits |
| `countr_bit1(v)` | Count trailing one bits |

### Highlight/Dim Functions

| Function | Description | Example |
|----------|-------------|---------|
| `hl_first_bit1(v)` | Highlight rightmost 1 | `10101000 → 00001000` |
| `hl_first_bit0(v)` | Highlight rightmost 0 | `10100111 → 00001000` |
| `dim_first_bit1(v)` | Dim rightmost 1 | `10101000 → 11110111` |
| `hlr_bit0(v)` | Highlight trailing zeros | `01011000 → 00000111` |
| `hlr_bit0_inc(v)` | Highlight trailing zeros + first 1 | `01011000 → 00001111` |
| `dimr_bit1(v)` | Dim trailing ones | `10100111 → 11111000` |
| `hlr_bit1_inc(v)` | Highlight trailing ones + first 0 | `01010111 → 00001111` |

### Power of 2 Functions

| Function | Description |
|----------|-------------|
| `is_pow2(v)` | Check if v is a power of 2 |
| `is_pow2_sub1(v)` | Check if v = 2^n - 1 |
| `is_pow2_sub_pow2(v)` | Check if v = 2^n - 2^m |
| `floor_to_pow2(v)` | Round down to power of 2 |
| `ceil_to_pow2(v)` | Round up to power of 2 |

### Get/Set Functions

| Function | Description |
|----------|-------------|
| `get_bit(v, pos)` | Get single bit at position |
| `get_bits(v, pos, n)` | Get n bits starting at position |
| `set_bit(v, pos)` | Set bit at position |
| `set_bits(v, pos, n)` | Set n bits starting at position |
| `reset_first(v)` | Clear rightmost 1 |
| `set_first(v)` | Set rightmost 0 |
| `resetr(v)` | Clear trailing 1s |
| `setr(v)` | Set trailing 0s |
| `reset_first_adjacent(v)` | Clear rightmost contiguous 1s |

## Common Patterns

### Iterate through set bits
```cpp
unsigned bits = 0b10101010;
while (bits != 0) {
    unsigned pos = es::countr_bit0(es::hl_first_bit1(bits));
    // Process bit at position pos
    bits = es::reset_first(bits);
}
```

### Find contiguous bit range
```cpp
unsigned value = 0b01110000;
unsigned start = es::countr_bit0(value);
unsigned length = es::count_bit1(es::hlr_bit0_inc(es::resetr(value)));
```

### Create bit mask
```cpp
unsigned mask = es::set_bits(0u, 4, 8);  // 0x00000FF0
```

## Notes

- All functions work only with unsigned integer types
- Functions are `constexpr` and can be used at compile-time
- Uses compiler intrinsics for optimal performance
