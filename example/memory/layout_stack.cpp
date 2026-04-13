// layout_stack.h example: memory segment stack
//
// layout_stack manages memory segments by using the unused object memory itself
// to store a pointer to the previous unused memory segment. This forms a linked
// list (stack) of free memory segments.
//
// Memory Layout (each segment uses its header to store predecessor pointer):
// ┌─────────────────────────────────────────────────────────────────┐
// │  Segment 1    │  Segment 2    │  Segment 3    │  ...           │
// │  ┌─────┬────┐ │  ┌─────┬────┐ │  ┌─────┬────┐ │                │
// │  │prev │data│ │  │prev │data│ │  │prev │data│ │                │
// │  │null │    │ │  │ &S1 │    │ │  │ &S2 │    │ │                │
// │  └─────┴────┘ │  └─────┴────┘ │  └─────┴────┘ │                │
// └─────────────────────────────────────────────────────────────────┘
//
// When a segment is pushed onto the stack:
//   - The first sizeof(void*) bytes store the address of the previous top
//   - The stack's top pointer is updated to this segment
//
// This design requires objects to be at least sizeof(void*) bytes, so they can
// hold a pointer when unused. For smaller objects, use layout_bit_mapping
// instead.

#include <cstring>
#include <estd/__memory/layout_stack.h>
#include <iostream>

int main() {
  std::cout << "=== layout_stack Example ===\n\n";

  std::cout << "=== Concept ===\n";
  std::cout
      << "layout_stack uses unused memory segments to form a linked list.\n";
  std::cout
      << "Each segment's first sizeof(void*) bytes store a pointer to the\n";
  std::cout << "previous segment in the stack.\n\n";

  std::cout << "Memory layout when segments are pushed:\n";
  std::cout << "  Initial: top = null\n\n";
  std::cout << "  After push(mem1): top = mem1, mem1[0..7] = null\n";
  std::cout << "  ┌────────────┐\n";
  std::cout << "  │ prev: null │ ← top\n";
  std::cout << "  │ data: ...  │\n";
  std::cout << "  └────────────┘ (mem1)\n\n";
  std::cout << "  After push(mem2): top = mem2, mem2[0..7] = mem1\n";
  std::cout << "  ┌────────────┐    ┌────────────┐\n";
  std::cout << "  │ prev: null │ ←──│ prev: mem1 │ ← top\n";
  std::cout << "  │ data: ...  │    │ data: ...  │\n";
  std::cout << "  └────────────┘    └────────────┘\n";
  std::cout << "  (mem1)            (mem2)\n\n";
  std::cout << "  After push(mem3): top = mem3, mem3[0..7] = mem2\n";
  std::cout << "  ┌────────────┐    ┌────────────┐    ┌────────────┐\n";
  std::cout << "  │ prev: null │ ←──│ prev: mem1 │ ←──│ prev: mem2 │ ← top\n";
  std::cout << "  │ data: ...  │    │ data: ...  │    │ data: ...  │\n";
  std::cout << "  └────────────┘    └────────────┘    └────────────┘\n";
  std::cout << "  (mem1)            (mem2)            (mem3)\n\n";

  std::cout << "1. Basic push and pop:\n";
  {
    es::memory::layout_stack stack;

    char mem1[1024];
    char mem2[1024];
    char mem3[1024];

    std::cout << "  Stack empty: " << std::boolalpha << stack.empty() << "\n";
    std::cout << "  Top: " << static_cast<void*>(stack.top()) << "\n\n";

    stack.push(mem1, sizeof(mem1));
    std::cout << "  Pushed mem1:\n";
    std::cout << "    Top: " << static_cast<void*>(stack.top()) << "\n";
    std::cout << "    mem1's prev ptr: "
              << static_cast<void*>(*reinterpret_cast<void**>(mem1)) << "\n\n";

    stack.push(mem2, sizeof(mem2));
    std::cout << "  Pushed mem2:\n";
    std::cout << "    Top: " << static_cast<void*>(stack.top()) << "\n";
    std::cout << "    mem2's prev ptr: "
              << static_cast<void*>(*reinterpret_cast<void**>(mem2)) << "\n";
    std::cout << "    (points to mem1: " << static_cast<void*>(mem1) << ")\n\n";

    stack.push(mem3, sizeof(mem3));
    std::cout << "  Pushed mem3:\n";
    std::cout << "    Top: " << static_cast<void*>(stack.top()) << "\n";
    std::cout << "    mem3's prev ptr: "
              << static_cast<void*>(*reinterpret_cast<void**>(mem3)) << "\n";
    std::cout << "    (points to mem2: " << static_cast<void*>(mem2) << ")\n\n";

    std::cout << "  Popping all segments:\n";
    char* p = stack.pop();
    std::cout << "    Popped: " << static_cast<void*>(p) << " (mem3)\n";
    std::cout << "    Top now: " << static_cast<void*>(stack.top()) << "\n";

    p = stack.pop();
    std::cout << "    Popped: " << static_cast<void*>(p) << " (mem2)\n";
    std::cout << "    Top now: " << static_cast<void*>(stack.top()) << "\n";

    p = stack.pop();
    std::cout << "    Popped: " << static_cast<void*>(p) << " (mem1)\n";
    std::cout << "    Top now: " << static_cast<void*>(stack.top()) << "\n";

    std::cout << "    Stack empty: " << stack.empty() << "\n";
  }
  std::cout << "\n";

  std::cout << "2. Why this design works:\n";
  {
    std::cout << "  sizeof(void*) = " << sizeof(void*) << " bytes\n\n";

    std::cout
        << "  Key insight: When memory is NOT in use (free), we can reuse\n";
    std::cout << "  its first " << sizeof(void*)
              << " bytes to store a pointer.\n\n";

    std::cout << "  Memory segment states:\n";
    std::cout << "    IN USE:  [  user data  |  user data  |  ...  ]\n";
    std::cout << "    FREE:    [ prev ptr    |  (unused)   |  ...  ]\n\n";

    std::cout << "  This is why layout_stack requires objects >= "
              << sizeof(void*) << " bytes.\n";
    std::cout << "  For smaller objects, use layout_bit_mapping instead.\n";
  }
  std::cout << "\n";

  std::cout << "3. Splice operation (merge two stacks):\n";
  {
    es::memory::layout_stack stack1;
    es::memory::layout_stack stack2;

    char mem1[512], mem2[512];
    char mem3[256], mem4[256];

    stack1.push(mem1, sizeof(mem1));
    stack1.push(mem2, sizeof(mem2));

    stack2.push(mem3, sizeof(mem3));
    stack2.push(mem4, sizeof(mem4));

    std::cout << "  stack1: mem2 → mem1 → null (2 segments)\n";
    std::cout << "  stack2: mem4 → mem3 → null (2 segments)\n\n";

    stack1.splice(stack2);
    std::cout << "  After splice(stack2):\n";
    std::cout << "    stack1: mem2 → mem1 → mem4 → mem3 → null\n";
    std::cout << "    stack2 empty: " << stack2.empty() << "\n\n";

    std::cout << "  Splice is useful for merging thread-local pools back to "
                 "global pool.\n";
  }
  std::cout << "\n";

  std::cout << "4. Memory required by stack:\n";
  std::cout << "  Each segment needs at least "
            << es::memory::layout_stack::memory_required_by_stack
            << " bytes for bookkeeping\n\n";

  std::cout << "5. Comparison with layout_bit_mapping:\n";
  std::cout << "  "
               "┌─────────────────────┬─────────────────────┬──────────────────"
               "───┐\n";
  std::cout << "  │     Aspect          │  layout_bit_mapping │     "
               "layout_stack    │\n";
  std::cout << "  "
               "├─────────────────────┼─────────────────────┼──────────────────"
               "───┤\n";
  std::cout << "  │ Min object size     │  1 byte             │  sizeof(void*) "
               "     │\n";
  std::cout << "  │ Tracking overhead   │  Bitmap header      │  None (in-band "
               "ptr) │\n";
  std::cout << "  │ Allocation          │  O(1)               │  O(1)          "
               "     │\n";
  std::cout << "  │ Deallocation        │  O(1)               │  O(1)          "
               "     │\n";
  std::cout << "  │ Best for            │  Small objects      │  Large objects "
               "     │\n";
  std::cout << "  "
               "└─────────────────────┴─────────────────────┴──────────────────"
               "───┘\n";

  return 0;
}
