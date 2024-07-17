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

DUI_BEGIN_MESSAGE_MAP(anx::ui::MainWindow, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

MainWindow::MainWindow() {}

MainWindow::~MainWindow() {}

void MainWindow::Show() {}

void MainWindow::Hide() {}

void MainWindow::Close() {}

void MainWindow::SetTitle(const std::string& title) {
  title_ = title;
}

void MainWindow::SetSize(int width, int height) {
  width_ = width;
  height_ = height;
}

void MainWindow::SetPosition(int x, int y) {
  x_ = x;
  y_ = y;
}

void MainWindow::InitWindow() {}

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
  if (msg.sType == _T("windowinit")) {
  } else if (msg.sType == _T("click")) {
    if (msg.pSender->GetName() == _T("closebtn")) {
      PostQuitMessage(0);
      return;
    } else if (msg.pSender->GetName() == _T("loginBtn")) {
      Close();
      return;
    } else {
    }
  } else {
  }
}

void MainWindow::OnPrepare(DuiLib::TNotifyUI& msg) {}

void MainWindow::OnExit(DuiLib::TNotifyUI& msg) {}

void MainWindow::OnTimer(DuiLib::TNotifyUI& msg) {}

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

}  // namespace ui
}  // namespace anx
