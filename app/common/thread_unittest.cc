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
}  // namespace
}  // namespace common
}  // namespace anx
