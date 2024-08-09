/**
 * @file work_window_tab_main_second_page_data.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window second page data ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_second_page_data.h"

#include <iostream>
#include <utility>

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_data_sample_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_amplitude_calibration_settings.h"
#include "app/ui/dialog_static_load_guaranteed_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPageData, DuiLib::CNotifyPump)
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
WorkWindowSecondPageData::WorkWindowSecondPageData(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow),
      paint_manager_ui_(paint_manager_ui),
      sample_start_pos_(0),
      sample_end_pos_(0),
      sample_time_interval_(0) {}

WorkWindowSecondPageData::~WorkWindowSecondPageData() {
  paint_manager_ui_->KillTimer(edit_sample_start_pos_, 1);
}

void WorkWindowSecondPageData::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
  } else if (msg.sType == _T("selectchanged")) {
    // TODO(hhool): add selectchanged action
  }
}

void WorkWindowSecondPageData::OnTimer(TNotifyUI& msg) {
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

void WorkWindowSecondPageData::Bind() {
  /// @brief device com interface initialization
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
  /// @brief sample mode option button exp
  option_sample_mode_exp_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("opt_sampling_exponent")));
  /// @brief sample mode option button linear
  option_sample_mode_linear_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("opt_sampling_linear")));
  /// @brief sample start pos edit
  edit_sample_start_pos_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_sampling_start_pos")));
  /// @brief sample end pos edit
  edit_sample_end_pos_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_sampling_end_pos")));
  /// @brief sample time duration edit
  edit_sample_interval_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_sampling_interval")));
  text_sample_interval_ = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("text_sampling_interval")));

  list_data_ = static_cast<DuiLib::CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph_data_list_data")));

  UpdateControlFromSettings();
  paint_manager_ui_->SetTimer(edit_sample_start_pos_, 1, 1000);
}

void WorkWindowSecondPageData::Unbind() {
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(edit_sample_start_pos_, 1);

  // release the device com interface
  device_com_sl_ = nullptr;
  device_com_ul_ = nullptr;
}

void WorkWindowSecondPageData::CheckDeviceComConnectedStatus() {}

void WorkWindowSecondPageData::RefreshExpClipTimeControl() {}

void WorkWindowSecondPageData::RefreshSampleTimeControl() {
  std::string value = ("=");
  int64_t sample_time_interval = _ttoll(edit_sample_interval_->GetText());
  if (sample_time_interval_ != sample_time_interval) {
    sample_time_interval_ = sample_time_interval;
    value += format_num(sample_time_interval * 100);
    value += "S";
    text_sample_interval_->SetText(anx::common::string2wstring(value).c_str());
  }
}

void WorkWindowSecondPageData::UpdateExpClipTimeFromControl() {
  sample_start_pos_ = _ttoll(edit_sample_start_pos_->GetText());
  sample_end_pos_ = _ttoll(edit_sample_end_pos_->GetText());
  sample_time_interval_ = _ttoll(edit_sample_interval_->GetText());
}

void WorkWindowSecondPageData::OnDataReceived(
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
  DuiLib::CListTextElementUI* item_no = new DuiLib::CListTextElementUI();
  list_data_->Add(item_no);
  item_no->SetText(0, anx::common::string2wstring(hex_str).c_str());
  item_no->SetText(1, anx::common::string2wstring(hex_str).c_str());
  item_no->SetText(2, anx::common::string2wstring(hex_str).c_str());
  item_no->SetText(3, anx::common::string2wstring(hex_str).c_str());
  item_no->SetText(4, anx::common::string2wstring(hex_str).c_str());
}

void WorkWindowSecondPageData::OnDataOutgoing(
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

void WorkWindowSecondPageData::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings> settings =
      anx::device::LoadDeviceExpDataSampleSettingsDefaultResource();
  if (settings != nullptr) {
    if (settings->sample_mode_ == 0) {
      option_sample_mode_exp_->Selected(true);
    } else {
      option_sample_mode_linear_->Selected(true);
    }
    edit_sample_start_pos_->SetText(
        anx::common::string2wstring(
            std::to_string(settings->sampling_start_pos_))
            .c_str());
    edit_sample_end_pos_->SetText(
        anx::common::string2wstring(std::to_string(settings->sampling_end_pos_))
            .c_str());
    edit_sample_interval_->SetText(
        anx::common::string2wstring(
            std::to_string(settings->sampling_interval_))
            .c_str());
  }
}

void WorkWindowSecondPageData::SaveSettingsFromControl() {
  anx::device::DeviceExpDataSampleSettings des;
  des.sampling_start_pos_ = _ttoi(edit_sample_start_pos_->GetText());
  des.sampling_end_pos_ = _ttoi(edit_sample_end_pos_->GetText());
  des.sampling_interval_ = _ttoi(edit_sample_interval_->GetText());
  if (option_sample_mode_exp_->IsSelected()) {
    des.sample_mode_ = 0;
  } else {
    des.sample_mode_ = 1;
  }
  anx::device::SaveDeviceExpDataSampleSettingsDefaultResource(des);
}

}  // namespace ui
}  // namespace anx
