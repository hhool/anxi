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
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
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

  label_displacement_ = static_cast<CLabelUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_label_displacement")));

  label_strength_ = static_cast<CLabelUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_label_strength")));

  check_box_display_send_ = static_cast<CCheckBoxUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_right_refresh_data")));

  check_box_display_recv_notify_ =
      static_cast<CCheckBoxUI*>(paint_manager_ui_->FindControl(
          _T("tab_page_three_right_stop_recv_output")));

  list_send_ = static_cast<CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_list_send")));

  list_recv_ = static_cast<CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_list_recv")));

  list_recv_notify_ = static_cast<CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_list_recv_notify")));

  UpdateControlFromSettings();

  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
}

void WorkWindowThirdPage::Unbind() {
  if (device_com_ul_ != nullptr) {
    device_com_ul_.reset();
  }
  if (device_com_sl_ != nullptr) {
    device_com_sl_.reset();
  }
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

void WorkWindowThirdPage::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  if (device == device_com_ul_.get()) {
  } else if (device == device_com_sl_.get()) {
  }

  std::string hex_str;
  hex_str = anx::common::ByteArrayToHexString(data, size);
  DuiLib::CListLabelElementUI* item = new DuiLib::CListLabelElementUI();
  DuiLib::CDuiString dui_string = anx::common::string2wstring(hex_str).c_str();
  dui_string.Append(anx::common::string2wstring(hex_str).c_str());
  item->SetText(dui_string);
  list_recv_notify_->Add(item);
  {
    DuiLib::CListLabelElementUI* item = new DuiLib::CListLabelElementUI();
    item->SetText(dui_string);
    list_recv_->Add(item);
  }
}

void WorkWindowThirdPage::OnDataOutgoing(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // do nothing
  if (device == device_com_ul_.get()) {
  } else if (device == device_com_sl_.get()) {
  }
  std::string hex_str;
  hex_str = anx::common::ByteArrayToHexString(data, size);
  DuiLib::CListLabelElementUI* item = new DuiLib::CListLabelElementUI();
  DuiLib::CDuiString dui_string = anx::common::string2wstring(hex_str).c_str();
  dui_string.Append(anx::common::string2wstring(hex_str).c_str());
  item->SetText(dui_string);
  list_send_->Add(item);
}

}  // namespace ui
}  // namespace anx
