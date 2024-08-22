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
#include "app/ui/dialog_common.h"
#include "app/ui/dialog_static_load_guaranteed_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"
#include "app/ui/work_window_tab_main_second_page_data.h"
#include "app/ui/work_window_tab_main_second_page_graph.h"

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
////////////////////////////////////////////////////////////////////////////////
/// @brief sampling interval 100ms
/// @note 10Hz sampling frequency
/// @details 1000ms / 100ms = 10Hz sampling frequency
/// TODO(hhool):
const int32_t kSamplingInterval = 100;

/// @brief timer id for sampling
/// @note 1
const int32_t kTimerIdSampling = 1;

/// @brief timer id for refresh
/// @note 2
const int32_t kTimerIdRefresh = 2;
}  // namespace

WorkWindowSecondPage::WorkWindowSecondPage(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow),
      paint_manager_ui_(paint_manager_ui),
      is_exp_state_(-1),
      exp_clip_time_duration_(0),
      exp_clip_time_paused_(0),
      exp_cycle_count_(0),
      exp_freq_fluctuations_range_(0) {
  WorkWindowSecondPageGraph* graph_page =
      new WorkWindowSecondPageGraph(pWorkWindow, paint_manager_ui);
  work_window_second_page_graph_virtual_wnd_ = graph_page;
  work_window_second_page_graph_notify_pump_.reset(graph_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPageGraph"),
                      work_window_second_page_graph_notify_pump_.get());

  WorkWindowSecondPageData* data_page =
      new WorkWindowSecondPageData(pWorkWindow, paint_manager_ui);
  work_window_second_page_data_virtual_wnd_ = data_page;
  work_window_second_page_data_notify_pump_.reset(data_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPageData"),
                      work_window_second_page_data_notify_pump_.get());
}

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
      this->pWorkWindow_->ClearArgsFreqNum();
      WorkWindowSecondPageData* data_page =
          reinterpret_cast<WorkWindowSecondPageData*>(
              work_window_second_page_data_notify_pump_.get());
      data_page->ClearExpData();
    } else if (msg.pSender == this->btn_exp_start_) {
      exp_start();
    } else if (msg.pSender == this->btn_exp_stop_) {
      int32_t result = 0;
      DialogCommon* about = new DialogCommon("提示", "是否停止试验", result);
      about->Create(*pWorkWindow_, _T("dialog_common"), UI_WNDSTYLE_FRAME,
                    WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
      about->CenterWindow();
      about->ShowModal();
      if (result == 0) {
        return;
      }
      exp_stop();
    } else if (msg.pSender == this->btn_exp_pause_) {
      exp_pause();
    } else if (msg.pSender == this->btn_exp_resume_) {
      exp_resume();
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
  if (id_timer == kTimerIdSampling) {
    if (is_exp_state_ == 1) {
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
  } else if (id_timer == kTimerIdRefresh) {
    CheckDeviceComConnectedStatus();
    RefreshExpClipTimeControl();
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
  btn_tablayout_->SelectItem(0);

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
  btn_exp_resume_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_resume")));
  layout_exp_pause_ = static_cast<DuiLib::CHorizontalLayoutUI*>(
      paint_manager_ui_->FindControl(_T("layout_exp_pause")));
  layout_exp_resume_ = static_cast<DuiLib::CHorizontalLayoutUI*>(
      paint_manager_ui_->FindControl(_T("layout_exp_resume")));

  // disable the exp start button
  btn_exp_start_->SetEnabled(false);
  btn_exp_stop_->SetEnabled(false);
  btn_exp_pause_->SetEnabled(false);
  layout_exp_resume_->SetVisible(false);

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

  /// @brief update the control from the settings file and set the timer
  /// to refresh the control
  UpdateControlFromSettings();
  paint_manager_ui_->SetTimer(btn_exp_start_, kTimerIdRefresh, 1000);

  work_window_second_page_graph_virtual_wnd_->Bind();
  work_window_second_page_data_virtual_wnd_->Bind();
}

void WorkWindowSecondPage::Unbind() {
  /// @brief remove the virtual window
  this->RemoveVirtualWnd(_T("WorkWindowSecondPageData"));
  work_window_second_page_data_virtual_wnd_->Unbind();
  WorkWindowSecondPageData* data_page =
      reinterpret_cast<WorkWindowSecondPageData*>(
          work_window_second_page_data_notify_pump_.release());
  delete data_page;

  this->RemoveVirtualWnd(_T("WorkWindowSecondPageGraph"));
  work_window_second_page_graph_virtual_wnd_->Unbind();
  WorkWindowSecondPageGraph* graph_page =
      reinterpret_cast<WorkWindowSecondPageGraph*>(
          work_window_second_page_graph_notify_pump_.release());
  delete graph_page;

  /// @brief save the settings from the control
  SaveSettingsFromControl();

  /// @brief kill the timer
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdSampling);
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdRefresh);

  /// @brief release the device com
  if (device_com_sl_ != nullptr) {
    device_com_sl_->RemoveListener(this);
    device_com_sl_ = nullptr;
  }
  if (device_com_ul_ != nullptr) {
    device_com_ul_->RemoveListener(this);
    device_com_ul_ = nullptr;
  }
}

void WorkWindowSecondPage::CheckDeviceComConnectedStatus() {
  if (pWorkWindow_->IsDeviceComInterfaceConnected()) {
    if (is_exp_state_ < 0) {
      is_exp_state_ = 0;
    }
    btn_exp_start_->SetEnabled(true);
    btn_exp_stop_->SetEnabled(true);
    btn_exp_pause_->SetEnabled(true);

    btn_sa_up_->SetEnabled(true);
    btn_sa_down_->SetEnabled(true);
    btn_sa_stop_->SetEnabled(true);
    UpdateUIWithExpStatus(is_exp_state_);
  } else {
    if (is_exp_state_ >= 0) {
      is_exp_state_ = -1;
    }
    btn_exp_start_->SetEnabled(false);
    btn_exp_stop_->SetEnabled(false);
    btn_exp_pause_->SetEnabled(false);

    btn_sa_up_->SetEnabled(false);
    btn_sa_down_->SetEnabled(false);
    btn_sa_stop_->SetEnabled(false);
    UpdateUIWithExpStatus(is_exp_state_);
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

    if (dus->exp_clipping_enable_ == 1) {
      chk_exp_clip_set_->Selected(true);
    } else {
      chk_exp_clip_set_->Selected(false);
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
  if (chk_exp_clip_set_->IsSelected()) {
    dus.exp_clipping_enable_ = 1;
  } else {
    dus.exp_clipping_enable_ = 0;
  }
  anx::device::SaveDeviceUltrasoundSettingsDefaultResource(dus);
}

void WorkWindowSecondPage::UpdateExpClipTimeFromControl() {
  exp_clip_time_duration_ = _ttoi(edit_exp_clip_time_duration_->GetText());
  exp_clip_time_paused_ = _ttoi(edit_exp_clip_time_paused_->GetText());

  // exp_clip_time_duration_ append "S"
  std::string value = ("=");
  value += format_num(exp_clip_time_duration_ * 100);
  value += "S";
  text_exp_clip_time_duration_->SetText(
      anx::common::string2wstring(value).c_str());
  // exp_clip_time_paused_ append "S"
  value = ("=");
  value += format_num(exp_clip_time_paused_ * 100);
  value += "S";
  text_exp_clip_time_paused_->SetText(
      anx::common::string2wstring(value).c_str());
}

void WorkWindowSecondPage::UpdateUIWithExpStatus(int status) {
  if (status == 0) {
    btn_exp_start_->SetEnabled(true);
    layout_exp_resume_->SetVisible(false);
    layout_exp_pause_->SetVisible(true);
    btn_exp_pause_->SetEnabled(false);
    btn_exp_stop_->SetEnabled(false);

    chk_exp_clip_set_->SetEnabled(true);
    edit_exp_clip_time_duration_->SetEnabled(true);
    edit_exp_clip_time_paused_->SetEnabled(true);
    edit_max_cycle_count_->SetEnabled(true);
    edit_max_cycle_power_->SetEnabled(true);
    edit_frequency_fluctuations_range_->SetEnabled(true);

    btn_sa_up_->SetEnabled(true);
    btn_sa_down_->SetEnabled(true);
    btn_sa_stop_->SetEnabled(true);
    btn_aa_setting_->SetEnabled(true);
  } else if (status == 1) {
    btn_exp_start_->SetEnabled(false);
    layout_exp_resume_->SetVisible(false);
    layout_exp_pause_->SetVisible(true);
    btn_exp_pause_->SetEnabled(true);
    btn_exp_stop_->SetEnabled(true);

    chk_exp_clip_set_->SetEnabled(false);
    edit_exp_clip_time_duration_->SetEnabled(false);
    edit_exp_clip_time_paused_->SetEnabled(false);
    edit_max_cycle_count_->SetEnabled(false);
    edit_max_cycle_power_->SetEnabled(false);
    edit_frequency_fluctuations_range_->SetEnabled(false);

    btn_sa_up_->SetEnabled(false);
    btn_sa_down_->SetEnabled(false);
    btn_sa_stop_->SetEnabled(false);
    btn_aa_setting_->SetEnabled(false);
  } else if (status == 2) {
    btn_exp_start_->SetEnabled(false);
    layout_exp_resume_->SetVisible(true);
    layout_exp_pause_->SetVisible(false);
    btn_exp_pause_->SetEnabled(false);
    btn_exp_stop_->SetEnabled(true);

    chk_exp_clip_set_->SetEnabled(true);
    edit_exp_clip_time_duration_->SetEnabled(true);
    edit_exp_clip_time_paused_->SetEnabled(true);
    edit_max_cycle_count_->SetEnabled(true);
    edit_max_cycle_power_->SetEnabled(true);
    edit_frequency_fluctuations_range_->SetEnabled(true);

    btn_sa_up_->SetEnabled(true);
    btn_sa_down_->SetEnabled(true);
    btn_sa_stop_->SetEnabled(true);
    btn_aa_setting_->SetEnabled(true);
  } else {
    // TODO(hhool):
  }
}

int32_t WorkWindowSecondPage::exp_start() {
  if (is_exp_state_) {
    return 0;
  }
  if (device_com_ul_ != nullptr) {
    return 1;
  }
  if (device_com_sl_ != nullptr) {
    return 2;
  }

  UpdateUIWithExpStatus(1);

  UpdateExpClipTimeFromControl();
  // create or get ultrasound device.
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
  is_exp_state_ = 1;
  // TODO(hhool): sample interval
  paint_manager_ui_->SetTimer(btn_exp_start_, 1, kSamplingInterval);
  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_start_;
  msg.sType = kClick;
  work_window_second_page_data_notify_pump_->NotifyPump(msg);
  work_window_second_page_graph_notify_pump_->NotifyPump(msg);
  return 0;
}

void WorkWindowSecondPage::exp_pause() {
  is_exp_state_ = 2;
  UpdateUIWithExpStatus(2);
  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_pause_;
  msg.sType = kClick;
  work_window_second_page_data_notify_pump_->NotifyPump(msg);
  work_window_second_page_graph_notify_pump_->NotifyPump(msg);
}

void WorkWindowSecondPage::exp_resume() {
  is_exp_state_ = 1;

  UpdateUIWithExpStatus(1);

  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_resume_;
  msg.sType = kClick;
  work_window_second_page_data_notify_pump_->NotifyPump(msg);
  work_window_second_page_graph_notify_pump_->NotifyPump(msg);
}

void WorkWindowSecondPage::exp_stop() {
  if (!is_exp_state_) {
    return;
  }

  UpdateUIWithExpStatus(0);

  is_exp_state_ = 0;

  // notify the exp stop
  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_stop_;
  msg.sType = kClick;
  work_window_second_page_data_notify_pump_->NotifyPump(msg);
  work_window_second_page_graph_notify_pump_->NotifyPump(msg);

  // stop the timer
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdSampling);

  // release the device com
  if (device_com_sl_ != nullptr) {
    device_com_sl_->RemoveListener(this);
    device_com_sl_ = nullptr;
  }
  if (device_com_ul_ != nullptr) {
    device_com_ul_->RemoveListener(this);
    device_com_ul_ = nullptr;
  }
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

}  // namespace ui
}  // namespace anx
