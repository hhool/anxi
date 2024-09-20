/**
 * @file alg_th3.cc
 * @author hhool (hhool@outlook.com)
 * @brief Algorithm class implementation file
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/esolution/algorithm/alg.h"

#include <cmath>

namespace anx {
namespace esolution {
namespace algorithm {

double CalcTh3Design(double E,
                     double h,
                     double rho,
                     int f,
                     double W,
                     double t,
                     double cval) {
  double dividend = E * h * h * 1000000000;
  double divisor = rho * f * f;
  double sqrt_value = sqrt(dividend / divisor);
  double sqrt_value_2 = sqrt(sqrt_value);
  double L = sqrt_value_2 * 2;
  return L * cval ;
}

}  // namespace algorithm
}  // namespace esolution
}  // namespace anx
