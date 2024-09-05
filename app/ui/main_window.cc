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
#include "app/esolution/solution_design.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

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
}

void MainWindow::OnFinalMessage(HWND hWnd) {
  __super::OnFinalMessage(hWnd);
  delete this;
}

LRESULT MainWindow::ResponseDefaultKeyEvent(WPARAM wParam, bool& bHandled) {
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
    // TODO(hhool):
    MessageBox(m_hWnd, _T("Not implemented yet!"), _T("Warning"),
               MB_OK | MB_ICONWARNING);
  } else {
    // TODO(hhool):
  }
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
}  // namespace ui
}  // namespace anx
