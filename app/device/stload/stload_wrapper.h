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

 protected:
  /// @brief force the module as the current working directory
  /// This is CTRL.dll specific, it will find dependency files in the same
  /// directory as the DLL. This is a workaround for the DLL not being able to
  /// find the dependency files. app open the file dialog, the file dialog will
  /// will open the file and will change the current working directory to the
  /// file directory. This will cause the DLL to not find the dependency files.
  static void ForceModuleAsCurrentWorkingDirectory();
};
}  // namespace stload
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_STLOAD_STLOAD_WRAPPER_H_
