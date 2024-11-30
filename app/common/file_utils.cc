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

#include <cassert>
#include <memory>
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

#if defined(_WIN32) || defined(_WIN64)
const char kPathSeparator[] = {"\\"};
#else
const char kPathSeparator[] = {"/"};
#endif
bool FileExists(const std::string& file_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string utf8_string = anx::common::ToUTF8(file_path);
  std::wstring w_path = anx::common::UTF8ToUnicode(utf8_string.c_str());
  return _waccess(w_path.c_str(), 0) == 0;
#else
  return access(file_path.c_str(), F_OK) == 0;
#endif
}

bool DirectoryExists(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string utf8_string = anx::common::ToUTF8(dir_path);
  std::wstring w_path = anx::common::UTF8ToUnicode(utf8_string.c_str());
  return _waccess(w_path.c_str(), 0) == 0;
#else
  return access(dir_path.c_str(), F_OK) == 0;
#endif
}

bool CreateFolder(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string utf8_string = anx::common::ToUTF8(dir_path);
  std::wstring w_path = anx::common::UTF8ToUnicode(utf8_string.c_str());
  return _wmkdir(w_path.c_str()) == 0;
#else
  return mkdir(dir_path.c_str(), 0777) == 0;
#endif
}

bool RemoveFolder(const std::string& dir_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string utf8_string = anx::common::ToUTF8(dir_path);
  std::wstring w_path = anx::common::UTF8ToUnicode(utf8_string.c_str());
  return _wrmdir(w_path.c_str()) == 0;
#else
  return rmdir(dir_path.c_str()) == 0;
#endif
}

bool RemoveFile(const std::string& file_path) {
#if defined(_WIN32) || defined(_WIN64)
  std::string utf8_string = anx::common::ToUTF8(file_path);
  std::wstring w_path = anx::common::UTF8ToUnicode(utf8_string.c_str());
  return _wremove(w_path.c_str()) == 0;
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
    size_t pos = folder_file_path.find_last_of(anx::common::kPathSeparator);
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

bool ReadFile(const std::string& file_path, std::string* content, bool binary) {
  assert(content != nullptr);
  if (content == nullptr) {
    LOG_F(LG_ERROR) << "content is nullptr";
    return false;
  }
  content->clear();
  FILE* file = fopen(file_path.c_str(), binary ? "rb" : "r");
  if (file == nullptr) {
    LOG_F(LG_ERROR) << "open file failed:" << file_path;
    return false;
  }
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  std::unique_ptr<char[]> file_content(new char[file_size]);
  size_t size = fread(file_content.get(), 1, file_size, file);
  if (size != file_size) {
    LOG_F(LG_ERROR) << "read file failed:" << file_path;
    fclose(file);
    return false;
  }
  fclose(file);
  content->assign(file_content.get(), file_size);
  return true;
}

bool WriteFile(const std::string& file_path,
               const std::string& content,
               bool binary) {
  FILE* file = fopen(file_path.c_str(), binary ? "wb" : "w");
  if (file == nullptr) {
    return false;
  }
  size_t size = content.size();
  size_t written = fwrite(content.c_str(), 1, size, file);
  fclose(file);
  return written == size;
}

bool CCopyFile(const std::string& src, const std::string& dst) {
#if defined(_WIN32) || defined(_WIN64)
  return CopyFileA(src.c_str(), dst.c_str(), FALSE) == TRUE;
#else
  return false;
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
    std::string src_file =
        src + anx::common::kPathSeparator + find_data.cFileName;
    std::string dest_file =
        dest + anx::common::kPathSeparator + find_data.cFileName;
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
