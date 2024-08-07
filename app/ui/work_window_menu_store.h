/**
 * @file work_window_menu_store.h
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_MENU_STORE_H_
#define APP_UI_WORK_WINDOW_MENU_STORE_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP

using namespace DuiLib;  // NOLINT

namespace anx {

namespace ui {
class CShadowWnd;
class CMenuStoreWnd : public CWindowWnd, public INotifyUI {
 public:
  CMenuStoreWnd();

  void Init(CControlUI* pOwner, POINT pt);

  void AdjustPostion();
  LPCTSTR GetWindowClassName() const;
  void OnFinalMessage(HWND /*hWnd*/);

  void Notify(TNotifyUI& msg);  // NOLINT

  HWND Create(HWND hwndParent,
              LPCTSTR pstrName,
              DWORD dwStyle,
              DWORD dwExStyle,
              int x = CW_USEDEFAULT,
              int y = CW_USEDEFAULT,
              int cx = CW_USEDEFAULT,
              int cy = CW_USEDEFAULT,
              HMENU hMenu = NULL);

  void ShowWindow(bool bShow = true, bool bTakeFocus = true);

  LRESULT OnCreate(UINT uMsg,
                   WPARAM wParam,
                   LPARAM lParam,
                   BOOL& bHandled);  // NOLINT

  LRESULT OnClose(UINT uMsg,
                  WPARAM wParam,
                  LPARAM lParam,
                  BOOL& bHandled);  // NOLINT

  LRESULT OnKillFocus(UINT uMsg,
                      WPARAM wParam,
                      LPARAM lParam,
                      BOOL& bHandled);  // NOLINT

  LRESULT OnKeyDown(UINT uMsg,
                    WPARAM wParam,
                    LPARAM lParam,
                    BOOL& bHandled);  // NOLINT
  LRESULT OnSize(UINT uMsg,
                 WPARAM wParam,
                 LPARAM lParam,
                 BOOL& bHandled);  // NOLINT

  LRESULT OnMove(UINT uMsg,
                 WPARAM wParam,
                 LPARAM lParam,
                 BOOL& bHandled);  // NOLINT

  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

 public:
  CPaintManagerUI m_pm;
  CControlUI* m_pOwner;
  POINT m_ptPos;
  CShadowWnd* m_pShadowWnd;
};

}  // namespace ui

}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_MENU_STORE_H_
