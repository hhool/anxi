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
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  if (app_data_dir.empty()) {
    LOG_F(LG_ERROR) << "GetApplicationDataPath failed";
    return -1;
  }

  std::string app_config_dir = app_data_dir;
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

  std::string app_resource_default =
      app_module_dir + anx::common::kPathSeparator + "default";
  std::string app_resource_target =
      app_config_dir + anx::common::kPathSeparator + "default";
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

  std::string app_resource_expdata =
      app_config_dir + anx::common::kPathSeparator + "expdata";
  if (!anx::common::MakeSureFolderPathExist(app_resource_expdata)) {
    LOG_F(LG_ERROR) << "MakeSureFolderPathExist failed:"
                    << app_resource_expdata;
    return -6;
  }

  std::string app_resource_expreport =
      app_config_dir + anx::common::kPathSeparator + "expreport";
  if (!anx::common::MakeSureFolderPathExist(app_resource_expreport)) {
    LOG_F(LG_ERROR) << "MakeSureFolderPathExist failed:"
                    << app_resource_expreport;
    return -7;
  }

  return 0;
}

int32_t AppConfig::ResetAppConfig() {
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  if (app_data_dir.empty()) {
    LOG_F(LG_ERROR) << "GetApplicationDataPath failed";
    return -1;
  }
  // remove the anxi folder
  std::string app_config_dir = app_data_dir;
  if (!anx::common::DirectoryExists(app_config_dir)) {
    return 0;
  }
  if (!anx::common::RemoveFolder(app_config_dir)) {
    LOG_F(LG_ERROR) << "RemoveFolder failed:" << app_config_dir;
    return -2;
  }
  return 0;
}

}  // namespace ui
}  // namespace anx
