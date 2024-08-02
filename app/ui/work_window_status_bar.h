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
using namespace DuiLib;

namespace anx {
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {
class WorkWindowStatusBar : public DuiLib::CNotifyPump,
                            public anx::ui::UIVirtualWndBase {
 public:
  WorkWindowStatusBar(WorkWindow* pOwner,
                      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowStatusBar();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);
  void OnTimer(TNotifyUI& msg);

 public:
  // implement the base class virtual function
  void Bind() override;
  void Unbind() override;

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_STATUS_BAR_H_
