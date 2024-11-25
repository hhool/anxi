/**
 * @file app_config.h
 * @author hhool (hhool@outlook.com)
 * @brief  app config file. manage the app config resource and settings
 * @version 0.1
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_APP_CONFIG_H_
#define APP_UI_APP_CONFIG_H_

#include <memory>
#include <string>
#include <vector>

#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

namespace anx {
namespace ui {

class AppConfig {
 public:
  AppConfig();
  virtual ~AppConfig();

 public:
  /// @brief load the app config
  /// @return int32_t 0 if success, -1 if failed
  /// @note copy the app config to the AppData/Roaming/anxi folder
  static int32_t PrepareAppConfig();

  /// @breif reset the app config
  /// @return int32_t 0 if success, -1 if failed
  static int32_t ResetAppConfig();

  /// @brief get the app data path with folder name
  /// @param folder_name the folder name
  /// @return the app data path
  static std::string GetAppDataPathWithFolderName(
      const std::string& folder_name);
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_APP_CONFIG_H_
