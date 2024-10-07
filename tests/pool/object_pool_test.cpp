#include <cxxaux/pool/object_pool.h>
#include <gtest/gtest.h>

TEST(ObjectPoolTest, SmallCase) {
  cxxaux::ObjectPool<int> pool;
  int* a = pool.operator_new(1);
  int* b = pool.operator_new(2);
  int* c = pool.operator_new(3);
  ASSERT_EQ(*a, 1);
  ASSERT_EQ(*b, 2);
  ASSERT_EQ(*c, 3);
  ASSERT_EQ(b, std::next(a));
  ASSERT_EQ(c, std::next(b));
  pool.operator_delete(c);
  pool.operator_delete(b);
  pool.operator_delete(a);
}

TEST(ObjectPoolTest, SmallCaseOverBlock) {
  cxxaux::ObjectPool<int> pool;
  int* a = pool.operator_new(1);
  int* b = pool.operator_new(2);
  std::vector<int*> tmp;
  for (size_t i = 0; i < (pool.block_size / pool.object_alloc_size); ++i) {
    tmp.push_back(pool.operator_new(i));
  }
  int* c = pool.operator_new(3);
  int* d = pool.operator_new(4);
  ASSERT_EQ(*a, 1);
  ASSERT_EQ(*b, 2);
  ASSERT_EQ(*c, 3);
  ASSERT_EQ(*d, 4);
  ASSERT_EQ(b, std::next(a));
  ASSERT_EQ(d, std::next(c));
  pool.operator_delete(d);
  pool.operator_delete(c);
  for (auto* p : tmp) {
    pool.operator_delete(p);
  }
  pool.operator_delete(b);
  pool.operator_delete(a);
}

TEST(ObjectPoolTest, SmallCaseReuse) {
  cxxaux::ObjectPool<int> pool;
  int* a = pool.operator_new(1);
  int* b = pool.operator_new(2);
  int* c = pool.operator_new(3);
  ASSERT_EQ(b, std::next(a));
  ASSERT_EQ(c, std::next(b));
  pool.operator_delete(b);
  pool.operator_delete(c);
  int* m = pool.operator_new(2);
  int* n = pool.operator_new(3);
  ASSERT_EQ(m, std::next(a));
  ASSERT_EQ(n, std::next(m));
  ASSERT_EQ(*a, 1);
  ASSERT_EQ(*m, 2);
  ASSERT_EQ(*n, 3);
  pool.operator_delete(c);
  pool.operator_delete(b);
  pool.operator_delete(a);
}

struct LargeObject {
  LargeObject(unsigned long long v) : first(v), second(v << 2) {}

  unsigned long long first;
  unsigned long long second;
};

TEST(ObjectPoolTest, LargeCase) {
  cxxaux::ObjectPool<LargeObject> pool;
  LargeObject* a = pool.operator_new(1);
  LargeObject* b = pool.operator_new(2);
  LargeObject* c = pool.operator_new(3);
  ASSERT_EQ(a->first, 1);
  ASSERT_EQ(b->first, 2);
  ASSERT_EQ(c->first, 3);
  ASSERT_EQ(a, std::next(b));
  ASSERT_EQ(b, std::next(c));
  pool.operator_delete(c);
  pool.operator_delete(b);
  pool.operator_delete(a);
}

TEST(ObjectPoolTest, LargeCaseOverBlock) {
  cxxaux::ObjectPool<LargeObject> pool;
  LargeObject* a = pool.operator_new(1);
  LargeObject* b = pool.operator_new(2);
  std::vector<LargeObject*> tmp;
  for (size_t i = 0; i < (pool.block_size / pool.object_alloc_size); ++i) {
    tmp.push_back(pool.operator_new(i));
  }
  LargeObject* c = pool.operator_new(3);
  LargeObject* d = pool.operator_new(4);
  ASSERT_EQ(a->first, 1);
  ASSERT_EQ(b->first, 2);
  ASSERT_EQ(c->first, 3);
  ASSERT_EQ(d->first, 4);
  ASSERT_EQ(a, std::next(b));
  ASSERT_EQ(c, std::next(d));
  pool.operator_delete(d);
  pool.operator_delete(c);
  for (auto* p : tmp) {
    pool.operator_delete(p);
  }
  pool.operator_delete(b);
  pool.operator_delete(a);
}

TEST(ObjectPoolTest, LargeCaseReuse) {
  cxxaux::ObjectPool<LargeObject> pool;
  LargeObject* a = pool.operator_new(1);
  LargeObject* b = pool.operator_new(2);
  LargeObject* c = pool.operator_new(3);
  ASSERT_EQ(a, std::next(b));
  ASSERT_EQ(b, std::next(c));
  pool.operator_delete(b);
  pool.operator_delete(c);
  LargeObject* m = pool.operator_new(2);
  LargeObject* n = pool.operator_new(3);
  ASSERT_EQ(a, std::next(n));
  ASSERT_EQ(n, std::next(m));
  ASSERT_EQ(a->first, 1);
  ASSERT_EQ(m->first, 2);
  ASSERT_EQ(n->first, 3);
  pool.operator_delete(c);
  pool.operator_delete(b);
  pool.operator_delete(a);
}

TEST(ObjectPoolTest, MakeUnique) {
  cxxaux::ObjectPool<int> pool;
  auto a = cxxaux::make_unique(pool, 1);
  ASSERT_EQ(*a, 1);
  {
    auto b = cxxaux::make_unique(pool, 2);
    ASSERT_EQ(*b, 2);
    ASSERT_EQ(b.get(), std::next(a.get()));
  }
  auto c = cxxaux::make_unique(pool, 3);
  ASSERT_EQ(*c, 3);
  ASSERT_EQ(c.get(), std::next(a.get()));
}

TEST(SubObjectPoolTest, SmallNoLock) {
  cxxaux::ObjectPool<int> parent;
  int* a = parent.operator_new(1);
  int* b;
  {
    cxxaux::SubObjectPool pool(parent);
    b = pool.operator_new(2);
    ASSERT_EQ(*b, 2);
  }
  int* c = parent.operator_new(3);
  ASSERT_EQ(*a, 1);
  ASSERT_EQ(*b, 2);
  ASSERT_EQ(*c, 3);
  ASSERT_EQ(c, std::next(b));

  parent.operator_delete(c);
  parent.operator_delete(b);
  parent.operator_delete(a);
}

TEST(SubObjectPoolTest, LargeNoLock) {
  cxxaux::ObjectPool<LargeObject> parent;
  LargeObject* a = parent.operator_new(1);
  LargeObject* b;
  {
    cxxaux::SubObjectPool pool(parent);
    b = pool.operator_new(2);
    ASSERT_EQ(b->first, 2);
  }
  LargeObject* c = parent.operator_new(3);
  ASSERT_EQ(a->first, 1);
  ASSERT_EQ(b->first, 2);
  ASSERT_EQ(c->first, 3);
  ASSERT_EQ(a, std::next(c));

  parent.operator_delete(c);
  parent.operator_delete(b);
  parent.operator_delete(a);
}

namespace {
struct Locker {
  Locker(std::pair<int, int>& lockerStatus) : lockerStatus(lockerStatus) {}
  void lock() { lockerStatus.first = 2; }
  void unlock() { lockerStatus.second = 9; }
  std::pair<int, int>& lockerStatus;
};
} // namespace

TEST(SubObjectPoolTest, SmallLock) {
  cxxaux::ObjectPool<int> parent;
  int* a = parent.operator_new(1);
  int* b;

  std::pair<int, int> lockerStatus{};
  {
    Locker locker{lockerStatus};
    cxxaux::SubObjectPool pool(parent, locker);
    b = pool.operator_new(2);
    ASSERT_EQ(*b, 2);

    ASSERT_EQ(lockerStatus.first, 0);
    ASSERT_EQ(lockerStatus.second, 0);
  }
  ASSERT_EQ(lockerStatus.first, 2);
  ASSERT_EQ(lockerStatus.second, 9);

  int* c = parent.operator_new(3);
  ASSERT_EQ(*a, 1);
  ASSERT_EQ(*b, 2);
  ASSERT_EQ(*c, 3);
  ASSERT_EQ(c, std::next(b));

  parent.operator_delete(c);
  parent.operator_delete(b);
  parent.operator_delete(a);
}

TEST(SubObjectPoolTest, LargeLock) {
  cxxaux::ObjectPool<LargeObject> parent;
  LargeObject* a = parent.operator_new(1);
  LargeObject* b;

  std::pair<int, int> lockerStatus{};
  {
    Locker locker{lockerStatus};
    cxxaux::SubObjectPool pool(parent, locker);
    b = pool.operator_new(2);
    ASSERT_EQ(b->first, 2);

    ASSERT_EQ(lockerStatus.first, 0);
    ASSERT_EQ(lockerStatus.second, 0);
  }
  ASSERT_EQ(lockerStatus.first, 2);
  ASSERT_EQ(lockerStatus.second, 9);

  LargeObject* c = parent.operator_new(3);
  ASSERT_EQ(a->first, 1);
  ASSERT_EQ(b->first, 2);
  ASSERT_EQ(c->first, 3);
  ASSERT_EQ(a, std::next(c));

  parent.operator_delete(c);
  parent.operator_delete(b);
  parent.operator_delete(a);
}