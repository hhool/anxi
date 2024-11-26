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

TEST(FileUtilsTest, FileCreate) {
  std::string file_path = "test.cc";
  FILE* file = fopen(file_path.c_str(), "w");
  fclose(file);
}

TEST(FileUtilsTest, FileExists) {
  std::string file_path = "test.cc";
  EXPECT_TRUE(FileExists(file_path));
}

TEST(FileUtilsTest, DirectoryExists) {
  std::string dir_path = ".";
  EXPECT_TRUE(DirectoryExists(dir_path));
}

TEST(FileUtilsTest, CreateFolder) {
  std::string dir_path = "test";
  EXPECT_TRUE(CreateFolder(dir_path));
  EXPECT_TRUE(DirectoryExists(dir_path));
  EXPECT_TRUE(RemoveFolder(dir_path));
}

TEST(FileUtilsTest, RemoveFolder) {
  std::string dir_path = "test";
  EXPECT_TRUE(CreateFolder(dir_path));
  EXPECT_TRUE(DirectoryExists(dir_path));
  EXPECT_TRUE(RemoveFolder(dir_path));
  EXPECT_FALSE(DirectoryExists(dir_path));
}

TEST(FileUtilsTest, RemoveFile) {
  std::string file_path = "test.txt";
  FILE* file = fopen(file_path.c_str(), "w");
  fclose(file);
  EXPECT_TRUE(FileExists(file_path));
  EXPECT_TRUE(RemoveFile(file_path));
  EXPECT_FALSE(FileExists(file_path));
}

TEST(FileUtilsTest, GetFilesInFolder) {
  std::string dir_path = ".";
  std::vector<std::string> files;
  EXPECT_TRUE(GetFilesInFolder(dir_path, &files));
  EXPECT_FALSE(files.empty());
}

TEST(FileUtilsTest, GetFilesInFolderWithExt) {
  std::string dir_path = ".";
  std::vector<std::string> files;
  EXPECT_TRUE(GetFilesInFolder(dir_path, "cc", &files));
  EXPECT_FALSE(files.empty());
}

}  // namespace
}  // namespace common
}  // namespace anx
