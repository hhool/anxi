/**
 * @file work_window_tab_main_third_page.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window tab main third page class definition
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_third_page.h"

#include <string>
#include <utility>

#include "app/common/string_utils.h"
#include "app/device/device_exp_load_static_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowThirdPage, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
namespace {
const int32_t kTimerID = 0x1;
const int32_t kTimerElapse = 1000;
}  // namespace
WorkWindowThirdPage::WorkWindowThirdPage(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow), paint_manager_ui_(paint_manager_ui) {}

WorkWindowThirdPage::~WorkWindowThirdPage() {}

void WorkWindowThirdPage::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
  } else if (msg.sType == kSelectChanged) {
  }
}

void WorkWindowThirdPage::OnTimer(TNotifyUI& msg) {
  if (msg.wParam == kTimerID) {
  } else {
  }
}

void WorkWindowThirdPage::Bind() {
  // bind timer
  DuiLib::CHorizontalLayoutUI* layout =
      static_cast<DuiLib::CHorizontalLayoutUI*>(
          paint_manager_ui_->FindControl(_T("tab_page_three")));
  // unbind timer
  paint_manager_ui_->SetTimer(layout, kTimerID, kTimerElapse);

  // bind control
  opt_direct_up_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_move_up")));
  opt_direct_down_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_move_down")));

  opt_action_pull_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_pull")));
  opt_action_push_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_push")));

  edit_speed_ = static_cast<CEditUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_speed")));

  edit_retention_ = static_cast<CEditUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_retention")));
  UpdateControlFromSettings();
}

void WorkWindowThirdPage::Unbind() {
  DuiLib::CHorizontalLayoutUI* layout =
      static_cast<DuiLib::CHorizontalLayoutUI*>(
          paint_manager_ui_->FindControl(_T("tab_page_three")));
  // unbind timer
  paint_manager_ui_->KillTimer(layout, kTimerID);
}

void WorkWindowThirdPage::UpdateControlFromSettings() {
  // update control from settings
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    return;
  }
  opt_direct_up_->Selected(lss->direct_ == 1);
  opt_direct_down_->Selected(lss->direct_ == 2);
  opt_action_pull_->Selected(lss->action_ == 1);
  opt_action_push_->Selected(lss->action_ == 2);
  edit_speed_->SetText(
      anx::common::string2wstring(std::to_string(lss->speed_).c_str()).c_str());
  edit_retention_->SetText(
      anx::common::string2wstring(std::to_string(lss->retention_).c_str())
          .c_str());
}
}  // namespace ui
}  // namespace anx
