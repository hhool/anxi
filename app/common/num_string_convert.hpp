/**
 * @file num_string_convert.hpp
 * @author hhool (hhool@outlook.com)
 * @brief number and string convert implementation.
 * @version 0.1
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_NUM_STRING_CONVERT_HPP__
#define APP_COMMON_NUM_STRING_CONVERT_HPP__

#include <iomanip>
#include <string>

namespace anx {
namespace common {

template <typename T>
std::string to_string_with_precision(const T a_value, const int n) {
  int nn = n;
  std::ostringstream out;
  out << std::fixed << std::setprecision(nn) << a_value;
  return out.str();
}

}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_NUM_STRING_CONVERT_HPP__
