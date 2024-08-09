/**
 * @file work_window_tab_main_second_page_graph.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window second page graph ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_second_page_graph.h"

#include <iostream>
#include <utility>

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_graph_settings.h"
#include "app/device/device_exp_ultrasound_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_amplitude_calibration_settings.h"
#include "app/ui/dialog_static_load_guaranteed_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPageGraph, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
namespace {

std::string format_num(int64_t num) {
  std::string value;
  int64_t integer_part = num / 1000;
  int64_t decimal_part = num % 1000;
  // remove the 0 at the end of the decimal.
  while (decimal_part % 10 == 0) {
    decimal_part /= 10;
    if (decimal_part == 0) {
      break;
    }
  }
  // format integer part
  value += std::to_string(integer_part);
  if (decimal_part != 0) {
    value += ".";
    value += std::to_string(decimal_part);
  }
  return value;
}
}  // namespace
WorkWindowSecondPageGraph::WorkWindowSecondPageGraph(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow), paint_manager_ui_(paint_manager_ui) {}

WorkWindowSecondPageGraph::~WorkWindowSecondPageGraph() {
  paint_manager_ui_->KillTimer(opt_graph_time_mode_now_, 1);
  device_com_sl_.reset();
  device_com_ul_.reset();
}

void WorkWindowSecondPageGraph::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
  } else if (msg.sType == _T("selectchanged")) {
    // TODO(hhool): add selectchanged action
  }
}

void WorkWindowSecondPageGraph::OnTimer(TNotifyUI& msg) {
  uint32_t id_timer = msg.wParam;
  if (id_timer == 1) {
  } else if (id_timer == 2) {
    CheckDeviceComConnectedStatus();
    RefreshExpClipTimeControl();
    RefreshSampleTimeControl();
  } else {
    std::cout << "";
  }
}

void WorkWindowSecondPageGraph::Bind() {
  /// @brief device com interface initialization
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
  /// @brife graph time mode pre hour
  opt_graph_time_mode_pre_hour_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_pre_hour")));
  opt_graph_time_mode_pre_hour_->Selected(false);
  opt_graph_time_mode_now_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_current_hour")));
  opt_graph_time_mode_now_->Selected(true);

  chk_graph_always_show_new_ = static_cast<DuiLib::CCheckBoxUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_always_new")));
  chk_graph_always_show_new_->Selected(true);

  opt_graph_time_range_5_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_5_minite")));
  opt_graph_time_range_5_mnitues_->Selected(false);
  opt_graph_time_range_10_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_10_minite")));
  opt_graph_time_range_10_mnitues_->Selected(false);
  opt_graph_time_range_30_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_30_minite")));
  opt_graph_time_range_30_mnitues_->Selected(false);
  opt_graph_time_range_60_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_60_minite")));
  opt_graph_time_range_60_mnitues_->Selected(true);

  /*list_data_ = static_cast<DuiLib::CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph_data_list_data")));*/

  UpdateControlFromSettings();
  // paint_manager_ui_->SetTimer(opt_graph_time_mode_now_, 2, 1000);
}

void WorkWindowSecondPageGraph::Unbind() {
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(opt_graph_time_mode_now_, 1);
  //  paint_manager_ui_->KillTimer(opt_graph_time_mode_now_, 2);
  // release the device com interface
  device_com_sl_ = nullptr;
  device_com_ul_ = nullptr;
}

void WorkWindowSecondPageGraph::CheckDeviceComConnectedStatus() {}

void WorkWindowSecondPageGraph::RefreshExpClipTimeControl() {}

void WorkWindowSecondPageGraph::RefreshSampleTimeControl() {}

void WorkWindowSecondPageGraph::UpdateExpClipTimeFromControl() {}

void WorkWindowSecondPageGraph::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool): process data
  std::string hex_str;
  if (device == device_com_ul_.get()) {
    // process the data from ultrasound device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
    // output the data as hex to the std::string
    hex_str = anx::common::ByteArrayToHexString(data, size);
    std::cout << hex_str << std::endl;
  } else if (device == device_com_sl_.get()) {
    // process the data from static load device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
    hex_str = anx::common::ByteArrayToHexString(data, size);
    std::cout << hex_str << std::endl;
  }
  // TODO(hhool): update the data to graph
}

void WorkWindowSecondPageGraph::OnDataOutgoing(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool):
  if (device == device_com_ul_.get()) {
    // process the data from ultrasound device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
  } else if (device == device_com_sl_.get()) {
    // process the data from static load device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
  }
}

void WorkWindowSecondPageGraph::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceExpGraphSettings> dcs =
      anx::device::LoadDeviceExpGraphSettingsDefaultResource();
  if (dcs != nullptr) {
    if (dcs->exp_graph_show_time_type_ == 0) {
      opt_graph_time_mode_pre_hour_->Selected(true);
      opt_graph_time_mode_now_->Selected(false);
    } else {
      opt_graph_time_mode_pre_hour_->Selected(false);
      opt_graph_time_mode_now_->Selected(true);
    }

    if (dcs->exp_graph_range_minitues_ == 0) {
      opt_graph_time_range_5_mnitues_->Selected(true);
      opt_graph_time_range_10_mnitues_->Selected(false);
      opt_graph_time_range_30_mnitues_->Selected(false);
      opt_graph_time_range_60_mnitues_->Selected(false);
    } else if (dcs->exp_graph_range_minitues_ == 1) {
      opt_graph_time_range_5_mnitues_->Selected(false);
      opt_graph_time_range_10_mnitues_->Selected(true);
      opt_graph_time_range_30_mnitues_->Selected(false);
      opt_graph_time_range_60_mnitues_->Selected(false);
    } else if (dcs->exp_graph_range_minitues_ == 2) {
      opt_graph_time_range_5_mnitues_->Selected(false);
      opt_graph_time_range_10_mnitues_->Selected(false);
      opt_graph_time_range_30_mnitues_->Selected(true);
      opt_graph_time_range_60_mnitues_->Selected(false);
    } else {
      opt_graph_time_range_5_mnitues_->Selected(false);
      opt_graph_time_range_10_mnitues_->Selected(false);
      opt_graph_time_range_30_mnitues_->Selected(false);
      opt_graph_time_range_60_mnitues_->Selected(true);
    }

    if (dcs->exp_graph_always_new_ == 1) {
      chk_graph_always_show_new_->Selected(true);
    } else {
      chk_graph_always_show_new_->Selected(false);
    }
  }
}

void WorkWindowSecondPageGraph::SaveSettingsFromControl() {
  anx::device::DeviceExpGraphSettings dcs;
  if (opt_graph_time_mode_pre_hour_->IsSelected()) {
    dcs.exp_graph_show_time_type_ = 0;
  } else {
    dcs.exp_graph_show_time_type_ = 1;
  }

  if (opt_graph_time_range_5_mnitues_->IsSelected()) {
    dcs.exp_graph_range_minitues_ = 0;
  } else if (opt_graph_time_range_10_mnitues_->IsSelected()) {
    dcs.exp_graph_range_minitues_ = 1;
  } else if (opt_graph_time_range_30_mnitues_->IsSelected()) {
    dcs.exp_graph_range_minitues_ = 2;
  } else {
    dcs.exp_graph_range_minitues_ = 3;
  }

  if (chk_graph_always_show_new_->IsSelected()) {
    dcs.exp_graph_always_new_ = 1;
  } else {
    dcs.exp_graph_always_new_ = 0;
  }
  anx::device::SaveDeviceExpGraphSettingsDefaultResource(dcs);
}

}  // namespace ui
}  // namespace anx
