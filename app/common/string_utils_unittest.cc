/**
 * @file string_utils_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief  string utils unit test
 * @version 0.1
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/string_utils.h"

#include <gtest/gtest.h>

namespace anx {
namespace common {
namespace {

TEST(StringUtilsTest, UTF8ToUnicode) {
  std::string utf8_str = "hello world";
  std::wstring unicode_str = UTF8ToUnicode(utf8_str);
  EXPECT_EQ(unicode_str, L"hello world");
}

TEST(StringUtilsTest, UnicodeToUTF8) {
  std::wstring unicode_str = L"hello world";
  std::string utf8_str = UnicodeToUTF8(unicode_str);
  EXPECT_EQ(utf8_str, "hello world");
}

TEST(StringUtilsTest, String2WString) {
  std::string str = "hello world";
  std::wstring wstr = String2WString(str.c_str());
  EXPECT_EQ(wstr, L"hello world");
}

TEST(StringUtilsTest, WString2String) {
  std::wstring wstr = L"hello world";
  std::string str = WString2String(wstr.c_str());
  EXPECT_EQ(str, "hello world");
}

}  // namespace
}  // namespace common
}  // namespace anx
