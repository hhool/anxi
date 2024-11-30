/**
 * @file module_utils_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief module utils unit test
 * @version 0.1
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/module_utils.h"

#include <gtest/gtest.h>

namespace anx {
namespace common {
namespace {

TEST(ModuleUtilsTest, GetAppPath) {
  std::string app_path = GetAppPath();
  EXPECT_FALSE(app_path.empty());
}

TEST(ModuleUtilsTest, GetModuleDir) {
  std::string module_dir = GetModuleDir();
  EXPECT_FALSE(module_dir.empty());
}

TEST(ModuleUtilsTest, GetModuleName) {
  std::string module_name = GetModuleName();
  EXPECT_FALSE(module_name.empty());
}

TEST(ModuleUtilsTest, GetApplicationDataPath) {
  std::string app_data_path = GetApplicationDataPath();
  EXPECT_FALSE(app_data_path.empty());
}

}  // namespace
}  // namespace common
}  // namespace anx
