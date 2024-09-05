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

#include "app/common/logger.h"

static std::shared_ptr<anx::common::FileLoggerSink> g_sink(
    new anx::common::FileLoggerSink("anxi.log"));

#if defined(WIN32)
#if !defined(UNDER_CE)
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR /*lpCmdLine*/,
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
    if (hMutex) {
      CloseHandle(hMutex);
      return -1;
    }
  }
#endif
#if defined(_DEBUG) && defined(_MSC_VER) && defined(WIN32)
  int Flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  Flag |= _CRTDBG_LEAK_CHECK_DF;
  _CrtSetDbgFlag(Flag);
  //  _CrtSetBreakAlloc(269);
#endif
  anx::common::Logger::add_sink(g_sink);
  anx::common::Logger::set_log_level(anx::common::LS_INFO);
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
