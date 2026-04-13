// memory_pool_uniform.h example: fixed-size memory pool

#include <estd/__memory/memory_pool_uniform.h>
#include <iostream>

struct Node {
  int value;
  Node* next;
  Node(int v) : value(v), next(nullptr) {}
};

int main() {
  std::cout << "=== MemoryPoolUniform Example ===\n\n";

  std::cout << "1. Basic allocation and deallocation:\n";
  {
    es::memory::MemoryPoolUniform<sizeof(Node)> pool;

    Node* n1 = static_cast<Node*>(pool.allocate());
    new (n1) Node(1);
    Node* n2 = static_cast<Node*>(pool.allocate());
    new (n2) Node(2);
    Node* n3 = static_cast<Node*>(pool.allocate());
    new (n3) Node(3);

    n1->next = n2;
    n2->next = n3;

    std::cout << "  Linked list: ";
    for (Node* p = n1; p != nullptr; p = p->next) {
      std::cout << p->value << " -> ";
    }
    std::cout << "null\n";

    n1->~Node();
    n2->~Node();
    n3->~Node();
    pool.deallocate(n1);
    pool.deallocate(n2);
    pool.deallocate(n3);
  }
  std::cout << "\n";

  std::cout << "2. Memory reuse:\n";
  {
    es::memory::MemoryPoolUniform<sizeof(int)> pool;

    int* arr[5];
    for (int i = 0; i < 5; ++i) {
      arr[i] = static_cast<int*>(pool.allocate());
      *arr[i] = i * 10;
      std::cout << "  Allocated: " << *arr[i] << " at " << arr[i] << "\n";
    }

    for (int i = 0; i < 5; ++i) {
      pool.deallocate(arr[i]);
    }

    std::cout << "  Re-allocating (memory reused):\n";
    for (int i = 0; i < 5; ++i) {
      arr[i] = static_cast<int*>(pool.allocate());
      *arr[i] = i * 100;
      std::cout << "  Allocated: " << *arr[i] << " at " << arr[i] << "\n";
    }

    for (int i = 0; i < 5; ++i) {
      pool.deallocate(arr[i]);
    }
  }
  std::cout << "\n";

  std::cout << "3. Pool merging:\n";
  {
    es::memory::MemoryPoolUniform<32> pool1;
    es::memory::MemoryPoolUniform<32> pool2;

    void* m1 = pool1.allocate();
    void* m2 = pool2.allocate();

    std::cout << "  pool1 allocated: " << m1 << "\n";
    std::cout << "  pool2 allocated: " << m2 << "\n";

    pool1.deallocate(m1);
    pool2.deallocate(m2);

    pool1.merge(pool2);
    std::cout << "  Merged pool2 into pool1\n";

    void* m3 = pool1.allocate();
    std::cout << "  pool1 allocated after merge: " << m3 << "\n";
    pool1.deallocate(m3);
  }
  std::cout << "\n";

  std::cout << "4. Different block sizes:\n";
  {
    using SmallPool = es::memory::MemoryPoolUniform<16, 1024>;
    using LargePool = es::memory::MemoryPoolUniform<16, 8192>;

    std::cout << "  SmallPool block size: 1024 bytes\n";
    std::cout << "  LargePool block size: 8192 bytes\n";
    std::cout << "  Both allocate 16-byte chunks\n";
  }

  return 0;
}
