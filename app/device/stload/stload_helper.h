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

/// @brief st load result
class STResult {
 public:
  STResult() : pos_(0), load_(0), status_(0) {}
  ~STResult() {}
  /// @brief position value in mm unit
  double pos_;
  /// @brief load value in N unit
  double load_;
  /// @brief status value of the st load
  uint32_t status_;
};

class STLoadHelper {
 public:
  static bool InitStLoad(int32_t verison = 1, const char* sensor = nullptr);
  static void UnInitStLoad();

  static anx::device::stload::STLoadLoader st_load_loader_;
  static bool Is_Stload_Simulation();

  static int32_t STLoadSetup();

  static int32_t STLoadVersion() { return version_; }

 private:
  static bool is_stload_simulation_;
  static int32_t version_;
  static int32_t sensor_;
};

}  // namespace stload
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_STLOAD_STLOAD_HELPER_H_
