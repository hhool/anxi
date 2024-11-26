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

TEST(StringUtilsTest, String2WString2String) {
  std::string str = "hello world";
  std::wstring wstr = String2WString(str.c_str());
  std::string str2 = WString2String(wstr.c_str());
  EXPECT_EQ(str, str2);
}

TEST(StringUtilsTest, WString2String2WString) {
  std::wstring wstr = L"hello world";
  std::string str = WString2String(wstr.c_str());
  std::wstring wstr2 = String2WString(str.c_str());
  EXPECT_EQ(wstr, wstr2);
}

TEST(StringUtilsTest, Trim) {
  std::string str = " hello world ";
  std::string trim_str = Trim(str);
  EXPECT_EQ(trim_str, "hello world");
}

TEST(StringUtilsTest, ToLower) {
  std::string str = "Hello World";
  std::string lower_str = ToLower(str);
  EXPECT_EQ(lower_str, "hello world");
}

TEST(StringUtilsTest, ToUpper) {
  std::string str = "Hello World";
  std::string upper_str = ToUpper(str);
  EXPECT_EQ(upper_str, "HELLO WORLD");
}

TEST(StringUtilsTest, Replace) {
  std::string str = "hello world";
  std::string from = "world";
  std::string to = "china";
  std::string replace_str = Replace(str, from, to);
  EXPECT_EQ(replace_str, "hello china");
}

TEST(StringUtilsTest, ReplaceAll) {
  std::string str = "hello world world";
  std::string from = "world";
  std::string to = "china";
  std::string replace_str = ReplaceAll(str, from, to);
  EXPECT_EQ(replace_str, "hello china china");
}

TEST(StringUtilsTest, Split) {
  std::string str = "hello,world";
  std::string delimiter = ",";
  std::vector<std::string> split_str = Split(str, delimiter);
  EXPECT_EQ(split_str.size(), 2);
  EXPECT_EQ(split_str[0], "hello");
  EXPECT_EQ(split_str[1], "world");
}

TEST(StringUtilsTest, Join) {
  std::vector<std::string> str = {"hello", "world"};
  std::string delimiter = ",";
  std::string join_str = Join(str, delimiter);
  EXPECT_EQ(join_str, "hello,world");
}

}  // namespace
}  // namespace common
}  // namespace anx

