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
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace anx {
namespace common {
std::string GetAppPath() {
#if defined(_WIN32) || defined(_WIN64)
  char path[MAX_PATH];
  GetModuleFileNameA(NULL, path, MAX_PATH);
  std::string app_path(path);
  return app_path.substr(0, app_path.find_last_of("\\"));
#else
  char path[1024];
  int count = readlink("/proc/self/exe", path, 1024);
  if (count < 0 || count >= 1024) {
    return "";
  }
  path[count] = '\0';
  std::string app_path(path);
  return app_path.substr(0, app_path.find_last_of("/"));
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
  return app_path.substr(app_path.find_last_of("\\") + 1);
#else
  char path[1024];
  int count = readlink("/proc/self/exe", path, 1024);
  if (count < 0 || count >= 1024) {
    return "";
  }
  path[count] = '\0';
  std::string app_path(path);
  return app_path.substr(app_path.find_last_of("/") + 1);
#endif
  return path;
}

bool MakeSureFolderPathExist(const std::string& path) {
  std::string dir = path;
  if (dir.empty()) {
    return false;
  }
  // find the last '/' or '\'
  size_t pos = dir.find_last_of("/\\");
  if (pos == std::string::npos) {
    return false;
  }
  dir = dir.substr(0, pos);
  bool ret = false;
#if defined(_WIN32) || defined(_WIN64)
  CreateDirectoryA(dir.c_str(), NULL);
  // TODO(hhool):
  ret = true;
#else
  ret = mkdir(dir.c_str(), 0777) == 0;
#endif
  return ret;
}

}  // namespace common
}  // namespace anx
