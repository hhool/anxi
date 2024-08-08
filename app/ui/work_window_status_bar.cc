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
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow), paint_manager_ui_(paint_manager_ui) {
  paint_manager_ui_->AddNotifier(this);
}

WorkWindowStatusBar::~WorkWindowStatusBar() {}

void WorkWindowStatusBar::Notify(TNotifyUI& msg) {}

void WorkWindowStatusBar::OnClick(TNotifyUI& msg) {
  if (msg.pSender != nullptr) {
  } else {
  }
}

void WorkWindowStatusBar::OnTimer(TNotifyUI& msg) {
  DuiLib::CTextUI* ui_text = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("status_bar_current_time")));
  int64_t current_time = anx::common::GetCurrentTimeMillis();
  char date_time[256] = {0};
  anx::common::GetCurrentDateTime(date_time);
  ui_text->SetText(anx::common::string2wstring(date_time).c_str());

  DuiLib::CTextUI* ui_text_connected = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("status_bar_connected")));
  if (pWorkWindow_->IsDeviceComInterfaceConnected()) {
    ui_text_connected->SetText(_T("联机"));
  } else {
    ui_text_connected->SetText(_T("脱机"));
  }

  int32_t connected_device_num = 0;
  if (pWorkWindow_->IsSLDeviceComInterfaceConnected()) {
    connected_device_num += 1;
  }
  if (pWorkWindow_->IsULDeviceComInterfaceConnected()) {
    connected_device_num += 1;
  }

  DuiLib::CTextUI* ui_text_device = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("status_bar_num_of_device")));
  std::wstring text = _T("设备数量:");
  text += anx::common::string2wstring(std::to_string(connected_device_num));
  text += _T("台");
  ui_text_device->SetText(text.c_str());
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
