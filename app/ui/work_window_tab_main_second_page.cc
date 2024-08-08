/**
 * @file work_window_tab_main_second_page.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window second page ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_second_page.h"

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
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPage, DuiLib::CNotifyPump)
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
WorkWindowSecondPage::WorkWindowSecondPage(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow),
      paint_manager_ui_(paint_manager_ui),
      is_exp_running_(false),
      exp_clip_time_duration_(0),
      exp_clip_time_paused_(0),
      exp_cycle_count_(0),
      exp_freq_fluctuations_range_(0),
      sample_start_pos_(0),
      sample_end_pos_(0),
      sample_time_interval_(0) {}

WorkWindowSecondPage::~WorkWindowSecondPage() {
  paint_manager_ui_->KillTimer(btn_exp_start_, 1);
  device_com_sl_.reset();
  device_com_ul_.reset();
}

void WorkWindowSecondPage::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender == btn_tab_graph_) {
      btn_tablayout_->SelectItem(0);
    } else if (msg.pSender == btn_tab_data_) {
      btn_tablayout_->SelectItem(1);
    } else if (msg.pSender == this->btn_sa_clear_) {
      // TODO(hhool): add clear action
    } else if (msg.pSender == this->btn_sa_setting_) {
      DialogStaticLoadGuaranteedSettings*
          dialog_static_load_guraranteed_settings =
              new DialogStaticLoadGuaranteedSettings();
      dialog_static_load_guraranteed_settings->Create(
          *pWorkWindow_, _T("dialog_static_load_guraranteed_settings"),
          UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
      dialog_static_load_guraranteed_settings->CenterWindow();
      dialog_static_load_guraranteed_settings->ShowModal();
    } else if (msg.pSender == this->btn_sa_reset_) {
      // TODO(hhool): add reset action
    } else if (msg.pSender == this->btn_exp_start_) {
      exp_start();
    } else if (msg.pSender == this->btn_exp_stop_) {
      exp_stop();
    } else if (msg.pSender == this->btn_exp_pause_) {
      exp_pause();
    } else if (msg.pSender == this->btn_sa_up_) {
      // TODO(hhool): add up action
    } else if (msg.pSender == this->btn_sa_down_) {
      // TODO(hhool): add down action
    } else if (msg.pSender == this->btn_sa_stop_) {
      // TODO(hhool): add stop action
    } else if (msg.pSender == this->btn_aa_setting_) {
      DialogAmplitudeCalibrationSettings*
          dialog_amplitude_calibration_settings =
              new DialogAmplitudeCalibrationSettings();
      dialog_amplitude_calibration_settings->Create(
          *pWorkWindow_, _T("dialog_amplitude_calibration_settings"),
          UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
      dialog_amplitude_calibration_settings->CenterWindow();
      dialog_amplitude_calibration_settings->ShowModal();
      // TODO(hhool): refresh the tab main third page.
    }
  } else if (msg.sType == _T("selectchanged")) {
    // TODO(hhool): add selectchanged action
  }
}

void WorkWindowSecondPage::OnTimer(TNotifyUI& msg) {
  uint32_t id_timer = msg.wParam;
  if (id_timer == 1) {
    if (is_exp_running_) {
      if (device_com_ul_) {
        // do something
        std::cout << "exp running" << std::endl;
        uint8_t hex[8];
        hex[0] = 0x01;
        hex[1] = 0x04;
        hex[2] = 0x00;
        hex[3] = 0x01;
        hex[4] = 0x00;
        hex[5] = 0x01;
        hex[6] = 0x60;
        hex[7] = 0x0A;
        int written = device_com_ul_->Write(hex, sizeof(hex));
        if (written < 0) {
          std::cout << "ul written error:" << written;
        }
      }
      if (device_com_sl_) {
        uint8_t hex[64] = {0};
        int32_t readed = device_com_sl_->Read(hex, sizeof(hex));
        if (readed > 0) {
          std::cout << "sl readed:" << readed;
        }
      }
      if (device_com_sl_) {
        uint8_t hex[8];
        hex[0] = 0xB0;
        hex[1] = 0x03;
        hex[2] = 0x00;
        hex[3] = 0x01;
        hex[4] = 0x00;
        hex[5] = 0x03;
        hex[6] = 0x4F;
        hex[7] = 0xEA;
        int written = device_com_sl_->Write(hex, sizeof(hex));
        if (written < 0) {
          std::cout << "sl written error:" << written;
        }
      }
      if (device_com_ul_) {
        uint8_t hex[64] = {0};
        int32_t readed = device_com_ul_->Read(hex, sizeof(hex));
        if (readed > 0) {
          std::cout << "ul readed:" << readed;
        }
      }
    }
  } else if (id_timer == 2) {
    CheckDeviceComConnectedStatus();
    RefreshExpClipTimeControl();
    RefreshSampleTimeControl();
  } else {
    std::cout << "";
  }
}

void WorkWindowSecondPage::Bind() {
  btn_tablayout_ = static_cast<DuiLib::CTabLayoutUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph_data")));
  btn_tab_graph_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph")));
  btn_tab_data_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("tab_data")));

  /// @brief Static aircraft releated button
  btn_sa_clear_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_clear")));
  btn_sa_setting_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_setting")));
  btn_sa_reset_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_reset")));

  /// @brief exp action releated button
  btn_exp_start_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_start")));
  btn_exp_stop_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_stop")));
  btn_exp_pause_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_pause")));

  // disable the exp start button
  btn_exp_start_->SetEnabled(false);
  btn_exp_stop_->SetEnabled(false);
  btn_exp_pause_->SetEnabled(false);

  /// @brief Static aircraft action releated button
  btn_sa_up_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_up")));
  btn_sa_down_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_down")));
  btn_sa_stop_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_stop")));
  /// @brief AA releated button
  btn_aa_setting_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_aa_setting")));

  // disable aircraft action button
  btn_sa_up_->SetEnabled(false);
  btn_sa_down_->SetEnabled(false);
  btn_sa_stop_->SetEnabled(false);

  /// @brief exp clip set checkbox
  chk_exp_clip_set_ = static_cast<DuiLib::CCheckBoxUI*>(
      paint_manager_ui_->FindControl(_T("exp_clip_set")));
  edit_exp_clip_time_duration_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_exp_clip_time_duration")));
  edit_exp_clip_time_paused_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_exp_clip_time_paused")));
  text_exp_clip_time_duration_ = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("text_exp_clip_time_duration")));
  text_exp_clip_time_paused_ = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("text_exp_clip_time_paused")));

  /// @brief max cycle count edit
  edit_max_cycle_count_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_exp_max_cycle_count")));
  edit_max_cycle_power_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_exp_max_cycle_power")));
  edit_frequency_fluctuations_range_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_frequency_fluctuations_range")));

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

  list_data_ = static_cast<DuiLib::CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph_data_list_data")));

  UpdateControlFromSettings();
  paint_manager_ui_->SetTimer(btn_exp_start_, 2, 1000);
}

void WorkWindowSecondPage::Unbind() {
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(btn_exp_start_, 1);
  device_com_sl_.reset();
  device_com_ul_.reset();
  paint_manager_ui_->KillTimer(btn_exp_start_, 2);
}

void WorkWindowSecondPage::CheckDeviceComConnectedStatus() {
  if (pWorkWindow_->IsDeviceComInterfaceConnected()) {
    btn_exp_start_->SetEnabled(true);
    btn_exp_stop_->SetEnabled(true);
    btn_exp_pause_->SetEnabled(true);
    btn_sa_up_->SetEnabled(true);
    btn_sa_down_->SetEnabled(true);
    btn_sa_stop_->SetEnabled(true);
  } else {
    btn_exp_start_->SetEnabled(false);
    btn_exp_stop_->SetEnabled(false);
    btn_exp_pause_->SetEnabled(false);
    btn_sa_up_->SetEnabled(false);
    btn_sa_down_->SetEnabled(false);
    btn_sa_stop_->SetEnabled(false);
  }
}

void WorkWindowSecondPage::RefreshExpClipTimeControl() {
  int64_t exp_clip_time_duration =
      _ttoll(edit_exp_clip_time_duration_->GetText());
  if (exp_clip_time_duration_ != exp_clip_time_duration) {
    exp_clip_time_duration_ = exp_clip_time_duration;
    std::string value = ("=");
    value += format_num(exp_clip_time_duration * 100);
    value += "S";
    text_exp_clip_time_duration_->SetText(
        anx::common::string2wstring(value).c_str());
  }

  int64_t exp_clip_time_paused = _ttoll(edit_exp_clip_time_paused_->GetText());
  if (exp_clip_time_paused_ != exp_clip_time_paused) {
    exp_clip_time_paused_ = exp_clip_time_paused;
    std::string value = ("=");
    value += format_num(exp_clip_time_paused * 100);
    value += ("S");
    text_exp_clip_time_paused_->SetText(
        anx::common::string2wstring(value).c_str());
  }
}

void WorkWindowSecondPage::RefreshSampleTimeControl() {
  std::string value = ("=");
  int64_t sample_time_interval = _ttoll(edit_sample_interval_->GetText());
  if (sample_time_interval_ != sample_time_interval) {
    sample_time_interval_ = sample_time_interval;
    value += format_num(sample_time_interval * 100);
    value += "S";
    text_sample_interval_->SetText(anx::common::string2wstring(value).c_str());
  }
}

int32_t WorkWindowSecondPage::exp_start() {
  if (is_exp_running_) {
    return 0;
  }
  if (device_com_ul_ != nullptr) {
    return 1;
  }
  if (device_com_sl_ != nullptr) {
    return 2;
  }

  UpdateExpClipTimeFromControl();
  // create or get ultrasound device.
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
  is_exp_running_ = true;
  paint_manager_ui_->SetTimer(btn_exp_start_, 1, 40);
  return 0;
}

void WorkWindowSecondPage::exp_pause() {
  is_exp_running_ = false;
}

void WorkWindowSecondPage::exp_resume() {
  is_exp_running_ = true;
}

void WorkWindowSecondPage::exp_stop() {
  is_exp_running_ = false;
  paint_manager_ui_->KillTimer(btn_exp_start_, 1);
  device_com_sl_.reset();
  device_com_ul_.reset();
}

void WorkWindowSecondPage::UpdateExpClipTimeFromControl() {
  exp_clip_time_duration_ = _ttoll(edit_exp_clip_time_duration_->GetText());
  exp_clip_time_paused_ = _ttoll(edit_exp_clip_time_paused_->GetText());
  int64_t exp_power = _ttoll(edit_max_cycle_power_->GetText());
  exp_cycle_count_ = _ttoll(edit_max_cycle_count_->GetText()) * exp_power;
  exp_freq_fluctuations_range_ =
      _ttoll(edit_frequency_fluctuations_range_->GetText());

  sample_start_pos_ = _ttoll(edit_sample_start_pos_->GetText());
  sample_end_pos_ = _ttoll(edit_sample_end_pos_->GetText());
  sample_time_interval_ = _ttoll(edit_sample_interval_->GetText());
}

void WorkWindowSecondPage::OnDataReceived(
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

void WorkWindowSecondPage::OnDataOutgoing(
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

void WorkWindowSecondPage::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceUltrasoundSettings> dus =
      anx::device::LoadDeviceUltrasoundSettingsDefaultResource();
  if (dus != nullptr) {
    edit_exp_clip_time_duration_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->exp_clip_time_duration_).c_str())
            .c_str());
    edit_exp_clip_time_paused_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->exp_clip_time_paused_).c_str())
            .c_str());
    edit_max_cycle_count_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->exp_max_cycle_count_).c_str())
            .c_str());
    edit_max_cycle_power_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->exp_max_cycle_power_).c_str())
            .c_str());
    edit_frequency_fluctuations_range_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->exp_frequency_fluctuations_range_).c_str())
            .c_str());
    edit_sample_start_pos_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->sampling_start_pos_).c_str())
            .c_str());
    edit_sample_end_pos_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->sampling_end_pos_).c_str())
            .c_str());
    edit_sample_interval_->SetText(
        anx::common::string2wstring(
            std::to_string(dus->sampling_interval_).c_str())
            .c_str());

    if (dus->exp_clipping_enable_ == 1) {
      chk_exp_clip_set_->Selected(true);
    } else {
      chk_exp_clip_set_->Selected(false);
    }
  }

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

void WorkWindowSecondPage::SaveSettingsFromControl() {
  anx::device::DeviceUltrasoundSettings dus;
  dus.exp_clip_time_duration_ = _ttoi(edit_exp_clip_time_duration_->GetText());
  dus.exp_clip_time_paused_ = _ttoi(edit_exp_clip_time_paused_->GetText());
  dus.exp_max_cycle_count_ = _ttoi(edit_max_cycle_count_->GetText());
  dus.exp_max_cycle_power_ = _ttoi(edit_max_cycle_power_->GetText());
  dus.exp_frequency_fluctuations_range_ =
      _ttoi(edit_frequency_fluctuations_range_->GetText());
  dus.sampling_start_pos_ = _ttoi(edit_sample_start_pos_->GetText());
  dus.sampling_end_pos_ = _ttoi(edit_sample_end_pos_->GetText());
  dus.sampling_interval_ = _ttoi(edit_sample_interval_->GetText());
  if (chk_exp_clip_set_->IsSelected()) {
    dus.exp_clipping_enable_ = 1;
  } else {
    dus.exp_clipping_enable_ = 0;
  }
  anx::device::SaveDeviceUltrasoundSettingsDefaultResource(dus);

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
