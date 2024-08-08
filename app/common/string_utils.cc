/**
 * @file stringutils.cc
 * @author hhool (hhool@outlook.com)
 * @brief string utils for the common string utils
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/string_utils.h"

#include <algorithm>
#include <cctype>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <iconv.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

namespace anx {
namespace common {

std::string Trim(const std::string& str) {
  std::string result = str;
  result.erase(result.find_last_not_of(" \n\r\t") + 1);
  result.erase(0, result.find_first_not_of(" \n\r\t"));
  return result;
}

std::string ToLower(const std::string& str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

std::string ToUpper(const std::string& str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), ::toupper);
  return result;
}

std::string Replace(const std::string& str,
                    const std::string& from,
                    const std::string& to) {
  std::string result = str;
  size_t start_pos = 0;
  while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
    result.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return result;
}

std::string ReplaceAll(const std::string& str,
                       const std::string& from,
                       const std::string& to) {
  std::string result = str;
  size_t start_pos = 0;
  while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
    result.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return result;
}

std::vector<std::string> Split(const std::string& str,
                               const std::string& delimiter) {
  std::vector<std::string> result;
  size_t start = 0;
  size_t end = 0;
  while ((end = str.find(delimiter, start)) != std::string::npos) {
    result.push_back(str.substr(start, end - start));
    start = end + delimiter.length();
  }
  result.push_back(str.substr(start));
  return result;
}

std::string Join(const std::vector<std::string>& vec,
                 const std::string& delimiter) {
  std::string result;
  for (size_t i = 0; i < vec.size(); ++i) {
    result += vec[i];
    if (i != vec.size() - 1) {
      result += delimiter;
    }
  }
  return result;
}

std::wstring ToWstring(const std::string& str) {
  std::wstring result;
  result.assign(str.begin(), str.end());
  return result;
}

std::string ToString(const std::wstring& wstr) {
  std::string result;
  result.assign(wstr.begin(), wstr.end());
  return result;
}
#if defined(_WIN32) || defined(_WIN64)
std::wstring string2wstring(std::string str) {
  std::wstring result;
  int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
  TCHAR* buffer = new TCHAR[len + 1];
  ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
  buffer[len] = '\0';
  result.append(buffer);
  delete[] buffer;
  return result;
}

std::string wstring2string(std::wstring wstr) {
  std::string result;
  int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0,
                                  NULL, NULL);
  char* buffer = new char[len + 1];
  ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL,
                        NULL);
  buffer[len] = '\0';
  result.append(buffer);
  delete[] buffer;
  return result;
}
#elif defined(__linux__)
std::string WString2String(const std::wstring& wstr) {
  setlocale(LC_ALL, "en_US.UTF-8");
  const wchar_t* wchSrc = wstr.c_str();
  size_t nWStr = wcstombs(NULL, wchSrc, 0) + 1;
  char* chDest = new char[nWStr];
  memset(chDest, 0, nWStr);
  wcstombs(chDest, wchSrc, nWStr);
  std::string strRes = chDest;
  delete[] chDest;

  return strRes;
}

std::wstring String2WString(const std::string& str) {
  std::string strUtf8 = str;
  setlocale(LC_ALL, "en_US.UTF-8");
  const char* chSrc = strUtf8.c_str();
  size_t nStr = mbstowcs(NULL, chSrc, 0) + 1;
  wchar_t* wchDest = new wchar_t[nStr];
  memset(wchDest, 0, nStr);
  mbstowcs(wchDest, chSrc, nStr);
  std::wstring wStrRes = wchDest;
  delete[] wchDest;
  return wStrRes;
}
#endif

std::string ByteArrayToHexString(const uint8_t* byte, int32_t size) {
  std::string result;
  for (int32_t i = 0; i < size; ++i) {
    char buffer[3];
    snprintf(buffer, sizeof(buffer), "%02X", byte[i]);
    result += buffer;
  }
  return result;
}
}  // namespace common
}  // namespace anx
