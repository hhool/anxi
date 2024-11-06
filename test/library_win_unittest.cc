/**
 * @file library_win_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief  test the library_win_unittest.cc
 * @version 0.1
 * @date 2024-09-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <gtest/gtest.h>

#include <windows.h>
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

TEST(Library_win, test_LoadLibrary) {
  std::string module_dir = anx::common::GetModuleDir();
#if defined(_WIN32) || defined(_WIN64)
  std::string module_path = module_dir + "\\pilot\\CTRL.dll";
#else
  std::string module_path = module_dir + "\\pilot\\CTRL.dll";
#endif
#if defined(UNICODE) || defined(_UNICODE)
  HMODULE hModule =
      LoadLibrary(anx::common::String2WString(module_path.c_str()).c_str());
#else
#endif
  EXPECT_NE(hModule, nullptr);
  if (hModule != nullptr) {
    FreeLibrary(hModule);
  }
  FreeLibrary(hModule);
}

TEST(Library_win, test_LoadLibrary_find_api) {
  std::string module_dir = anx::common::GetModuleDir();
#if defined(_WIN32) || defined(_WIN64)
  std::string module_path = module_dir + "\\pilot\\CTRL.dll";
#else
  std::string module_path = module_dir + "\\pilot\\CTRL.dll";
#endif
#if defined(UNICODE) || defined(_UNICODE)
  HMODULE hModule =
      LoadLibrary(anx::common::String2WString(module_path.c_str()).c_str());
#else
#endif
  EXPECT_NE(hModule, nullptr);
  typedef BOOL(__stdcall * OpenDevice)(long uUnit);
  typedef BOOL(__stdcall * LoadHareWareParameters)(int nMachineType);
  /*
  BOOL __stdcall OnLine(
      long channelNo = 1, int nLoadIndex = 0, int nExtensionIndex = 0,
      int nExtendIndex = 0, int rate = 30, int MachineType = MACHINE_HUT_SP,
      int DTCType = DTC_HUT, BYTE sensorPosition = SENSOR_MIDBEAM_UP,
      BYTE TestSpace = SPACE_DOWN, int dataBlockSize = 2, bool isAE = true);*/
  typedef BOOL(__stdcall * OnLine)(
      long channelNo, int nLoadIndex, int nExtensionIndex, int nExtendIndex,
      int rate, int MachineType, int DTCType, BYTE sensorPosition,
      BYTE TestSpace, int dataBlockSize, bool isAE);
  OpenDevice open_device = nullptr;
  LoadHareWareParameters load_hardware_parameters = nullptr;
  OnLine online = nullptr;
  if (hModule != nullptr) {
    open_device = (OpenDevice)GetProcAddress(hModule, "OpenDevice");
    load_hardware_parameters = (LoadHareWareParameters)GetProcAddress(
        hModule, "LoadHareWareParameters");
    online = (OnLine)GetProcAddress(hModule, "OnLine");
  }
  if (hModule != nullptr) {
    FreeLibrary(hModule);
  }
  FreeLibrary(hModule);
}