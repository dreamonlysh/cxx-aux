// Version management example demonstrating VersionID, VersionIDSet, and VersionManager

#include <cxxaux/utility/version.h>
#include <iostream>

using VID = cxxaux::VersionID<uint32_t>;
using VIDSet = cxxaux::VersionIDSet<VID>;
using VInfo = cxxaux::VersionInfo<VID>;
using VMgr = cxxaux::VersionManager<VInfo>;

int main() {
  std::cout << "=== VersionID Example ===\n\n";

  // Create VersionIDs from offsets
  VID v1(0);
  VID v2(5);
  VID v3(10);

  std::cout << "v1 offset: " << v1.offset() << ", value: " << v1.value() << "\n";
  std::cout << "v2 offset: " << v2.offset() << ", value: " << v2.value() << "\n";
  std::cout << "v3 offset: " << v3.offset() << ", value: " << v3.value() << "\n";

  std::cout << "\n=== VersionIDSet Example ===\n\n";

  // Create a set of VersionIDs
  VIDSet set1(v1, v2, v3);
  std::cout << "set1 size: " << set1.size() << "\n";

  // Iterate over the set
  std::cout << "set1 offsets: ";
  set1.foreach([](VID id) { std::cout << id.offset() << " "; });
  std::cout << "\n";

  // Set operations
  VIDSet set2(VID(0), VID(5));
  VIDSet set3(VID(10), VID(15));

  std::cout << "set1 has set2: " << (set1.has(set2) ? "yes" : "no") << "\n";
  std::cout << "set1 has set3: " << (set1.has(set3) ? "yes" : "no") << "\n";
  std::cout << "set1 has_any set3: " << (set1.has_any(set3) ? "yes" : "no") << "\n";

  // Union and intersection
  VIDSet unionSet = set1 | set3;
  VIDSet intersectSet = set1 & set3;

  std::cout << "union size: " << unionSet.size() << "\n";
  std::cout << "intersection size: " << intersectSet.size() << "\n";

  std::cout << "\n=== VersionManager Example ===\n\n";

  VMgr mgr;
  std::cout << "Manager capacity: " << VMgr::capacity() << "\n";

  // Add versions using emplace
  auto [ptr1, inserted1] = mgr.emplace(VID(0), VID(0), "v1.0");
  std::cout << "Added v1.0: " << (inserted1 ? "yes" : "no") << "\n";

  auto [ptr2, inserted2] = mgr.emplace(VID(5), VID(5), "v2.0");
  std::cout << "Added v2.0: " << (inserted2 ? "yes" : "no") << "\n";

  // Try to add duplicate
  auto [ptr3, inserted3] = mgr.emplace(VID(0), VID(0), "v1.0-duplicate");
  std::cout << "Added duplicate v1.0: " << (inserted3 ? "yes" : "no") << "\n";
  std::cout << "Existing name: " << ptr3->name << "\n";

  // Find versions
  if (auto* found = mgr.find(VID(5))) {
    std::cout << "Found version at offset 5: " << found->name << "\n";
  }

  // Check existence
  std::cout << "Has offset 0: " << (mgr.has(VID(0)) ? "yes" : "no") << "\n";
  std::cout << "Has offset 10: " << (mgr.has(VID(10)) ? "yes" : "no") << "\n";

  // Iterate over all versions
  std::cout << "All versions: ";
  mgr.foreach([](VInfo& info) {
    std::cout << info.name << " (offset " << info.id.offset() << ") ";
  });
  std::cout << "\n";

  std::cout << "\nManager size: " << mgr.size() << "\n";

  return 0;
}
