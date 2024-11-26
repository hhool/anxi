/**
 * @file thread_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief thread class unit test
 * @version 0.1
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/thread.h"

#include <gtest/gtest.h>

namespace anx {
namespace common {
namespace {
TEST(ThreadTest, Mutex) {
  Mutex mutex;
  mutex.lock();
  mutex.unlock();
}

TEST(ThreadTest, AutoLock) {
  Mutex mutex;
  AutoLock auto_lock(&mutex);
}

class TestRunnable : public Runnable {
 public:
  void run() override {}
};

TEST(ThreadTest, Thread) {
  TestRunnable runnable;
  Thread thread(&runnable);
  thread.start();
  thread.join();
}

class TestCondition : public Condition {
 public:
  void wait() {
    Mutex mutex;
    __super::wait(&mutex);
  }
};

TEST(ThreadTest, Condition) {
  TestCondition condition;
  condition.signal();
  condition.broadcast();
}

class TestAutoLock : public AutoLock {
 public:
  explicit TestAutoLock(Mutex* mutex) : AutoLock(mutex) {}
};

TEST(ThreadTest, AutoLock2) {
  Mutex mutex;
  TestAutoLock auto_lock(&mutex);
}

class TestThread : public Thread {
 public:
  explicit TestThread(Runnable* runnable) : Thread(runnable) {}
};

TEST(ThreadTest, Thread2) {
  TestRunnable runnable;
  TestThread thread(&runnable);
  thread.start();
  thread.join();
}

class TestThread2 : public Thread {
 public:
  TestThread2() = default;
};

TEST(ThreadTest, Thread3) {
  TestThread2 thread;
  thread.start();
  thread.join();
}
}  // namespace
}  // namespace common
}  // namespace anx
