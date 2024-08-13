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
#include <vector>

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_graph_settings.h"
#include "app/device/device_exp_ultrasound_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_amplitude_calibration_settings.h"
#include "app/ui/dialog_static_load_guaranteed_settings.h"
#include "app/ui/dmgraph.tlh"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPageGraph, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
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

/// @brief timer id for the graph update
const int32_t kTimerGraphId = 1;

/// @brief period for the graph update in ms
const int32_t kTimerGraphIdPeriod = 1000;

/////////////////////////////////////////////////////////////////////////////
/// @brief the graph control sample total minutes for the graph control.
const int32_t kGraphCtrlSampleTotalMinutesFive = 5;
const int32_t kGraphCtrlSampleTotalMinutesTen = 10;
const int32_t kGraphCtrlSampleTotalMinutesThirty = 30;
const int32_t kGraphCtrlSampleTotalMinutesSixty = 60;
}  // namespace

////////////////////////////////////////////////////////////////////////////////
WorkWindowSecondPageGraph::WorkWindowSecondPageGraph(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow), paint_manager_ui_(paint_manager_ui) {}

WorkWindowSecondPageGraph::~WorkWindowSecondPageGraph() {
  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
  device_com_sl_.reset();
  device_com_ul_.reset();
}

void WorkWindowSecondPageGraph::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      OnExpStart();
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      OnExpStop();
    } else if (msg.pSender->GetName() == _T("btn_exp_pause")) {
      OnExpPause();
    } else if (msg.pSender->GetName() == _T("btn_exp_resume")) {
      OnExpResume();
    }
  } else if (msg.sType == _T("selectchanged")) {
    // TODO(hhool): add selectchanged action
  }
}

bool WorkWindowSecondPageGraph::OnOptGraphTimeModeChange(void* param) {
  // TODO(hhool):
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  return false;
}

bool WorkWindowSecondPageGraph::OnChkGraphAlwaysShowNewChange(void* param) {
  // TODO(hhool):
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  return false;
}

bool WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  int32_t graphctrl_sample_total_minutes = -1;
  if (pMsg->pSender == opt_graph_time_range_5_mnitues_) {
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesFive;
  } else if (pMsg->pSender == opt_graph_time_range_10_mnitues_) {
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesTen;
  } else if (pMsg->pSender == opt_graph_time_range_30_mnitues_) {
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesThirty;
  } else if (pMsg->pSender == opt_graph_time_range_60_mnitues_) {
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesSixty;
  } else {
    return false;
  }
  graphctrl_sample_total_minutes_ = graphctrl_sample_total_minutes;

  if (page_graph_amplitude_ctrl_ != nullptr) {
    if (page_graph_amplitude_ctrl_->GetSamplingTotalMinutes() !=
        graphctrl_sample_total_minutes) {
      CActiveXUI* activex = static_cast<CActiveXUI*>(
          paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
      std::vector<Element2DPoint> element_list =
          page_graph_amplitude_ctrl_->GetGraphDataList();
      page_graph_amplitude_ctrl_->Release();
      page_graph_amplitude_ctrl_.reset();
      page_graph_amplitude_ctrl_.reset(
          new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
              activex, "amp", graphctrl_sample_total_minutes, 15, 5));
      page_graph_amplitude_ctrl_->Init(element_list);
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  if (page_graph_stress_ctrl_ != nullptr) {
    if (page_graph_stress_ctrl_->GetSamplingTotalMinutes() !=
        graphctrl_sample_total_minutes) {
      CActiveXUI* activex = static_cast<CActiveXUI*>(
          paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
      std::vector<Element2DPoint> element_list =
          page_graph_stress_ctrl_->GetGraphDataList();
      page_graph_stress_ctrl_->Release();
      page_graph_stress_ctrl_.reset();
      page_graph_stress_ctrl_.reset(
          new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
              activex, "stress", graphctrl_sample_total_minutes, 2, 5));
      page_graph_stress_ctrl_->Init(element_list);
    }
  }

  return true;
}

bool WorkWindowSecondPageGraph::OnTimer(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender != btn_graph_amplitude_title_) {
    return false;
  }
  if (pMsg->wParam == kTimerGraphId) {
    // update the graph
    RefreshExpGraphTitelControl();
    return true;
  } else {
    // TODO(hhool): do nothing;
    return false;
  }
  return true;
}

void WorkWindowSecondPageGraph::Bind() {
  /// @brief device com interface initialization
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
  /////////////////////////////////////////////////////////////////////////////
  /// @brief graph time mode pre hour
  opt_graph_time_mode_pre_hour_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_pre_hour")));
  opt_graph_time_mode_pre_hour_->Selected(false);
  opt_graph_time_mode_now_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_current_hour")));
  opt_graph_time_mode_now_->Selected(true);
  /// @brief bind the option button event
  opt_graph_time_mode_pre_hour_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeModeChange);
  opt_graph_time_mode_now_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeModeChange);

  /////////////////////////////////////////////////////////////////////////////
  /// @brief graph time alway show new
  chk_graph_always_show_new_ = static_cast<DuiLib::CCheckBoxUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_always_new")));
  chk_graph_always_show_new_->Selected(true);
  /// @brief bind the check box event
  chk_graph_always_show_new_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnChkGraphAlwaysShowNewChange);

  /////////////////////////////////////////////////////////////////
  /// @brief graph time range option
  opt_graph_time_range_5_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_5_minite")));
  opt_graph_time_range_5_mnitues_->Selected(true);
  opt_graph_time_range_10_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_10_minite")));
  opt_graph_time_range_10_mnitues_->Selected(false);
  opt_graph_time_range_30_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_30_minite")));
  opt_graph_time_range_30_mnitues_->Selected(false);
  opt_graph_time_range_60_mnitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_60_minite")));
  opt_graph_time_range_60_mnitues_->Selected(false);
  /// @brief bind the option button event
  opt_graph_time_range_5_mnitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);
  opt_graph_time_range_10_mnitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);
  opt_graph_time_range_30_mnitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);
  opt_graph_time_range_60_mnitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);

  btn_graph_amplitude_title_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_amplitude_title")));
  btn_graph_amplitude_canvas_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
  btn_graph_amplitude_canvas_->SetEnabled(false);

  btn_graph_stress_title_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_stress_title")));
  btn_graph_stress_canvas_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
  btn_graph_stress_canvas_->SetEnabled(false);

  UpdateControlFromSettings();
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            activex, "amp", graphctrl_sample_total_minutes_, 15, 5));
    page_graph_amplitude_ctrl_->Init(std::vector<Element2DPoint>());
  }
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            activex, "stress", graphctrl_sample_total_minutes_, 2, 5));
    page_graph_stress_ctrl_->Init(std::vector<Element2DPoint>());
  }
  btn_graph_amplitude_title_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageGraph::OnTimer);
}

void WorkWindowSecondPageGraph::Unbind() {
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
  if (page_graph_amplitude_ctrl_ != nullptr) {
    page_graph_amplitude_ctrl_->Release();
    page_graph_amplitude_ctrl_.reset();
  }
  if (page_graph_stress_ctrl_ != nullptr) {
    page_graph_stress_ctrl_->Release();
    page_graph_stress_ctrl_.reset();
  }
  // release the device com interface
  if (device_com_sl_ != nullptr) {
    device_com_sl_->RemoveListener(this);
    device_com_sl_ = nullptr;
  }
  if (device_com_ul_ != nullptr) {
    device_com_ul_->RemoveListener(this);
    device_com_ul_ = nullptr;
  }
}

void WorkWindowSecondPageGraph::CheckDeviceComConnectedStatus() {}

void WorkWindowSecondPageGraph::RefreshExpGraphTitelControl() {
  // get current system time get current hour and minute
  struct tm timeinfo;
  anx::common::GetLocalTime(&timeinfo);

  // format the time string like 00:00 and append to the title string
  // like "最大静载 00:00" and set to the title control. %02d:%02d is
  // used to format the time string to 00:00 format from the timeinfo
  // struct. The timeinfo struct is filled with the current time info.
  char time_str[256];
  snprintf(time_str, sizeof(time_str), "%02d:%02d", timeinfo.tm_hour,
           timeinfo.tm_min);

  // get the current time's hour and minute form timeinfo
  std::string tile_time_str = "底端振幅";
  tile_time_str += time_str;
  DuiLib::CDuiString str_title_with_time_amp =
      anx::common::string2wstring(tile_time_str.c_str()).c_str();
  btn_graph_amplitude_title_->SetText(str_title_with_time_amp);

  // update the graph canvas tile with the current time's hour and minute
  tile_time_str = "最大静载";
  tile_time_str += time_str;
  DuiLib::CDuiString str_title_with_time_stress =
      anx::common::string2wstring(tile_time_str.c_str()).c_str();
  btn_graph_stress_title_->SetText(str_title_with_time_stress);
  // update the graph stress canvas title with the current time's hour and
  // minute
}

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
  page_graph_amplitude_ctrl_->ProcessDataSampleIncoming(rand() %     // NOLINT
                                                        200);        // NOLINT
  page_graph_stress_ctrl_->ProcessDataSampleIncoming(rand() % 200);  // NOLINT

  RefreshExpGraphTitelControl();
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
  RefreshExpGraphTitelControl();
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
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesFive;
    } else if (dcs->exp_graph_range_minitues_ == 1) {
      opt_graph_time_range_5_mnitues_->Selected(false);
      opt_graph_time_range_10_mnitues_->Selected(true);
      opt_graph_time_range_30_mnitues_->Selected(false);
      opt_graph_time_range_60_mnitues_->Selected(false);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesTen;
    } else if (dcs->exp_graph_range_minitues_ == 2) {
      opt_graph_time_range_5_mnitues_->Selected(false);
      opt_graph_time_range_10_mnitues_->Selected(false);
      opt_graph_time_range_30_mnitues_->Selected(true);
      opt_graph_time_range_60_mnitues_->Selected(false);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesThirty;
    } else {
      opt_graph_time_range_5_mnitues_->Selected(false);
      opt_graph_time_range_10_mnitues_->Selected(false);
      opt_graph_time_range_30_mnitues_->Selected(false);
      opt_graph_time_range_60_mnitues_->Selected(true);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesSixty;
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

void WorkWindowSecondPageGraph::OnExpStart() {
  RefreshExpGraphTitelControl();
  paint_manager_ui_->SetTimer(btn_graph_amplitude_title_, kTimerGraphId,
                              kTimerGraphIdPeriod);
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            activex, "amp", graphctrl_sample_total_minutes_, 15, 5));
    page_graph_amplitude_ctrl_->Init(std::vector<Element2DPoint>());
  }
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            activex, "stress", graphctrl_sample_total_minutes_, 2, 5));
    page_graph_stress_ctrl_->Init(std::vector<Element2DPoint>());
  }
}

void WorkWindowSecondPageGraph::OnExpStop() {
  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
}

void WorkWindowSecondPageGraph::OnExpPause() {
  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
}

void WorkWindowSecondPageGraph::OnExpResume() {
  paint_manager_ui_->SetTimer(btn_graph_amplitude_title_, kTimerGraphId,
                              kTimerGraphIdPeriod);
}

}  // namespace ui
}  // namespace anx
