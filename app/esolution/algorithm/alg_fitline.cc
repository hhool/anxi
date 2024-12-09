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

namespace anx {
namespace esolution {
namespace algorithm {

void LineFit(float x[], float y[], int n, float* a, float* b) {
  float sumx = 0.0;
  float sumy = 0.0;
  float sumx2 = 0.0;
  float sumxy = 0.0;
  for (int i = 0; i < n; i++) {
    sumx += x[i];
    sumy += y[i];
    sumx2 += x[i] * x[i];
    sumxy += x[i] * y[i];
  }
  float a1 = n * sumxy - sumx * sumy;
  float a2 = n * sumx2 - sumx * sumx;
  *a = a1 / a2;
  *b = (sumy - (*a) * sumx) / n;
}

void lineFit(float x[], float y[], int n, float* a, float* b) {
  float avgX = 0, avgY = 0;
  float Lxx = 0, Lyy = 0, Lxy = 0;
  for (int i = 0; i < n; i++) {
    avgX += x[i] / n;
    avgY += y[i] / n;
  }
  for (int i = 0; i < n; i++) {
    Lxy += (x[i] - avgX) * (y[i] - avgY);
    Lxx += (x[i] - avgX) * (x[i] - avgX);
    Lyy += (y[i] - avgY) * (y[i] - avgY);
  }
  *a = Lxy / Lxx;
  *b = avgY - (*a) * avgX;
  std::cout << "相关系数r=" << Lxy / sqrt(Lxx * Lyy) << std::endl;
  std::cout << "线性方程:" << "y=" << *a << "+" << *b << "x" << std::endl;
}

}  // namespace algorithm
}  // namespace esolution
}  // namespace anx
