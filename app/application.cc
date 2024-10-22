/**
 * @file application.cc
 * @author hhool (hhool@outlook.com)
 * @brief application class implementation
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/application.h"

#include "app/ui/main_window.h"

#include "third_party/duilib/source/DuiLib/UIlib.h"

#include "app/device/device_com_factory.h"

#include "app/db/database_factory.h"

#include "resource.h"

namespace anx {
namespace app {

static void* gs_app_ = nullptr;

void* CreateApp(void* hinst) {
  if (gs_app_ != nullptr) {
    return gs_app_;
  }
  gs_app_ = new Application(hinst);
  return gs_app_;
}

int32_t Run(void* handle_app) {
  Application* app = reinterpret_cast<Application*>(handle_app);
  if (app == nullptr) {
    return -1;
  }
  return app->Run();
}

void Exit(void* handle_app) {
  Application* app = reinterpret_cast<Application*>(handle_app);
  app->Exit();
}

void DestroyApp(void* handle_app) {
  Application* app = reinterpret_cast<Application*>(handle_app);
  if (app == nullptr) {
    return;
  }
  assert(app == gs_app_);
  delete app;
  gs_app_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
HINSTANCE g_hInstance_;

Application::Application(HANDLE hinst) : main_window_(nullptr) {
  // Initialize COM
  HINSTANCE hInstance = static_cast<HINSTANCE>(hinst);
  if (hInstance == NULL) {
    hInstance = GetModuleHandle(nullptr);
  }
  g_hInstance_ = hInstance;
  DuiLib::CPaintManagerUI::SetInstance(hInstance);

#if defined(WIN32) && !defined(UNDER_CE)
  HRESULT Hr = ::CoInitialize(NULL);
#else
  HRESULT Hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
  if (FAILED(Hr)) {
    // TODO(hhool): process error
    return;
  }
  anx::db::DatabaseFactory::Instance();
  anx::device::DeviceComFactory::Instance();
}

Application::~Application() {
  anx::device::DeviceComFactory::ReleaseInstance();
  anx::db::DatabaseFactory::ReleaseInstance();
  ::CoUninitialize();
}

int32_t Application::Run() {
  main_window_ = std::make_unique<ui::MainWindow>();
  main_window_->Create(nullptr, _T("main_window"), UI_WNDSTYLE_FRAME,
                       WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  ::SetWindowLong(main_window_->GetHWND(), GWL_STYLE,
                  ::GetWindowLong(main_window_->GetHWND(), GWL_STYLE) &
                      ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
  HICON hIcon = ::LoadIcon(g_hInstance_, MAKEINTRESOURCE(IDI_ICON_APP));
  ::SendMessage(main_window_->GetHWND(), STM_SETICON, IMAGE_ICON,
                (LPARAM)(UINT)hIcon);

  main_window_->CenterWindow();
  main_window_->ShowWindow(true, true);
  DuiLib::CPaintManagerUI::MessageLoop();
  DuiLib::CPaintManagerUI::Term();
  return 0;
}

void Application::Exit() {
  ::PostQuitMessage(0);
  CoUninitialize();
}

}  // namespace app
}  // namespace anx
