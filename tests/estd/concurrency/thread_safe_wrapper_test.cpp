#include <estd/__concurrency/thread_safe_wrapper.h>
#include <gtest/gtest.h>
#include <string>

namespace {

struct A {
  void call(std::string& s) { s.push_back('0'); }
};

struct Locker {
  Locker(std::string& s) : s(s) {}

  void lock() { s.push_back('1'); }
  void unlock() { s.push_back('2'); }
  void try_lock() { s.push_back('3'); }

  std::string& s;
};

TEST(ThreadSafeWrapper, ObjectMemberFunction) {
  std::string buffer;
  Locker lock(buffer);
  A a;
  es::ThreadSafeFunctionWrapper a_call(a, &A::call, lock);
  a_call(buffer);
  ASSERT_EQ(buffer, "102");
}

TEST(ThreadSafeWrapper, Object) {
  std::string buffer;
  Locker lock(buffer);
  A a;
  es::ThreadSafeObjectWrapper wrapper(a, lock);
  wrapper.invoke(&A::call, buffer);
  ASSERT_EQ(buffer, "102");
}

void call(std::string& s) { s.push_back('9'); }

TEST(ThreadSafeWrapper, Function) {
  std::string buffer;
  Locker lock(buffer);
  es::ThreadSafeFunctionWrapper wrapper(call, lock);
  wrapper(buffer);
  ASSERT_EQ(buffer, "192");
}

TEST(ThreadSafeWrapper, FunctionLambda) {
  std::string buffer;
  Locker lock(buffer);
  auto call2 = [](std::string& s) { s.push_back('8'); };
  es::ThreadSafeFunctionWrapper wrapper(call2, lock);
  wrapper(buffer);
  ASSERT_EQ(buffer, "182");
}

TEST(ThreadSafeWrapper, FunctionLambdaByRef) {
  std::string buffer;
  Locker lock(buffer);
  auto call2 = [](std::string& s) { s.push_back('8'); };
  es::ThreadSafeFunctionWrapper wrapper(std::ref(call2), lock);
  wrapper(buffer);
  ASSERT_EQ(buffer, "182");
}

TEST(ThreadSafeWrapper, FunctionLambdaByRValue) {
  std::string buffer;
  Locker lock(buffer);
  es::ThreadSafeFunctionWrapper wrapper(
      [](std::string& s) { s.push_back('8'); }, lock);
  wrapper(buffer);
  ASSERT_EQ(buffer, "182");
}

} // namespace
