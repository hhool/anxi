/**
 * @file main.cc
 * @author hhool (hhool@outlook.com)
 * @brief main entry of the application
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/appinterface.h"

#if defined(WIN32)
#include <Windows.h>
#endif
#include <tchar.h>

#if defined(_DEBUG) && defined(_MSC_VER) && defined(WIN32)
#include <crtdbg.h>
#endif

#include "app/common/cmd_parser.h"
#include "app/common/logger.h"

static std::shared_ptr<anx::common::FileLoggerSink> g_sink(
    new anx::common::FileLoggerSink("anxi.log"));

#if defined(WIN32)
#if !defined(UNDER_CE)
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
  HANDLE hMutex = NULL;
#else
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPTSTR lpCmdLine,
                     int nCmdShow) {
  HANDLE hMutex = NULL;
#endif
#else
int main() {
  HINSTANCE hInstance = nullptr;
#endif
#if defined(WIN32)
  hMutex = CreateMutex(NULL, FALSE,
                       _T("Global\\73E21C80-1960-472F-BF0B-3EE7CC7AF17E"));
  DWORD dwError = GetLastError();
  if (ERROR_ALREADY_EXISTS == dwError || ERROR_ACCESS_DENIED == dwError) {
    HWND hWnd = FindWindow(_T("work_window"), nullptr);
    if (hWnd == nullptr) {
      hWnd = FindWindow(_T("main_window"), nullptr);
    }
    if (hWnd) {
      SetForegroundWindow(hWnd);
    }
    if (hMutex) {
      CloseHandle(hMutex);
    }
    return -1;
  }
#endif
#if defined(_DEBUG) && defined(_MSC_VER) && defined(WIN32)
  int Flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  Flag |= _CRTDBG_LEAK_CHECK_DF;
  _CrtSetDbgFlag(Flag);
  // _CrtSetBreakAlloc(1343);
#endif
  /// parse the command line
  /// -le  mean log level, value is 0: sensitive, 1: info, 2: warn, 3: error, 4:
  /// fatal
  /// anxi.exe -le 0

  std::string cmd_line = lpCmdLine;
  anx::common::CmdParser cmd_parser(cmd_line);
#if defined(_DEBUG)
  int32_t log_level = cmd_parser.GetKeyValue("-le", anx::common::LS_INFO);
#else
  int32_t log_level = cmd_parser.GetKeyValue("-le", anx::common::LS_ERROR);
#endif
  anx::common::Logger::add_sink(g_sink);
  anx::common::Logger::set_log_level(log_level);
  void* handle_app = anx::app::CreateApp(hInstance);
  if (handle_app == nullptr) {
#if defined(WIN32)
    if (hMutex) {
      CloseHandle(hMutex);
      return -1;
    }
#endif
    return -1;
  }
  anx::app::Run(handle_app);
  anx::app::DestroyApp(handle_app);
#if defined(WIN32)
  if (hMutex) {
    CloseHandle(hMutex);
  }
#endif
  return 0;
}
