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

bool FolderPathExist(const std::string& dir) {
#if defined(_WIN32) || defined(_WIN64)
  DWORD dwAttr = GetFileAttributes(String2WString(dir).c_str());
  if (dwAttr == 0xffffffff || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
    return false;
  }
  return true;
#else
  if (access(dir.c_str(), 0) != 0) {
    return false;
  }
  return true;
#endif
}

int32_t CopyFolder(const std::string& src, const std::string& dest) {
  // check if the source folder exists
  if (!DirectoryExists(src)) {
    return -1;
  }
  // check if the destination folder exists
  if (!MakeSureFolderPathExist(dest)) {
    return -2;
  }
  // copy the folder
#if defined(_WIN32) || defined(_WIN64)
  // enumerate the source folder
  std::string src_folder = src + "\\*";
  WIN32_FIND_DATAA find_data;
  HANDLE hFind = FindFirstFileA(src_folder.c_str(), &find_data);
  if (hFind == INVALID_HANDLE_VALUE) {
    return -3;
  }
  do {
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      continue;
    }
    std::string src_file = src + "\\" + find_data.cFileName;
    std::string dest_file = dest + "\\" + find_data.cFileName;
    if (!CopyFileA(src_file.c_str(), dest_file.c_str(), FALSE)) {
      FindClose(hFind);
      return -4;
    }
  } while (FindNextFileA(hFind, &find_data) != 0);
  FindClose(hFind);
#else
  // TODO(hhool): implement the copy folder for linux
  return -5;
#endif
  return 0;
}

bool MakeSureFolderPathExist(const std::string& path) {
  std::string dir = path;
  if (dir.empty()) {
    return false;
  }
  /// check if the directory exists
  if (FolderPathExist(dir)) {
    return true;
  }
  /// check last word of the path is not a directory, then remove the last word.
  /// get the last word of the path
  std::string::size_type last_pos = dir.find_last_of("\\/");
  if (last_pos == std::string::npos) {
    return false;
  }
  std::string last_word = dir.substr(last_pos + 1);
  if (last_word.empty()) {
    return false;
  }
  /// check last word with file extension
  if (last_word.find(".") != std::string::npos) {
    dir = dir.substr(0, last_pos);
  }
  /// create the directory. loop to create the directory
  std::string::size_type pos = 0;
  bool reached_end = false;
  while (!reached_end) {
    pos = dir.find_first_of("\\/", pos + 1);
    if (pos == std::string::npos) {
      pos = dir.size();
      reached_end = true;
    }
    std::string sub_dir = dir.substr(0, pos);
    if (!FolderPathExist(sub_dir)) {
#if defined(_WIN32) || defined(_WIN64)
      if (!CreateDirectory(anx::common::UTF8ToUnicode(sub_dir).c_str(), NULL)) {
        return false;
      }
#else
      if (mkdir(sub_dir.c_str(), 0777) != 0) {
        return false;
      }
#endif
    }
  }
  return true;
}
}  // namespace common
}  // namespace anx
