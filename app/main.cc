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

#if defined(_DEBUG) && defined(_MSC_VER) && defined(WIN32)
#include <crtdbg.h>
#endif

#if defined(WIN32)
#if !defined(UNDER_CE)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow) {
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int nCmdShow) {
#endif
#else
int main() {
#endif 
#if defined(_DEBUG) && defined(_MSC_VER) && defined(WIN32)
  int Flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  Flag |= _CRTDBG_LEAK_CHECK_DF;
  _CrtSetDbgFlag(Flag);
   // _CrtSetBreakAlloc(2840);
#endif
  void* handle_app = anx::app::CreateApp(hInstance);
  if (handle_app == nullptr) {
	  return -1;
  }
  anx::app::Run(handle_app);
  anx::app::DestroyApp(handle_app);
  return 0;
}
