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


namespace anx {
namespace app {

static void* gs_app_ = nullptr;

void* CreateApp() {
  if (gs_app_ != nullptr) {
    return gs_app_;
  }
  gs_app_ = new Application();
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
Application::Application() : main_window_(nullptr) {
  // Initialize COM
  CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  HINSTANCE hInstance = GetModuleHandle(nullptr);
  DuiLib::CPaintManagerUI::SetInstance(hInstance);

  HRESULT Hr = ::CoInitialize(NULL);
  if (FAILED(Hr)) {
  }
}

Application::~Application() {
  ::CoUninitialize();
}

int32_t Application::Run() {
  main_window_ = std::make_unique<ui::MainWindow>();
  main_window_->Create(nullptr, _T("main_window"), WS_OVERLAPPEDWINDOW, 0);
  main_window_->CenterWindow();
  main_window_->ShowModal();
  DuiLib::CPaintManagerUI::MessageLoop();
  return 0;
}

void Application::Exit() {
  ::PostQuitMessage(0);
  CoUninitialize();
}

}  // namespace app
}  // namespace anx
