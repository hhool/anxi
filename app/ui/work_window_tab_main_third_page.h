/**
 * @file work_window_tab_main_third_page.h
 * @author hhool (hhool@outlook.com)
 * @brief work window third page ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_

#include <memory>
#include <string>

#include "app/ui/ui_virtual_wnd_base.h"

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {
class WorkWindowThirdPage : public DuiLib::CNotifyPump,
                            public UIVirtualWndBase {
 public:
  WorkWindowThirdPage(WorkWindow* pOwner,
                      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowThirdPage();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnTimer(TNotifyUI& msg);  // NOLINT

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 protected:
  void UpdateControlFromSettings();

 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;

  COptionUI* opt_direct_up_;
  COptionUI* opt_direct_down_;
  COptionUI* opt_action_pull_;
  COptionUI* opt_action_push_;

  CEditUI* edit_speed_;
  CEditUI* edit_retention_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_
