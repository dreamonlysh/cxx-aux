# Memory Management Examples

This directory contains examples demonstrating the `es::memory` library for efficient memory management.

## Examples

### 1. layout\_bit\_mapping.cpp

**Bitmap-based memory allocation**

```cpp
#include <estd/__memory/layout_bit_mapping.h>

char memory[4096];
es::memory::layout_bit_mapping<4096, 64> allocator(memory);
allocator.reset();

void* member = allocator.acquire();
allocator.release(member);
```

**Compile:**

```bash
g++ -std=c++17 -I include example/memory/layout_bit_mapping.cpp -o layout_bit_mapping
./layout_bit_mapping
```

### 2. layout\_stack.cpp

**Memory segment stack**

```cpp
#include <estd/__memory/layout_stack.h>

es::memory::layout_stack stack;
char mem[1024];
stack.push(mem, sizeof(mem));
char* top = stack.pop();
```

**Compile:**

```bash
g++ -std=c++17 -I include example/memory/layout_stack.cpp -o layout_stack
./layout_stack
```

### 3. memory\_pool\_uniform.cpp

**Fixed-size memory pool**

```cpp
#include <estd/__memory/memory_pool_uniform.h>

es::memory::MemoryPoolUniform<64> pool;
void* mem = pool.allocate();
pool.deallocate(mem);
```

**Compile:**

```bash
g++ -std=c++17 -I include example/memory/memory_pool_uniform.cpp -o memory_pool_uniform
./memory_pool_uniform
```

### 4. object\_pool.cpp

**Type-safe object pool with SubObjectPool**

```cpp
#include <estd/__memory/object_pool.h>

es::memory::ObjectPool<MyClass> pool;
MyClass* obj = pool.operator_new(arg1, arg2);
pool.operator_delete(obj);

// Or use RAII with make_unique
auto obj = es::memory::make_unique(pool, arg1, arg2);
```

**Compile:**

```bash
g++ -std=c++17 -pthread -I include example/memory/object_pool.cpp -o object_pool
./object_pool
```

## API Summary

### layout\_bit\_mapping\<MemoryBytes, MemberBytes, MappingType>

Bitmap-based memory allocator for fixed-size members.

**Design Purpose:**

- Manages a large memory block with a bitmap header for tracking allocation status
- Specifically designed for small objects (< sizeof(void\*)) that cannot hold a pointer
- For objects >= sizeof(void\*), use layout\_stack instead (no header overhead)

**Memory Layout:**

```
┌─────────────────────────────────────────────────────────────────┐
│  Header (bitmap)  │         Data Area (members)      │   Tail   │
│  N bytes          │  member_capacity * MemberBytes   │ unused   │
└─────────────────────────────────────────────────────────────────┘
```

**Why bitmap for small objects?**

- Objects < sizeof(void\*) cannot store a pointer for linked-list tracking
- Bitmap header provides external tracking: 1 bit per slot
- bit 0: slot is available (not acquired)
- bit 1: slot is in use (acquired)

| Method       | Description                               |
| ------------ | ----------------------------------------- |
| `reset()`    | Initialize all members as available       |
| `acquire()`  | Acquire a member, returns nullptr if full |
| `release(p)` | Release a member back to pool             |

| Constant                      | Description               |
| ----------------------------- | ------------------------- |
| `member_capacity`             | Maximum number of members |
| `member_bytes_offset`         | Header overhead in bytes  |
| `memory_tail_bytes_unmapping` | Unused trailing bytes     |

### layout\_stack

Stack structure for managing memory segments using in-band pointers.

**Design Purpose:**

- Uses unused object memory itself to store predecessor pointer
- Forms a linked list (stack) of free memory segments
- Requires objects >= sizeof(void\*) to hold a pointer
- For smaller objects, use layout\_bit\_mapping instead

**Memory Layout:**

```
┌─────────────────────────────────────────────────────────────────┐
│  Segment 1    │  Segment 2    │  Segment 3    │  ...            │
│  ┌─────┬────┐ │  ┌─────┬────┐ │  ┌─────┬────┐ │                 │
│  │prev │data│ │  │prev │data│ │  │prev │data│ │                 │
│  │null │    │ │  │ &S1 │    │ │  │ &S2 │    │ │                 │
│  └─────┴────┘ │  └─────┴────┘ │  └─────┴────┘ │                 │
└─────────────────────────────────────────────────────────────────┘
```

**Memory segment states:**

- IN USE:  `[  user data  |  user data  |  ...  ]`
- FREE:    `[ prev ptr    |  (unused)   |  ...  ]`

| Method                  | Description                          |
| ----------------------- | ------------------------------------ |
| `push(v, sz)`           | Push memory segment onto stack       |
| `pop()`                 | Pop memory segment from stack        |
| `top()`                 | Get top memory segment               |
| `empty()`               | Check if stack is empty              |
| `splice(other, bottom)` | Move all segments from another stack |

| Constant                   | Description                                |
| -------------------------- | ------------------------------------------ |
| `memory_required_by_stack` | Minimum bytes per segment (sizeof(void\*)) |

### Comparison: layout\_bit\_mapping vs layout\_stack

| Aspect            | layout\_bit\_mapping             | layout\_stack                     |
| ----------------- | -------------------------------- | --------------------------------- |
| Min object size   | 1 byte                           | sizeof(void\*)                    |
| Tracking overhead | Bitmap header                    | None (in-band ptr)                |
| Allocation        | O(1)                             | O(1)                              |
| Deallocation      | O(1)                             | O(1)                              |
| Best for          | Small objects (< sizeof(void\*)) | Large objects (>= sizeof(void\*)) |

### MemoryPoolUniform\<Size, BlockSize>

A memory pool for fixed-size allocations.

| Method          | Description                                |
| --------------- | ------------------------------------------ |
| `allocate()`    | Allocate memory block of `Size` bytes      |
| `deallocate(p)` | Return memory block to pool                |
| `merge(other)`  | Merge another pool's memory into this pool |

### ObjectPool\<T, Align, BlockSize>

A type-safe object pool for efficient object allocation.

| Method                        | Description                              |
| ----------------------------- | ---------------------------------------- |
| `operator_new<Args>(args...)` | Allocate and construct object            |
| `operator_delete(p)`          | Destroy object and return memory to pool |
| `merge(other)`                | Merge another pool's memory              |

| Constant            | Description             |
| ------------------- | ----------------------- |
| `block_size`        | Bytes per memory block  |
| `object_alloc_size` | Aligned size per object |

### make\_unique(pool, args...)

RAII-style object allocation with automatic cleanup.

```cpp
es::memory::ObjectPool<Person> pool;
{
    auto p = es::memory::make_unique(pool, "Alice", 25);
    // Use p...
} // Automatically returned to pool
```

### SubObjectPool\<T, Align, BlockSize, LockerT>

Thread-local object pool that merges back to parent on destruction.

```cpp
es::memory::ObjectPool<Node> parent_pool;
std::mutex mtx;

void thread_func() {
    es::memory::SubObjectPool<Node, alignof(Node), 4096, std::mutex>
        sub_pool(parent_pool, mtx);
    // Allocate locally...
    Node* n = sub_pool.operator_new(42);
    // sub_pool merges back to parent_pool on destruction
}
```

## Multi-threaded SubObjectPool Pattern

```cpp
#include <estd/__memory/object_pool.h>
#include <mutex>
#include <thread>
#include <vector>

struct Task {
    int id;
    Task(int i) : id(i) {}
};

es::memory::ObjectPool<Task> global_pool;
std::mutex merge_mutex;

void worker_thread(int thread_id, int num_tasks) {
    es::memory::SubObjectPool<Task, alignof(Task), 4096, std::mutex>
        local_pool(global_pool, merge_mutex);

    std::vector<Task*> tasks;
    for (int i = 0; i < num_tasks; ++i) {
        tasks.push_back(local_pool.operator_new(thread_id * 100 + i));
    }

    // Do work with tasks...

    // Note: Do NOT delete tasks here!
    // When local_pool is destroyed, its memory blocks are merged to global_pool.
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker_thread, i, 10);
    }

    for (auto& t : threads) {
        t.join();
    }

    // global_pool now contains all merged memory blocks
    Task* t = global_pool.operator_new(999);
    global_pool.operator_delete(t);

    return 0;
}
```

**Key benefits:**

- No contention during allocation (each thread has its own pool)
- Automatic memory block merge to parent on thread exit
- Thread-safe merge with mutex protection
- Memory blocks are reused, not freed

## Performance Characteristics

| Component            | Allocation | Deallocation | Memory Overhead |
| -------------------- | ---------- | ------------ | --------------- |
| MemoryPoolUniform    | O(1)       | O(n)\*       | Minimal         |
| ObjectPool           | O(1)       | O(1)         | Aligned size    |
| layout\_bit\_mapping | O(1)       | O(1)         | Bitmap header   |

\*MemoryPoolUniform deallocation is O(n) in number of blocks for finding the right block.

## Notes

- All pools are thread-unsafe; use external synchronization or SubObjectPool
- BlockSize must be a multiple of 1024 bytes
- Objects are constructed in-place using placement new
- Pools never return nullptr on allocation (throw on failure)

