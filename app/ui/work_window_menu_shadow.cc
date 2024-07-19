/**
 * @file work_window_menu_shadow.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_menu_shadow.h"

#include "app/ui/work_window.h"

namespace anx {
namespace ui {

HBITMAP CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits) {
  LPBITMAPINFO lpbiSrc = NULL;
  lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
  if (lpbiSrc == NULL)
    return NULL;

  lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  lpbiSrc->bmiHeader.biWidth = cx;
  lpbiSrc->bmiHeader.biHeight = cy;
  lpbiSrc->bmiHeader.biPlanes = 1;
  lpbiSrc->bmiHeader.biBitCount = 32;
  lpbiSrc->bmiHeader.biCompression = BI_RGB;
  lpbiSrc->bmiHeader.biSizeImage = cx * cy;
  lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
  lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
  lpbiSrc->bmiHeader.biClrUsed = 0;
  lpbiSrc->bmiHeader.biClrImportant = 0;

  HBITMAP hBitmap =
      CreateDIBSection(hDC, lpbiSrc, DIB_RGB_COLORS,
                       reinterpret_cast<void**>(pBits), NULL, NULL);
  delete[] lpbiSrc;
  return hBitmap;
}

CShadowWnd::CShadowWnd() {
  m_di.sDrawString = _T("file='menu_bk.png' corner='40,8,8,8'");
}

CShadowWnd::~CShadowWnd() {}

LPCTSTR CShadowWnd::GetWindowClassName() const {
  return _T("UIShadow");
}

UINT CShadowWnd::GetClassStyle() const {
  return UI_CLASSSTYLE_FRAME;
}

void CShadowWnd::OnFinalMessage(HWND /*hWnd*/) {
  delete this;
}

void CShadowWnd::RePaint() {
  RECT rcClient = {0};
  ::GetClientRect(m_hWnd, &rcClient);
  LONG dwWidth = static_cast<LONG>(rcClient.right - rcClient.left);
  LONG dwHeight = static_cast<LONG>(rcClient.bottom - rcClient.top);

  HDC hDcPaint = ::GetDC(m_hWnd);
  HDC hDcBackground = ::CreateCompatibleDC(hDcPaint);
  COLORREF* pBackgroundBits;
  HBITMAP hbmpBackground =
      CreateMyBitmap(hDcPaint, dwWidth, dwHeight, &pBackgroundBits);
  ::ZeroMemory(pBackgroundBits, dwWidth * dwHeight * 4);
  HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hDcBackground, hbmpBackground);

  CRenderEngine::DrawImage(hDcBackground, &m_pm, rcClient, rcClient, m_di);

  RECT rcWnd = {0};
  ::GetWindowRect(m_hWnd, &rcWnd);

  BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
  POINT ptPos = {rcWnd.left, rcWnd.top};
  SIZE sizeWnd = {dwWidth, dwHeight};
  POINT ptSrc = {0, 0};
  UpdateLayeredWindow(m_hWnd, hDcPaint, &ptPos, &sizeWnd, hDcBackground, &ptSrc,
                      0, &bf, ULW_ALPHA);

  ::SelectObject(hDcBackground, hOldBitmap);
  if (hDcBackground != NULL)
    ::DeleteDC(hDcBackground);
  if (hbmpBackground != NULL)
    ::DeleteObject(hbmpBackground);
  ::ReleaseDC(m_hWnd, hDcPaint);

  m_bNeedUpdate = false;
}

LRESULT CShadowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_CREATE) {
    SetTimer(m_hWnd, 9000, 10, NULL);
  } else if (uMsg == WM_SIZE) {
    m_bNeedUpdate = true;
  } else if (uMsg == WM_CLOSE) {
    KillTimer(m_hWnd, 9000);
  } else if (uMsg == WM_TIMER) {
    if (LOWORD(wParam) == 9000 && m_bNeedUpdate == true) {
      if (!::IsIconic(m_hWnd)) {
        RePaint();
        return 0;
      }
    }
  } else {
    m_bNeedUpdate = true;
  }

  return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
}  // namespace ui
}  // namespace anx
