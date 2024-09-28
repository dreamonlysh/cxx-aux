#include <cxxaux/utility/pimpl.h>
#include <gtest/gtest.h>
#include <sstream>

class A : public cxxaux::Pimpl<A, 64> {
public:
  A(std::ostringstream& oss, int delta);

  int add(int lhs, int rhs);
};

class AImpl {
public:
  AImpl(std::ostringstream& oss, int delta) : oss(oss), delta(delta) {
    oss << "AImpl";
  }

  ~AImpl() noexcept { oss << "~AImpl"; }

  int add(int lhs, int rhs) { return lhs + rhs + delta; }

private:
  std::ostringstream& oss;
  int delta;
};

template <>
struct cxxaux::pimpl_traits<A> {
  using impl_type = AImpl;
  using pimpl_type = cxxaux::Pimpl<A, 64>; // optional
};

A::A(std::ostringstream& oss, int delta) : cxxaux::Pimpl<A, 64>(oss, delta) {}

int A::add(int lhs, int rhs) { return pimpl_cast(this)->add(lhs, rhs); }

TEST(PimplTest, WithStorageEmbed) {
  std::ostringstream oss;
  {
    A a(oss, 10);
    ASSERT_EQ(oss.str(), "AImpl");
    oss.str("");
    ASSERT_EQ(a.add(1, 2), 13);
  }

  ASSERT_EQ(oss.str(), "~AImpl");
}

class B : public cxxaux::Pimpl<B> {
public:
  B(std::ostringstream& oss, int delta);

  int add(int lhs, int rhs);
};

class BImpl {
public:
  BImpl(std::ostringstream& oss, int delta) : oss(oss), delta(delta) {
    oss << "BImpl";
  }

  ~BImpl() noexcept { oss << "~BImpl"; }

  int add(int lhs, int rhs) { return lhs + rhs + delta; }

private:
  std::ostringstream& oss;
  int delta;
};

template <>
struct cxxaux::pimpl_traits<B> {
  using impl_type = BImpl;
  using pimpl_type = cxxaux::Pimpl<B>; // optional
};

B::B(std::ostringstream& oss, int delta) : cxxaux::Pimpl<B>(oss, delta) {}

int B::add(int lhs, int rhs) { return pimpl_cast(this)->add(lhs, rhs); }

TEST(PimplTest, Default) {
  std::ostringstream oss;
  {
    B b(oss, 10);
    ASSERT_EQ(oss.str(), "BImpl");
    oss.str("");
    ASSERT_EQ(b.add(1, 2), 13);
  }

  ASSERT_EQ(oss.str(), "~BImpl");
}