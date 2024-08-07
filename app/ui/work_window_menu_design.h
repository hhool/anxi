/**
 * @file work_window_menu_design.h
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_MENU_DESIGN_H_
#define APP_UI_WORK_WINDOW_MENU_DESIGN_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP

using DuiLib::CControlUI;
using DuiLib::CPaintManagerUI;
using DuiLib::CWindowWnd;
using DuiLib::INotifyUI;

namespace anx {
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx
namespace anx {

namespace ui {

class CShadowWnd;
class CMenuDesignWnd : public CWindowWnd, public INotifyUI {
 public:
  explicit CMenuDesignWnd(WorkWindow* pWorkWindow);

  void Init(CControlUI* pOwner, POINT pt);

  void AdjustPostion();
  LPCTSTR GetWindowClassName() const;
  void OnFinalMessage(HWND /*hWnd*/);

  void Notify(DuiLib::TNotifyUI& msg) override;

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
  WorkWindow* pWorkWindow_;
  CPaintManagerUI m_pm;
  CControlUI* m_pOwner;
  POINT m_ptPos;
  CShadowWnd* m_pShadowWnd;
};

}  // namespace ui

}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_MENU_DESIGN_H_
