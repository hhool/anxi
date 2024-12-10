/**
 * @file alg_fitline_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief  Algorithm class unit test file
 * @version 0.1
 * @date 2024-12-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "gtest/gtest.h"

#include "app/esolution/algorithm/alg.h"

namespace anx {
namespace esolution {
namespace algorithm {

TEST(AlgTest, FitLine) {
  float x[] = {37.8f, 46.8f, 55.6f, 63.f};
  float y[] = {20, 40, 60, 80};

  float a, b;
  LineFit(x, y, 4, &a, &b);
  EXPECT_NEAR(a, 2.36494f, 0.0001);
  EXPECT_NEAR(b, -70.1390, 0.0001);
  float x5 = 28.34f * 2.0f;
  float y5 = a * x5 + b;
  int32_t y5_i = static_cast<int32_t>(y5);
  EXPECT_NEAR(y5, 63.9058, 0.0001);
}
}  // namespace algorithm
}  // namespace esolution
}  // namespace anx
