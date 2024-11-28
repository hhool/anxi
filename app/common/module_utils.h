/**
 * @file module_utils.h
 * @author hhool (hhool@outlook.com)
 * @brief module utils define file for the sysutils
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_MODULE_UTILS_H_
#define APP_COMMON_MODULE_UTILS_H_

#include <string>

namespace anx {
namespace common {
/// @brief Get the application path
/// @return the application path
std::string GetAppPath();

/// @brief Get the module directory
/// @return the module directory
std::string GetModuleDir();

/// @brief Get the module name
/// @return the module name
std::string GetModuleName();

/// @brief Get the AppData path
/// @return the AppData path
std::string GetApplicationDataPath();

/// @brief Get the AppData path
/// @param app_name the application name
/// @return the AppData path
std::string GetApplicationDataPath(const std::string& app_name);

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

#endif  // APP_COMMON_MODULE_UTILS_H_
