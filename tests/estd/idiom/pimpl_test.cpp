#include <estd/idiom/pimpl.h>
#include <gtest/gtest.h>
#include <sstream>

class A : public es::idiom::pimpl<A, 64> {
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
struct es::idiom::pimpl_traits<A> {
  using impl_type = AImpl;
  using pimpl_type = es::idiom::pimpl<A, 64>; // optional
};

A::A(std::ostringstream& oss, int delta)
    : es::idiom::pimpl<A, 64>(oss, delta) {}

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

class B : public es::idiom::pimpl<B> {
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
struct es::idiom::pimpl_traits<B> {
  using impl_type = BImpl;
  using pimpl_type = es::idiom::pimpl<B>; // optional
};

B::B(std::ostringstream& oss, int delta) : es::idiom::pimpl<B>(oss, delta) {}

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

class C : public es::idiom::pimpl<C, 4> {
public:
  C(int data);
  int get() const;
};

class CImpl {
public:
  CImpl(int data) : data(data) {}
  int get() const { return data; }

private:
  int data;
};

template <>
struct es::idiom::pimpl_traits<C> {
  using impl_type = CImpl;
};

C::C(int data) : es::idiom::pimpl<C, 4>(data) {}

int C::get() const { return pimpl_cast(this)->get(); }

TEST(PimplTest, MoveWithStorageEmbed) {
  C c(10);
  C c2 = std::move(c);
  ASSERT_EQ(c2.get(), 10);
  C c3 = std::move(c2);
  ASSERT_EQ(c3.get(), 10);
}

class D : public es::idiom::pimpl<D> {
public:
  D(int data);
  int get() const;
};

class DImpl {
public:
  DImpl(int data) : data(data) {}
  int get() const { return data; }

private:
  int data;
};

template <>
struct es::idiom::pimpl_traits<D> {
  using impl_type = DImpl;
};

D::D(int data) : es::idiom::pimpl<D>(data) {}

int D::get() const { return pimpl_cast(this)->get(); }

TEST(PimplTest, MoveDefault) {
  D d(10);
  D d2 = std::move(d);
  ASSERT_EQ(d2.get(), 10);
  D d3 = std::move(d2);
  ASSERT_EQ(d3.get(), 10);
}
