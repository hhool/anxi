/**
 * @file work_window_status_bar.h
 * @author hhool (hhool@outlook.com)
 * @brief work window status bar ui
 * @version 0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_STATUS_BAR_H_
#define APP_UI_WORK_WINDOW_STATUS_BAR_H_

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
class WorkWindowStatusBar : public DuiLib::CNotifyPump,
                            public DuiLib::INotifyUI,
                            public anx::ui::UIVirtualWndBase {
 public:
  WorkWindowStatusBar(WorkWindow* pWorkWindow,
                      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowStatusBar();

 public:
  void Notify(TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnTimer(TNotifyUI& msg);  // NOLINT

 public:
  // implement the base class virtual function
  void Bind() override;
  void Unbind() override;

 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;

  CButtonUI* btn_menu_about_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_STATUS_BAR_H_
