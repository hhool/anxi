/**
 * @file ultras_helper.h
 * @author hhool (hhool@outlook.com)
 * @brief  ultrasonic helper
 * @version 0.1
 * @date 2024-09-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_ULTRASONIC_ULTRA_HELPER_H_
#define APP_DEVICE_ULTRASONIC_ULTRA_HELPER_H_

#include <memory>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

namespace anx {
namespace device {
namespace ultrasonic {

class UltrasonicHelper {
 public:
  static bool InitUltrasonic();
  static void UnInitUltrasonic();

  static bool Is_Ultrasonic_Simulation();

 private:
  static bool is_ultrasonic_simulation_;
};
}  // namespace ultrasonic
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_ULTRASONIC_ULTRA_HELPER_H_
