#include <cxxaux/utility/guard.h>
#include <gtest/gtest.h>
#include <map>
#include <vector>

struct Object {
  Object(std::pair<int, int>& status) : status(status) { status.first += 1; }
  ~Object() noexcept { status.second += 1; }

  std::pair<int, int>& status;
};

TEST(GuardTest, Object) {
  std::pair<int, int> status{0, 0};
  {
    auto a = new Object(status);
    CXXAUX_OBJECT_GUARD(a);
    ASSERT_EQ(status.first, 1);
    ASSERT_EQ(status.second, 0);
  }
  ASSERT_EQ(status.second, 1);
}

TEST(GuardTest, Vector) {
  std::pair<int, int> status{0, 0};
  {
    std::vector<Object*> vec;
    CXXAUX_GUARD(vec) {
      for (auto* p : vec)
        delete p;
    };
    vec.push_back(new Object(status));
    vec.push_back(new Object(status));
    ASSERT_EQ(status.first, 2);
    ASSERT_EQ(status.second, 0);
  }
  ASSERT_EQ(status.second, 2);
}

TEST(GuardTest, Map) {
  std::pair<int, int> status{0, 0};
  {
    std::map<int, Object*> map;
    CXXAUX_GUARD(map) {
      for (auto [_, p] : map)
        delete p;
    };
    map[0] = new Object(status);
    map[1] = new Object(status);
    ASSERT_EQ(status.first, 2);
    ASSERT_EQ(status.second, 0);
  }
  ASSERT_EQ(status.second, 2);
}
