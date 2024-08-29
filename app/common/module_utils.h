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

/// @brief Make sure the directory exists
/// @param dir the directory
/// @return true if success, otherwise false
bool MakeSureFolderPathExist(const std::string& dir);
}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_MODULE_UTILS_H_
