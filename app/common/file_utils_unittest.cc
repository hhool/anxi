/**
 * @file file_utils_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief file utils unit test
 * @version 0.1
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/file_utils.h"

#include <gtest/gtest.h>

namespace anx {
namespace common {
namespace {
TEST(FileUtilsTest, FileExists) {
  std::string file_path = "file_utils_unittest.cc";
  EXPECT_TRUE(FileExists(file_path));
}

}  // namespace
}  // namespace common
}  // namespace anx