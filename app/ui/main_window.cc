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

#include "app/common/defines.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"
#include "app/esolution/solution_design.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::MainWindow, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

MainWindow::MainWindow() {}

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
  btn_work_pilot_e10c_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_pilot_e10c")));
  third_app_list_ = LoadThirdAppList();
  std::map<std::string, std::string> app_config = LoadAppConfig();
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
  } else {
    // TODO(hhool):
  }
}

LRESULT anx::ui::MainWindow::OnClose(UINT uMsg,
                                     WPARAM wParam,
                                     LPARAM lParam,
                                     BOOL& bHandled) {
  bHandled = TRUE;
  PostQuitMessage(0);
  return 0;
}

LRESULT anx::ui::MainWindow::OnSetFocus(UINT /*uMsg*/,
                                        WPARAM /*wParam*/,
                                        LPARAM /*lParam*/,
                                        BOOL& bHandled) {
  bHandled = TRUE;
  /// @note redraw the window when it is shown again.
  /// @note it is necessary to redraw the window when it is shown again.
  /// @note otherwise, the window will can't be redrawed completely.
  ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
  ::InvalidateRect(m_hWnd, NULL, TRUE);
  return 0;
}
void anx::ui::MainWindow::Switch_Axially_Symmetrical() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window = new ui::WorkWindow(
      this, anx::esolution::kSolutionName_Axially_Symmetrical);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void anx::ui::MainWindow::Switch_Stresses_Adjustable() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window = new ui::WorkWindow(
      this, anx::esolution::kSolutionName_Stresses_Adjustable);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void anx::ui::MainWindow::Switch_Th3point_Bending() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Th3point_Bending);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void anx::ui::MainWindow::Switch_Vibration_Bending() {
  this->ShowWindow(false, false);
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Vibration_Bending);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::PostMessage(*work_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void anx::ui::MainWindow::Switch_ThirdApp() {
  if (third_app_list_.empty()) {
    return;
  }
  std::string path = third_app_list_.begin()->c_str();
  ShellExecute(NULL, _T("open"),
               anx::common::string2wstring(path.c_str()).c_str(), NULL, NULL,
               SW_SHOW);
}

LRESULT anx::ui::MainWindow::OnNcHitTest(UINT uMsg,
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

std::vector<std::string> anx::ui::MainWindow::LoadThirdAppList() {
  std::string module_dir = anx::common::GetModuleDir();
  if (module_dir.empty()) {
    return std::vector<std::string>();
  }
  std::string module_path = module_dir + "\\default\\third_app.xml";
  std::vector<std::string> third_app_list;
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(module_path.c_str()) != tinyxml2::XML_SUCCESS) {
    return std::vector<std::string>();
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return std::vector<std::string>();
  }
  tinyxml2::XMLElement* ele_item = root->FirstChildElement("item");
  if (ele_item == nullptr) {
    return std::vector<std::string>();
  }
  do {
    if (ele_item == nullptr) {
      break;
    }
    tinyxml2::XMLElement* ele_name = ele_item->FirstChildElement("name");
    if (ele_name == nullptr) {
      break;
    }
    tinyxml2::XMLElement* ele_path = ele_item->FirstChildElement("path");
    if (ele_path == nullptr) {
      break;
    }
    third_app_list.push_back(ele_path->GetText());
  } while (ele_item = ele_item->NextSiblingElement("item"));
  return third_app_list;
}

std::map<std::string, std::string> MainWindow::LoadAppConfig() {
  std::map<std::string, std::string> app_config;
  std::string module_dir = anx::common::GetModuleDir();
  std::string module_path = module_dir + "\\default\\config_app.xml";
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(module_path.c_str()) != tinyxml2::XML_SUCCESS) {
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
