/**
 * @file alg.h
 * @author hhool (hhool@outlook.com)
 * @brief  Algorithm class header file
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_ESOLUTION_ALGORITHM_ALG_H_
#define APP_ESOLUTION_ALGORITHM_ALG_H_

#include <stdint.h>

namespace anx {
namespace esolution {
namespace algorithm {
/// @note: keyword 弹性模量 in Chinese is used to search for the definition of E
/// in the code， 单位为GPa 10^9Pa
/// @note: keyword 密度 in Chinese is used to search for the definition of rho
/// in the code， 单位为kg/m^3
/// @note: keyword 长度 in Chinese is used to search for the definition of L in
/// the code， 单位为mm
/// @note: keyword 宽度 in Chinese is used to search for the definition of W in
/// the code， 单位为mm
/// @note: keyword 厚度 in Chinese is used to search for the definition of t in
/// the code， 单位为mm
/// @note: keyword 频率 in Chinese is used to search for the definition of f in
/// the code， 单位为KHz

const double kConstForLenghtOfTh3Design = 0.494049f;
const double kConstForLength0OfTh3Design = 0.268999f;
/**
 * @brief Calculate the length of the third harmonic design
 * @param E 弹性模量
 * @param h 厚度
 * @param rho 密度
 * @param f 频率
 * @param W 宽度
 * @param t 厚度
 * @return double
 */
double CalcTh3Design(double E,
                     double h,
                     double rho,
                     int f,
                     double W,
                     double t,
                     double cval);

/**
 * @brief Line fitting algorithm
 * @param x x axis data
 * @param y y axis data
 * @param n data count
 * @param a a value
 * @param b b value
 */
void LineFit(float x[], float y[], int n, float* a, float* b);
/// TODO(hhool): will remove
void lineFit(float x[], float y[], int n, float* a, float* b);
}  // namespace algorithm
}  // namespace esolution
}  // namespace anx

#endif  // APP_ESOLUTION_ALGORITHM_ALG_H_
