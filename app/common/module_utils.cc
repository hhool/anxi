/**
 * @file module_utils.cc
 * @author hhool (hhool@outlook.com)
 * @brief module utils file for the module utils
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/module_utils.h"

#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <shlobj.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "app/common/file_utils.h"
#include "app/common/string_utils.h"

namespace anx {
namespace common {
std::string GetAppPath() {
#if defined(_WIN32) || defined(_WIN64)
  char path[MAX_PATH];
  GetModuleFileNameA(NULL, path, MAX_PATH);
  std::string app_path(path);
  return app_path.substr(0, app_path.find_last_of(anx::common::kPathSeparator));
#else
  char path[1024];
  int count = readlink("/proc/self/exe", path, 1024);
  if (count < 0 || count >= 1024) {
    return "";
  }
  path[count] = '\0';
  std::string app_path(path);
  return app_path.substr(0, app_path.find_last_of(anx::common::kPathSeparator));
#endif
  return path;
}

std::string GetModuleDir() {
  return GetAppPath();
}

std::string GetModuleName() {
#if defined(_WIN32) || defined(_WIN64)
  char path[MAX_PATH];
  GetModuleFileNameA(NULL, path, MAX_PATH);
  std::string app_path(path);
  return app_path.substr(app_path.find_last_of(anx::common::kPathSeparator) +
                         1);
#else
  char path[1024];
  int count = readlink("/proc/self/exe", path, 1024);
  if (count < 0 || count >= 1024) {
    return "";
  }
  path[count] = '\0';
  std::string app_path(path);
  return app_path.substr(app_path.find_last_of(anx::common::kPathSeparator) +
                         1);
#endif
  return path;
}

std::string GetApplicationDataPath() {
#if defined(_WIN32) || defined(_WIN64)
  char path[MAX_PATH];
  SHGetSpecialFolderPathA(NULL, path, CSIDL_APPDATA, FALSE);
  std::string app_path(path);
  return app_path;
#else
  return "";
#endif
}

std::string GetApplicationDataPath(const std::string& app_name) {
  std::string app_data_path = GetApplicationDataPath();
  if (app_data_path.empty()) {
    return "";
  }
  app_data_path += anx::common::kPathSeparator;
  app_data_path += app_name;
  return app_data_path;
}

}  // namespace common
}  // namespace anx
