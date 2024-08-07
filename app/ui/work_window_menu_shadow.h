/**
 * @file work_window_menu_shadow.h
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_MENU_SHADOW_H_
#define APP_UI_WORK_WINDOW_MENU_SHADOW_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP

using namespace DuiLib;  //NOLINT

namespace anx {

namespace ui {

HBITMAP CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits);

class CShadowWnd : public CWindowWnd {
 private:
  bool m_bNeedUpdate;
  TDrawInfo m_di;
  CPaintManagerUI m_pm;

 public:
  CShadowWnd();
  ~CShadowWnd();
  LPCTSTR GetWindowClassName() const;
  UINT GetClassStyle() const;
  void OnFinalMessage(HWND /*hWnd*/);

  void RePaint();
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}  // namespace ui

}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_MENU_SHADOW_H_
