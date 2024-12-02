/**
 * @file stringutils.h
 * @author hhool (hhool@outlook.com)
 * @brief string utils for the common string utils
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_STRING_UTILS_H_
#define APP_COMMON_STRING_UTILS_H_

#include <string>
#include <vector>
#include <iomanip>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace anx {
namespace common {
    /// @brief Trim the string
/// @param str the string to be trimmed
/// @return the trimmed string
std::string Trim(const std::string& str);

/// @brief Convert the string to lower case
/// @param str the string to be converted
std::string ToLower(const std::string& str);

/// @brief Convert the string to upper case
/// @param str the string to be converted
std::string ToUpper(const std::string& str);

/// @brief Replace the string from with to
/// @param str the string to be replaced
/// @param from the string to be replaced
/// @param to the string to be replaced with
std::string Replace(const std::string& str,
                    const std::string& from,
                    const std::string& to);

/// @brief Replace all the string from with to
/// @param str the string to be replaced
/// @param from the string to be replaced
/// @param to the string to be replaced with
std::string ReplaceAll(const std::string& str,
                       const std::string& from,
                       const std::string& to);

/// @brief Split the string with the delimiter
/// @param str the string to be split
/// @param delimiter the delimiter to split the string
/// @return the splited string
std::vector<std::string> Split(const std::string& str,
                               const std::string& delimiter);

/// @brief Join the string with the delimiter
/// @param str the string to be joined
/// @param delimiter the delimiter to join the string
/// @return the joined string
std::string Join(const std::vector<std::string>& str,
                 const std::string& delimiter);

//// @brief Convert from string to wstring
/// @param str the string to be converted
std::wstring String2WString(const std::string& str);

/// @brief Convert from wstring to string
/// @param str the string to be converted
std::string WString2String(const std::wstring& wstr);

/// @brief  Convert from utf8 to unicode
/// @param str  the string to be converted
/// @return  the converted string
std::wstring UTF8ToUnicode(const std::string& str);

/// @brief  Convert from unicode to utf8
/// @param str  the string to be converted
/// @return  the converted string
std::string UnicodeToUTF8(const std::wstring& str);

/// @brief  Convert from ansii to utf8
/// @param str  the string to be converted
/// @return  the converted string
std::string AnsiToUTF8(const std::string& str);

/// @brief  Convert from utf8 to ansii
/// @param str  the string to be converted
/// @return  the converted string
std::string UTF8ToAnsi(const std::string& str);

/// @brief  Convert any string to utf8
/// @param str  the string to be converted
/// @return  the converted string
std::string ToUTF8(const std::string& str);

/// @brief byte array to hex string
/// @param byte the bytes to be converted
std::string ByteArrayToHexString(const uint8_t* byte, int32_t size);

}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_STRING_UTILS_H_
