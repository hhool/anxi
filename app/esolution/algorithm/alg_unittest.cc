/**
 * @file alg_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief  Algorithm class unit test file
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "gtest/gtest.h"

#include "app/esolution/algorithm/alg.h"

namespace anx {
namespace esolution {
namespace algorithm {

TEST(AlgTest, CalcLengthOfTh3Design) {
  double E = 116;                            // 10^9 MPa
  double h = 4;                              // height mm
  double rho = 4379;                         // rho kg/m3
  int f = 20;                                // freq 20 khz
  double W = 10;                             // width
  double t = 4;                              // height
  double cval = kConstForLenghtOfTh3Design;  // const value for length
  double result = CalcTh3Design(E, h, rho, f, W, t, cval);
  EXPECT_NEAR(result, 31.70, 0.000001);
}

TEST(AlgTest, CalcSpanOfTh3Design) {
  double E = 116;                             // 10^9 MPa
  double h = 4;                               // height mm
  double rho = 4379;                          // rho kg/m3
  int f = 20;                                 // freq 20 khz
  double W = 10;                              // width
  double t = 4;                               // height
  double cval = kConstForLength0OfTh3Design;  // const value for length
  double result = CalcTh3Design(E, h, rho, f, W, t, cval);
  EXPECT_NEAR(result, 17.23, 0.000001);
}

}  // namespace algorithm
}  // namespace esolution
}  // namespace anx
