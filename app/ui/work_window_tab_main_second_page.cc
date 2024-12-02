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
#undef max
#undef min
#include <limits>  // std::numeric_limits
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "app/common/logger.h"
#include "app/common/num_string_convert.hpp"
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
#include "app/ui/ui_num_string_convert.hpp"
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

/// @brief exp clip max count 10^18
const int64_t kExpClipMaxCount = 1000000000000000000LL;
}  // namespace

WorkWindowSecondPage::WorkWindowSecondPage(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow),
      paint_manager_ui_(paint_manager_ui),
      is_exp_state_(kExpStateUnvalid) {
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
    } else if (msg.pSender == this->btn_sa_keep_load_) {
      OnButtonStaticAircraftKeepLoad();
    } else if (msg.pSender == this->btn_exp_reset_) {
      OnButtonExpReset();
    } else if (msg.pSender == this->btn_exp_start_) {
      OnExpStart();
    } else if (msg.pSender == this->btn_exp_stop_) {
      int32_t result = anx::ui::DialogCommon::ShowDialog(
          *pWorkWindow_, "提示", "是否停止试验",
          anx::ui::DialogCommon::kDialogCommonStyleOkCancel);
      if (result == anx::ui::DialogCommon::DC_Cancel) {
        return;
      }
      OnExpStop();
    } else if (msg.pSender == this->btn_exp_pause_) {
      int32_t result = anx::ui::DialogCommon::ShowDialog(
          *pWorkWindow_, "提示", "是否暂停试验",
          anx::ui::DialogCommon::kDialogCommonStyleOkCancel);
      if (result == anx::ui::DialogCommon::DC_Cancel) {
        return;
      }
      OnExpPause();
    } else if (msg.pSender == this->btn_exp_resume_) {
      OnExpResume();
    } else if (msg.pSender == this->btn_sa_up_) {
      /// update lss_;
      lss_ =
          std::move(anx::device::LoadDeviceLoadStaticSettingsDefaultResource());
      OnButtonStaticAircraftUp();
    } else if (msg.pSender == this->btn_sa_down_) {
      /// update lss_;
      lss_ =
          std::move(anx::device::LoadDeviceLoadStaticSettingsDefaultResource());
      OnButtonStaticAircraftDown();
    } else if (msg.pSender == this->btn_sa_stop_) {
      /// update lss_;
      lss_ =
          std::move(anx::device::LoadDeviceLoadStaticSettingsDefaultResource());
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
    if (is_exp_state_ != kExpStateUnvalid) {
      if (ultra_device_) {
        cur_freq_ = ultra_device_->GetCurrentFreq();
        cur_power_ = ultra_device_->GetCurrentPower();
        if (exp_pause_stop_reason_ == kExpPauseStopReasonNone &&
            (cur_freq_ < 0 || cur_power_ < 0)) {
          LOG_F(LG_ERROR) << "exp_stop: cur_freq:" << cur_freq_
                          << " cur_power:" << cur_power_;
          exp_pause_stop_reason_ = kExpPauseStopReasonUnkown;
          exp_stop();
          /// @note msg box with unkown reason
          anx::ui::DialogCommon::ShowDialog(
              *pWorkWindow_, "提示", "设备未知错误,请检查硬件连接",
              anx::ui::DialogCommon::kDialogCommonStyleOk);
          return;
        }
        if (exp_pause_stop_reason_ == kExpPauseStopReasonNone &&
            (fabs(cur_freq_ - initial_frequency_) >
             dus_.exp_frequency_fluctuations_range_)) {
          LOG_F(LG_ERROR) << "exp_stop: frequency fluctuation:" << cur_freq_
                          << " initial frequency:" << initial_frequency_;
          exp_pause_stop_reason_ = kExpPauseStopReasonOutFrequecyRange;
          exp_pause();
          /// @note msg box with frequency fluctuation reason
          anx::ui::DialogCommon::ShowDialog(
              *pWorkWindow_, "提示", "超出频率波动范围",
              anx::ui::DialogCommon::kDialogCommonStyleOk);
          return;
        }
        /////////////////////////////////////////////////////////////////////////
        /// get current cycle count and total time  cycle_count / x kHZ
        double f_exp_max_cycle_count = dus_.exp_max_cycle_count_;
        for (int32_t i = 0; i < dus_.exp_max_cycle_power_; i++) {
          f_exp_max_cycle_count *= 10;
        }
        int64_t exp_max_cycle_count =
            static_cast<int64_t>(f_exp_max_cycle_count);
        if (cur_total_cycle_count_ >= exp_max_cycle_count) {
          LOG_F(LG_WARN) << "exp_stop: cur_cycle_count:"
                         << cur_total_cycle_count_
                         << " exp_max_cycle_count:" << exp_max_cycle_count;
          return;
        }
        /////////////////////////////////////////////////////////////////////////
        /// get current cycle count and total time  cycle_count / x kHZ
        int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
        this->pWorkWindow_->UpdateArgsArea(-1, cur_freq_);
        if (dedss_->sampling_start_pos_ > 0) {
          if ((current_time_ms - exp_data_graph_info_.exp_start_time_ms_) >=
                  dedss_->sampling_start_pos_ * 100 &&
              !start_time_pos_has_deal_) {
            LOG_F(LG_INFO) << "exp_stop: current_time_ms:" << current_time_ms
                           << " exp_start_time_ms:"
                           << this->exp_data_graph_info_.exp_start_time_ms_
                           << " sampling_start_pos:"
                           << this->dedss_->sampling_start_pos_;
            if (ultra_device_->StartUltra() < 0) {
              /// TODO(hhool): msg box with unkown reason.
              is_exp_state_ = kExpStateStop;
              return;
            }
            exp_data_graph_info_.exp_start_time_ms_ = current_time_ms;
            start_time_pos_has_deal_ = true;
          }
        }
        /// @note process intermittent exp clipping enabled
        if (this->dus_.exp_clipping_enable_ == 1 &&
            is_exp_state_ == kExpStateStart) {
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
            if (user_exp_state_ == kExpStatePause) {
              LOG_F(LG_INFO) << "exp_pause: user_exp_state_:" << user_exp_state_
                             << " time:" << time;
              return;
            }
            if (time >= exp_clip_time_duration &&
                state_ultrasound_exp_clip_ == 1) {
              // pause ultrasound
              LOG_F(LG_INFO) << "pause ultrasound";
              ultra_device_->StopUltra();
              pre_total_data_table_no_ = exp_data_list_info_.exp_data_table_no_;
              pre_total_cycle_count_ = cur_total_cycle_count_;
              LOG_F(LG_INFO)
                  << "cur_total_cycle_count_:" << cur_total_cycle_count_ << " "
                  << "pre_total_cycle_count_:" << pre_total_cycle_count_ << " "
                  << "pre_total_data_table_no_:" << pre_total_data_table_no_;
              state_ultrasound_exp_clip_ = 2;
            } else if (time < exp_clip_time_duration &&
                       state_ultrasound_exp_clip_ == 2) {
              // resume ultrasound
              pre_clip_paused_ms_ = 0;
              exp_data_list_info_.exp_start_time_ms_ =
                  anx::common::GetCurrentTimeMillis();
              exp_data_list_info_.exp_time_interval_num_ = 0;
              exp_data_list_info_.exp_freq_total_count_ = 0;
              dedss_ = std::move(
                  anx::device::
                      LoadDeviceExpDataSampleSettingsDefaultResource());
              exp_data_list_info_.exp_sample_interval_ms_ =
                  dedss_->sampling_interval_ * 100;
              pre_exp_start_time_ms_ = exp_data_list_info_.exp_start_time_ms_;
              LOG_F(LG_INFO) << "resume ultrasound: pre_exp_start_time_ms_："
                             << pre_exp_start_time_ms_;
              if (ultra_device_->StartUltra() < 0) {
                is_exp_state_ = kExpStateStop;
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
        if (st_load_is_running_ == false) {
          return;
        }
        /// @note static load aircraft action from button up and down
        /// ignore the load value.
        if (st_load_event_from_ == kSTLoadEventFromButtonUpDown) {
          return;
        }
        LOG_F(LG_INFO) << "st_result->status_:" << st_result->status_ << " "
                       << "st_result->load_:" << st_result->load_ << " "
                       << "st_result->position_:" << st_result->pos_ << " "
                       << "lss_->retention_:" << lss_->retention_ << " "
                       << "lss_->direct_:" << lss_->direct_ << " "
                       << "lss_->speed_:" << lss_->speed_;
        if ((st_result->status_ & DSP_CMDEND) == DSP_CMDEND) {
          LOG_F(LG_INFO) << "static load aircraft achieve the target load:"
                         << st_result->load_;
          if (st_posi_reach_first_time_ == 0) {
            st_posi_reach_first_time_ = anx::common::GetCurrentTimeMillis();
          } else {
            bool need_stop = true;
            int64_t current_time = anx::common::GetCurrentTimeMillis();
            int64_t cur_duration = current_time - st_posi_reach_first_time_;
            if (cur_duration > 1000) {
              /// @note static load aircraft reach the target value for 1000 ms
              /// then stop the action.
              LOG_F(LG_INFO) << "static load aircraft reach the target position"
                             << " for 1000 ms, stop";
              st_posi_reach_first_time_ = 0;
              /// @note static load aircraft stop action for delay stop of
              /// st_load_achieve_target_keep_duration_ms_. will be remove
              /// keep load logic. @related st_load_keep_load_ etc variables.
              if (lss_->ctrl_type_ == CTRL_LOAD) {
                if (st_load_achieve_target_keep_duration_ms_ > 0) {
                  st_load_keep_load_ = 1;
                  need_stop = false;
                  /// record first time achieve the target load time point in ms
                  /// value -1 is not achieve the target load, value > 0 is
                  /// achieve the target load first time point
                  if (st_load_achieve_target_time_ < 0) {
                    st_load_achieve_target_time_ =
                        anx::common::GetCurrentTimeMillis();
                  }
                }
              }
              /// ctrl_type is CTRL_POSI, then stop the action
              need_stop ? OnButtonStaticAircraftStop() : 0;
            } else {
              // do nothing
            }
          }
        } else {
          st_posi_reach_first_time_ = 0;
        }
        /// @note static load aircraft keep the target load value for
        /// st_load_achieve_target_keep_duration_ms_ ms. will be remove keep
        /// load logic. @related st_load_keep_load_ etc variables.
        if (st_load_keep_load_ == 1) {
          int64_t current_time = anx::common::GetCurrentTimeMillis();
          int64_t cur_duration = current_time - st_load_achieve_target_time_;
          if (cur_duration > st_load_achieve_target_keep_duration_ms_) {
            LOG_F(LG_INFO) << "static load aircraft keep the target load:"
                           << st_result->load_ << " for "
                           << st_load_achieve_target_keep_duration_ms_ << " ms"
                           << " stop";
            st_load_achieve_target_time_ = -1;
            OnButtonStaticAircraftStop();
          } else {
            LOG_F(LG_INFO) << "static load aircraft keep the target load:"
                           << st_result->load_ << " for "
                           << st_load_achieve_target_keep_duration_ms_ << " ms"
                           << " target load:" << lss_->retention_ << " continue"
                           << " cur_duration:" << cur_duration << " ms"
                           << " cur_load:" << st_result->load_;
            double target_load = lss_->retention_;
            if (target_load - st_result->load_ > 0.1) {
              LOG_F(LG_INFO) << "static load aircraft achieve the target load:"
                             << st_result->load_;
              if (lss_->direct_ == 1) {
                this->StaticAircraftDoMoveUp();
              } else if (lss_->direct_ == 2) {
                this->StaticAircraftDoMoveDown();
              } else {
                // do nothing
              }
              /// value 2 keep load action is done
              st_load_keep_load_ = 2;
            } else {
              // do nothing
            }
          }
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
        exp_pause_stop_reason_ = kExpPauseStopReasonSystemStandby;
        exp_stop();
      } else {
        // TODO(hhool): do nothing
      }
    } else {
      // TODO(hhool): do nothing
    }
  }
}

bool WorkWindowSecondPage::OnExpClipChanged(void* msg) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(msg);
  if (pMsg == nullptr || pMsg->pSender == nullptr) {
    return false;
  }
  LOG_F(LG_INFO) << pMsg->pSender->GetName() << " " << pMsg->sType << " "
                 << pMsg->wParam << " " << pMsg->lParam;
  if (pMsg->sType != DUI_MSGTYPE_KILLFOCUS &&
      pMsg->sType != DUI_MSGTYPE_RETURN) {
    return false;
  }

  bool save = false;
  if (pMsg->pSender == edit_exp_clip_time_duration_) {
    int64_t exp_clip_time_duration = 0;
    if (get_value_from_control(edit_exp_clip_time_duration_,
                               &exp_clip_time_duration)) {
      if (exp_clip_time_duration >= 1 &&
          exp_clip_time_duration <= kExpClipMaxCount) {
        dus_.exp_clip_time_duration_ = exp_clip_time_duration;
        save = true;
      }
    }
    set_value_to_edit(edit_exp_clip_time_duration_,
                      dus_.exp_clip_time_duration_);
  } else if (pMsg->pSender == edit_exp_clip_time_paused_) {
    int64_t exp_clip_time_paused = 0;
    if (get_value_from_control(edit_exp_clip_time_paused_,
                               &exp_clip_time_paused)) {
      if (exp_clip_time_paused >= 1 &&
          exp_clip_time_paused <= kExpClipMaxCount) {
        dus_.exp_clip_time_paused_ = exp_clip_time_paused;
        save = true;
      }
    }
    set_value_to_edit(edit_exp_clip_time_paused_, dus_.exp_clip_time_paused_);
  } else if (pMsg->pSender == edit_max_cycle_count_) {
    double exp_max_cycle_count =
        get_value_from_control<double>(edit_max_cycle_count_);
    int64_t max_cycle_count = 0;
    if (exp_max_cycle_count >= 1.0f) {
      // check the max cycle count with the max cycle power is valid
      int32_t exp_max_cycle_power = dus_.exp_max_cycle_power_;
      double f_max_cycle_count = exp_max_cycle_count;
      for (int32_t i = 0; i < exp_max_cycle_power; i++) {
        f_max_cycle_count *= 10;
      }
      if (static_cast<int64_t>(f_max_cycle_count) < 1) {
      } else {
        dus_.exp_max_cycle_count_ = exp_max_cycle_count;
        max_cycle_count = static_cast<int64_t>(f_max_cycle_count);
        save = true;
      }
    }
    set_value_to_edit(edit_max_cycle_count_, dus_.exp_max_cycle_count_, 3);
    if (save) {
      if (cur_total_cycle_count_ >= max_cycle_count) {
        if (is_exp_state_ == kExpStatePause) {
          btn_exp_resume_->SetEnabled(false);
        } else if (is_exp_state_ == kExpStateStop) {
          btn_exp_start_->SetEnabled(false);
        }
      } else {
        if (is_exp_state_ == kExpStatePause) {
          btn_exp_resume_->SetEnabled(true);
        } else if (is_exp_state_ == kExpStateStop) {
          btn_exp_start_->SetEnabled(true);
        }
      }
    }
  } else if (pMsg->pSender == edit_max_cycle_power_) {
    int32_t exp_max_cycle_power =
        get_value_from_control<int32_t>(edit_max_cycle_power_);
    int64_t max_cycle_count = 0;
    if (exp_max_cycle_power < 1 || exp_max_cycle_power > 18) {
      exp_max_cycle_power = dus_.exp_max_cycle_power_;
    } else {
      // check the max cycle count with the max cycle power is valid
      double exp_max_cycle_count = dus_.exp_max_cycle_count_;
      double f_max_cycle_count = exp_max_cycle_count;
      for (int32_t i = 0; i < exp_max_cycle_power; i++) {
        f_max_cycle_count *= 10;
      }
      if (static_cast<int64_t>(f_max_cycle_count) < 1) {
      } else {
        dus_.exp_max_cycle_power_ = exp_max_cycle_power;
        max_cycle_count = static_cast<int64_t>(f_max_cycle_count);
        save = true;
      }
    }
    set_value_to_edit(edit_max_cycle_power_, dus_.exp_max_cycle_power_);
    if (save) {
      if (cur_total_cycle_count_ >= max_cycle_count) {
        if (is_exp_state_ == kExpStatePause) {
          btn_exp_resume_->SetEnabled(false);
        } else if (is_exp_state_ == kExpStateStop) {
          btn_exp_start_->SetEnabled(false);
        }
      } else {
        if (is_exp_state_ == kExpStatePause) {
          btn_exp_resume_->SetEnabled(true);
        } else if (is_exp_state_ == kExpStateStop) {
          btn_exp_start_->SetEnabled(true);
        }
      }
    }
  } else if (pMsg->pSender == edit_frequency_fluctuations_range_) {
    int64_t exp_frequency_fluctuations_range = 0;
    if (get_value_from_control(edit_frequency_fluctuations_range_,
                               &exp_frequency_fluctuations_range)) {
      if (exp_frequency_fluctuations_range >= 1) {
        exp_frequency_fluctuations_range =
            dus_.exp_frequency_fluctuations_range_ =
                static_cast<int32_t>(exp_frequency_fluctuations_range);
        save = true;
      }
      set_value_to_edit(edit_frequency_fluctuations_range_,
                        dus_.exp_frequency_fluctuations_range_);
    }
  } else {
    return false;
  }
  return true;
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
  btn_sa_keep_load_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_setting")));
  btn_exp_reset_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_reset")));

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
  edit_exp_clip_time_duration_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPage::OnExpClipChanged);
  edit_exp_clip_time_paused_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPage::OnExpClipChanged);
  /// @brief max cycle count edit
  edit_max_cycle_count_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_exp_max_cycle_count")));
  edit_max_cycle_power_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_exp_max_cycle_power")));
  edit_frequency_fluctuations_range_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_frequency_fluctuations_range")));
  text_max_cycle_duration_ = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("text_exp_max_cycle_count_duration")));
  edit_max_cycle_count_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPage::OnExpClipChanged);
  edit_max_cycle_power_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPage::OnExpClipChanged);
  edit_frequency_fluctuations_range_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPage::OnExpClipChanged);

  btn_reset_exp_clip_setting_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_params_reset")));
  btn_reset_exp_clip_setting_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPage::OnBtnResetExpClipSetting);

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

  UpdateExpClipTimeFromControl();

  RefreshExpClipTimeControl(true);
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
  SaveExpClipSettingsFromControl();

  /// @brief kill the timer
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdSampling);
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdRefresh);

  /// @note stop ultra_device
  if (ultra_device_ != nullptr) {
    // stop the ultrasound
    if (ultra_device_->IsUltraStarted()) {
      ultra_device_->StopUltra();
    }
    ultra_device_->GetPortDevice()->RemoveListener(this);
    ultra_device_ = nullptr;
  }

  /// @note stop stload device
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.stop_run();
  /// direct to the settings of the static load
  /// and save it to the resource file
  lss_ = std::move(anx::device::LoadDeviceLoadStaticSettingsDefaultResource());
  lss_->direct_ = 0;
  anx::device::SaveDeviceLoadStaticSettingsDefaultResource(*lss_);
  /// @brief drop the exp_data table
  anx::db::helper::DropDataTable(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataGraph);
  anx::db::helper::DropDataTable(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataList);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Static aircraft releated button
void WorkWindowSecondPage::OnButtonStaticAircraftUp() {
  if (st_load_is_running_) {
    LOG_F(LG_WARN) << "static load is running, can not up";
    return;
  }
  st_load_event_from_ = kSTLoadEventFromButtonUpDown;
  if (!StaticAircraftDoMoveUp()) {
    LOG_F(LG_ERROR) << "StaticAircraftDoMoveUp error";
    st_load_event_from_ = kSTLoadEventNone;
    return;
  }
  st_load_is_running_ = true;
  /// update the releated button state
  btn_sa_keep_load_->SetEnabled(false);
  btn_sa_up_->SetEnabled(false);
  btn_sa_down_->SetEnabled(false);
  btn_sa_stop_->SetEnabled(true);
  //// direct to the settings of the static load
  /// and save it to the resource file
  lss_->direct_ = 1;
  anx::device::SaveDeviceLoadStaticSettingsDefaultResource(*lss_);
}

void WorkWindowSecondPage::OnButtonStaticAircraftDown() {
  if (st_load_is_running_) {
    LOG_F(LG_WARN) << "static load is running, can not down";
    return;
  }
  st_load_event_from_ = kSTLoadEventFromButtonUpDown;
  if (!StaticAircraftDoMoveDown()) {
    LOG_F(LG_ERROR) << "StaticAircraftDoMoveDown error";
    st_load_event_from_ = kSTLoadEventNone;
    return;
  }
  st_load_is_running_ = true;

  /// update the releated button state
  btn_sa_keep_load_->SetEnabled(false);
  btn_sa_up_->SetEnabled(false);
  btn_sa_down_->SetEnabled(false);
  btn_sa_stop_->SetEnabled(true);
  /// direct to the settings of the static load
  /// and save it to the resource file
  lss_->direct_ = 2;
  anx::device::SaveDeviceLoadStaticSettingsDefaultResource(*lss_);
}

void WorkWindowSecondPage::OnButtonStaticAircraftStop() {
  // anx::device::stload::STLoadHelper::st_load_loader_.st_api_.end_read();
  LOG_F(LG_INFO) << "stop the static load";
  StaticAircraftStop();
  btn_sa_keep_load_->SetEnabled(true);
  btn_sa_clear_->SetEnabled(true);
  btn_sa_up_->SetEnabled(true);
  btn_sa_down_->SetEnabled(true);
}

void WorkWindowSecondPage::OnButtonStaticAircraftClear() {
  // clear the data
  if (is_exp_state_ > kExpStateStop) {
    LOG_F(LG_WARN) << "exp is running, can not clear the data";
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

void WorkWindowSecondPage::OnButtonStaticAircraftKeepLoad() {
  int32_t result;
  DialogStaticLoadGuaranteedSettings* dialog_static_load_guaranteed_settings =
      new DialogStaticLoadGuaranteedSettings(&result);
  dialog_static_load_guaranteed_settings->Create(
      *pWorkWindow_, _T("dialog_static_load_guaranteed_settings"),
      UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
  dialog_static_load_guaranteed_settings->CenterWindow();
  dialog_static_load_guaranteed_settings->ShowModal();
  if (!result) {
    return;
  }
  lss_ = std::move(anx::device::LoadDeviceLoadStaticSettingsDefaultResource());
  assert(lss_ != nullptr);
  if (lss_ == nullptr) {
    // TODO(hhool): msgbox or tips
    return;
  }
  /// @brief static aircraft stop.
  // StaticAircraftStop();
  /// @brief static aircraft keep load.
  st_load_event_from_ = kSTLoadEventFromKeepLoadButton;
  if (lss_->direct_ == 1) {
    if (!StaticAircraftDoMoveUp()) {
      LOG_F(LG_ERROR) << "StaticAircraftDoMoveUp error";
      st_load_event_from_ = kSTLoadEventNone;
      return;
    }
  } else {
    if (!StaticAircraftDoMoveDown()) {
      LOG_F(LG_ERROR) << "StaticAircraftDoMoveDown error";
      st_load_event_from_ = kSTLoadEventNone;
      return;
    }
  }
  st_load_is_running_ = true;
  /// update the releated button state
  btn_sa_keep_load_->SetEnabled(false);
  btn_sa_up_->SetEnabled(false);
  btn_sa_down_->SetEnabled(false);
  btn_sa_stop_->SetEnabled(true);
  btn_sa_clear_->SetEnabled(false);
}

void WorkWindowSecondPage::OnButtonExpReset() {
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
  exp_data_list_info_.exp_freq_total_count_ = 0;

  cur_total_cycle_count_ = 0;

  pre_exp_start_time_ms_ = 0;
  pre_total_cycle_count_ = 0;
  pre_total_data_table_no_ = 0;

  if (is_exp_state_ == kExpStatePause) {
    btn_exp_resume_->SetEnabled(true);
  } else if (is_exp_state_ == kExpStateStop) {
    btn_exp_start_->SetEnabled(true);
  }
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

void WorkWindowSecondPage::OnExpStart() {
  user_exp_state_ = kExpStateStart;
  if (is_exp_state_ != kExpStateStop) {
    LOG_F(LG_WARN) << "exp_start: is_exp_state_:"
                   << ExpStateToString(is_exp_state_);
    return;
  }
  exp_pause_stop_reason_ = kExpPauseStopReasonNone;
  /// @note start the ultrasound device
  exp_start();
}

void WorkWindowSecondPage::OnExpStop() {
  if (is_exp_state_ == kExpStateStop) {
    LOG_F(LG_WARN) << "exp_stop: is_exp_state_:"
                   << ExpStateToString(is_exp_state_);
    return;
  }
  /// @note stop the ultrasound device
  exp_stop();
  exp_pause_stop_reason_ = kExpPauseStopReasonNone;
}

void WorkWindowSecondPage::OnExpPause() {
  user_exp_state_ = kExpStatePause;
  if (is_exp_state_ == kExpStatePause) {
    LOG_F(LG_WARN) << "exp_pause: is_exp_state_:"
                   << ExpStateToString(is_exp_state_);
    return;
  }
  /// @note pause the ultrasound device
  exp_pause();
}

void WorkWindowSecondPage::OnExpResume() {
  user_exp_state_ = kExpStateStart;
  if (is_exp_state_ != kExpStatePause) {
    LOG_F(LG_WARN) << "exp_resume: is_exp_state_:"
                   << ExpStateToString(is_exp_state_);
    return;
  }
  /// @note resume the ultrasound device
  exp_resume();
}

bool WorkWindowSecondPage::OnBtnResetExpClipSetting(void* params) {
  if (params == nullptr) {
    return false;
  }
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(params);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->sType != DUI_MSGTYPE_CLICK) {
    return false;
  }
  int32_t ret = anx::device::ResetDeviceUltrasoundSettingsDefaultResource();
  if (ret != 0) {
    LOG_F(LG_ERROR) << "reset the exp clip setting error";
    return false;
  }
  UpdateControlFromSettings();
  return false;
}

void WorkWindowSecondPage::CheckDeviceComConnectedStatus() {
  /// exp_start, exp_stop, exp_pause, exp_resume button state
  /// if the device com interface is connected then enable the exp_start
  /// button else disable the exp_start button and exp_stop button and
  /// exp_pause button
  if (pWorkWindow_->IsULDeviceComInterfaceConnected()) {
    if (is_exp_state_ < kExpStateStop) {
      is_exp_state_ = kExpStateStop;
    }
    UpdateUIButton();
  } else {
    if (is_exp_state_ >= kExpStateStop) {
      is_exp_state_ = kExpStateUnvalid;
    }
    UpdateUIButton();
  }
}

void WorkWindowSecondPage::RefreshExpClipTimeControl(bool forced) {
  if (!edit_exp_clip_time_duration_->GetText().IsEmpty()) {
    int64_t exp_clip_time_duration =
        get_value_from_control<int64_t>(edit_exp_clip_time_duration_);
    if (dus_.exp_clip_time_duration_ != exp_clip_time_duration || forced) {
      bool is_changed = false;
      if (exp_clip_time_duration >= 1 &&
          exp_clip_time_duration <= kExpClipMaxCount) {
        dus_.exp_clip_time_duration_ = exp_clip_time_duration;
        std::string value = ("=");
        double f_value = static_cast<double>(exp_clip_time_duration);
        f_value /= 10.0f;
        value += anx::common::to_string_with_precision(f_value, 1);
        value += "S";
        text_exp_clip_time_duration_->SetText(
            anx::common::String2WString(value).c_str());
      } else {
        // TODO(hhool): tips the user the value is invalid
      }
    }

    if (!edit_exp_clip_time_paused_->GetText().IsEmpty()) {
      int64_t exp_clip_time_paused =
          get_value_from_control<int64_t>(edit_exp_clip_time_paused_);
      if (dus_.exp_clip_time_paused_ != exp_clip_time_paused || forced) {
        if (exp_clip_time_paused >= 1 &&
            exp_clip_time_paused <= kExpClipMaxCount) {
          dus_.exp_clip_time_paused_ = exp_clip_time_paused;
          std::string value = ("=");
          double f_value = static_cast<double>(exp_clip_time_paused);
          f_value /= 10.0f;
          value += anx::common::to_string_with_precision(f_value, 1);
          value += ("S");
          text_exp_clip_time_paused_->SetText(
              anx::common::String2WString(value).c_str());
        } else {
          // TODO(hhool): tips the user the value is invalid
        }
      }
    }
    if (!edit_max_cycle_count_->GetText().IsEmpty() &&
        !edit_max_cycle_power_->GetText().IsEmpty()) {
      double exp_max_cycle_count =
          get_value_from_control<double>(edit_max_cycle_count_);
      if (exp_max_cycle_count >= 1) {
        // check the max cycle count with the max cycle power is valid
        int64_t exp_max_cycle_power =
            get_value_from_control<int64_t>(edit_max_cycle_power_);
        double max_cycle_count = exp_max_cycle_count;
        for (int32_t i = 0; i < exp_max_cycle_power; i++) {
          max_cycle_count *= 10;
        }
        if (max_cycle_count >= 1) {
          dus_.exp_max_cycle_count_ = exp_max_cycle_count;

          int64_t max_cycle_count_for_text =
              static_cast<int64_t>(max_cycle_count);
          max_cycle_count_for_text /= 20000;
          std::string value = ("≈");
          int64_t part_integer = max_cycle_count_for_text / 3600;
          int64_t part_decimal = max_cycle_count_for_text % 3600;
          double f_part_decimal = static_cast<double>(part_decimal) / 3600;
          double f_part_integer_value = static_cast<double>(part_integer);
          double f_part_decimal_value = static_cast<double>(f_part_decimal);
          double f_value = f_part_integer_value + f_part_decimal_value;
          value += anx::common::to_string_with_precision(f_value, 3);
          value += "H";
          text_max_cycle_duration_->SetText(
              anx::common::UTF8ToUnicode(value).c_str());
        }

        // check the max cycle count with the max cycle power is valid
        int64_t max_cycle_count_for_btn = static_cast<int64_t>(max_cycle_count);
        if (cur_total_cycle_count_ >= max_cycle_count_for_btn) {
          if (is_exp_state_ == kExpStatePause) {
            btn_exp_resume_->SetEnabled(false);
          } else if (is_exp_state_ == kExpStateStop) {
            btn_exp_start_->SetEnabled(false);
          }
        } else {
          if (is_exp_state_ == kExpStatePause) {
            btn_exp_resume_->SetEnabled(true);
          } else if (is_exp_state_ == kExpStateStop) {
            btn_exp_start_->SetEnabled(true);
          }
        }
      }
    }
    if (!edit_max_cycle_power_->GetText().IsEmpty() &&
        !edit_max_cycle_count_->GetText().IsEmpty()) {
      int32_t exp_max_cycle_power =
          get_value_from_control<int32_t>(edit_max_cycle_power_);
      if (dus_.exp_max_cycle_power_ != exp_max_cycle_power || forced) {
        if (exp_max_cycle_power >= 1 && exp_max_cycle_power <= 18) {
          // check the max cycle count with the max cycle power is valid
          double exp_max_cycle_count =
              get_value_from_control<double>(edit_max_cycle_count_);
          double max_cycle_count = exp_max_cycle_count;
          for (int32_t i = 0; i < exp_max_cycle_power; i++) {
            max_cycle_count *= 10;
          }
          if (max_cycle_count >= 1.0f) {
            dus_.exp_max_cycle_power_ = exp_max_cycle_power;

            int64_t max_cycle_count_for_text =
                static_cast<int64_t>(max_cycle_count);
            max_cycle_count_for_text /= 20000;
            std::string value = ("≈");
            int64_t part_integer = max_cycle_count_for_text / 3600;
            int64_t part_decimal = max_cycle_count_for_text % 3600;
            double f_part_decimal = static_cast<double>(part_decimal) / 3600;
            double f_part_integer_value = static_cast<double>(part_integer);
            double f_part_decimal_value = static_cast<double>(f_part_decimal);
            double f_value = f_part_integer_value + f_part_decimal_value;
            value += anx::common::to_string_with_precision(f_value, 3);
            value += "H";
            text_max_cycle_duration_->SetText(
                anx::common::UTF8ToUnicode(value).c_str());
          }
          // check the max cycle count with the max cycle power is valid
          int64_t max_cycle_count_for_btn =
              static_cast<int64_t>(max_cycle_count);
          if (cur_total_cycle_count_ >= max_cycle_count_for_btn) {
            if (is_exp_state_ == kExpStatePause) {
              btn_exp_resume_->SetEnabled(false);
            } else if (is_exp_state_ == kExpStateStop) {
              btn_exp_start_->SetEnabled(false);
            }
          } else {
            if (is_exp_state_ == kExpStatePause) {
              btn_exp_resume_->SetEnabled(true);
            } else if (is_exp_state_ == kExpStateStop) {
              btn_exp_start_->SetEnabled(true);
            }
          }
        } else {
          // TODO(hhool): tips the user the value is invalid
        }
      }

      if (!edit_frequency_fluctuations_range_->GetText().IsEmpty()) {
        int32_t exp_frequency_fluctuations_range =
            get_value_from_control<int32_t>(edit_frequency_fluctuations_range_);
        if (dus_.exp_frequency_fluctuations_range_ !=
            exp_frequency_fluctuations_range) {
          if (exp_frequency_fluctuations_range >= 1) {
            dus_.exp_frequency_fluctuations_range_ =
                exp_frequency_fluctuations_range;
          }
        }
      }
    }
  }
}

void WorkWindowSecondPage::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceUltrasoundSettings> dus =
      anx::device::LoadDeviceUltrasoundSettingsDefaultResource();
  if (dus != nullptr) {
    set_value_to_edit(edit_exp_clip_time_duration_,
                      dus->exp_clip_time_duration_);
    set_value_to_edit(edit_exp_clip_time_paused_, dus->exp_clip_time_paused_);
    set_value_to_edit(edit_max_cycle_count_, dus->exp_max_cycle_count_, 3);
    set_value_to_edit(edit_max_cycle_power_, dus->exp_max_cycle_power_);
    set_value_to_edit(edit_frequency_fluctuations_range_,
                      dus->exp_frequency_fluctuations_range_);

    if (dus->exp_clipping_enable_ == 1) {
      /// @note not apply according to customer opinions
      chk_exp_clip_set_->Selected(false);
    } else {
      chk_exp_clip_set_->Selected(false);
    }
  }
}

void WorkWindowSecondPage::SaveExpClipSettingsFromControl() {
  dus_.exp_clip_time_duration_ =
      get_value_from_control<int64_t>(edit_exp_clip_time_duration_);
  dus_.exp_clip_time_paused_ =
      get_value_from_control<int64_t>(edit_exp_clip_time_paused_);
  dus_.exp_max_cycle_count_ =
      get_value_from_control<double>(edit_max_cycle_count_);
  dus_.exp_max_cycle_power_ =
      get_value_from_control<int32_t>(edit_max_cycle_power_);
  dus_.exp_frequency_fluctuations_range_ =
      get_value_from_control<int32_t>(edit_frequency_fluctuations_range_);
  if (chk_exp_clip_set_->IsSelected()) {
    dus_.exp_clipping_enable_ = 1;
  } else {
    dus_.exp_clipping_enable_ = 0;
  }
  anx::device::SaveDeviceUltrasoundSettingsDefaultResource(dus_);
}

void WorkWindowSecondPage::UpdateExpClipTimeFromControl() {
  int64_t exp_clip_time_duration =
      get_value_from_control<int64_t>(edit_exp_clip_time_duration_);
  int64_t exp_clip_time_paused =
      get_value_from_control<int64_t>(edit_exp_clip_time_paused_);

  // exp_clip_time_duration append "S"
  std::string value = ("=");
  double f_value = static_cast<double>(exp_clip_time_duration);
  f_value /= 10.0f;
  value += anx::common::to_string_with_precision(f_value, 1);
  value += "S";
  text_exp_clip_time_duration_->SetText(
      anx::common::String2WString(value).c_str());
  // exp_clip_time_paused append "S"
  value = ("=");
  f_value = static_cast<double>(exp_clip_time_paused);
  f_value /= 10.0f;
  value += anx::common::to_string_with_precision(f_value, 1);
  value += "S";
  text_exp_clip_time_paused_->SetText(
      anx::common::String2WString(value).c_str());
}

int64_t WorkWindowSecondPage::MaxExpCycleCountFromControl() {
  double max_cycle_count = 0;
  double exp_max_cycle_count =
      get_value_from_control<double>(edit_max_cycle_count_);
  if (exp_max_cycle_count >= 1.f) {
    // check the max cycle count with the max cycle power is valid
    int64_t exp_max_cycle_power =
        get_value_from_control<int64_t>(edit_max_cycle_power_);
    max_cycle_count = exp_max_cycle_count;
    for (int32_t i = 0; i < exp_max_cycle_power; i++) {
      max_cycle_count *= 10;
    }
    if (max_cycle_count >= 1) {
      dus_.exp_max_cycle_count_ = exp_max_cycle_count;
    }
  }
  return static_cast<int64_t>(max_cycle_count);
}

void WorkWindowSecondPage::UpdateUIButton() {
  // check the device com connected status
  // enable btn sa up and down
  // if the device com interface is connected and the exp is not running
  // and the static load is not running then enable the sa up and down
  // button else disable the sa up and down button sa_stop button state is
  // the opposite of sa_up and sa_down button if sa_up and sa_down button is
  // enabled then sa_stop button is disabled if sa_up and sa_down button is
  // disabled then sa_stop button is enabled
  bool is_st_device_com_interface_connected =
      pWorkWindow_->IsSLDeviceComInterfaceConnected();
  bool sa_up_down_enable = true;
  if (!is_st_device_com_interface_connected) {
    sa_up_down_enable = false;
  } else {
    if (is_exp_state_ == kExpStateStart) {
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
    /// if the static load keep is running then disable the clear button
    if (st_load_event_from_ == kSTLoadEventFromKeepLoadButton) {
      sa_clear_enable = false;
    } else {
      sa_clear_enable = true;
    }
  }
  bool sa_keep_load_enable = false;
  if (!is_st_device_com_interface_connected) {
    sa_keep_load_enable = false;
  } else {
    if (st_load_is_running_) {
      sa_keep_load_enable = false;
    } else {
      sa_keep_load_enable = true;
    }
  }

  btn_sa_up_->SetEnabled(sa_up_down_enable);
  btn_sa_down_->SetEnabled(sa_up_down_enable);
  btn_sa_stop_->SetEnabled(sa_stop_enable);
  btn_sa_clear_->SetEnabled(sa_clear_enable);
  btn_sa_keep_load_->SetEnabled(sa_keep_load_enable);

  bool is_ul_device_com_interface_connected =
      pWorkWindow_->IsULDeviceComInterfaceConnected();
  // check the device com connected status
  if (!is_ul_device_com_interface_connected) {
    btn_exp_reset_->SetEnabled(false);
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
  }

  if (is_exp_state_ == kExpStateStop) {
    btn_exp_reset_->SetEnabled(true);
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
    btn_reset_exp_clip_setting_->SetEnabled(true);
  } else if (is_exp_state_ == kExpStateStart) {
    btn_exp_reset_->SetEnabled(false);
    btn_exp_start_->SetEnabled(false);
    layout_exp_resume_->SetVisible(false);
    btn_exp_resume_->SetEnabled(false);
    layout_exp_pause_->SetVisible(true);
    btn_exp_pause_->SetEnabled(true);
    btn_exp_stop_->SetEnabled(true);

    chk_exp_clip_set_->SetEnabled(false);
    edit_exp_clip_time_duration_->SetEnabled(false);
    edit_exp_clip_time_paused_->SetEnabled(false);
    edit_max_cycle_count_->SetEnabled(false);
    edit_max_cycle_power_->SetEnabled(false);
    edit_frequency_fluctuations_range_->SetEnabled(false);
    btn_reset_exp_clip_setting_->SetEnabled(false);
  } else if (is_exp_state_ == kExpStatePause) {
    btn_exp_reset_->SetEnabled(true);
    btn_exp_start_->SetEnabled(false);
    layout_exp_resume_->SetVisible(true);
    if (cur_total_cycle_count_ >= MaxExpCycleCountFromControl()) {
      btn_exp_resume_->SetEnabled(false);
    } else {
      btn_exp_resume_->SetEnabled(true);
    }
    layout_exp_pause_->SetVisible(false);
    btn_exp_pause_->SetEnabled(false);
    btn_exp_stop_->SetEnabled(true);

    chk_exp_clip_set_->SetEnabled(true);
    edit_exp_clip_time_duration_->SetEnabled(true);
    edit_exp_clip_time_paused_->SetEnabled(true);
    edit_max_cycle_count_->SetEnabled(true);
    edit_max_cycle_power_->SetEnabled(true);
    edit_frequency_fluctuations_range_->SetEnabled(true);
    btn_reset_exp_clip_setting_->SetEnabled(false);
  } else {
    // TODO(hhool):
  }
}

int32_t WorkWindowSecondPage::exp_start() {
  if (is_exp_state_ != kExpStateStop) {
    LOG_F(LG_WARN) << "is_exp_state_: " << ExpStateToString(is_exp_state_);
    return 0;
  }

  UpdateUIButton();

  UpdateExpClipTimeFromControl();

  SaveExpClipSettingsFromControl();

  /// @brief reset the database exp_data table.
  /// @note the table name is exp_data, delete exp_data table and create a
  /// new one.
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
  exp_data_list_info_.exp_freq_total_count_ = 0;

  // set the ultrasound on state
  int32_t ret = ultra_device_->SetWedingTime(0);
  if (ret < 0) {
    is_exp_state_ = kExpStateUnvalid;
    LOG_F(LG_WARN) << "SetWedingTime failed";
    return -1;
  }
  cur_freq_ = initial_frequency_ = ultra_device_->GetCurrentFreq();
  cur_power_ = initial_power_ = ultra_device_->GetCurrentPower();
  if (initial_frequency_ < 0 || initial_power_ < 0) {
    is_exp_state_ = kExpStateUnvalid;
    LOG_F(LG_WARN) << "initial_frequency_:" << initial_frequency_
                   << " initial_power_:" << initial_power_;
    return -1;
  }
  /// @brief start the ultrasound device if the sampling start pos is 0
  /// else will done with OnTimer
  if (dedss_->sampling_start_pos_ == 0) {
    if (ultra_device_->StartUltra() < 0) {
      is_exp_state_ = kExpStateUnvalid;
      LOG_F(LG_WARN) << "StartUltra failed";
      return -1;
    }
  }

  cur_total_cycle_count_ = 0;
  pre_exp_start_time_ms_ = 0;
  pre_total_cycle_count_ = 0;
  pre_total_data_table_no_ = 0;

  start_time_pos_has_deal_ = false;
  pre_clip_paused_ms_ = 0;
  paint_manager_ui_->SetTimer(btn_exp_start_, kTimerIdSampling,
                              kSamplingInterval);

  state_ultrasound_exp_clip_ = 1;

  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_start_;
  msg.sType = kClick;
  if (work_window_second_page_data_notify_pump_.get() != nullptr) {
    work_window_second_page_data_notify_pump_->NotifyPump(msg);
  }
  if (work_window_second_page_graph_notify_pump_.get() != nullptr) {
    work_window_second_page_graph_notify_pump_->NotifyPump(msg);
  }

  is_exp_state_ = kExpStateStart;
  LOG_F(LG_INFO);
  return 0;
}

void WorkWindowSecondPage::exp_pause() {
  if (is_exp_state_ == kExpStatePause) {
    LOG_F(LG_WARN) << "is_exp_state_: " << ExpStateToString(is_exp_state_);
    return;
  }
  is_exp_state_ = kExpStatePause;
  UpdateUIButton();

  // stop the ultrasound
  if (ultra_device_->IsUltraStarted()) {
    ultra_device_->StopUltra();
  } else {
    // do nothing
  }

  pre_total_cycle_count_ = cur_total_cycle_count_;
  pre_total_data_table_no_ = exp_data_list_info_.exp_data_table_no_;
  LOG_F(LG_INFO) << "cur_total_cycle_count_:" << cur_total_cycle_count_ << " "
                 << "pre_total_cycle_count_:" << pre_total_cycle_count_ << " "
                 << "pre_total_data_table_no_:" << pre_total_data_table_no_;
  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_pause_;
  msg.sType = kClick;
  if (work_window_second_page_data_notify_pump_.get() != nullptr) {
    work_window_second_page_data_notify_pump_->NotifyPump(msg);
  }
  if (work_window_second_page_graph_notify_pump_.get() != nullptr) {
    work_window_second_page_graph_notify_pump_->NotifyPump(msg);
  }
}

void WorkWindowSecondPage::exp_resume() {
  if (is_exp_state_ != kExpStatePause) {
    LOG_F(LG_WARN) << "is_exp_state_: " << is_exp_state_;
    return;
  }
  double f_exp_max_cycle_count = dus_.exp_max_cycle_count_;
  for (int32_t i = 0; i < dus_.exp_max_cycle_power_; i++) {
    f_exp_max_cycle_count *= 10;
  }
  int64_t exp_max_cycle_count = static_cast<int64_t>(f_exp_max_cycle_count);
  if (cur_total_cycle_count_ > exp_max_cycle_count) {
    LOG_F(LG_WARN) << "exp_resume:" << cur_total_cycle_count_ << " > "
                   << exp_max_cycle_count;
    // TODO(hhool): show tips
    return;
  }
  is_exp_state_ = kExpStateStart;

  UpdateUIButton();

  UpdateExpClipTimeFromControl();

  SaveExpClipSettingsFromControl();

  exp_data_list_info_.exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_data_list_info_.exp_time_interval_num_ = 0;
  exp_data_list_info_.exp_freq_total_count_ = 0;
  dedss_ =
      std::move(anx::device::LoadDeviceExpDataSampleSettingsDefaultResource());
  exp_data_list_info_.exp_sample_interval_ms_ =
      dedss_->sampling_interval_ * 100;
  pre_exp_start_time_ms_ = exp_data_list_info_.exp_start_time_ms_;
  pre_clip_paused_ms_ = 0;
  // start the ultrasound
  ultra_device_->StartUltra();

  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_resume_;
  msg.sType = kClick;
  if (work_window_second_page_data_notify_pump_.get() != nullptr) {
    work_window_second_page_data_notify_pump_->NotifyPump(msg);
  }
  if (work_window_second_page_graph_notify_pump_.get() != nullptr) {
    work_window_second_page_graph_notify_pump_->NotifyPump(msg);
  }
}

void WorkWindowSecondPage::exp_stop() {
  if (is_exp_state_ == kExpStateStop) {
    LOG_F(LG_WARN) << "is_exp_state_: " << ExpStateToString(is_exp_state_);
    return;
  }
  is_exp_state_ = kExpStateStop;
  exp_data_pre_duration_exponential_ = 0;
  UpdateUIButton();
  // stop the ultrasound
  ultra_device_->StopUltra();
  // notify the exp stop
  DuiLib::TNotifyUI msg;
  msg.pSender = btn_exp_stop_;
  msg.sType = kClick;
  if (work_window_second_page_data_notify_pump_.get() != nullptr) {
    work_window_second_page_data_notify_pump_->NotifyPump(msg);
  }
  if (work_window_second_page_graph_notify_pump_.get() != nullptr) {
    work_window_second_page_graph_notify_pump_->NotifyPump(msg);
  }

  // stop the timer
  paint_manager_ui_->KillTimer(btn_exp_start_, kTimerIdSampling);
  LOG_F(LG_INFO);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Static aircraft releated
bool WorkWindowSecondPage::StaticAircraftDoMoveUp() {
  assert(lss_ != nullptr);
  assert(st_load_event_from_ != kSTLoadEventNone);
  st_load_achieve_target_keep_duration_ms_ = lss_->keep_load_duration_ * 1000;
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(1);
  float speed = lss_->speed_ * 1.0f;
  /// RUN the static load, the direction is up and the speed is 2.0f / 60.0f
  int32_t ctrl_type = CTRL_LOAD;
  int32_t endtype = END_LOAD;
  float end_value = lss_->retention_ * 1.0f;
  if (st_load_event_from_ == kSTLoadEventFromButtonUpDown) {
    // TODO(hhool): read from setting 10000 mm
    ctrl_type = CTRL_POSI;
    endtype = END_POSI;
    end_value = 10000.0f;
    speed = 50.0f / 60.0f;
  } else if (st_load_event_from_ == kSTLoadEventFromKeepLoadButton) {
    if (lss_->ctrl_type_ == CTRL_POSI) {
      ctrl_type = CTRL_POSI;
      endtype = END_POSI;
      end_value = lss_->displacement_ * 1.0f;
      speed = speed / 60.0f;
    }
  }
  if (anx::device::stload::STLoadHelper::STLoadVersion() == 2) {
    if (st_load_event_from_ == kSTLoadEventFromButtonUpDown) {
      LOG_F(LG_INFO) << "carry_pid ctrl_type:" << ctrl_type
                     << " endtype:" << endtype << " speed:" << speed
                     << " end_value:" << end_value << " version:"
                     << anx::device::stload::STLoadHelper::STLoadVersion()
                     << " "
                     << "st_load_event_from_:" << st_load_event_from_;
      bool bSuccess =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
              ctrl_type, 100, 1, 3);
      if (!bSuccess) {
        LOG_F(LG_ERROR) << "carry_pid failed";
        return false;
      }
    } else {
      LOG_F(LG_INFO) << "carry_pid ctrl_type:" << ctrl_type
                     << " endtype:" << endtype << " speed:" << speed
                     << " end_value:" << end_value << " version:"
                     << anx::device::stload::STLoadHelper::STLoadVersion()
                     << " "
                     << "st_load_event_from_:" << st_load_event_from_;
      bool bSuccess =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
              ctrl_type, 100, 1, 3);
      if (!bSuccess) {
        LOG_F(LG_ERROR) << "carry_pid failed";
        return false;
      }
    }
  }
  LOG_F(LG_INFO) << "carry_200:" << ctrl_type << " " << endtype << " " << speed
                 << " " << end_value;
  bool bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_200(
          ctrl_type, endtype, speed, end_value, 0, true, DIR_NO, 0, 1, 0)
          ? true
          : false;
  if (!bSuccess) {
    LOG_F(LG_ERROR) << "carry_200 error";
    return false;
  }
  return true;
}

bool WorkWindowSecondPage::StaticAircraftDoMoveDown() {
  assert(lss_ != nullptr);
  assert(st_load_event_from_ != kSTLoadEventNone);
  st_load_achieve_target_keep_duration_ms_ = lss_->keep_load_duration_ * 1000;
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(0);
  float speed = lss_->speed_ * 1.0f;
  int32_t ctrl_type = CTRL_LOAD;
  int32_t endtype = END_LOAD;
  float end_value = lss_->retention_ * 1.0f;
  if (st_load_event_from_ == kSTLoadEventFromButtonUpDown) {
    // TODO(hhool): read from setting 10000 mm
    ctrl_type = CTRL_POSI;
    endtype = END_POSI;
    end_value = 10000.0f;
    speed = 50.0f / 60.0f;
  } else if (st_load_event_from_ == kSTLoadEventFromKeepLoadButton) {
    if (lss_->ctrl_type_ == CTRL_POSI) {
      ctrl_type = CTRL_POSI;
      endtype = END_POSI;
      end_value = lss_->displacement_ * 1.0f;
      speed = speed / 60.0f;
    }
  }
  if (anx::device::stload::STLoadHelper::STLoadVersion() == 2) {
    if (st_load_event_from_ == kSTLoadEventFromButtonUpDown) {
      LOG_F(LG_INFO) << "carry_pid ctrl_type:" << ctrl_type
                     << " endtype:" << endtype << " speed:" << speed
                     << " end_value:" << end_value << " version:"
                     << anx::device::stload::STLoadHelper::STLoadVersion()
                     << " "
                     << "st_load_event_from_:" << st_load_event_from_;
      bool bSuccess =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
              ctrl_type, 100, 1, 3);
      if (!bSuccess) {
        LOG_F(LG_ERROR) << "carry_pid failed";
        return false;
      }
    } else {
      LOG_F(LG_INFO) << "carry_pid ctrl_type:" << ctrl_type
                     << " endtype:" << endtype << " speed:" << speed
                     << " end_value:" << end_value << " version:"
                     << anx::device::stload::STLoadHelper::STLoadVersion()
                     << " "
                     << "st_load_event_from_:" << st_load_event_from_;
      bool bSuccess =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
              ctrl_type, 100, 1, 3);
      if (!bSuccess) {
        LOG_F(LG_ERROR) << "carry_pid failed";
        return false;
      }
    }
  }

  LOG_F(LG_INFO) << "carry_200:" << ctrl_type << " " << endtype << " " << speed
                 << " " << end_value;
  bool bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_200(
          ctrl_type, endtype, speed, end_value, 0, true, DIR_NO, 0, 1, 0)
          ? true
          : false;
  if (!bSuccess) {
    LOG_F(LG_ERROR) << "carry_200 error";
    return false;
  }
  return true;
}

bool WorkWindowSecondPage::StaticAircraftStop() {
  LOG_F(LG_INFO) << "stop the static load";
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.stop_run();
  st_load_is_running_ = false;
  st_load_achieve_target_time_ = -1;
  st_load_keep_load_ = -1;
  st_load_event_from_ = kSTLoadEventNone;
  return true;
}

void WorkWindowSecondPage::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  if (is_exp_state_ > kExpStateStop) {
    /////////////////////////////////////////////////////////////////////////
    /// check the exp data received reach the max cycle count
    /// get current cycle count and total time  cycle_count / x kHZ
    bool bReachEnd = false;
    std::string message;
    double f_exp_max_cycle_count = dus_.exp_max_cycle_count_;
    for (int32_t i = 0; i < dus_.exp_max_cycle_power_; i++) {
      f_exp_max_cycle_count *= 10;
    }
    int64_t exp_max_cycle_count = static_cast<int64_t>(f_exp_max_cycle_count);
    bool reach_max_cycle_count = false;
    if (cur_total_cycle_count_ >= exp_max_cycle_count) {
      reach_max_cycle_count = true;
      /// avoid renter;
      if (is_exp_state_ != kExpStatePause) {
        exp_pause();
        exp_pause_stop_reason_ = kExpPauseStopReasonReachMaxCycle;
        LOG_F(LG_WARN) << "msgbox:exp_pause:" << cur_total_cycle_count_ << " > "
                       << exp_max_cycle_count;
        message = "已达到最大循环次数";
        bReachEnd = true;
      } else {
        LOG_F(LG_WARN) << "ignore state exp_pause:" << cur_total_cycle_count_
                       << " > " << exp_max_cycle_count;
      }
    }
    int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
    if (dedss_->sampling_end_pos_ > dedss_->sampling_start_pos_) {
      int64_t sampling_start_end_diff =
          dedss_->sampling_end_pos_ - dedss_->sampling_start_pos_;
      int64_t sampling_start_end_diff_ms = sampling_start_end_diff * 100;
      int64_t time_ms_diff_from_start =
          current_time_ms - exp_data_list_info_.exp_start_time_ms_;
      if (time_ms_diff_from_start > sampling_start_end_diff_ms) {
        LOG_F(LG_WARN) << "msgbox:exp_pause";
        /// avoid renter;
        if (is_exp_state_ != kExpStatePause) {
          exp_pause();
          exp_pause_stop_reason_ = kExpPauseStopReasonReachRangeTimeEndPos;
          message = "已到达循环结束时间点";
          bReachEnd = true;
        } else {
          LOG_F(LG_WARN) << "ignore state exp_pause:" << time_ms_diff_from_start
                         << " > " << time_ms_diff_from_start;
        }
      }
    }
    if (bReachEnd) {
      btn_exp_resume_->SetEnabled(false);
      anx::ui::DialogCommon::ShowDialog(
          *pWorkWindow_, "提示", message,
          anx::ui::DialogCommon::kDialogCommonStyleOk);
      return;
    }
    /////////////////////////////////////////////////////////////////////////
    /// process the exp data
    double f_cur_freq = cur_freq_ * 1.0f;
    double um = exp_amplitude_;
    double MPa = std::fabsl(exp_statc_load_mpa_);
    double date = anx::common::GetCurrrentSystimeAsVarTime();
    exp_data_graph_info_.amp_freq_ = f_cur_freq;
    exp_data_graph_info_.amp_um_ = um;
    exp_data_graph_info_.stress_value_ = MPa;

    exp_data_list_info_.amp_freq_ = f_cur_freq;
    exp_data_list_info_.amp_um_ = um;
    exp_data_list_info_.stress_value_ = exp_max_stress_MPa_;

    LOG_F(LG_SENSITIVE) << "exp data graph: exp_data_table_no_:"
                        << exp_data_graph_info_.exp_data_table_no_
                        << " exp data list: exp_data_table_no_:"
                        << exp_data_list_info_.exp_data_table_no_;
    this->ProcessDataGraph();
    /// @note process intermittent exp clipping enabled
    if (ultra_device_->IsUltraStarted() && !reach_max_cycle_count) {
      assert(dedss_ != nullptr);
      if (dedss_->sample_mode_ ==
          anx::device::DeviceExpDataSample::kSampleModeExponent) {
        this->ProcessDataListModeExponential();
      } else {
        this->ProcessDataListModeLinear();
      }
    }
  }
}  // namespace ui

void WorkWindowSecondPage::ProcessDataGraph() {
  int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
  int64_t time_ms_diff =
      current_time_ms - exp_data_graph_info_.exp_start_time_ms_;
  int64_t time_interval_num =
      time_ms_diff / exp_data_graph_info_.exp_sample_interval_ms_;
  if (time_interval_num > exp_data_graph_info_.exp_time_interval_num_) {
    exp_data_graph_info_.exp_time_interval_num_ = time_interval_num;
    exp_data_graph_info_.exp_data_table_no_++;
    // update the data to the database table amp, stress, um
    int64_t cycle_count = exp_data_graph_info_.exp_data_table_no_ *
                          static_cast<int64_t>(exp_data_graph_info_.amp_freq_);
    double date = anx::common::GetCurrrentSystimeAsVarTime();
    // save to database
    // format cycle_count, KHz, MPa, um to the sql string and insert to the
    // database
    std::string sql_str = ("INSERT INTO ");
    sql_str.append(anx::db::helper::kTableExpDataGraph);
    sql_str.append((" (cycle, KHz, MPa, um, state, date) VALUES ("));
    sql_str.append(std::to_string(cycle_count));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_graph_info_.amp_freq_));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_graph_info_.stress_value_));
    sql_str.append(", ");
    sql_str.append(std::to_string(exp_data_graph_info_.amp_um_));
    sql_str.append(", ");
    sql_str.append(std::to_string((is_exp_state_ == kExpStateStart) ? 1 : 0));
    sql_str.append(", ");
    sql_str.append(std::to_string(date));
    sql_str.append(");");
    anx::db::helper::InsertDataTable(anx::db::helper::kDefaultDatabasePathname,
                                     anx::db::helper::kTableExpDataGraph,
                                     sql_str);
  }
}

void WorkWindowSecondPage::ProcessDataListModeLinear() {
  /// @note get the current time and calculate the time interval num
  int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
  /// @note check the time interval num and the sampling end pos, if the
  /// time interval num greater than the sampling end pos then return else
  /// check the time diff and the sampling end pos, if the time diff greater
  /// than the sampling end pos then return
  /// @warning sampling_start_end_diff_ms contain ultra 2000C device pause
  /// status time and resume status time.

  if (pre_exp_start_time_ms_ <= 0) {
    pre_exp_start_time_ms_ = exp_data_list_info_.exp_start_time_ms_;
  }
  int64_t time_ms_diff =
      current_time_ms - pre_exp_start_time_ms_ - pre_clip_paused_ms_;
  assert(dedss_ != nullptr);
  /// @note check the time diff and the sampling start pos, if the time diff
  /// less than the sampling start pos then return, else time diff minus the
  /// sampling start pos
  if (time_ms_diff < 0) {
    LOG_F(LG_WARN) << "time_ms_diff:" << time_ms_diff;
    return;
  }
  int64_t time_interval_num = 0;
  int64_t need_store_count = 0;
  int64_t need_store_count_left = 0;
  do {
    time_interval_num = time_ms_diff / 100;
    if (time_interval_num > exp_data_list_info_.exp_time_interval_num_) {
      int64_t diff_interval_num =
          time_interval_num - exp_data_list_info_.exp_time_interval_num_;
      need_store_count = diff_interval_num / dedss_->sampling_interval_;
      need_store_count_left = diff_interval_num % dedss_->sampling_interval_;
      int64_t cur_freq_count_100ms = static_cast<int64_t>(
          exp_data_list_info_.amp_freq_ * dedss_->sampling_interval_ / 10.f);
      for (int i = 0; i < need_store_count; i++) {
        exp_data_list_info_.exp_freq_total_count_ += cur_freq_count_100ms;
        exp_data_list_info_.exp_data_table_no_++;
        // 1. save to database
        // format cycle_count, KHz, MPa, um to the sql string and insert to
        // the database
        /*int64_t item_time_ms =
            exp_data_list_info_.exp_pre_sample_timestamp_ms_ *
            dedss_->sampling_interval_ * 100;*/
        ///
        StoreDataListItem(
            pre_total_cycle_count_ + exp_data_list_info_.exp_freq_total_count_,
            anx::common::GetCurrrentSystimeAsVarTime());
      }
      if (need_store_count > 0) {
        exp_data_list_info_.exp_time_interval_num_ = time_interval_num;
      }
    }
  } while (0);

  /// @note calculate the cycle count and update the args area,
  /// cycle_exp_clip_count that cantian the current cycle count and the exp
  /// clip count and the pre all cycle count from exp start action.
  if (need_store_count) {
    cur_total_cycle_count_ =
        pre_total_cycle_count_ + exp_data_list_info_.exp_freq_total_count_;
  } else {
    cur_total_cycle_count_ =
        pre_total_cycle_count_ + exp_data_list_info_.exp_freq_total_count_ +
        static_cast<int64_t>(
            (time_interval_num - exp_data_list_info_.exp_time_interval_num_) *
            exp_data_list_info_.amp_freq_ / 10);
  }
  this->pWorkWindow_->UpdateArgsArea(cur_total_cycle_count_);
  LOG_F(LG_SENSITIVE) << "cur_total_cycle_count_:" << cur_total_cycle_count_
                      << " pre_total_cycle_count_:" << pre_total_cycle_count_
                      << " exp_data_list_info_.exp_time_interval_num_:"
                      << exp_data_list_info_.exp_time_interval_num_
                      << " need_store_count:" << need_store_count
                      << " need_store_count_left:" << need_store_count_left;
  exp_data_list_info_.exp_pre_sample_timestamp_ms_ = current_time_ms;
}

void WorkWindowSecondPage::ProcessDataListModeExponential() {
  /// @note get the current time and calculate the time interval num
  int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
  /// @note check the time interval num and the sampling end pos, if the
  /// time interval num greater than the sampling end pos then return else
  /// check the time diff and the sampling end pos, if the time diff greater
  /// than the sampling end pos then return
  /// @warning sampling_start_end_diff_ms contain ultra 2000C device pause
  /// status time and resume status time.

  if (pre_exp_start_time_ms_ <= 0) {
    pre_exp_start_time_ms_ = exp_data_list_info_.exp_start_time_ms_;
  }
  int64_t time_ms_diff =
      current_time_ms - pre_exp_start_time_ms_ - pre_clip_paused_ms_;
  assert(dedss_ != nullptr);
  /// @note check the time diff and the sampling start pos, if the time diff
  /// less than the sampling start pos then return, else time diff minus the
  /// sampling start pos
  if (time_ms_diff < 0) {
    LOG_F(LG_WARN) << "time_ms_diff:" << time_ms_diff;
    return;
  }
  int64_t time_interval_num = 0;
  int64_t need_store_count = 0;
  do {
    time_interval_num = time_ms_diff / 100;
    if (time_interval_num > exp_data_list_info_.exp_time_interval_num_) {
      int64_t diff_interval_num =
          time_interval_num - exp_data_list_info_.exp_time_interval_num_;
      need_store_count = diff_interval_num;
      int64_t cur_freq_count_100ms =
          static_cast<int64_t>(exp_data_list_info_.amp_freq_ / 10.f);
      for (int i = 0; i < need_store_count; i++) {
        exp_data_list_info_.exp_freq_total_count_ += cur_freq_count_100ms;

        ///////////////////////////////////////////////////////////////////////
        /// @note check the time interval num for sample mode Exponent 10^n
        /// check n is 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
        /// 100000000 then update the data to the database, else return.
        {
          if (exp_data_pre_duration_exponential_ == 0) {
            if (cur_total_cycle_count_ > 0) {
              exp_data_pre_duration_exponential_ =
                  static_cast<int64_t>(exp_data_list_info_.amp_freq_);
            } else {
              break;
            }
          } else if ((cur_total_cycle_count_ <
                      exp_data_pre_duration_exponential_) != 0) {
            break;
          } else {
            exp_data_pre_duration_exponential_ *= 10;
          }
        }
        exp_data_list_info_.exp_data_table_no_++;
        {
          int multiplier = 1;
          for (int i = 1; i < exp_data_list_info_.exp_data_table_no_; i++) {
            multiplier *= 10;
          }
          int64_t cycle_count = static_cast<int64_t>(
              multiplier * exp_data_list_info_.amp_freq_ / 10.f);
          LOG_F(LG_SENSITIVE)
              << "exp data list: cycle_count:" << cycle_count
              << " cur_total_cycle_count_:" << cur_total_cycle_count_
              << " exp_time_interval_num_:" << time_interval_num
              << " exp_data_table_no_:"
              << exp_data_list_info_.exp_data_table_no_
              << " cur_freq_:" << cur_freq_;
          // 1. save to database
          // format cycle_count, KHz, MPa, um to the sql string and insert to
          // the database
          StoreDataListItem(cycle_count,
                            anx::common::GetCurrrentSystimeAsVarTime());
        }
        ///////////////////////////////////////////////////////////////////////
      }
      if (need_store_count > 0) {
        exp_data_list_info_.exp_time_interval_num_ = time_interval_num;
      }
    }
  } while (0);

  /// @note calculate the cycle count and update the args area,
  /// cycle_exp_clip_count that cantian the current cycle count and the exp
  /// clip count and the pre all cycle count from exp start action.
  if (need_store_count) {
    cur_total_cycle_count_ =
        pre_total_cycle_count_ + exp_data_list_info_.exp_freq_total_count_;
  } else {
    cur_total_cycle_count_ =
        pre_total_cycle_count_ + exp_data_list_info_.exp_freq_total_count_ +
        static_cast<int64_t>(
            (time_interval_num - exp_data_list_info_.exp_time_interval_num_) *
            exp_data_list_info_.amp_freq_ / 10);
  }
  this->pWorkWindow_->UpdateArgsArea(cur_total_cycle_count_);
  LOG_F(LG_SENSITIVE) << "cur_total_cycle_count_:" << cur_total_cycle_count_
                      << " pre_total_cycle_count_:" << pre_total_cycle_count_
                      << " exp_data_list_info_.exp_time_interval_num_:"
                      << exp_data_list_info_.exp_time_interval_num_;
  exp_data_list_info_.exp_pre_sample_timestamp_ms_ = current_time_ms;
}

void WorkWindowSecondPage::StoreDataListItem(int64_t cycle_count, double date) {
  std::string sql_str = ("INSERT INTO ");
  sql_str.append(anx::db::helper::kTableExpDataList);
  sql_str.append((" (cycle, KHz, MPa, um, date) VALUES ("));
  sql_str.append(std::to_string(cycle_count));
  sql_str.append(", ");
  sql_str.append(anx::common::to_string_with_precision(
      exp_data_list_info_.amp_freq_ * 0.001, 3));
  sql_str.append(", ");
  sql_str.append(std::to_string(exp_data_list_info_.stress_value_));
  sql_str.append(", ");
  sql_str.append(std::to_string(exp_data_list_info_.amp_um_));
  sql_str.append(", ");
  sql_str.append(std::to_string(date));
  sql_str.append(");");
  anx::db::helper::InsertDataTable(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableExpDataList, sql_str);
}
}  // namespace ui
}  // namespace anx
