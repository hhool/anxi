/**
 * @file time_utils_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief time utils unit test
 * @version 0.1
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/time_utils.h"

#include <gtest/gtest.h>

namespace anx {
namespace common {
namespace {

TEST(TimeUtilsTest, GetCurrentTimeMicros) {
  uint64_t time = GetCurrentTimeMicros();
  EXPECT_GT(time, 0);
}

TEST(TimeUtilsTest, GetCurrentTimeMillis) {
  int64_t time = GetCurrentTimeMillis();
  EXPECT_GT(time, 0);
}

TEST(TimeUtilsTest, GetCurrentTimeSeconds) {
  int64_t time = GetCurrentTimeSeconds();
  EXPECT_GT(time, 0);
}

TEST(TimeUtilsTest, sleep_ms) {
  int64_t time = GetCurrentTimeMillis();
  sleep_ms(1000);
  int64_t time2 = GetCurrentTimeMillis();
  EXPECT_GT(time2, time);
}

}  // namespace
}  // namespace common
}  // namespace anx
