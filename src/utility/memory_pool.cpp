#include "cxxaux/utility/memory_pool.h"
#include <array>
#include <cstdint>
#include <deque>
#include <list>

namespace {

constexpr uint32_t kMemoryPoolBlockBytes = 1024;

// create pool for every 8 bytes, the maximum is 128 bytes
// TODO: memory is not dividable should be reused for small pool
constexpr uint32_t kMemoryPoolMaximum = 128;

constexpr uint32_t kMemoryPoolInterval = 8;

class SizedMemoryPool {
public:
  SizedMemoryPool(uint32_t slotBytes) : slotBytes(slotBytes) {}

  void* allocate() {
    if (!idle.empty()) {
      void* ret = idle.back();
      idle.pop_back();
      return ret;
    }

    if ((kMemoryPoolBlockBytes - cursor) < slotBytes) {
      blocks.emplace_back();
      cursor = 0;
    }

    uint32_t curCursor = cursor;
    cursor += slotBytes;
    return std::next(blocks.back().m, curCursor);
  }

  void deallocate(void* ptr) { idle.push_back(ptr); }

private:
  const uint32_t slotBytes = 0;

  uint32_t cursor = kMemoryPoolBlockBytes;
  struct MemoryBlock {
    char m[kMemoryPoolBlockBytes];
  };
  std::list<MemoryBlock> blocks;

  std::deque<void*> idle;
};

class MemoryPoolImpl {

public:
  MemoryPoolImpl() : pool1Byte(1), pool2Bytes(2), pool4Bytes(4) {
    for (uint32_t i = 0; i < (kMemoryPoolMaximum / kMemoryPoolInterval); ++i) {
      pools[i] = new SizedMemoryPool((i + 1) * kMemoryPoolInterval);
    }
  }
  ~MemoryPoolImpl() noexcept {
    for (uint32_t i = 0; i < (kMemoryPoolMaximum / kMemoryPoolInterval); ++i) {
      delete pools[i];
    }
  }

  static void* allocate(size_t bytes) {
    if (bytes == 0)
      return nullptr;

    if (bytes > kMemoryPoolMaximum) {
      return malloc(bytes);
    }

    return singleton.findPool(bytes).allocate();
  }

  static void deallocate(void* ptr, size_t bytes) {
    if (bytes > kMemoryPoolMaximum) {
      free(static_cast<void*>(ptr));
    }

    singleton.findPool(bytes).deallocate(ptr);
  }

private:
  SizedMemoryPool& findPool(size_t bytes) {
    if (bytes == 1)
      return pool1Byte;
    if (bytes == 2)
      return pool2Bytes;
    if (bytes <= 4)
      return pool4Bytes;
    return *pools[bytes / kMemoryPoolInterval - 1];
  }

private:
  static MemoryPoolImpl singleton;

  SizedMemoryPool pool1Byte;
  SizedMemoryPool pool2Bytes;
  SizedMemoryPool pool4Bytes;

  std::array<SizedMemoryPool*, kMemoryPoolMaximum / kMemoryPoolInterval> pools;
};

MemoryPoolImpl MemoryPoolImpl::singleton;
} // namespace

namespace cxxaux {
void* MemoryPool::allocate(size_t bytes) {
  return MemoryPoolImpl::allocate(bytes);
}

void MemoryPool::deallocate(void* ptr, size_t bytes) {
  MemoryPoolImpl::deallocate(ptr, bytes);
}
} // namespace cxxaux
