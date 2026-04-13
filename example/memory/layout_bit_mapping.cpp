// layout_bit_mapping.h example: bitmap-based memory allocation
//
// layout_bit_mapping manages a large memory block with a bitmap header for
// tracking allocation status. It is specifically designed for small objects
// that are too small to hold a pointer (e.g., objects smaller than
// sizeof(void*)).
//
// Memory Layout:
// ┌─────────────────────────────────────────────────────────────────┐
// │  Header (bitmap)  │         Data Area (members)      │   Tail   │
// │  N bytes          │  member_capacity * MemberSize    │ unused   │
// └─────────────────────────────────────────────────────────────────┘
//
// The header is a bitmap where each bit represents whether a member slot is:
//   - bit 0: slot is available (not acquired)
//   - bit 1: slot is in use (acquired)
//
// For small objects that cannot store a pointer, we cannot use a linked list
// to track free slots. Instead, the bitmap header provides O(1) allocation
// and deallocation by tracking slot status externally.

#include <cstring>
#include <estd/__memory/layout_bit_mapping.h>
#include <iostream>

int main() {
  std::cout << "=== layout_bit_mapping Example ===\n\n";

  std::cout << "=== Concept ===\n";
  std::cout
      << "layout_bit_mapping uses a bitmap header to track slot status.\n";
  std::cout
      << "This is ideal for small objects (< sizeof(void*)) that cannot\n";
  std::cout << "hold a pointer for linked-list based tracking.\n\n";

  std::cout << "Memory layout example (256 bytes, 32-byte members):\n";
  std::cout << "┌──────────┬────────────────────────────────────┬──────┐\n";
  std::cout << "│  Header  │        Data Area (members)         │ Tail │\n";
  std::cout << "│  8 bytes │  7 slots × 32 bytes = 224 bytes   │24 B  │\n";
  std::cout << "└──────────┴────────────────────────────────────┴──────┘\n\n";

  std::cout << "1. Basic allocation and release:\n";
  {
    constexpr size_t MemorySize = 256;
    constexpr size_t MemberSize = 32;
    char memory[MemorySize];
    std::memset(memory, 0, MemorySize);

    es::memory::layout_bit_mapping<MemorySize, MemberSize> allocator(memory);

    std::cout << "  Before reset - bitmap header is uninitialized\n";
    allocator.reset();
    std::cout
        << "  After reset - bitmap header marks all slots as available\n\n";

    std::cout << "  Member capacity: " << decltype(allocator)::member_capacity
              << "\n";
    std::cout << "  Header overhead: "
              << decltype(allocator)::member_bytes_offset << " bytes\n";
    std::cout << "  Tail bytes: "
              << decltype(allocator)::memory_tail_bytes_unmapping << " bytes\n";
    std::cout << "  Usable data: "
              << MemorySize - decltype(allocator)::member_bytes_offset
              << " bytes\n\n";

    std::cout << "  Bitmap state: [0 0 0 0 0 0 0] (0=available, 1=used)\n";

    void* m1 = allocator.acquire();
    std::cout << "  After acquire m1: [1 0 0 0 0 0 0]\n";

    void* m2 = allocator.acquire();
    std::cout << "  After acquire m2: [1 1 0 0 0 0 0]\n";

    void* m3 = allocator.acquire();
    std::cout << "  After acquire m3: [1 1 1 0 0 0 0]\n";

    std::cout << "  Acquired addresses:\n";
    std::cout << "    m1: " << m1 << " (offset "
              << static_cast<char*>(m1) - memory << ")\n";
    std::cout << "    m2: " << m2 << " (offset "
              << static_cast<char*>(m2) - memory << ")\n";
    std::cout << "    m3: " << m3 << " (offset "
              << static_cast<char*>(m3) - memory << ")\n\n";

    allocator.release(m2);
    std::cout
        << "  After release m2: [1 0 1 0 0 0 0] (slot 1 becomes available)\n";

    void* m4 = allocator.acquire();
    std::cout << "  After acquire m4: [1 1 1 0 0 0 0] (reuses slot 1)\n";
    std::cout << "  m4 = " << m4
              << " (same as m2: " << (m4 == m2 ? "yes" : "no") << ")\n\n";

    allocator.release(m1);
    allocator.release(m3);
    allocator.release(m4);
    std::cout << "  After release all: [0 0 0 0 0 0 0]\n";
  }
  std::cout << "\n";

  std::cout << "2. Why bitmap for small objects:\n";
  {
    std::cout << "  sizeof(void*) = " << sizeof(void*)
              << " bytes on this platform\n";
    std::cout << "  For objects >= " << sizeof(void*)
              << " bytes, we can use the object's\n";
    std::cout << "  memory to store a pointer to the next free slot (like "
                 "layout_stack).\n\n";

    std::cout << "  For objects < " << sizeof(void*) << " bytes:\n";
    std::cout << "    - Cannot store a pointer in the object's memory\n";
    std::cout << "    - Need external tracking mechanism\n";
    std::cout << "    - Bitmap header is the solution: 1 bit per slot\n\n";

    std::cout << "  Example: 4-byte objects\n";
    std::cout << "    - With 8-byte bitmap header, can track up to 64 slots\n";
    std::cout << "    - Each slot is only 4 bytes, too small for a pointer\n";
    std::cout
        << "    - Bitmap efficiently tracks which 4-byte slots are free\n";
  }
  std::cout << "\n";

  std::cout << "3. Hierarchy levels for larger memory:\n";
  {
    std::cout
        << "  For small memory blocks (< ~512 bytes with small members):\n";
    std::cout << "    Hierarchy 1: Single bitmap header\n";
    std::cout << "    ┌────────┬─────────────────┐\n";
    std::cout << "    │ H1 map │  Member slots   │\n";
    std::cout << "    └────────┴─────────────────┘\n\n";

    std::cout << "  For larger memory blocks:\n";
    std::cout << "    Hierarchy 2: Two-level bitmap\n";
    std::cout << "    ┌────────┬────────────────┬─────────────────┐\n";
    std::cout << "    │ H1 map │  H2 maps       │  Member slots   │\n";
    std::cout << "    └────────┴────────────────┴─────────────────┘\n";
    std::cout << "    H1 map points to H2 maps, H2 maps point to members\n\n";

    using SmallMapping = es::memory::layout_bit_mapping<64, 8, unsigned char>;
    using LargeMapping =
        es::memory::layout_bit_mapping<4096, 64, unsigned long long>;

    std::cout << "  SmallMapping (64 bytes, 8-byte members, unsigned char):\n";
    std::cout << "    Header: " << SmallMapping::member_bytes_offset
              << " byte (1 byte = 8 bits)\n";
    std::cout << "    Capacity: " << SmallMapping::member_capacity
              << " slots\n";
    std::cout << "    Uses Hierarchy 1 (single level)\n\n";

    std::cout << "  LargeMapping (4096 bytes, 64-byte members, unsigned long "
                 "long):\n";
    std::cout << "    Header: " << LargeMapping::member_bytes_offset
              << " bytes\n";
    std::cout << "    Capacity: " << LargeMapping::member_capacity
              << " slots\n";
  }
  std::cout << "\n";

  std::cout << "4. Exhausting the pool:\n";
  {
    constexpr size_t MemorySize = 128;
    constexpr size_t MemberSize = 16;
    char memory[MemorySize];
    std::memset(memory, 0, MemorySize);

    es::memory::layout_bit_mapping<MemorySize, MemberSize> allocator(memory);
    allocator.reset();

    std::cout << "  Memory: " << MemorySize << " bytes, Member: " << MemberSize
              << " bytes\n";
    std::cout << "  Capacity: " << decltype(allocator)::member_capacity
              << " members\n\n";

    void* members[10];
    int count = 0;
    while (void* m = allocator.acquire()) {
      members[count++] = m;
    }

    std::cout << "  Allocated all " << count << " slots\n";
    std::cout << "  Bitmap: [1 1 1 1 1 1 1] (all in use)\n\n";

    std::cout << "  Trying to acquire one more...\n";
    void* failed = allocator.acquire();
    std::cout << "  Result: " << (failed ? "got memory" : "nullptr (pool full)")
              << "\n\n";

    allocator.release(members[2]);
    std::cout << "  Released slot 2: [1 1 0 1 1 1 1]\n";

    void* reused = allocator.acquire();
    std::cout << "  Acquired again: " << reused << "\n";
    std::cout << "  Reused slot 2: " << (reused == members[2] ? "yes" : "no")
              << "\n";
  }

  return 0;
}
