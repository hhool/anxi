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

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/db/database_helper.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_com_settings_helper.h"
#include "app/device/device_exp_data_sample_settings.h"
#include "app/device/device_exp_graph_settings.h"
#include "app/device/device_exp_load_static_settings.h"
#include "app/device/device_exp_ultrasound_settings.h"
#include "app/device/ultrasonic/ultra_helper.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_amplitude_calibration_settings.h"
#include "app/ui/dialog_common.h"
#include "app/ui/dialog_static_load_guaranteed_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"
#include "app/ui/work_window_tab_main_page_base.h"
#include "app/ui/work_window_tab_main_second_page_data.h"
#include "app/ui/work_window_tab_main_second_page_graph.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPage, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
namespace {

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2) {
  int nn = n;
  std::ostringstream out;
  out << std::fixed << std::setprecision(nn) << a_value;
  return out.str();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief sampling interval 100ms
/// @note 10Hz sampling frequency
/// @details 1000ms / 100ms = 10Hz sampling frequency
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
      is_exp_state_(-1) {
  WorkWindowSecondPageGraph* graph_page = new WorkWindowSecondPageGraph(
      pWorkWindow, paint_manager_ui, &exp_data_graph_info_);
  work_window_second_page_graph_virtual_wnd_ = graph_page;
  work_window_second_page_graph_notify_pump_.reset(graph_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPageGraph"),
                      work_window_second_page_graph_notify_pump_.get());

  WorkWindowSecondPageData* data_page = new WorkWindowSecondPageData(
      pWorkWindow, paint_manager_ui, &exp_data_list_info_);
  work_window_second_page_data_virtual_wnd_ = data_page;
  work_window_second_page_data_notify_pump_.reset(data_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPageData"),
                      work_window_second_page_data_notify_pump_.get());
}

WorkWindowSecondPage::~WorkWindowSecondPage() {
  paint_manager_ui_->KillTimer(btn_exp_start_, 1);
}

void WorkWindowSecondPage::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender == btn_tab_graph_) {
      btn_tablayout_->SelectItem(0);
    } else if (msg.pSender == btn_tab_data_) {
      btn_tablayout_->SelectItem(1);
    } else if (msg.pSender == this->btn_sa_clear_) {
      OnButtonStaticAircraftClear();
    } else if (msg.pSender == this->btn_sa_setting_) {
      OnButtonStaticAircraftSetting();
    } else if (msg.pSender == this->btn_sa_reset_) {
      OnButtonStaticAircraftReset();
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
      OnButtonStaticAircraftUp();
    } else if (msg.pSender == this->btn_sa_down_) {
      OnButtonStaticAircraftDown();
    } else if (msg.pSender == this->btn_sa_stop_) {
      OnButtonStaticAircraftStop();
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
  } else {
    // TODO(hhool): do nothing
  }
}

void WorkWindowSecondPage::OnTimer(TNotifyUI& msg) {
  uint32_t id_timer = msg.wParam;
  if (id_timer == kTimerIdSampling) {
    if (is_exp_state_ == 1) {
      if (ultra_device_) {
        cur_freq_ = ultra_device_->GetCurrentFreq();
        cur_power_ = ultra_device_->GetCurrentPower();
        if (cur_freq_ < 0 || cur_power_ < 0) {
          LOG_F(LG_ERROR) << "exp_stop: cur_freq:" << cur_freq_
                          << " cur_power:" << cur_power_;
          exp_stop();
          return;
        }
        if (fabs(cur_freq_ - initial_frequency_) >
            dus_.exp_frequency_fluctuations_range_) {
          LOG_F(LG_ERROR) << "exp_stop: frequency fluctuation:" << cur_freq_
                          << " initial frequency:" << initial_frequency_;
          exp_stop();
          return;
        }
        /////////////////////////////////////////////////////////////////////////
        /// get current cycle count and total time  cycle_count / x kHZ
        int64_t exp_max_cycle_count = dus_.exp_max_cycle_count_;
        for (int32_t i = 0; i < dus_.exp_max_cycle_power_; i++) {
          exp_max_cycle_count *= 10;
        }
        int64_t total_des_time_ms = exp_max_cycle_count / 20;
        int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
        int64_t duration =
            current_time_ms - exp_data_graph_info_.exp_start_time_ms_;
        if (duration >= total_des_time_ms) {
          LOG_F(LG_ERROR) << "exp_stop: duration:" << duration
                          << " total_des_time_ms:" << total_des_time_ms;
          exp_stop();
          return;
        }
        this->pWorkWindow_->UpdateArgsArea(-1, cur_freq_);
        int64_t curtime = anx::common::GetCurrentTimeMillis();
        if (dedss_->sampling_start_pos_ > 0) {
          if ((curtime - exp_data_graph_info_.exp_start_time_ms_) >=
                  dedss_->sampling_start_pos_ * 100 &&
              !start_time_pos_has_deal_) {
            LOG_F(LG_INFO) << "exp_stop: curtime:" << curtime
                           << " exp_start_time_ms:"
                           << this->exp_data_graph_info_.exp_start_time_ms_
                           << " sampling_start_pos:"
                           << this->dedss_->sampling_start_pos_;
            if (ultra_device_->StartUltra() < 0) {
              is_exp_state_ = 0;
              return;
            }
            start_time_pos_has_deal_ = true;
          }
        }
        /// exp clipping enabled
        if (this->dus_.exp_clipping_enable_ == 1) {
          if (this->dus_.exp_clip_time_duration_ > 0) {
            int64_t exp_clip_time_duration =
                this->dus_.exp_clip_time_duration_ * 100;
            int64_t exp_clip_time_paused =
                this->dus_.exp_clip_time_paused_ * 100;
            int64_t duration =
                current_time_ms - exp_data_graph_info_.exp_start_time_ms_;
            int64_t duration_total =
                exp_clip_time_duration + exp_clip_time_paused;
            int64_t time = duration % duration_total;
            if (time >= exp_clip_time_duration &&
                state_ultrasound_exp_clip_ == 1) {
              // pause ultrasound
              LOG_F(LG_INFO) << "pause ultrasound";
              ultra_device_->StopUltra();
              state_ultrasound_exp_clip_ = 2;
            } else if (time < exp_clip_time_duration &&
                       state_ultrasound_exp_clip_ == 2) {
              // resume ultrasound
              LOG_F(LG_INFO) << "resume ultrasound";
              if (ultra_device_->StartUltra() < 0) {
                is_exp_state_ = 0;
                return;
              }
              state_ultrasound_exp_clip_ = 1;
            }
          }
        }
      }
    }
  } else if (id_timer == kTimerIdRefresh) {
    CheckDeviceComConnectedStatus();
    RefreshExpClipTimeControl();
  } else {
    std::cout << "timer id:" << id_timer << std::endl;
  }
}

void WorkWindowSecondPage::OnValueChanged(TNotifyUI& msg) {
  if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
    if (msg.pSender->GetName() == _T("work_args_area")) {
      ENMsgStruct* enmsg = reinterpret_cast<ENMsgStruct*>(msg.wParam);
      if (enmsg == nullptr) {
        return;
      }
      if (enmsg->type_ == enmsg_type_stload_value_cur) {
        anx::device::stload::STResult* st_result =
            reinterpret_cast<anx::device::stload::STResult*>(enmsg->ptr_);
        if ((st_result->status_ & DSP_CMDEND) == DSP_CMDEND) {
          LOG_F(LG_INFO) << "static load aircraft stop";
          OnButtonStaticAircraftStop();
        }
        st_load_result_ = *st_result;
      } else if (enmsg->type_ == enmsg_type_exp_stress_amp) {
        anx::esolution::SolutionDesign* design =
            reinterpret_cast<anx::esolution::SolutionDesign*>(enmsg->ptr_);
        if (design == nullptr) {
          return;
        }
        double exp_max_stress = design->base_param_->f_max_stress_MPa_;
        double exp_amplitude =
            reinterpret_cast<anx::esolution::ExpDesignResult0*>(
                design->result_.get())
                ->f_eamplitude_;
        double exp_statc_load_mpa = 0.0f;
        if (design->result_->solution_type_ ==
            anx::esolution::kSolutionName_Stresses_Adjustable) {
          exp_statc_load_mpa =
              reinterpret_cast<
                  anx::esolution::ExpDesignResultStressesAdjustable*>(
                  design->result_.get())
                  ->f_static_load_MPa_;
        } else if (design->result_->solution_type_ ==
                   anx::esolution::kSolutionName_Th3point_Bending) {
          exp_statc_load_mpa =
              reinterpret_cast<anx::esolution::ExpDesignResultTh3pointBending*>(
                  design->result_.get())
                  ->f_static_load_MPa_;
        }
        LOG_F(LG_INFO) << "exp_amplitude:" << exp_amplitude
                       << " exp_statc_load_mpa:" << exp_statc_load_mpa
                       << " exp_max_stress:" << exp_max_stress;
        exp_max_stress_MPa_ = exp_max_stress;
        exp_amplitude_ = exp_amplitude;
        exp_statc_load_mpa_ = exp_statc_load_mpa;
        st_load_result_.load_ = exp_statc_load_mpa_;
        if (work_window_second_page_graph_notify_pump_ != nullptr) {
          work_window_second_page_graph_notify_pump_->NotifyPump(msg);
        }
        if (work_window_second_page_data_notify_pump_ != nullptr) {
          work_window_second_page_data_notify_pump_->NotifyPump(msg);
        }
      }
    } else if (msg.pSender->GetName() == _T("args_area_value_amplitude")) {
      if (msg.wParam == PBT_APMQUERYSUSPEND) {
        exp_stop();
      } else {
        // do nothing
      }
    } else {
      // do nothing
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

  // create or get ultrasound device.
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  ultra_device_ =
      reinterpret_cast<anx::device::UltraDevice*>(device_com_ul->Device());
  work_window_second_page_data_virtual_wnd_->Bind();
  work_window_second_page_graph_virtual_wnd_->Bind();

  // disable the exp start button
  UpdateUIButton();
}

void WorkWindowSecondPage::Unbind() {
  /// @brief remove the virtual window

  this->RemoveVirtualWnd(_T("WorkWindowSecondPageGraph"));
  work_window_second_page_graph_virtual_wnd_->Unbind();
  WorkWindowSecondPageGraph* graph_page =
      reinterpret_cast<WorkWindowSecondPageGraph*>(
          work_window_second_page_graph_notify_pump_.release());
  delete graph_page;

  this->RemoveVirtualWnd(_T("WorkWindowSecondPageData"));
  work_window_second_page_data_virtual_wnd_->Unbind();
  WorkWindowSecondPageData* data_page =
      reinterpret_cast<WorkWindowSecondPageData*>(
          work_window_second_page_data_notify_pump_.release());
  delete data_page;

  /// @brief save the settings from the control
  SaveSettingsFromControl();

  /// @brief kill the timer
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdSampling);
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdRefresh);

  if (ultra_device_ != nullptr) {
    ultra_device_->GetPortDevice()->RemoveListener(this);
    ultra_device_ = nullptr;
  }
}

void WorkWindowSecondPage::CheckDeviceComConnectedStatus() {
  /// exp_start, exp_stop, exp_pause, exp_resume button state
  /// if the device com interface is connected then enable the exp_start button
  /// else disable the exp_start button and exp_stop button and exp_pause button
  if (pWorkWindow_->IsULDeviceComInterfaceConnected()) {
    if (is_exp_state_ < 0) {
      is_exp_state_ = 0;
    }
    UpdateUIButton();
  } else {
    if (is_exp_state_ >= 0) {
      is_exp_state_ = -1;
    }
    UpdateUIButton();
  }
}

void WorkWindowSecondPage::RefreshExpClipTimeControl() {
  int32_t exp_clip_time_duration =
      _ttoi(edit_exp_clip_time_duration_->GetText());
  if (dus_.exp_clip_time_duration_ != exp_clip_time_duration) {
    dus_.exp_clip_time_duration_ = exp_clip_time_duration;
    std::string value = ("=");
    value += to_string_with_precision((exp_clip_time_duration * 100) / 1000.0f);
    value += "S";
    text_exp_clip_time_duration_->SetText(
        anx::common::string2wstring(value).c_str());
  }

  int32_t exp_clip_time_paused = _ttoi(edit_exp_clip_time_paused_->GetText());
  if (dus_.exp_clip_time_paused_ != exp_clip_time_paused) {
    dus_.exp_clip_time_paused_ = exp_clip_time_paused;
    std::string value = ("=");
    value += to_string_with_precision((exp_clip_time_paused * 100) / 1000.0f);
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
  dus_.exp_clip_time_duration_ = _ttoi(edit_exp_clip_time_duration_->GetText());
  dus_.exp_clip_time_paused_ = _ttoi(edit_exp_clip_time_paused_->GetText());
  dus_.exp_max_cycle_count_ = _ttoi(edit_max_cycle_count_->GetText());
  dus_.exp_max_cycle_power_ = _ttoi(edit_max_cycle_power_->GetText());
  dus_.exp_frequency_fluctuations_range_ =
      _ttoi(edit_frequency_fluctuations_range_->GetText());
  if (chk_exp_clip_set_->IsSelected()) {
    dus_.exp_clipping_enable_ = 1;
  } else {
    dus_.exp_clipping_enable_ = 0;
  }
  anx::device::SaveDeviceUltrasoundSettingsDefaultResource(dus_);
}

void WorkWindowSecondPage::UpdateExpClipTimeFromControl() {
  int64_t exp_clip_time_duration =
      _ttoi(edit_exp_clip_time_duration_->GetText());
  int64_t exp_clip_time_paused = _ttoi(edit_exp_clip_time_paused_->GetText());

  // exp_clip_time_duration append "S"
  std::string value = ("=");
  value += to_string_with_precision((exp_clip_time_duration * 100) / 1000.0f);
  value += "S";
  text_exp_clip_time_duration_->SetText(
      anx::common::string2wstring(value).c_str());
  // exp_clip_time_paused append "S"
  value = ("=");
  value += to_string_with_precision((exp_clip_time_paused * 100) / 1000.0f);
  value += "S";
  text_exp_clip_time_paused_->SetText(
      anx::common::string2wstring(value).c_str());
}

void WorkWindowSecondPage::UpdateUIButton() {
  // check the device com connected status
  // enable btn sa up and down
  // if the device com interface is connected and the exp is not running
  // and the static load is not running then enable the sa up and down button
  // else disable the sa up and down button
  // sa_stop button state is the opposite of sa_up and sa_down button
  // if sa_up and sa_down button is enabled then sa_stop button is disabled
  // if sa_up and sa_down button is disabled then sa_stop button is enabled
  bool is_st_device_com_interface_connected =
      pWorkWindow_->IsSLDeviceComInterfaceConnected();
  bool sa_up_down_enable = true;
  if (!is_st_device_com_interface_connected) {
    sa_up_down_enable = false;
  } else {
    if (is_exp_state_ == 1) {
      sa_up_down_enable = false;
    } else if (st_load_is_running_) {
      sa_up_down_enable = false;
    } else {
      // sa_up_down_enable = true;
    }
  }
  bool sa_stop_enable = false;
  if (!is_st_device_com_interface_connected) {
    sa_stop_enable = false;
  } else {
    if (st_load_is_running_) {
      sa_stop_enable = true;
    } else {
      // sa_stop_enable = false;
    }
  }
  bool sa_clear_enable = true;
  if (!is_st_device_com_interface_connected) {
    sa_clear_enable = false;
  } else {
    sa_clear_enable = true;
  }
  btn_sa_up_->SetEnabled(sa_up_down_enable);
  btn_sa_down_->SetEnabled(sa_up_down_enable);
  btn_sa_stop_->SetEnabled(sa_stop_enable);
  btn_sa_clear_->SetEnabled(sa_clear_enable);

  bool is_ul_device_com_interface_connected =
      pWorkWindow_->IsULDeviceComInterfaceConnected();
  // check the device com connected status
  if (!is_ul_device_com_interface_connected) {
    btn_sa_reset_->SetEnabled(false);
    btn_exp_start_->SetEnabled(false);
    btn_exp_pause_->SetEnabled(false);
    btn_exp_stop_->SetEnabled(false);
    layout_exp_resume_->SetVisible(false);
    layout_exp_pause_->SetVisible(true);
    chk_exp_clip_set_->SetEnabled(false);
    edit_exp_clip_time_duration_->SetEnabled(false);
    edit_exp_clip_time_paused_->SetEnabled(false);
    edit_max_cycle_count_->SetEnabled(false);
    edit_max_cycle_power_->SetEnabled(false);
    edit_frequency_fluctuations_range_->SetEnabled(false);
  } else {
    if (is_exp_state_ == 0) {
      btn_sa_reset_->SetEnabled(true);
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
    } else if (is_exp_state_ == 1) {
      btn_sa_reset_->SetEnabled(false);
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
    } else if (is_exp_state_ == 2) {
      btn_sa_reset_->SetEnabled(true);
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
    } else {
      // TODO(hhool):
    }
  }
}

int32_t WorkWindowSecondPage::exp_start() {
  if (is_exp_state_) {
    return 0;
  }

  UpdateUIButton();

  UpdateExpClipTimeFromControl();

  SaveSettingsFromControl();

  /// @brief reset the database exp_data table.
  /// @note the table name is exp_data, delete exp_data table and create a new
  /// one.
  anx::db::helper::DropDataTable(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataGraph);
  anx::db::helper::DropDataTable(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataList);

  std::string db_filepathname;
  anx::db::helper::DefaultDatabasePathname(&db_filepathname);
  auto db = anx::db::DatabaseFactory::Instance()->CreateOrGetDatabase(
      db_filepathname);
  db->Execute(anx::db::helper::sql::kCreateTableExpDataGraphSqlFormat);
  db->Execute(anx::db::helper::sql::kCreateTableExpDataListSqlFormat);

  /// @brief get the exp data sample settings and set the exp start time
  /// and exp sample interval
  exp_data_graph_info_.exp_data_table_no_ = 0;
  exp_data_graph_info_.exp_time_interval_num_ = 0;
  exp_data_graph_info_.exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_data_graph_info_.exp_sample_interval_ms_ = 2000;

  dedss_ =
      std::move(anx::device::LoadDeviceExpDataSampleSettingsDefaultResource());
  exp_data_list_info_.exp_data_table_no_ = 0;
  exp_data_list_info_.exp_time_interval_num_ = 0;
  exp_data_list_info_.exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_data_list_info_.exp_sample_interval_ms_ =
      dedss_->sampling_interval_ * 100;

  // set the ultrasound on state
  int32_t ret = 0;
  if (this->dus_.exp_clipping_enable_) {
    ret = ultra_device_->SetWedingTime(this->dus_.exp_clip_time_duration_);
  } else {
    ret = ultra_device_->SetWedingTime(0);
  }
  if (ret < 0) {
    is_exp_state_ = 0;
    return -1;
  }
  cur_freq_ = initial_frequency_ = ultra_device_->GetCurrentFreq();
  cur_power_ = initial_power_ = ultra_device_->GetCurrentPower();
  if (initial_frequency_ < 0 || initial_power_ < 0) {
    is_exp_state_ = 0;
    return -1;
  }
  if (dedss_->sampling_start_pos_ == 0) {
    if (ultra_device_->StartUltra() < 0) {
      is_exp_state_ = 0;
      return -1;
    }
  }
  start_time_pos_has_deal_ = false;
  paint_manager_ui_->SetTimer(btn_exp_start_, kTimerIdSampling,
                              kSamplingInterval);

  state_ultrasound_exp_clip_ = 1;

  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_start_;
  msg.sType = kClick;
  work_window_second_page_data_notify_pump_->NotifyPump(msg);
  work_window_second_page_graph_notify_pump_->NotifyPump(msg);

  is_exp_state_ = 1;
  return 0;
}

void WorkWindowSecondPage::exp_pause() {
  is_exp_state_ = 2;
  UpdateUIButton();
  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_pause_;
  msg.sType = kClick;
  work_window_second_page_data_notify_pump_->NotifyPump(msg);
  work_window_second_page_graph_notify_pump_->NotifyPump(msg);
}

void WorkWindowSecondPage::exp_resume() {
  is_exp_state_ = 1;

  UpdateUIButton();

  UpdateExpClipTimeFromControl();

  SaveSettingsFromControl();

  dedss_ =
      std::move(anx::device::LoadDeviceExpDataSampleSettingsDefaultResource());
  exp_data_list_info_.exp_sample_interval_ms_ =
      dedss_->sampling_interval_ * 100;

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
  is_exp_state_ = 0;
  exp_data_pre_duration_exponential_ = 0;
  UpdateUIButton();

  // notify the exp stop
  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_stop_;
  msg.sType = kClick;
  work_window_second_page_data_notify_pump_->NotifyPump(msg);
  work_window_second_page_graph_notify_pump_->NotifyPump(msg);

  // stop the timer
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdSampling);
}

void WorkWindowSecondPage::OnButtonStaticAircraftClear() {
  // clear the data
  if (is_exp_state_) {
    return;
  }
  LOG_F(LG_INFO) << "clear the data";
  if (anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_ext1) {
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_ext1();
  }
  if (anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_posi) {
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_posi();
  }
  if (anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_load) {
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_load();
  }
  if (anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_time) {
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.tare_time();
  }
}

void WorkWindowSecondPage::OnButtonStaticAircraftSetting() {
  DialogStaticLoadGuaranteedSettings* dialog_static_load_guaranteed_settings =
      new DialogStaticLoadGuaranteedSettings();
  dialog_static_load_guaranteed_settings->Create(
      *pWorkWindow_, _T("dialog_static_load_guaranteed_settings"),
      UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  dialog_static_load_guaranteed_settings->CenterWindow();
  dialog_static_load_guaranteed_settings->ShowModal();
}

void WorkWindowSecondPage::OnButtonStaticAircraftReset() {
  // reset the data
  this->pWorkWindow_->ClearArgsFreqNum();
  // drop the exp_data table
  anx::db::helper::DropDataTable(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataGraph);
  anx::db::helper::DropDataTable(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataList);

  // create the exp_data_graph table
  std::string db_filepathname;
  anx::db::helper::DefaultDatabasePathname(&db_filepathname);
  auto db = anx::db::DatabaseFactory::Instance()->CreateOrGetDatabase(
      db_filepathname);
  db->Execute(anx::db::helper::sql::kCreateTableExpDataGraphSqlFormat);
  db->Execute(anx::db::helper::sql::kCreateTableExpDataListSqlFormat);

  /// @brief get the exp data sample settings and set the exp start time
  /// and exp sample interval
  exp_data_graph_info_.exp_data_table_no_ = 0;
  exp_data_graph_info_.exp_time_interval_num_ = 0;
  exp_data_graph_info_.exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_data_graph_info_.exp_sample_interval_ms_ = 2000;

  dedss_ = anx::device::LoadDeviceExpDataSampleSettingsDefaultResource();
  exp_data_list_info_.exp_data_table_no_ = 0;
  exp_data_list_info_.exp_time_interval_num_ = 0;
  exp_data_list_info_.exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_data_list_info_.exp_sample_interval_ms_ =
      dedss_->sampling_interval_ * 100;
  // clear graph data
  WorkWindowSecondPageGraph* graph_page =
      reinterpret_cast<WorkWindowSecondPageGraph*>(
          work_window_second_page_graph_notify_pump_.get());
  graph_page->ClearGraphData();
  // clear the exp data
  WorkWindowSecondPageData* data_page =
      reinterpret_cast<WorkWindowSecondPageData*>(
          work_window_second_page_data_notify_pump_.get());
  data_page->ClearExpData();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Static aircraft releated button
void WorkWindowSecondPage::OnButtonStaticAircraftUp() {
  if (st_load_is_running_) {
    return;
  }
  float speed = 2.0f / 60.0f;
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("静载机配置文件加载失败"),
               _T("错误"), MB_OK);
    return;
  }
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(1);
  speed = lss->speed_ / 60.0f;
  float end_value = lss->retention_ * 1.0f;
  /// RUN the static load, the direction is up and the speed is 2.0f / 60.0f
  bool bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_200(
          CTRL_LOAD, END_LOAD, speed, end_value, 1, true, DIR_NO, 0, 1, 0)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("carry_200_error"),
               _T("carry_200"), MB_OK);
  }

  btn_sa_up_->SetEnabled(false);
  btn_sa_down_->SetEnabled(false);
  btn_sa_stop_->SetEnabled(true);
  st_load_is_running_ = true;
}

void WorkWindowSecondPage::OnButtonStaticAircraftDown() {
  bool isAE = false;
  float speed = 2.0f / 60.0f;
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("静载机配置文件加载失败"),
               _T("错误"), MB_OK);
    return;
  }
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(0);
  speed = lss->speed_ / 60.0f;
  float end_value = lss->retention_ * 1.0f;

  /// RUN the static load, the direction is up and the speed is 2.0f / 60.0f
  bool bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_200(
          CTRL_LOAD, END_LOAD, speed, end_value, 1, true, DIR_NO, 0, 1, 0)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("carry_200_error"),
               _T("carry_200"), MB_OK);
  }

  btn_sa_up_->SetEnabled(false);
  btn_sa_down_->SetEnabled(false);
  btn_sa_stop_->SetEnabled(true);
  st_load_is_running_ = true;
}

void WorkWindowSecondPage::OnButtonStaticAircraftStop() {
  // anx::device::stload::STLoadHelper::st_load_loader_.st_api_.end_read();
  LOG_F(LG_INFO) << "stop the static load";
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.stop_run();
  btn_sa_up_->SetEnabled(true);
  btn_sa_down_->SetEnabled(true);
  st_load_is_running_ = false;
}

void WorkWindowSecondPage::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool): process data
  if (is_exp_state_ > 0) {
    double KHz = cur_freq_ * 1.0f;
    // TODO(hhool): random KHz, um, MPa
    double um = exp_amplitude_;
    double MPa = std::fabsl(exp_statc_load_mpa_);
    double date = anx::common::GetCurrrentDateTime();
    exp_data_graph_info_.amp_freq_ = KHz;
    exp_data_graph_info_.amp_um_ = um;
    exp_data_graph_info_.stress_value_ = MPa;

    exp_data_list_info_.amp_freq_ = KHz;
    exp_data_list_info_.amp_um_ = um;
    exp_data_list_info_.stress_value_ = exp_max_stress_MPa_;

    this->ProcessDataGraph();
    if (ultra_device_->IsUltraStarted()) {
      this->ProcessDataList();
    }
  }
}

void WorkWindowSecondPage::ProcessDataGraph() {
  int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
  int64_t time_diff = current_time_ms - exp_data_graph_info_.exp_start_time_ms_;
  int64_t time_interval_num =
      time_diff / exp_data_graph_info_.exp_sample_interval_ms_;
  if (time_interval_num > exp_data_graph_info_.exp_time_interval_num_) {
    exp_data_graph_info_.exp_time_interval_num_ = time_interval_num;
    exp_data_graph_info_.exp_data_table_no_++;
    // update the data to the database table amp, stress, um
    int64_t cycle_count =
        exp_data_graph_info_.exp_data_table_no_ * cur_freq_ * 2;
    double date = anx::common::GetCurrrentDateTime();
    // save to database
    // format cycle_count, KHz, MPa, um to the sql string and insert to the
    // database
    std::string sql_str = ("INSERT INTO ");
    sql_str.append(anx::db::helper::kTableExpDataGraph);
    sql_str.append((" (cycle, KHz, MPa, um, date) VALUES ("));
    sql_str.append(std::to_string(cycle_count));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_graph_info_.amp_freq_));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_graph_info_.stress_value_));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_graph_info_.amp_um_));
    sql_str.append(", ");
    sql_str.append(std::to_string(date));
    sql_str.append(");");
    anx::db::helper::InsertDataTable(anx::db::helper::kDefaultDatabasePathname,
                                     anx::db::helper::kTableExpDataGraph,
                                     sql_str);
  }
}

void WorkWindowSecondPage::ProcessDataList() {
  int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
  int64_t time_diff = current_time_ms - exp_data_list_info_.exp_start_time_ms_;
  assert(dedss_ != nullptr);
  if (dedss_->sample_mode_ == 1) {
    if (time_diff < (dedss_->sampling_start_pos_ * 100)) {
      return;
    }
    time_diff -= dedss_->sampling_start_pos_ * 100;
  }
  int64_t time_interval_num;
  if (dedss_->sample_mode_ == 0) {
    time_interval_num = time_diff / 100;
  } else {
    time_interval_num = time_diff / (dedss_->sampling_interval_ * 100);
  }
  if (dedss_->sampling_end_pos_ > dedss_->sampling_start_pos_) {
    int32_t sampling_start_end_diff =
        dedss_->sampling_end_pos_ - dedss_->sampling_start_pos_;
    int64_t sampling_start_end_diff_ms = sampling_start_end_diff * 100;
    if (time_diff > sampling_start_end_diff_ms) {
      LOG_F(LG_INFO) << "exp data list stop";
      return;
    }
  }

  this->pWorkWindow_->UpdateArgsArea(
      time_diff * static_cast<int32_t>(exp_data_list_info_.amp_freq_ / 1000.f));

  // check the time interval num
  if (dedss_->sample_mode_ == 0) {
    if (exp_data_pre_duration_exponential_ == 0) {
      if (time_interval_num > 0) {
        exp_data_pre_duration_exponential_ = time_interval_num;
      } else {
        return;
      }
    } else if ((time_interval_num % exp_data_pre_duration_exponential_) != 0) {
      return;
    } else if (time_interval_num / exp_data_pre_duration_exponential_ < 10) {
      return;
    } else {
      exp_data_pre_duration_exponential_ *= 10;
    }
  } else {
    // do nothing
    // linear dedss_->sample_mode_ == 1
  }
  if (time_interval_num > exp_data_list_info_.exp_time_interval_num_) {
    exp_data_list_info_.exp_time_interval_num_ = time_interval_num;
    exp_data_list_info_.exp_data_table_no_++;
    // update the data to the database table amp, stress, um
    int64_t cycle_count =
        exp_data_list_info_.exp_data_table_no_ * cur_freq_ *
        static_cast<int32_t>((dedss_->sampling_interval_ * 100) / 1000.f);
    if (dedss_->sample_mode_ == 0) {
      int multiplier = 1;
      for (int i = 1; i < exp_data_list_info_.exp_data_table_no_; i++) {
        multiplier *= 10;
      }
      cycle_count = static_cast<int64_t>(multiplier *
                                         exp_data_list_info_.amp_freq_ / 10.f);
    }
    double date = anx::common::GetCurrrentDateTime();
    // 1. save to database
    // format cycle_count, KHz, MPa, um to the sql string and insert to the
    // database
    std::string sql_str = ("INSERT INTO ");
    sql_str.append(anx::db::helper::kTableExpDataList);
    sql_str.append((" (cycle, KHz, MPa, um, date) VALUES ("));
    sql_str.append(std::to_string(cycle_count));
    sql_str.append(", ");
    sql_str.append(to_string_with_precision(exp_data_list_info_.amp_freq_, 0));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_list_info_.stress_value_));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_list_info_.amp_um_));
    sql_str.append(", ");
    sql_str.append(std::to_string(date));
    sql_str.append(");");
    anx::db::helper::InsertDataTable(anx::db::helper::kDefaultDatabasePathname,
                                     anx::db::helper::kTableExpDataList,
                                     sql_str);
  }
}

}  // namespace ui
}  // namespace anx
