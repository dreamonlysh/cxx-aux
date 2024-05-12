#include <estd/idiom/pimpl.h>
#include <gtest/gtest.h>
#include <sstream>

class A : public es::Pimpl<A, 64> {
public:
  A(std::ostringstream& oss, int delta);

  int add(int lhs, int rhs);
};

class AImpl {
public:
  AImpl(std::ostringstream& oss, int delta) : oss(oss), delta(delta) {
    oss << "AImpl";
  }

  ~AImpl() noexcept {
    oss << "~AImpl";
  }

  int add(int lhs, int rhs) {
    return lhs + rhs + delta;
  }

private:
  std::ostringstream& oss;
  int delta;
};

template <>
struct es::pimpl_traits<A> {
  using impl_type = AImpl;
  using pimpl_type = es::Pimpl<A, 64>; // optional
};

A::A(std::ostringstream& oss, int delta) : es::Pimpl<A, 64>(oss, delta) {}

int A::add(int lhs, int rhs) {
  return pimpl_cast(this)->add(lhs, rhs); 
}

TEST(PimplTest, test) {
  std::ostringstream oss;
  {
    A a(oss, 10);
    ASSERT_EQ(oss.str(), "AImpl");
    oss.str("");
    ASSERT_EQ(a.add(1, 2), 13);
  }

  ASSERT_EQ(oss.str(), "~AImpl");
}