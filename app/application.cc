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
Application::Application(HANDLE hinst) : main_window_(nullptr) {
  // Initialize COM
  HINSTANCE hInstance = static_cast<HINSTANCE>(hinst);
  if (hInstance == NULL) {
    hInstance = GetModuleHandle(nullptr);
  }
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
  anx::device::DeviceComFactory::Instance();
}

Application::~Application() {
  anx::device::DeviceComFactory::ReleaseInstance();
  ::CoUninitialize();
}

int32_t Application::Run() {
  main_window_ = std::make_unique<ui::MainWindow>();
  main_window_->Create(nullptr, _T("main_window"), UI_WNDSTYLE_FRAME,
                       WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  main_window_->CenterWindow();
  main_window_->ShowWindow(true, true);
  return DuiLib::CPaintManagerUI::MessageLoop();
}

void Application::Exit() {
  ::PostQuitMessage(0);
  CoUninitialize();
}

}  // namespace app
}  // namespace anx
