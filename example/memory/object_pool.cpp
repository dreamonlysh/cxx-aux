// object_pool.h example: type-safe object pool

#include <estd/__memory/object_pool.h>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct Node {
  int value;
  Node* next;
  Node(int v) : value(v), next(nullptr) {}
};

struct Person {
  std::string name;
  int age;
  Person(const std::string& n, int a) : name(n), age(a) {
    std::cout << "  Person constructed: " << name << ", " << age << "\n";
  }
  ~Person() { std::cout << "  Person destructed: " << name << "\n"; }
};

int main() {
  std::cout << "=== ObjectPool Example ===\n\n";

  std::cout << "1. Basic object allocation:\n";
  {
    es::memory::ObjectPool<Node> pool;

    Node* n1 = pool.operator_new(10);
    Node* n2 = pool.operator_new(20);
    Node* n3 = pool.operator_new(30);

    n1->next = n2;
    n2->next = n3;

    std::cout << "  Linked list: ";
    for (Node* p = n1; p != nullptr; p = p->next) {
      std::cout << p->value << " -> ";
    }
    std::cout << "null\n";

    pool.operator_delete(n1);
    pool.operator_delete(n2);
    pool.operator_delete(n3);
  }
  std::cout << "\n";

  std::cout << "2. make_unique (RAII):\n";
  {
    es::memory::ObjectPool<Person> pool;

    {
      auto p1 = es::memory::make_unique(pool, std::string("Alice"), 25);
      auto p2 = es::memory::make_unique(pool, std::string("Bob"), 30);

      std::cout << "  Using persons: " << p1->name << " (" << p1->age << "), "
                << p2->name << " (" << p2->age << ")\n";
      std::cout << "  Exiting scope...\n";
    }
    std::cout << "  Persons automatically returned to pool\n";
  }
  std::cout << "\n";

  std::cout << "3. Memory reuse:\n";
  {
    es::memory::ObjectPool<int> pool;

    int* arr[5];
    for (int i = 0; i < 5; ++i) {
      arr[i] = pool.operator_new(i * 10);
      std::cout << "  Allocated: " << *arr[i] << " at " << arr[i] << "\n";
    }

    for (int i = 0; i < 5; ++i) {
      pool.operator_delete(arr[i]);
    }

    std::cout << "  Re-allocating (memory reused):\n";
    for (int i = 0; i < 5; ++i) {
      arr[i] = pool.operator_new(i * 100);
      std::cout << "  Allocated: " << *arr[i] << " at " << arr[i] << "\n";
    }

    for (int i = 0; i < 5; ++i) {
      pool.operator_delete(arr[i]);
    }
  }
  std::cout << "\n";

  std::cout << "4. Pool constants:\n";
  {
    using PoolType = es::memory::ObjectPool<Node>;
    std::cout << "  block_size: " << PoolType::block_size << " bytes\n";
    std::cout << "  object_alloc_size: " << PoolType::object_alloc_size
              << " bytes\n";
    std::cout << "  objects per block: ~"
              << (PoolType::block_size / PoolType::object_alloc_size) << "\n";
  }
  std::cout << "\n";

  std::cout << "5. SubObjectPool - thread-local pools merging to parent:\n";
  {
    es::memory::ObjectPool<Node> parent_pool;
    std::mutex merge_mutex;

    std::cout << "  Creating 3 threads, each with its own SubObjectPool...\n";

    auto thread_func = [&parent_pool, &merge_mutex](int thread_id,
                                                    int num_objects) {
      es::memory::SubObjectPool<Node, alignof(Node), 4096, std::mutex>
          local_pool(parent_pool, merge_mutex);

      std::vector<Node*> nodes;
      for (int i = 0; i < num_objects; ++i) {
        Node* n = local_pool.operator_new(thread_id * 100 + i);
        nodes.push_back(n);
      }

      std::cout << "  Thread " << thread_id << ": allocated " << nodes.size()
                << " objects (first: " << nodes[0]->value << ")\n";

      std::cout << "  Thread " << thread_id
                << ": exiting (SubObjectPool merges to parent)\n";

      // Note: We do NOT delete nodes here!
      // When local_pool is destroyed, its memory is merged to parent_pool.
      // The parent_pool can then reuse this memory for new allocations.
    };

    std::thread t1(thread_func, 1, 5);
    std::thread t2(thread_func, 2, 3);
    std::thread t3(thread_func, 3, 4);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "  All threads finished. Parent pool now has merged memory.\n";
    std::cout << "  Allocating from parent pool (reuses merged memory):\n";

    Node* n = parent_pool.operator_new(999);
    std::cout << "  Allocated from parent: " << n->value << " at " << n << "\n";
    parent_pool.operator_delete(n);
  }

  return 0;
}
