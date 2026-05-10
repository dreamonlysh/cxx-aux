#include <cxxaux/utility/version.h>
#include <gtest/gtest.h>

using VID = cxxaux::VersionID<uint32_t>;
using VIDSet = cxxaux::VersionIDSet<VID>;
using VInfo = cxxaux::VersionInfo<VID>;
using VMgr = cxxaux::VersionManager<VInfo>;

// ============================================
// VersionID Tests
// ============================================

TEST(VersionIDTest, DefaultConstruct) {
  VID id;
  EXPECT_FALSE(id.valid());
  EXPECT_EQ(id.value(), 0u);
}

TEST(VersionIDTest, ConstructWithOffset) {
  VID id(5);
  EXPECT_TRUE(id.valid());
  EXPECT_EQ(id.value(), 1u << 5);
  EXPECT_EQ(id.offset(), 5u);
}

TEST(VersionIDTest, Set) {
  VID id;
  id.set(3);
  EXPECT_TRUE(id.valid());
  EXPECT_EQ(id.value(), 1u << 3);
  EXPECT_EQ(id.offset(), 3u);
}

TEST(VersionIDTest, Valid) {
  VID id1;
  EXPECT_FALSE(id1.valid());

  VID id2(2);
  EXPECT_TRUE(id2.valid());
}

TEST(VersionIDTest, Offset) {
  for (unsigned i = 0; i < 32; ++i) {
    VID id(i);
    EXPECT_EQ(id.offset(), i);
  }
}

TEST(VersionIDTest, Comparison) {
  VID id1(2);
  VID id2(2);
  VID id3(3);

  EXPECT_TRUE(id1 == id2);
  EXPECT_FALSE(id1 == id3);
  EXPECT_FALSE(id1 != id2);
  EXPECT_TRUE(id1 != id3);
}

// ============================================
// VersionIDSet Tests
// ============================================

TEST(VersionIDSetTest, DefaultConstruct) {
  VIDSet set;
  EXPECT_TRUE(set.empty());
  EXPECT_EQ(set.size(), 0u);
}

TEST(VersionIDSetTest, ConstructWithIDs) {
  VIDSet set(VID(0), VID(2), VID(4));
  EXPECT_FALSE(set.empty());
  EXPECT_EQ(set.size(), 3u);
}

TEST(VersionIDSetTest, AddRemove) {
  VIDSet set;
  VID id(3);

  set.add(id);
  EXPECT_TRUE(set.has(id));
  EXPECT_EQ(set.size(), 1u);

  set.remove(id);
  EXPECT_FALSE(set.has(id));
  EXPECT_TRUE(set.empty());
}

TEST(VersionIDSetTest, HasSet) {
  VIDSet set1(VID(0), VID(2));
  VIDSet set2(VID(0), VID(2), VID(4));
  VIDSet set3(VID(0));

  EXPECT_TRUE(set1.has(set3));
  EXPECT_FALSE(set1.has(set2));
  EXPECT_TRUE(set2.has(set1));
}

TEST(VersionIDSetTest, HasAny) {
  VIDSet set1(VID(0), VID(2));
  VIDSet set2(VID(4));
  VIDSet set3(VID(0), VID(4));

  EXPECT_FALSE(set1.has_any(set2));
  EXPECT_TRUE(set1.has_any(set3));
}

TEST(VersionIDSetTest, AtAndFirst) {
  VIDSet set(VID(0), VID(2), VID(4));

  EXPECT_EQ(set.first(), VID(0));
  EXPECT_EQ(set.at(0), VID(0));
  EXPECT_EQ(set.at(1), VID(2));
  EXPECT_EQ(set.at(2), VID(4));
  EXPECT_EQ(set[0], VID(0));
  EXPECT_EQ(set[1], VID(2));
  EXPECT_EQ(set[2], VID(4));
}

TEST(VersionIDSetTest, Operators) {
  VIDSet set1(VID(0), VID(2));
  VIDSet set2(VID(2), VID(4));

  auto orSet = set1 | set2;
  EXPECT_EQ(orSet.size(), 3u);
  EXPECT_TRUE(orSet.has(VID(0)));
  EXPECT_TRUE(orSet.has(VID(2)));
  EXPECT_TRUE(orSet.has(VID(4)));

  auto andSet = set1 & set2;
  EXPECT_EQ(andSet.size(), 1u);
  EXPECT_TRUE(andSet.has(VID(2)));

  VIDSet set3;
  set3 |= VID(0);
  set3 |= VID(2);
  EXPECT_EQ(set3.size(), 2u);
  EXPECT_TRUE(set3.has(VID(0)));
  EXPECT_TRUE(set3.has(VID(2)));

  set3 &= VID(0);
  EXPECT_EQ(set3.size(), 1u);
  EXPECT_TRUE(set3.has(VID(0)));
  EXPECT_FALSE(set3.has(VID(2)));
}

TEST(VersionIDSetTest, Foreach) {
  VIDSet set(VID(0), VID(2), VID(4));

  unsigned count = 0;
  uint32_t mask = 0;
  set.foreach ([&](VID id) {
    ++count;
    mask |= id.value();
  });

  EXPECT_EQ(count, 3u);
  EXPECT_EQ(mask, (1u << 0) | (1u << 2) | (1u << 4));
}

// ============================================
// VersionInfo Tests
// ============================================

TEST(VersionInfoTest, DefaultConstruct) {
  VInfo info;
  EXPECT_FALSE(info.id.valid());
  EXPECT_EQ(info.name, nullptr);
}

TEST(VersionInfoTest, ConstructWithArgs) {
  VID id(3);
  VInfo info(id, "test");

  EXPECT_EQ(info.id, id);
  EXPECT_STREQ(info.name, "test");
}

TEST(VersionInfoTest, As) {
  struct DerivedInfo : VInfo {
    int value;
    DerivedInfo(VID id, const char* name, int v) : VInfo{id, name}, value(v) {}
  };

  DerivedInfo derived(VID(2), "derived", 42);
  VInfo* base = &derived;

  auto* d = base->as<DerivedInfo>();
  EXPECT_EQ(d->value, 42);
}

// ============================================
// VersionManager Tests
// ============================================

TEST(VersionManagerTest, Capacity) {
  EXPECT_EQ(VMgr::capacity(), 32u);

  using VID64 = cxxaux::VersionID<uint64_t>;
  using VInfo64 = cxxaux::VersionInfo<VID64>;
  using VMgr64 = cxxaux::VersionManager<VInfo64>;
  EXPECT_EQ(VMgr64::capacity(), 64u);
}

TEST(VersionManagerTest, Empty) {
  VMgr mgr;
  EXPECT_EQ(mgr.size(), 0u);
  EXPECT_FALSE(mgr.has(VID(0)));
}

TEST(VersionManagerTest, Emplace) {
  VMgr mgr;
  VID id(3);

  auto [ptr1, inserted1] = mgr.emplace(id, id, "test1");
  EXPECT_TRUE(inserted1);
  EXPECT_NE(ptr1, nullptr);
  EXPECT_EQ(ptr1->id, id);
  EXPECT_STREQ(ptr1->name, "test1");
  EXPECT_EQ(mgr.size(), 1u);

  auto [ptr2, inserted2] = mgr.emplace(id, id, "test2");
  EXPECT_FALSE(inserted2);
  EXPECT_EQ(ptr2, ptr1);
  EXPECT_STREQ(ptr2->name, "test1");
  EXPECT_EQ(mgr.size(), 1u);
}

TEST(VersionManagerTest, Add) {
  VMgr mgr;
  VID id(5);
  VInfo info(id, "added");

  auto [ptr1, inserted1] = mgr.add(id, info);
  EXPECT_TRUE(inserted1);
  EXPECT_NE(ptr1, nullptr);
  EXPECT_EQ(ptr1->id, id);
  EXPECT_STREQ(ptr1->name, "added");

  auto [ptr2, inserted2] = mgr.add(id, info);
  EXPECT_FALSE(inserted2);
  EXPECT_EQ(ptr2, ptr1);
}

TEST(VersionManagerTest, Find) {
  VMgr mgr;
  VID id1(0);
  VID id2(2);
  VID id3(4);

  mgr.emplace(id1, id1, "v1");
  mgr.emplace(id2, id2, "v2");

  auto* found1 = mgr.find(id1);
  EXPECT_NE(found1, nullptr);
  EXPECT_STREQ(found1->name, "v1");

  auto* found2 = mgr.find(id2);
  EXPECT_NE(found2, nullptr);
  EXPECT_STREQ(found2->name, "v2");

  auto* found3 = mgr.find(id3);
  EXPECT_EQ(found3, nullptr);
}

TEST(VersionManagerTest, Has) {
  VMgr mgr;
  VID id1(0);
  VID id2(2);
  VID id3(4);

  mgr.emplace(id1, id1, "v1");
  mgr.emplace(id2, id2, "v2");

  EXPECT_TRUE(mgr.has(id1));
  EXPECT_TRUE(mgr.has(id2));
  EXPECT_FALSE(mgr.has(id3));

  VIDSet set1(id1, id2);
  VIDSet set2(id1, id3);

  EXPECT_TRUE(mgr.has(set1));
  EXPECT_FALSE(mgr.has(set2));
  EXPECT_TRUE(mgr.has_any(set2));
}

TEST(VersionManagerTest, Foreach) {
  VMgr mgr;
  VID id1(0);
  VID id2(2);
  VID id3(4);

  mgr.emplace(id1, id1, "v1");
  mgr.emplace(id2, id2, "v2");
  mgr.emplace(id3, id3, "v3");

  unsigned count = 0;
  mgr.foreach ([&](VInfo& info) {
    ++count;
    EXPECT_TRUE(info.id.valid());
  });

  EXPECT_EQ(count, 3u);
}

TEST(VersionManagerTest, At) {
  VMgr mgr;
  VID id(3);

  mgr.emplace(id, id, "test");

  auto* ptr = mgr.at(3);
  EXPECT_NE(ptr, nullptr);
  EXPECT_STREQ(ptr->name, "test");
}

TEST(VersionManagerTest, Destructor) {
  VID id1(0);
  VID id2(2);

  {
    VMgr mgr;
    mgr.emplace(id1, id1, "v1");
    mgr.emplace(id2, id2, "v2");
  }
}
