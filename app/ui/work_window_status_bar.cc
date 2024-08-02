/**
 * @file work_window_status_bar.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window status bar ui
 * @version 0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_status_bar.h"

#include <utility>

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/ui/work_window.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowStatusBar, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
WorkWindowStatusBar::WorkWindowStatusBar(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pOwner_(pOwner), paint_manager_ui_(paint_manager_ui) {}

WorkWindowStatusBar::~WorkWindowStatusBar() {}

void WorkWindowStatusBar::OnClick(TNotifyUI& msg) {}

void anx::ui::WorkWindowStatusBar::OnTimer(TNotifyUI& msg) {
  DuiLib::CTextUI* ui_text = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("status_bar_current_time")));
  int64_t current_time = anx::common::GetCurrentTimeMillis();
  char date_time[256] = {0};
  anx::common::GetCurrentDateTime(date_time);
  ui_text->SetText(anx::common::string2wstring(date_time).c_str());
}

void WorkWindowStatusBar::Bind() {
  DuiLib::CTextUI* ui_text = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("status_bar_current_time")));
  paint_manager_ui_->SetTimer(ui_text, 1, 1000);
}

void WorkWindowStatusBar::Unbind() {
  paint_manager_ui_->KillTimer(
      paint_manager_ui_->FindControl(_T("status_bar_current_time")), 1);
}

}  // namespace ui
}  // namespace anx
