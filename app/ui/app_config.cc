/**
 * @file app_config.cc
 * @author hhool (hhool@outlook.com)
 * @brief  app config file. manage the app config resource and settings
 * @version 0.1
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/app_config.h"

#include <memory>
#include <string>
#include <vector>

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

namespace anx {
namespace ui {

AppConfig::AppConfig() {}

AppConfig::~AppConfig() {}

int32_t AppConfig::PrepareAppConfig() {
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  if (app_data_dir.empty()) {
    LOG_F(LG_ERROR) << "GetApplicationDataPath failed";
    return -1;
  }

#if defined(_WIN32) || defined(_WIN64)
  std::string app_config_dir = app_data_dir + "\\anxi";
#else
  std::string app_config_dir = app_data_dir + "/.anxi";
#endif
  // check the folder exist, if exist, return
  if (anx::common::DirectoryExists(app_config_dir)) {
    return 0;
  }

  if (!anx::common::MakeSureFolderPathExist(app_config_dir)) {
    LOG_F(LG_ERROR) << "MakeSureFolderPathExist failed:" << app_config_dir;
    return -2;
  }

  std::string app_module_dir = anx::common::GetModuleDir();
  if (app_module_dir.empty()) {
    LOG_F(LG_ERROR) << "GetModuleDir failed";
    return -3;
  }

  std::string app_resource_default = app_module_dir + "/default";
  std::string app_resource_target = app_config_dir + "/default";
  if (!anx::common::MakeSureFolderPathExist(app_resource_target)) {
    LOG_F(LG_ERROR) << "MakeSureFolderPathExist failed:" << app_resource_target;
    return -4;
  }
  // copy the resource folder to the app config folder
  int32_t ret =
      anx::common::CopyFolder(app_resource_default, app_resource_target);
  if (ret != 0) {
    LOG_F(LG_ERROR) << "CopyFolder failed:" << app_resource_default << " to "
                    << app_resource_target << " ret:" << ret;
    return -5;
  }

  return 0;
}

int32_t AppConfig::ResetAppConfig() {
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  if (app_data_dir.empty()) {
    LOG_F(LG_ERROR) << "GetApplicationDataPath failed";
    return -1;
  }
  // remove the anxi folder
  std::string app_config_dir = app_data_dir + "/anxi";
  if (!anx::common::DirectoryExists(app_config_dir)) {
    return 0;
  }
  if (!anx::common::RemoveFolder(app_config_dir)) {
    LOG_F(LG_ERROR) << "RemoveFolder failed:" << app_config_dir;
    return -2;
  }
  return 0;
}

std::string AppConfig::GetAppDataPathWithFolderName(
    const std::string& folder_name) {
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  if (app_data_dir.empty()) {
    return "";
  }
#if defined(_WIN32) || defined(_WIN64)
  return app_data_dir + "\\" + folder_name;
#else
  return app_data_dir + "/" + folder_name;
#endif
}

}  // namespace ui
}  // namespace anx
