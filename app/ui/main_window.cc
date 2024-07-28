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

void MainWindow::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == kCloseButtonControlName) {
      PostQuitMessage(0);
      return;
    } else if (msg.pSender->GetName() == _T("work_axially_symmetrical")) {
      this->ShowWindow(false, false);
      Switch_Axially_Symmetrical();
      return;
    } else if (msg.pSender->GetName() == _T("work_stresses_adjustable")) {
      this->ShowWindow(false, false);
      Switch_Stresses_Adjustable();
      return;
    } else if (msg.pSender->GetName() == _T("work_3point_bending")) {
      this->ShowWindow(false, false);
      Switch_th3point_Bending();
      return;
    } else if (msg.pSender->GetName() == _T("work_vibration_bending")) {
      this->ShowWindow(false, false);
      Switch_Vibration_Bending();
      return;
    } else {
      // TODO(hhool):
    }
  } else {
    // TODO(hhool):
  }
}

void MainWindow::OnPrepare(const DuiLib::TNotifyUI& msg) {}

void MainWindow::OnExit(const DuiLib::TNotifyUI& msg) {}

void MainWindow::OnTimer(const DuiLib::TNotifyUI& msg) {}

void MainWindow::OnClick(DuiLib::TNotifyUI& msg) {}

DuiLib::CDuiString MainWindow::GetSkinFolder() {
#ifdef _DEBUG
  return _T("skin\\anxi\\used\\");
#else
  return _T("skin\\");
#endif
}

DuiLib::CDuiString MainWindow::GetSkinFile() {
  return _T("main_window.xml");
}

DuiLib::UILIB_RESOURCETYPE MainWindow::GetResourceType() const {
#ifdef _DEBUG
  return DuiLib::UILIB_FILE;
#else
  return DuiLib::UILIB_ZIP;
#endif
}

LPCTSTR MainWindow::GetWindowClassName(void) const {
  return _T("main_window");
}

LRESULT MainWindow::OnSysCommand(UINT uMsg,
                                 WPARAM wParam,
                                 LPARAM lParam,
                                 BOOL& bHandled) {
#if defined(WIN32) && !defined(UNDER_CE)
  BOOL bZoomed = ::IsZoomed(m_hWnd);
  LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
  if (::IsZoomed(m_hWnd) != bZoomed) {
    if (!bZoomed) {
      DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kMaxButtonControlName));
      if (pControl)
        pControl->SetVisible(false);
      pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kRestoreButtonControlName));
      if (pControl)
        pControl->SetVisible(true);
    } else {
      DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kMaxButtonControlName));
      if (pControl)
        pControl->SetVisible(true);
      pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kRestoreButtonControlName));
      if (pControl)
        pControl->SetVisible(false);
    }
  }
#else
  return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

  return 0;
}

void anx::ui::MainWindow::Switch_Axially_Symmetrical() {
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Axially_Symmetrical);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::ShowWindow(*work_window, SW_SHOWMAXIMIZED);
}

void anx::ui::MainWindow::Switch_Stresses_Adjustable() {
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Stresses_Adjustable);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::ShowWindow(*work_window, SW_SHOWMAXIMIZED);
}

void anx::ui::MainWindow::Switch_th3point_Bending() {
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Th3point_Bending);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::ShowWindow(*work_window, SW_SHOWMAXIMIZED);
}

void anx::ui::MainWindow::Switch_Vibration_Bending() {
  ui::WorkWindow* work_window =
      new ui::WorkWindow(this, anx::esolution::kSolutionName_Vibration_Bending);
  work_window->Create(m_hWnd, _T("work_window"), UI_WNDSTYLE_FRAME,
                      WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  work_window->CenterWindow();
  work_window->ShowWindow(true, true);
  ::ShowWindow(*work_window, SW_SHOWMAXIMIZED);
}
}  // namespace ui
}  // namespace anx
