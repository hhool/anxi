/**
 * @file usb_auth.cc
 * @author hhool (hhool@oulook.com)
 * @brief usb auth
 * @version 0.1
 * @date 2025-01-06
 */

#include "app/common/usb_auth.h"
#include "app/common/logger.h"

#if defined(_WIN32)
// clang-format off
#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <Usbiodef.h>
// clang-format on
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "setupapi.lib")

#else
#error "Not support platform"
#endif

namespace anx {
namespace common {
namespace {
#if defined(_WIN32)
// @brief find removable drives
std::vector<std::wstring> GetUSBDriveLetters() {
  std::vector<std::wstring> usbLetters;
  DWORD dwSize = GetLogicalDriveStrings(0, nullptr);
  std::wstring szLogicalDrives(dwSize, 0);
  GetLogicalDriveStrings(dwSize, &szLogicalDrives[0]);
  for (size_t i = 0; i < dwSize; i += 4) {
    if (GetDriveType(&szLogicalDrives[i]) == DRIVE_REMOVABLE) {
      usbLetters.push_back(szLogicalDrives.substr(i, 2));
    }
  }
  return usbLetters;
}

// @brief read file from usb
bool ReadFileFromUSB(const std::wstring& driveLetter,
                     const std::wstring& filePath) {
  std::wstring fullPath = driveLetter + filePath;
  std::ifstream file(fullPath);
  if (file.is_open()) {
    return true;
  }
  return false;
}
#else
#error "Not support platform"
#endif
}  // namespace

/**
 * @brief check usb auth
 * @return 0 if success, -1 if failed
 */

int32_t CheckUsbAuth() {
  bool pass = false;
  std::vector<std::wstring> usbLetters = GetUSBDriveLetters();
  if (usbLetters.empty()) {
    LOG_F(LG_ERROR) << "No USB storage devices found.";
    return pass;
  }

  for (const auto& letter : usbLetters) {
    LOG_F(LG_INFO) << "USB Device: "
                   << std::string(letter.begin(), letter.end());
    pass = ReadFileFromUSB(letter, L".anxi/auth.key");
    if (pass) {
      break;
    }
  }
  return pass;
}
}  // namespace common
}  // namespace anx
