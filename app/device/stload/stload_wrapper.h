/**
 * @file stload_wrapper.h
 * @author hhool (hhool@outlook.com)
 * @brief st load api wrapper, load the st load api dll and call the api.
 * @version 0.1
 * @date 2024-09-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_STLOAD_STLOAD_WRAPPER_H_
#define APP_DEVICE_STLOAD_STLOAD_WRAPPER_H_

#include <memory>
#include <string>
#include <vector>

#include "app/device/stload/stload_common.h"

namespace anx {
namespace device {
namespace stload {

class STLoadLoader {
 public:
  STLoadLoader();
  ~STLoadLoader();

  static bool Load(const std::string& dll_path);
  static void Unload();
  static void* handle_;
  static stload_api st_api_;
};
}  // namespace stload
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_STLOAD_STLOAD_WRAPPER_H_
