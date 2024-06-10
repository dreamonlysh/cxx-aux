#include <cxxaux/utility/memory_pool.h>
#include <gtest/gtest.h>
#include <memory>

template <typename T>
using pool_ptr = std::unique_ptr<T, cxxaux::MemoryPoolAllocator<T>>;

template <typename T, typename... Args>
T* pool_new(Args&&... args) {
  return cxxaux::MemoryPool::operator_new<T>(std::forward<Args>(args)...);
}

template <typename T>
void pool_delete(T* ptr) {
  return cxxaux::MemoryPool::operator_delete<T>(ptr);
}

TEST(MemoryPool, int) {
  int* a = pool_new<int>(1);
  ASSERT_EQ(*a, 1);
  int* b = pool_new<int>(2);
  ASSERT_EQ(*b, 2);
  pool_delete(b);
  int* c = pool_new<int>(4);
  ASSERT_EQ(*c, 4);
  pool_delete(c);
  pool_delete(a);
}

struct Byte15 {
  char m[15];
};

TEST(MemoryPool, Byte15) {
  auto* a = pool_new<Byte15>();
  for (char i = 0; i < 15; ++i)
    a->m[i] = i + 1;
  ASSERT_EQ(a->m[8], 9);
  auto* b = pool_new<Byte15>();
  for (char i = 0; i < 15; ++i)
    b->m[i] = i + 3;
  ASSERT_EQ(b->m[8], 11);
  pool_delete(b);
  auto* c = pool_new<Byte15>();
  for (char i = 0; i < 15; ++i)
    c->m[i] = i + 5;
  ASSERT_EQ(c->m[8], 13);
  pool_delete(c);
  pool_delete(a);
}