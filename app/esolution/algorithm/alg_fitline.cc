/**
 * @file alg_fitline.cc
 * @author hhool (hhool@outlook.com)
 * @brief  fit line algorithm
 * @version 0.1
 * @date 2024-12-09
 *
 * @copyright Copyright (c) 2024
 * @see https://www.cnblogs.com/ybqjymy/p/18040134.html
 */

#include "esolution/algorithm/alg.h"

#include <math.h>
#include <iostream>

#include "app/common/logger.h"

namespace anx {
namespace esolution {
namespace algorithm {

void LineFit(float x[], float y[], int n, float* a, float* b) {
  float sumx = 0.0;
  float sumy = 0.0;
  float sumx2 = 0.0;
  float sumxy = 0.0;
  for (int i = 0; i < n; i++) {
    LOG_F(LG_SENSITIVE) << "x=" << x[i] << ", y=" << y[i];
    sumx += x[i];
    sumy += y[i];
    sumx2 += x[i] * x[i];
    sumxy += x[i] * y[i];
  }
  float a1 = n * sumxy - sumx * sumy;
  float a2 = n * sumx2 - sumx * sumx;
  *a = a1 / a2;
  LOG_F(LG_SENSITIVE) << "a1=" << a1 << ", a2=" << a2 << ", a=" << *a;
  *b = (sumy - (*a) * sumx) / n;
  LOG_F(LG_SENSITIVE) << "b=" << *b << ", sumy=" << sumy << ", sumx=" << sumx;
}
}  // namespace algorithm
}  // namespace esolution
}  // namespace anx
