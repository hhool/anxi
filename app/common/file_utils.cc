/**
 * @file file_utils.cc
 * @author hhool (hhool@outlook.com)
 * @brief  file utils for file operation in the system
 * @version 0.1
 * @date 2024-08-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/file_utils.h"

#include <string>

#include "app/common/logger.h"
#include "app/common/string_utils.h"

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#include <io.h>
#include <shellapi.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace anx {
namespace common {

bool FileExists(const std::string& file_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string path = file_path;
  /// transfer db_name to Unicode
#if defined(WIN32)
  std::wstring w_path = anx::common::String2WString(file_path.c_str());
  path = anx::common::UnicodeToUTF8(w_path.c_str());
#endif
  return _access(path.c_str(), 0) == 0;
#else
  return access(file_path.c_str(), F_OK) == 0;
#endif
}

bool DirectoryExists(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string path = dir_path;
  /// transfer db_name to Unicode
#if defined(WIN32)
  std::wstring w_path = anx::common::String2WString(dir_path.c_str());
  path = anx::common::UnicodeToUTF8(w_path.c_str());
#endif
  return _access(path.c_str(), 0) == 0;
#else
  return access(dir_path.c_str(), F_OK) == 0;
#endif
}

bool CreateFolder(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string path = dir_path;
  /// transfer db_name to Unicode
#if defined(WIN32)
  std::wstring w_path = anx::common::String2WString(dir_path.c_str());
  path = anx::common::UnicodeToUTF8(w_path.c_str());
#endif
  return _rmdir(path.c_str()) == 0;
#else
  return mkdir(dir_path.c_str(), 0777) == 0;
#endif
}

bool RemoveFolder(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string path = dir_path;
  /// transfer db_name to Unicode
#if defined(WIN32)
  std::wstring w_path = anx::common::String2WString(dir_path.c_str());
  path = anx::common::UnicodeToUTF8(w_path.c_str());
#endif
  return _rmdir(path.c_str()) == 0;
#else
  return rmdir(dir_path.c_str()) == 0;
#endif
}

bool RemoveFile(const std::string& file_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string path = file_path;
  /// transfer db_name to Unicode
#if defined(WIN32)
  std::wstring w_path = anx::common::String2WString(file_path.c_str());
  path = anx::common::UnicodeToUTF8(w_path.c_str());
#endif
  return remove(path.c_str()) == 0;
#else
  return remove(file_path.c_str()) == 0;
#endif
}

bool GetFilesInFolder(const std::string& dir_path,
                      std::vector<std::string>* files) {
  if (files == nullptr) {
    return false;
  }
  files->clear();
#if defined(_WIN32) || defined(_WIN64)
  intptr_t handle;
  struct _finddata_t file_info;
  handle = _findfirst((dir_path + "/*").c_str(), &file_info);
  if (handle == -1) {
    return false;
  }
  do {
    if (file_info.attrib & _A_SUBDIR) {
      continue;
    }
    files->push_back(file_info.name);
  } while (_findnext(handle, &file_info) == 0);
  _findclose(handle);
#else
  DIR* dir = opendir(dir_path.c_str());
  if (dir == nullptr) {
    return false;
  }
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    if (entry->d_type == DT_REG) {
      files->push_back(entry->d_name);
    }
  }
  closedir(dir);
#endif
  return true;
}

bool GetFilesInFolder(const std::string& dir_path,
                      const std::string& ext,
                      std::vector<std::string>* files) {
  if (files == nullptr) {
    return false;
  }
  files->clear();
#if defined(_WIN32) || defined(_WIN64)
  intptr_t handle;
  struct _finddata_t file_info;
  handle = _findfirst((dir_path + "/*" + ext).c_str(), &file_info);
  if (handle == -1) {
    return false;
  }
  do {
    if (file_info.attrib & _A_SUBDIR) {
      continue;
    }
    files->push_back(file_info.name);
  } while (_findnext(handle, &file_info) == 0);
  _findclose(handle);
#else
  DIR* dir = opendir(dir_path.c_str());
  if (dir == nullptr) {
    return false;
  }
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    if (entry->d_type == DT_REG) {
      std::string file_name(entry->d_name);
      if (file_name.find(ext) != std::string::npos) {
        files->push_back(file_name);
      }
    }
  }
  closedir(dir);
#endif
  return true;
}

bool OpenFolder(const std::string& folder_file_path) {
#if defined(_WIN32) || defined(_WIN64)
  /// check if the folder_file_path is a folder path or a file path
  bool is_folder = false;
  std::string folder_path = folder_file_path;
  if (FileExists(folder_file_path)) {
    size_t pos = folder_file_path.find_last_of("\\");
    if (pos == std::string::npos) {
      return false;
    }
    /// get the filename from the folder_file_path
    std::string file_name = folder_file_path.substr(pos + 1);
    /// if the file_name is empty, mark as a folder path
    if (file_name.empty()) {
      is_folder = true;
    } else {
      is_folder = false;
    }
  }
  /// browse the folder in the explorer window
  /// open the folder in the explorer window
  /// if the folder_file_path is a folder path, open the folder and select none
  /// file, otherwise open the folder and select and highlight the file in the
  /// explorer window
  if (is_folder) {
    ShellExecuteA(nullptr, "open", folder_path.c_str(), nullptr, nullptr,
                  SW_SHOWMAXIMIZED);
  } else {
    ShellExecuteA(nullptr, nullptr, "explorer.exe",
                  ("/select," + folder_path).c_str(), nullptr,
                  SW_SHOWMAXIMIZED);
  }
#else
  /// TODO(hhool): open the folder in the linux system
  std::string cmd = "xdg-open ";
  cmd += folder_file_path;
  system(cmd.c_str());
#endif
  return true;
}

}  // namespace common
}  // namespace anx
