#include <estd/__concurrency/mutex_silent.h>
#include <gtest/gtest.h>

TEST(MutexSilent, BasicLock) {
  es::mutex_silent mtx;

  mtx.lock();
  mtx.unlock();
}

TEST(MutexSilent, TryLockAlwaysReturnsFalse) {
  es::mutex_silent mtx;

  bool result = mtx.try_lock();
  EXPECT_FALSE(result);
}

TEST(MutexSilent, DefaultInstance) {
  es::default_mutex_silent.lock();
  es::default_mutex_silent.unlock();
}
