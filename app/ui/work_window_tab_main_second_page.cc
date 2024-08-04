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
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
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
    } else if (msg.pSender == this->btn_exp_start_) {
      exp_start();
    } else if (msg.pSender == this->btn_exp_stop_) {
      exp_stop();
    } else if (msg.pSender == this->btn_exp_pause_) {
      exp_pause();
    } else if (msg.pSender == this->btn_sa_up_) {
    } else if (msg.pSender == this->btn_sa_down_) {
    } else if (msg.pSender == this->btn_sa_stop_) {
    } else if (msg.pSender == this->btn_aa_setting_) {
      DialogAmplitudeCalibrationSettings*
          dialog_amplitude_calibration_settings =
              new DialogAmplitudeCalibrationSettings();
      dialog_amplitude_calibration_settings->Create(
          *pWorkWindow_, _T("dialog_amplitude_calibration_settings"),
          UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
      dialog_amplitude_calibration_settings->CenterWindow();
      dialog_amplitude_calibration_settings->ShowModal();
    }
  } else if (msg.sType == _T("selectchanged")) {
  }
}

void WorkWindowSecondPage::OnTimer(TNotifyUI& msg) {
  if (msg.sType == _T("timer")) {
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
      }
    } else if (id_timer == 2) {
      CheckDeviceComConnectedStatus();
    } else {
      std::cout << "";
    }
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

  // disable the static aircraft button
  btn_sa_setting_->SetEnabled(false);

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
  btn_aa_setting_->SetEnabled(false);

  /// @brief exp clip set checkbox
  chk_exp_clip_set_ = static_cast<DuiLib::CCheckBoxUI*>(
      paint_manager_ui_->FindControl(_T("exp_clip_set")));
  edit_exp_clip_time_duration_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("exp_clip_time_duration")));
  edit_exp_clip_time_paused_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("exp_clip_time_paused")));

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

  paint_manager_ui_->SetTimer(btn_exp_start_, 2, 1000);
}

void WorkWindowSecondPage::Unbind() {
  paint_manager_ui_->KillTimer(btn_exp_start_, 1);
  device_com_sl_.reset();
  device_com_ul_.reset();
  paint_manager_ui_->KillTimer(btn_exp_start_, 2);
}

void WorkWindowSecondPage::CheckDeviceComConnectedStatus() {
  if (pWorkWindow_->IsDeviceComInterfaceConnected()) {
    btn_sa_setting_->SetEnabled(true);
    btn_exp_start_->SetEnabled(true);
    btn_exp_stop_->SetEnabled(true);
    btn_exp_pause_->SetEnabled(true);
    btn_sa_up_->SetEnabled(true);
    btn_sa_down_->SetEnabled(true);
    btn_sa_stop_->SetEnabled(true);
    btn_aa_setting_->SetEnabled(true);
  } else {
    btn_sa_setting_->SetEnabled(false);
    btn_exp_start_->SetEnabled(false);
    btn_exp_stop_->SetEnabled(false);
    btn_exp_pause_->SetEnabled(false);
    btn_sa_up_->SetEnabled(false);
    btn_sa_down_->SetEnabled(false);
    btn_sa_stop_->SetEnabled(false);
    btn_aa_setting_->SetEnabled(false);
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
          anx::device::kDeviceCom_Ultrasound);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad);
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
  exp_clip_time_duration_ = _ttoi(edit_exp_clip_time_duration_->GetText());
  exp_clip_time_paused_ = _ttoi(edit_exp_clip_time_paused_->GetText());
  int32_t exp_power = _ttoi(edit_max_cycle_power_->GetText());
  exp_cycle_count_ = _ttoi(edit_max_cycle_count_->GetText()) * exp_power;
  exp_freq_fluctuations_range_ =
      _ttoi(edit_frequency_fluctuations_range_->GetText());

  sample_start_pos_ = _ttoi(edit_sample_start_pos_->GetText());
  sample_end_pos_ = _ttoi(edit_sample_end_pos_->GetText());
  sample_time_interval_ = _ttoi(edit_sample_interval_->GetText());
}

}  // namespace ui
}  // namespace anx
