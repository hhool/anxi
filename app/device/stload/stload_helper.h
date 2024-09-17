/**
 * @file stload_helper.h
 * @author hhool (hhool@outlook.com)
 * @brief st load helper
 * @version 0.1
 * @date 2024-09-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_STLOAD_STLOAD_HELPER_H_
#define APP_DEVICE_STLOAD_STLOAD_HELPER_H_

#include "app/device/stload/stload_wrapper.h"

#include <memory>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

namespace anx {
namespace device {
namespace stload {
class STLoadHelper {
 public:
  static bool InitStLoad();
  static void UnInitStLoad();

  static anx::device::stload::STLoadLoader st_load_loader_;
  static bool Is_Stload_Simulation();

 private:
  static bool is_stload_simulation_;
};

}  // namespace stload
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_STLOAD_STLOAD_HELPER_H_
