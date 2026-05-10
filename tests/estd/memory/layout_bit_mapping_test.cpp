#include <cstdint>
#include <cstring>
#include <estd/__memory/layout_bit_mapping.h>
#include <gtest/gtest.h>
#include <vector>

TEST(LayoutBitMapping, BasicAcquire) {
  alignas(8) char buffer[64] = {0};

  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<64, 8, Mapping> lm(buffer);

  void* ptr1 = lm.acquire();
  EXPECT_NE(ptr1, nullptr);

  void* ptr2 = lm.acquire();
  EXPECT_NE(ptr2, nullptr);

  EXPECT_NE(ptr1, ptr2);
}

TEST(LayoutBitMapping, AcquireAndRelease) {
  alignas(8) char buffer[64] = {0};

  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<64, 8, Mapping> lm(buffer);

  void* ptr1 = lm.acquire();
  EXPECT_NE(ptr1, nullptr);

  bool released = lm.release(ptr1);
  EXPECT_TRUE(released);

  void* ptr2 = lm.acquire();
  EXPECT_EQ(ptr1, ptr2);
}

TEST(LayoutBitMapping, AcquireReleaseCycle) {
  alignas(8) char buffer[32] = {0};

  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<32, 8, Mapping> lm(buffer);

  void* ptr1 = lm.acquire();
  EXPECT_NE(ptr1, nullptr);

  void* ptr2 = lm.acquire();
  EXPECT_NE(ptr2, nullptr);

  bool released = lm.release(ptr1);
  EXPECT_TRUE(released);

  void* ptr3 = lm.acquire();
  EXPECT_EQ(ptr1, ptr3);
}

TEST(LayoutBitMapping, ReleaseInvalidPointer) {
  alignas(8) char buffer[64] = {0};
  alignas(8) char invalid_buffer[8] = {0};

  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<64, 8, Mapping> lm(buffer);

  bool released = lm.release(invalid_buffer);
  EXPECT_FALSE(released);
}

TEST(LayoutBitMapping, ResetAndAcquire) {
  alignas(8) char buffer[64] = {0};

  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<64, 8, Mapping> lm(buffer);

  void* ptr1 = lm.acquire();
  void* ptr2 = lm.acquire();

  lm.reset();

  void* ptr3 = lm.acquire();
  void* ptr4 = lm.acquire();

  EXPECT_EQ(ptr1, ptr3);
  EXPECT_EQ(ptr2, ptr4);
}

TEST(LayoutBitMapping, FullAllocation) {
  alignas(8) char buffer[64] = {0};
  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<64, 8, Mapping> lm(buffer);
  lm.reset();
  constexpr size_t capacity = decltype(lm)::member_capacity;
  std::vector<void*> ptrs;
  for (size_t i = 0; i < capacity; ++i) {
    void* p = lm.acquire();
    ASSERT_NE(p, nullptr);
    ptrs.push_back(p);
  }
  void* overflow = lm.acquire();
  EXPECT_EQ(overflow, nullptr);
  for (auto* p : ptrs) {
    lm.release(p);
  }
}

TEST(LayoutBitMapping, ReleaseAndReacquire) {
  alignas(8) char buffer[64] = {0};
  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<64, 8, Mapping> lm(buffer);
  void* p1 = lm.acquire();
  void* p2 = lm.acquire();
  ASSERT_NE(p1, nullptr);
  ASSERT_NE(p2, nullptr);
  lm.release(p1);
  void* p3 = lm.acquire();
  EXPECT_EQ(p1, p3);
  lm.release(p2);
  lm.release(p3);
}

TEST(LayoutBitMapping, ResetRestoresAllSlots) {
  alignas(8) char buffer[64] = {0};
  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<64, 8, Mapping> lm(buffer);
  lm.reset();
  constexpr size_t capacity = decltype(lm)::member_capacity;
  std::vector<void*> ptrs;
  for (size_t i = 0; i < capacity; ++i) {
    ptrs.push_back(lm.acquire());
  }
  EXPECT_EQ(lm.acquire(), nullptr);
  lm.reset();
  for (size_t i = 0; i < capacity; ++i) {
    void* p = lm.acquire();
    EXPECT_NE(p, nullptr);
  }
}

TEST(LayoutBitMapping, SingleSlot) {
  alignas(8) char buffer[9] = {0};
  using Mapping = uint8_t;
  es::memory::layout_bit_mapping<9, 8, Mapping> lm(buffer);
  lm.reset();
  EXPECT_EQ(decltype(lm)::member_capacity, 1u);
  void* p1 = lm.acquire();
  ASSERT_NE(p1, nullptr);
  EXPECT_EQ(lm.acquire(), nullptr);
  lm.release(p1);
  void* p2 = lm.acquire();
  EXPECT_EQ(p1, p2);
  lm.release(p2);
}
