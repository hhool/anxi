/**
 * @file work_window_menu_design.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_menu_design.h"

#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"
#include "app/ui/work_window_menu_shadow.h"

namespace anx {
namespace ui {

//////////////////////////////////////////////////////////////////////////

CMenuDesignWnd::CMenuDesignWnd(WorkWindow* pWorkWindow)
    : pWorkWindow_(pWorkWindow), m_pOwner(NULL), m_pShadowWnd(NULL) {}

void CMenuDesignWnd::Init(CControlUI* pOwner, POINT pt) {
  if (pOwner == NULL)
    return;
  m_pOwner = pOwner;
  m_ptPos = pt;
  Create(pOwner->GetManager()->GetPaintWindow(), NULL,
         WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
         WS_EX_TOOLWINDOW);
  HWND hWndParent = m_hWnd;
  while (::GetParent(hWndParent) != NULL)
    hWndParent = ::GetParent(hWndParent);
  ::ShowWindow(m_hWnd, SW_SHOW);
  ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void CMenuDesignWnd::AdjustPostion() {
  CDuiRect rcWnd;
  GetWindowRect(m_hWnd, &rcWnd);
  int nWidth = rcWnd.GetWidth();
  int nHeight = rcWnd.GetHeight();
  rcWnd.left = m_ptPos.x;
  rcWnd.top = m_ptPos.y;
  rcWnd.right = rcWnd.left + nWidth;
  rcWnd.bottom = rcWnd.top + nHeight;
  MONITORINFO oMonitor = {};
  oMonitor.cbSize = sizeof(oMonitor);
  ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY),
                   &oMonitor);
  CDuiRect rcWork = oMonitor.rcWork;

  if (rcWnd.bottom > rcWork.bottom) {
    if (nHeight >= rcWork.GetHeight()) {
      rcWnd.top = 0;
      rcWnd.bottom = nHeight;
    } else {
      rcWnd.bottom = rcWork.bottom;
      rcWnd.top = rcWnd.bottom - nHeight;
    }
  }
  if (rcWnd.right > rcWork.right) {
    if (nWidth >= rcWork.GetWidth()) {
      rcWnd.left = 0;
      rcWnd.right = nWidth;
    } else {
      rcWnd.right = rcWork.right;
      rcWnd.left = rcWnd.right - nWidth;
    }
  }
  ::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(),
                 rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

LPCTSTR CMenuDesignWnd::GetWindowClassName() const {
  return _T("MenuWnd");
}

void CMenuDesignWnd::OnFinalMessage(HWND /*hWnd*/) {
  delete this;
}

void CMenuDesignWnd::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kItemSelect) {
    Close();
  } else if (msg.sType == kItemClick) {
    if (m_pOwner)
      m_pOwner->GetManager()->SendNotify(m_pOwner, msg.pSender->GetName(), 0, 0,
                                         true);
  }
}

HWND CMenuDesignWnd::Create(HWND hwndParent,
                            LPCTSTR pstrName,
                            DWORD dwStyle,
                            DWORD dwExStyle,
                            int x,
                            int y,
                            int cx,
                            int cy,
                            HMENU hMenu) {
  if (m_pShadowWnd == NULL)
    m_pShadowWnd = new CShadowWnd;
  m_pShadowWnd->Create(hwndParent, _T(""),
                       WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS,
                       WS_EX_LAYERED | WS_EX_TOOLWINDOW, x, y, cx, cy, NULL);

  dwExStyle |= WS_EX_TOOLWINDOW;
  return CWindowWnd::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx,
                            cy, hMenu);
}

void CMenuDesignWnd::ShowWindow(bool bShow, bool bTakeFocus) {
  if (m_pShadowWnd != NULL)
    m_pShadowWnd->ShowWindow(bShow, false);
  CWindowWnd::ShowWindow(bShow, bTakeFocus);
}

LRESULT CMenuDesignWnd::OnCreate(UINT uMsg,
                                 WPARAM wParam,
                                 LPARAM lParam,
                                 BOOL& bHandled) {
  m_pm.Init(m_hWnd);
  CDialogBuilder builder;
  CControlUI* pRoot =
      builder.Create(_T("menu_design.xml"), (UINT)0, NULL, &m_pm);
  ASSERT(pRoot && "Failed to parse XML");
  m_pm.AttachDialog(pRoot);
  m_pm.AddNotifier(this);
  m_pm.SetRoundCorner(3, 3);

  AdjustPostion();
  if (pWorkWindow_->IsDeviceComInterfaceConnected()) {
    CControlUI* pControl = m_pm.FindControl(_T("menu_design_item_connect"));
    if (pControl != NULL) {
      pControl->SetEnabled(false);
    }
    pControl = m_pm.FindControl(_T("menu_design_item_disconnect"));
    if (pControl != NULL) {
      pControl->SetEnabled(true);
    }
  } else {
    CControlUI* pControl = m_pm.FindControl(_T("menu_design_item_connect"));
    if (pControl != NULL) {
      pControl->SetEnabled(true);
    }
    pControl = m_pm.FindControl(_T("menu_design_item_disconnect"));
    if (pControl != NULL) {
      pControl->SetEnabled(false);
    }
  }
  return 0;
}

LRESULT CMenuDesignWnd::OnClose(UINT uMsg,
                                WPARAM wParam,
                                LPARAM lParam,
                                BOOL& bHandled) {
  if (m_pShadowWnd != NULL)
    m_pShadowWnd->Close();
  bHandled = FALSE;
  return 0;
}

LRESULT CMenuDesignWnd::OnKillFocus(UINT uMsg,
                                    WPARAM wParam,
                                    LPARAM lParam,
                                    BOOL& bHandled) {
  if ((HWND)wParam == m_hWnd) {
    bHandled = TRUE;
  } else if (m_pShadowWnd != NULL && (HWND)wParam == m_pShadowWnd->GetHWND()) {
    CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    ::SetFocus(m_hWnd);
    bHandled = TRUE;
  } else {
    Close();
    bHandled = FALSE;
  }
  return 0;
}

LRESULT CMenuDesignWnd::OnKeyDown(UINT uMsg,
                                  WPARAM wParam,
                                  LPARAM lParam,
                                  BOOL& bHandled) {
  if (wParam == VK_ESCAPE)
    Close();
  return 0;
}

LRESULT CMenuDesignWnd::OnSize(UINT uMsg,
                               WPARAM wParam,
                               LPARAM lParam,
                               BOOL& bHandled) {
  if (!::IsIconic(*this)) {
    CDuiRect rcWnd;
    ::GetWindowRect(*this, &rcWnd);
    rcWnd.Offset(-rcWnd.left, -rcWnd.top);
    HRGN hRgn = ::CreateRectRgn(rcWnd.left + 8, rcWnd.top + 8, rcWnd.right - 8,
                                rcWnd.bottom - 8);
    ::SetWindowRgn(*this, hRgn, TRUE);
    ::DeleteObject(hRgn);
  }
  if (m_pShadowWnd != NULL) {
    RECT rcWnd = {0};
    ::GetWindowRect(m_hWnd, &rcWnd);
    ::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top,
                   rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
                   SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
  }
  bHandled = FALSE;
  return 0;
}

LRESULT CMenuDesignWnd::OnMove(UINT uMsg,
                               WPARAM wParam,
                               LPARAM lParam,
                               BOOL& bHandled) {
  if (m_pShadowWnd != NULL) {
    RECT rcWnd = {0};
    ::GetWindowRect(m_hWnd, &rcWnd);
    ::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top,
                   rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
                   SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
  }
  bHandled = FALSE;
  return 0;
}

LRESULT CMenuDesignWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  LRESULT lRes = 0;
  BOOL bHandled = TRUE;
  switch (uMsg) {
    case WM_CREATE:
      lRes = OnCreate(uMsg, wParam, lParam, bHandled);
      break;
    case WM_CLOSE:
      lRes = OnClose(uMsg, wParam, lParam, bHandled);
      break;
    case WM_KILLFOCUS:
      lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
      break;
    case WM_KEYDOWN:
      lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
      break;
    case WM_MOUSEWHEEL:
      break;
    case WM_SIZE:
      lRes = OnSize(uMsg, wParam, lParam, bHandled);
      break;
    case WM_MOVE:
      lRes = OnMove(uMsg, wParam, lParam, bHandled);
      break;
    default:
      bHandled = FALSE;
  }
  if (bHandled)
    return lRes;
  if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
    return lRes;
  return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

}  // namespace ui
}  // namespace anx
