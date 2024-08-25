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
#include "app/common/time_utils.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_data_sample_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPageData, DuiLib::CNotifyPump)
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

////////////////////////////////////////////////////////////////////////////////
/// @brief timer id for refresh control
const uint32_t kTimerIdRefreshControl = 1;
}  // namespace
WorkWindowSecondPageData::WorkWindowSecondPageData(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow), paint_manager_ui_(paint_manager_ui) {
  device_exp_data_settings_.reset(
      new anx::device::DeviceExpDataSampleSettings());
}

WorkWindowSecondPageData::~WorkWindowSecondPageData() {
  device_exp_data_settings_.reset();
}

void WorkWindowSecondPageData::OnClick(TNotifyUI& msg) {
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

bool WorkWindowSecondPageData::OnTimer(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender != text_sample_interval_) {
    return false;
  }
  if (pMsg->wParam == kTimerIdRefreshControl) {
    RefreshSampleTimeControl();
    return true;
  } else {
    // TODO(hhool): do nothing;
    return false;
  }
  return true;
}

LPCTSTR WorkWindowSecondPageData::GetItemText(CControlUI* pControl,
                                              int iItem,
                                              int iSubItem) {
  std::cout << "GetItemText iItem:" << iItem << std::endl;
  if (iSubItem == 0) {
    pControl->SetUserData(
        anx::common::string2wstring(std::to_string(exp_datas_[iItem].id_))
            .c_str());
    return pControl->GetUserData();
  } else if (iSubItem == 1) {
    pControl->SetUserData(anx::common::string2wstring(
                              std::to_string(exp_datas_[iItem].cycle_count_))
                              .c_str());
    return pControl->GetUserData();
  } else if (iSubItem == 2) {
    int64_t KHz = static_cast<int64_t>(exp_datas_[iItem].KHz_ * 1000);
    std::wstring KHz_str = anx::common::string2wstring(format_num(KHz));
    pControl->SetUserData(KHz_str.c_str());
    return pControl->GetUserData();
  } else if (iSubItem == 3) {
    int64_t MPa = static_cast<int64_t>(exp_datas_[iItem].MPa_ * 1000);
    std::wstring MPa_str = anx::common::string2wstring(format_num(MPa));
    pControl->SetUserData(MPa_str.c_str());
    return pControl->GetUserData();
  } else if (iSubItem == 4) {
    int64_t um = static_cast<int64_t>(exp_datas_[iItem].um_ * 1000);
    std::wstring um_str = anx::common::string2wstring(format_num(um));
    pControl->SetUserData(um_str.c_str());
    return pControl->GetUserData();
  } else {
    assert(false);
    pControl->SetUserData(_T(""));
    return pControl->GetUserData();
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

  list_data_->SetTextCallback(this);
  UpdateControlFromSettings();

  paint_manager_ui_->SetTimer(text_sample_interval_, kTimerIdRefreshControl,
                              1000);
  text_sample_interval_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnTimer);
}

void WorkWindowSecondPageData::Unbind() {
  // save the settings from the control
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(text_sample_interval_, kTimerIdRefreshControl);

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

void WorkWindowSecondPageData::RefreshSampleTimeControl() {
  std::string value = ("=");
  int32_t sample_time_interval = _ttol(edit_sample_interval_->GetText());
  if (device_exp_data_settings_->sampling_interval_ != sample_time_interval) {
    device_exp_data_settings_->sampling_interval_ = sample_time_interval;
    value += format_num(sample_time_interval * 100);
    value += "S";
    text_sample_interval_->SetText(anx::common::string2wstring(value).c_str());
  }
}

std::unique_ptr<anx::device::DeviceExpDataSampleSettings>
WorkWindowSecondPageData::UpdateExpClipTimeFromControl() {
  anx::device::DeviceExpDataSampleSettings des;
  DuiLib::CDuiString str = edit_sample_start_pos_->GetText();
  if (str.IsEmpty()) {
    return nullptr;
  }
  str = edit_sample_end_pos_->GetText();
  if (str.IsEmpty()) {
    return nullptr;
  }
  str = edit_sample_interval_->GetText();
  if (str.IsEmpty()) {
    return nullptr;
  }
  int64_t sample_start_pos = 0;
  int64_t sample_end_pos = 0;
  int64_t sample_time_interval = 0;
  sample_start_pos = _ttoll(edit_sample_start_pos_->GetText());
  sample_end_pos = _ttoll(edit_sample_end_pos_->GetText());
  sample_time_interval = _ttoll(edit_sample_interval_->GetText());
  if (sample_start_pos >= sample_end_pos) {
    return nullptr;
  }
  if (sample_time_interval <= 0) {
    return nullptr;
  }
  if (sample_time_interval > (sample_end_pos - sample_start_pos)) {
    return nullptr;
  }
  if (option_sample_mode_exp_->IsSelected()) {
    des.sample_mode_ = 0;
  } else {
    des.sample_mode_ = 1;
  }
  des.sampling_start_pos_ = static_cast<int32_t>(sample_start_pos);
  des.sampling_end_pos_ = static_cast<int32_t>(sample_end_pos);
  des.sampling_interval_ = static_cast<int32_t>(sample_time_interval);
  return std::unique_ptr<anx::device::DeviceExpDataSampleSettings>(
      new anx::device::DeviceExpDataSampleSettings(des));
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

    device_exp_data_settings_.reset(settings.release());
  }
}

void WorkWindowSecondPageData::SaveSettingsFromControl() {
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings> des =
      UpdateExpClipTimeFromControl();
  if (des != nullptr) {
    anx::device::SaveDeviceExpDataSampleSettingsDefaultResource(*des);
  }
}

void anx::ui::WorkWindowSecondPageData::UpdateUIWithExpStatus(int status) {
  if (status == 0) {
    // stop
    edit_sample_start_pos_->SetEnabled(true);
    edit_sample_end_pos_->SetEnabled(true);
    edit_sample_interval_->SetEnabled(true);

    option_sample_mode_exp_->SetEnabled(true);
    option_sample_mode_linear_->SetEnabled(true);
  } else if (status == 1) {
    // start
    edit_sample_start_pos_->SetEnabled(false);
    edit_sample_end_pos_->SetEnabled(false);
    edit_sample_interval_->SetEnabled(false);

    option_sample_mode_exp_->SetEnabled(false);
    option_sample_mode_linear_->SetEnabled(false);
  } else if (status == 2) {
    // pause
    edit_sample_start_pos_->SetEnabled(true);
    edit_sample_end_pos_->SetEnabled(true);
    edit_sample_interval_->SetEnabled(true);

    option_sample_mode_exp_->SetEnabled(true);
    option_sample_mode_linear_->SetEnabled(true);
  }
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
  if (is_exp_state_ == 1) {
    int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
    int64_t time_diff = current_time_ms - exp_start_time_ms_;
    int64_t time_interval_num = time_diff / exp_sample_interval_ms_;
    // TODO(hhool): update the data to the data table
    if (time_interval_num > exp_time_interval_num_) {
      exp_time_interval_num_ = time_interval_num;
      exp_data_table_no_++;
      // update the data to the database table amp, stress, um
      uint64_t cycle_count = exp_data_table_no_ * 500;
      float KHz = 208.230f;
      float MPa = 102.080f;
      float um = 1023.230f;
      /*anx::common::DatabaseHelper::InsertExpData(
          exp_data_table_no_, KHz, Mpa, um,
         anx::common::GetCurrrentDateTime());*/
      // update the data to the data table
      DuiLib::CListTextElementUI* item_no = new DuiLib::CListTextElementUI();
      item_no->SetTag(exp_data_table_no_);
      list_data_->Add(item_no);
      anx::expdata::ExperimentData exp_data;
      exp_data.id_ = exp_data_table_no_;
      exp_data.cycle_count_ = cycle_count;
      exp_data.KHz_ = 208.230f;
      exp_data.MPa_ = 102.080f;
      exp_data.um_ = 1023.230f;
      exp_datas_.push_back(exp_data);
    }
  }
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

void WorkWindowSecondPageData::OnExpStart() {
  is_exp_state_ = 1;
  UpdateUIWithExpStatus(1);
  list_data_->RemoveAll();
  exp_datas_.clear();
  exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_start_date_time_ = time(nullptr);
  exp_data_incoming_num_ = 0;
  exp_sample_interval_ms_ = device_exp_data_settings_->sampling_interval_ * 100;
}

void WorkWindowSecondPageData::OnExpStop() {
  is_exp_state_ = 0;
  UpdateUIWithExpStatus(0);
  // TODO(hhool): auto save the data to the file
  // file path is execuatable path/recored/xxx.csv
  // file format is csv
  anx::expdata::SaveExperimentDataToCsvWithDefaultPath(exp_datas_,
                                                       exp_start_date_time_);
  // reset exp params;
  exp_start_time_ms_ = 0;
  exp_data_incoming_num_ = 0;
  exp_time_interval_num_ = 0;
  exp_data_table_no_ = 0;
}

void WorkWindowSecondPageData::OnExpPause() {
  UpdateUIWithExpStatus(2);
  is_exp_state_ = 2;
}

void WorkWindowSecondPageData::OnExpResume() {
  UpdateUIWithExpStatus(1);
  is_exp_state_ = 1;
}

void WorkWindowSecondPageData::ClearExpData() {
  list_data_->RemoveAll();
  exp_datas_.clear();
  exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_start_date_time_ = time(nullptr);
  exp_data_incoming_num_ = 0;
  exp_time_interval_num_ = 0;
  exp_data_table_no_ = 0;
}

}  // namespace ui
}  // namespace anx
