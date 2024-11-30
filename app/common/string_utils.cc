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

namespace {
bool isUTF8(const std::string& str) {
  size_t i = 0;
  int nBytes = 0;
  unsigned char chr;
  bool bAllAscii = true;
  for (i = 0; i < str.length(); i++) {
    chr = str[i];
    if ((chr & 0x80) != 0) {
      bAllAscii = false;
    }
    if (nBytes == 0) {
      if (chr >= 0x80) {
        if (chr >= 0xFC && chr <= 0xFD) {
          nBytes = 6;
        } else if (chr >= 0xF8) {
          nBytes = 5;
        } else if (chr >= 0xF0) {
          nBytes = 4;
        } else if (chr >= 0xE0) {
          nBytes = 3;
        } else if (chr >= 0xC0) {
          nBytes = 2;
        } else {
          return false;
        }
        nBytes--;
      }
    } else {
      if ((chr & 0xC0) != 0x80) {
        return false;
      }
      nBytes--;
    }
  }
  if (nBytes > 0) {
    return false;
  }
  if (bAllAscii) {
    return false;
  }
  return true;
}

bool isUnicode(const std::string& str) {
  int nBytes = 0;
  for (size_t i = 0; i < str.length(); i++) {
    if (nBytes == 0) {
      if ((str[i] & 0x80) != 0) {
        if ((str[i] & 0x80) == 0xC0 || (str[i] & 0x80) == 0xC1) {
          return false;
        }
        if ((str[i] & 0xF0) == 0xF0) {
          nBytes = 4;
        } else if ((str[i] & 0xE0) == 0xE0) {
          nBytes = 3;
        } else if ((str[i] & 0xC0) == 0xC0) {
          nBytes = 2;
        } else {
          return false;
        }
        nBytes--;
      }
    } else {
      if ((str[i] & 0xC0) != 0x80) {
        return false;
      }
      nBytes--;
    }
  }
  if (nBytes > 0) {
    return false;
  }
  return true;
}

bool isAnsi(const std::string& str) {
  return isUTF8(str) || isUnicode(str);
}
}  // namespace

#if defined(_WIN32) || defined(_WIN64)
std::wstring String2WString(const std::string& str) {
  std::wstring result;
  int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
  TCHAR* buffer = new TCHAR[len + 1];
  ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
  buffer[len] = '\0';
  result.append(buffer);
  delete[] buffer;
  return result;
}

std::string WString2String(const std::wstring& wstr) {
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

std::wstring UTF8ToUnicode(const std::string& str) {
  std::wstring result;
  int len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
  TCHAR* buffer = new TCHAR[len + 1];
  ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer, len);
  buffer[len] = '\0';
  result.append(buffer);
  delete[] buffer;
  return result;
}

std::string UnicodeToUTF8(const std::wstring& str) {
  std::string result;
  int len = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0,
                                  NULL, NULL);
  char* buffer = new char[len + 1];
  ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), buffer, len, NULL,
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

std::string UTF8ToUnicode(const std::string& str) {
  return WString2String(String2WString(str));
}

std::string UnicodeToUTF8(const std::string& str) {
  return WString2String(String2WString(str));
}
#endif

std::string AnsiToUTF8(const std::string& str) {
  std::string result;
  std::wstring wstr = String2WString(str);
  result = UnicodeToUTF8(wstr);
  return result;
}

std::string UTF8ToAnsi(const std::string& str) {
  std::string result;
  std::wstring wstr = UTF8ToUnicode(str);
  result = WString2String(wstr);
  return result;
}

std::string ToUTF8(const std::string& str) {
  /// check if the string is utf8
  if (isUTF8(str)) {
    return str;
  }
  std::string result;
  std::wstring wstr = String2WString(str);
  result = UnicodeToUTF8(wstr);
  return result;
}

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
