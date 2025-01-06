/**
 * @file main_window.cc
 * @author hhool (hhool@outlook.com)
 * @brief main window class definition
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/main_window.h"

#include "app/common/file_utils.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"
#include "app/common/usb_auth.h"
#include "app/esolution/solution_design.h"
#include "app/ui/app_config.h"
#include "app/ui/dialog_app_settings.h"
#include "app/ui/dialog_common.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::MainWindow, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

MainWindow::MainWindow() {
  AppConfig::PrepareAppConfig();
}

MainWindow::~MainWindow() {}

void MainWindow::InitWindow() {
  __super::InitWindow();
  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_work_axially_symmetrical_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("work_axially_symmetrical")));
  btn_work_stresses_adjustable_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("work_stresses_adjustable")));
  btn_work_th3point_bending_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("work_3point_bending")));
  btn_work_vibration_bending_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("work_vibration_bending")));
  hlayout_work_pilot_e10c_ = static_cast<CHorizontalLayoutUI*>(
      m_PaintManager.FindControl(_T("hlayout_work_pilot_e10c")));
  btn_work_pilot_e10c_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_pilot_e10c")));
  std::string name = anx::settings::SettingAppThird::GetThirdAppName();
  if (!name.empty()) {
    btn_work_pilot_e10c_->SetText(anx::common::UTF8ToUnicode(name).c_str());
  } else {
    hlayout_work_pilot_e10c_->SetVisible(false);
  }
  std::map<std::string, std::string> app_config = LoadAppConfig();
  btn_app_settings_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("btn_app_settings")));
  CLabelUI* lb_code =
      static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lb_code")));
  CDuiString lb_text_code =
      anx::common::UTF8ToUnicode(app_config["code"].c_str()).c_str();
  lb_code->SetText(lb_text_code);
  CLabelUI* lb_copyright =
      static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lb_copyright")));
  CDuiString lb_text_copyright =
      anx::common::UTF8ToUnicode(app_config["copyright"].c_str()).c_str();
  lb_copyright->SetText(lb_text_copyright);
}

void MainWindow::OnFinalMessage(HWND hWnd) {
  __super::OnFinalMessage(hWnd);
  delete this;
}

LRESULT MainWindow::ResponseDefaultKeyEvent(WPARAM wParam) {
  if (wParam == VK_RETURN) {
    return FALSE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void MainWindow::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_close_) {
    PostQuitMessage(0);
  } else if (msg.pSender == btn_work_axially_symmetrical_) {
    Switch_Axially_Symmetrical();
  } else if (msg.pSender == btn_work_stresses_adjustable_) {
    Switch_Stresses_Adjustable();
  } else if (msg.pSender == btn_work_th3point_bending_) {
    Switch_Th3point_Bending();
  } else if (msg.pSender == btn_work_vibration_bending_) {
    Switch_Vibration_Bending();
  } else if (msg.pSender == btn_work_pilot_e10c_) {
    Switch_ThirdApp();
  } else if (msg.pSender == btn_app_settings_) {
    OnBtnAppSettings();
  } else {
    // TODO(hhool):
  }
}

LRESULT MainWindow::OnClose(UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam,
                            BOOL& bHandled) {
  bHandled = TRUE;
  PostQuitMessage(0);
  return 0;
}

LRESULT MainWindow::OnSetFocus(UINT /*uMsg*/,
                               WPARAM /*wParam*/,
                               LPARAM /*lParam*/,
                               BOOL& bHandled) {
  if (!anx::common::CheckUsbAuth()) {
    if (is_deal_with_quit_) {
      return 0;
    }
    is_deal_with_quit_ = true;
    anx::ui::DialogCommon::ShowDialog(
        m_hWnd, "错误", "授权信息读取失败, 自动退出系统。",
        anx::ui::DialogCommon::kDialogCommonStyleOk);
    PostQuitMessage(0);
    bHandled = TRUE;
    return 0;
  }
  bHandled = TRUE;
  /// @note redraw the window when it is shown again.
  /// @note it is necessary to redraw the window when it is shown again.
  /// @note otherwise, the window will can't be redrawed completely.
  ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
  ::InvalidateRect(m_hWnd, NULL, TRUE);
  return 0;
}

LRESULT MainWindow::OnNcHitTest(UINT uMsg,
                                WPARAM wParam,
                                LPARAM lParam,
                                BOOL& bHandled) {
  POINT pt;
  pt.x = GET_X_LPARAM(lParam);
  pt.y = GET_Y_LPARAM(lParam);
  ::ScreenToClient(*this, &pt);

  RECT rcClient;
  ::GetClientRect(*this, &rcClient);

  RECT rcCaption = m_PaintManager.GetCaptionRect();
  CControlUI* pControl =
      static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
  if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
      _tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
      _tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
    return HTCAPTION;

  return HTCLIENT;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_LBUTTONDBLCLK) {
    return 0;
  }
  return __super::HandleMessage(uMsg, wParam, lParam);
}

void MainWindow::Switch_Axially_Symmetrical() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window = new ui::WorkWindow(
      this, anx::esolution::kSolutionName_Axially_Symmetrical);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void MainWindow::Switch_Stresses_Adjustable() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window = new ui::WorkWindow(
      this, anx::esolution::kSolutionName_Stresses_Adjustable);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void MainWindow::Switch_Th3point_Bending() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Th3point_Bending);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void MainWindow::Switch_Vibration_Bending() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Vibration_Bending);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void MainWindow::Switch_ThirdApp() {
  std::string filepath = anx::settings::SettingAppThird::GetThirdAppPath();
  if (filepath.empty()) {
    return;
  }
  // check the path is valid and exists;
  if (!anx::common::FileExists(filepath)) {
    return;
  }
#if defined(WIN32)
  std::string utf8_string = anx::common::ToUTF8(filepath);
  ShellExecute(NULL, _T("open"),
               anx::common::UTF8ToUnicode(utf8_string.c_str()).c_str(), NULL,
               NULL, SW_SHOW);
#else
  /// TODO(hhool)
#endif
}

void MainWindow::OnBtnAppSettings() {
  DialogAppSettings* dialog_app_settings = new DialogAppSettings(this);
  dialog_app_settings->Create(*this, _T("dialog_app_settings"),
                              UI_WNDSTYLE_FRAME,
                              WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  dialog_app_settings->CenterWindow();
  dialog_app_settings->ShowModal();
  /// @note update the third app name and path
  std::string name = anx::settings::SettingAppThird::GetThirdAppName();
  if (!name.empty()) {
    btn_work_pilot_e10c_->SetText(anx::common::UTF8ToUnicode(name).c_str());
  } else {
    hlayout_work_pilot_e10c_->SetVisible(false);
  }
}

std::map<std::string, std::string> MainWindow::LoadAppConfig() {
  std::map<std::string, std::string> app_config;
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  std::string config_pathname = app_data_dir + "\\default\\config_app.xml";
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(config_pathname.c_str()) != tinyxml2::XML_SUCCESS) {
    return app_config;
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return app_config;
  }
  tinyxml2::XMLElement* ele = root->FirstChildElement("app");
  if (ele == nullptr) {
    return app_config;
  }
  do {
    if (ele == nullptr) {
      break;
    }
    tinyxml2::XMLElement* ele_code = ele->FirstChildElement("code");
    if (ele_code != nullptr) {
      app_config["code"] = ele_code->GetText();
    }
    tinyxml2::XMLElement* ele_copyright = ele->FirstChildElement("copyright");
    if (ele_copyright != nullptr) {
      app_config["copyright"] = ele_copyright->GetText();
    }
  } while (false);
  return app_config;
}

}  // namespace ui
}  // namespace anx
