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

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace anx {
namespace common {

bool FileExists(const std::string& file_path) {
#if defined(_WIN32) || defined(_WIN64)
  return _access(file_path.c_str(), 0) == 0;
#else
  return access(file_path.c_str(), F_OK) == 0;
#endif
}

bool DirectoryExists(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  return _access(dir_path.c_str(), 0) == 0;
#else
  return access(dir_path.c_str(), F_OK) == 0;
#endif
}

bool CreateFolder(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  return _mkdir(dir_path.c_str()) == 0;
#else
  return mkdir(dir_path.c_str(), 0777) == 0;
#endif
}

bool RemoveFolder(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  return _rmdir(dir_path.c_str()) == 0;
#else
  return rmdir(dir_path.c_str()) == 0;
#endif
}

bool RemoveFile(const std::string& file_path) {
  return remove(file_path.c_str()) == 0;
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
}  // namespace common
}  // namespace anx
