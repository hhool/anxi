/**
 * @file defines.h
 * @author hhool (hhool@outlook.com)
 * @brief  define file for the solution
 * @version 0.1
 * @date 2024-07-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_DEFINES_H_
#define APP_COMMON_DEFINES_H_

#include <memory>
#include <string>

namespace anx {
namespace common {
const std::string kAppVersion = "0.1";
const std::string kAppDate = "2024-07-24";
const std::string kAppCopyRight = "2024";

const std::string kAppTitle = "Anxi";

const int32_t kSolutionName_Axially_Symmetrical = 0;
const int32_t kSolutionName_Stresses_Adjustable = 1;
const int32_t kSolutionName_3point_Bending = 2;
const int32_t kSolutionName_Vibration_Bending = 3;
}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_DEFINES_H_
