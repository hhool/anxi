/**
 * @file file_utils.h
 * @author hhool (hhool@outlook.com)
 * @brief file utils for file operation in the system
 * @version 0.1
 * @date 2024-08-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_FILE_UTILS_H_
#define APP_COMMON_FILE_UTILS_H_

#include <string>
#include <vector>

namespace anx {
namespace common {

extern const char kPathSeparator[];
/// @brief  check the file exists or not in the system
/// @param file_path
/// @return  true if the file exists
bool FileExists(const std::string& file_path);

/// @brief  check the directory exists or not in the system
/// @param dir_path
/// @return  true if the directory exists
bool DirectoryExists(const std::string& dir_path);

/// @brief  create the folder in the system
/// @param dir_path
/// @return  true if the folder created
bool CreateFolder(const std::string& dir_path);

/// @brief  remove the folder in the system
/// @param dir_path
/// @return  true if the folder removed
bool RemoveFolder(const std::string& dir_path);

/// @brief  remove the file in the system
/// @param file_path  the file path
/// @return  true if the file removed
bool RemoveFile(const std::string& file_path);

/// @brief  get the files in the folder
/// @param dir_path  the folder path
/// @param files  the files in the folder
/// @return  true if the files got
bool GetFilesInFolder(const std::string& dir_path,
                      std::vector<std::string>* files);

/// @brief  get the files in the folder with the extension
/// @param dir_path  the folder path
/// @param ext  the extension
/// @param files  the files in the folder
bool GetFilesInFolder(const std::string& dir_path,
                      const std::string& ext,
                      std::vector<std::string>* files);
/// @brief  open the folder in the system, if folder_file_path is a file path,
/// open the folder which contains the file and select the file in the folder
/// otherwise open the folder
/// @param folder_file_path  the folder path or the file path
bool OpenFolder(const std::string& folder_file_path);

/// @brief Read the file content
/// @param file_path  the file path
/// @param content  the file content
/// @param binary  true if read the file in binary mode
/// @return  true if read the file success
bool ReadFile(const std::string& file_path,
              std::string* content,
              bool binary = false);

/// @brief  write the content to the file
/// @param file_path  the file path
/// @param content  the content to write
/// @param binary  true if write the file in binary mode
/// @return  true if write the file success
bool WriteFile(const std::string& file_path,
               const std::string& content,
               bool binary = false);

/// @brief  copy the file from src to dst path
/// @param src  the source file path
/// @param dst  the destination file path
/// @return  true if copy the file success
bool CCopyFile(const std::string& src, const std::string& dst);


/// @brief Copy the folder
/// @param src the source folder
/// @param dst the destination folder
/// @return 0 if success, <0 if failed
int32_t CopyFolder(const std::string& src, const std::string& dest);

/// @brief Make sure the directory exists
/// @param dir the directory
/// @return true if success, otherwise false
/// @note if the directory does not exist, it will create the directory
/// @note if the directory exists, it will return true
/// @note if the directory exists but not a directory, it will return false
/// @note if the directory exists but not writable, it will return false
/// @note if the directory exists but not readable, it will return false
/// @note e.g. MakeSureFolderPathExist("C:/test") will return true if the
/// directory exists, otherwise it will create the directory and return true
bool MakeSureFolderPathExist(const std::string& dir);

}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_FILE_UTILS_H_
