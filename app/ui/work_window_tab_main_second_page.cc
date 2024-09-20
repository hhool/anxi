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
#include <vector>

#include "app/common/defines.h"
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
      is_exp_state_(-1) {
  WorkWindowSecondPageGraph* graph_page = new WorkWindowSecondPageGraph(
      pWorkWindow, paint_manager_ui, &exp_data_info_);
  work_window_second_page_graph_virtual_wnd_ = graph_page;
  work_window_second_page_graph_notify_pump_.reset(graph_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPageGraph"),
                      work_window_second_page_graph_notify_pump_.get());

  WorkWindowSecondPageData* data_page = new WorkWindowSecondPageData(
      pWorkWindow, paint_manager_ui, &exp_data_info_);
  work_window_second_page_data_virtual_wnd_ = data_page;
  work_window_second_page_data_notify_pump_.reset(data_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPageData"),
                      work_window_second_page_data_notify_pump_.get());
}

WorkWindowSecondPage::~WorkWindowSecondPage() {
  paint_manager_ui_->KillTimer(btn_exp_start_, 1);
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
      // TODO(hhool): add down action
      OnButtonStaticAircraftDown();
    } else if (msg.pSender == this->btn_sa_stop_) {
      // TODO(hhool): add stop action
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
        {
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
        {
          uint8_t hex[64];
          int read = device_com_ul_->Read(hex, sizeof(hex));
          if (read < 0) {
            std::cout << "ul read error:" << read;
          }
        }
        /////////////////////////////////////////////////////////////////////////
        /// get current cycle count and total time  cycle_count / x kHZ
        int64_t exp_max_cycle_count = dus_.exp_max_cycle_count_;
        for (int32_t i = 0; i < dus_.exp_max_cycle_power_; i++) {
          exp_max_cycle_count *= 10;
        }
        int64_t total_des_time_ms = exp_max_cycle_count / 20;
        int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
        int64_t duration = current_time_ms - exp_data_info_.exp_start_time_ms_;
        if (duration >= total_des_time_ms) {
          LOG_F(LG_INFO) << "exp stop";
          exp_stop();
        }
        /// exp clipping enabled
        if (this->dus_.exp_clipping_enable_ == 1) {
          if (this->dus_.exp_clip_time_duration_ > 0) {
            int64_t exp_clip_time_duration =
                this->dus_.exp_clip_time_duration_ * 100;
            int64_t exp_clip_time_paused =
                this->dus_.exp_clip_time_paused_ * 100;
            int64_t duration =
                current_time_ms - exp_data_info_.exp_start_time_ms_;
            int64_t duration_total =
                exp_clip_time_duration + exp_clip_time_paused;
            int64_t time = duration % duration_total;
            if (time >= exp_clip_time_duration &&
                state_ultrasound_exp_clip_ == 1) {
              // pause ultrasound
              LOG_F(LG_INFO) << "pause ultrasound";
              uint8_t hex[8] = {0x01, 0x05, 0x00, 0x02, 0x00, 0x00, 0x6C, 0x0A};
              device_com_ul_->Write(hex, sizeof(hex));
              state_ultrasound_exp_clip_ = 2;
            } else if (time < exp_clip_time_duration &&
                       state_ultrasound_exp_clip_ == 2) {
              // resume ultrasound
              LOG_F(LG_INFO) << "resume ultrasound";
              uint8_t hex[8] = {0x01, 0x05, 0x00, 0x02, 0x00, 0x01, 0x2D, 0x0A};
              device_com_ul_->Write(hex, sizeof(hex));
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

  // create or get ultrasound device.
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);

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
  if (dus_.exp_clip_time_duration_ != exp_clip_time_duration) {
    dus_.exp_clip_time_duration_ = exp_clip_time_duration;
    std::string value = ("=");
    value += format_num(exp_clip_time_duration * 100);
    value += "S";
    text_exp_clip_time_duration_->SetText(
        anx::common::string2wstring(value).c_str());
  }

  int64_t exp_clip_time_paused = _ttoll(edit_exp_clip_time_paused_->GetText());
  if (dus_.exp_clip_time_paused_ != exp_clip_time_paused) {
    dus_.exp_clip_time_paused_ = exp_clip_time_paused;
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
  value += format_num(exp_clip_time_duration * 100);
  value += "S";
  text_exp_clip_time_duration_->SetText(
      anx::common::string2wstring(value).c_str());
  // exp_clip_time_paused append "S"
  value = ("=");
  value += format_num(exp_clip_time_paused * 100);
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

  UpdateUIWithExpStatus(1);

  UpdateExpClipTimeFromControl();

  SaveSettingsFromControl();

  is_exp_state_ = 1;

  /// @brief reset the database exp_data table.
  /// @note the table name is exp_data, delete exp_data table and create a new
  /// one.
  anx::db::helper::DropDataTable(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpData);

  std::string db_filepathname;
  anx::db::helper::DefaultDatabasePathname(&db_filepathname);
  auto db = anx::db::DatabaseFactory::Instance()->CreateOrGetDatabase(
      db_filepathname);
  db->Execute(anx::db::helper::sql::kCreateTableAmpSqlFormat);

  /// @brief get the exp data sample settings and set the exp start time
  /// and exp sample interval
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings> dedss =
      anx::device::LoadDeviceExpDataSampleSettingsDefaultResource();
  exp_data_info_.exp_data_table_no_ = 0;
  exp_data_info_.exp_time_interval_num_ = 0;
  exp_data_info_.exp_start_time_ms_ = anx::common::GetCurrentTimeMillis();
  exp_data_info_.exp_sample_interval_ms_ = dedss->sampling_interval_ * 100;

  // TODO(hhool): sample interval
  // set the ultrasound on state
  uint8_t hex[8] = {0x01, 0x05, 0x00, 0x02, 0xFF, 0x00, 0x2D, 0xFA};
  device_com_ul_->Write(hex, sizeof(hex));
  paint_manager_ui_->SetTimer(btn_exp_start_, kTimerIdSampling,
                              kSamplingInterval);
  state_ultrasound_exp_clip_ = 1;

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
}

void WorkWindowSecondPage::OnButtonStaticAircraftClear() {
  // clear the data
  // TODO(hhool): clear the data
  if (is_exp_state_) {
    return;
  }
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
  // TODO(hhool): add reset action
  this->pWorkWindow_->ClearArgsFreqNum();
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
  int machineType = 4;
  int nDTCType = 2;
  int nCommport = 4;
  int nChannelNo = 2;
  int rate = 30;
  int sensorPosition = 0;
  int TestSpace = 0;
  int nDataBlockSize = 2;
  bool isAE = false;
  float speed = 2.0f / 60.0f;

  // 力传感器P值
  int lLoad_P = 20;
  int lLoad_I = 0;
  int lLoad_D = 0;

  // 位移传感器P值
  int lPosi_P = 20;
  int lPosi_I = 0;
  int lPosi_D = 0;

  // 引伸计P值
  int lExt_P = 30;
  int lExt_I = 0;
  int lExt_D = 0;
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("静载机配置文件加载失败"),
               _T("错误"), MB_OK);
    return;
  }
  if (lss->direct_ == 1) {
    // up
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(1);
  } else if (lss->direct_ == 2) {
    // down
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(0);
  } else {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("静载机配置文件方向配置错误"),
               _T("错误"), MB_OK);
  }
  speed = lss->speed_ / 60.0f;
  BOOL bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.on_line(
          nChannelNo, 0, 0, 0, rate, machineType, nDTCType, sensorPosition,
          TestSpace, nDataBlockSize, isAE)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"on_line_error", L"on_line",
               MB_OK);
    return;
  }

  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_LOAD, lLoad_P, lLoad_I, lLoad_D)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid-CH_LOAD_error",
               L"carry_pid-CH_LOAD", MB_OK);
    return;
  }

  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_POSI, lPosi_P, lPosi_I, lPosi_D)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid-CH_POSI_error",
               L"carry_pid-CH_POSI", MB_OK);
    return;
  }

  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_EXTN, lExt_P, lExt_I, lExt_D)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid-CH_EXTN_error",
               L"carry_pid-CH_EXTN", MB_OK);
    return;
  }
  int ret = MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid",
                       L"carry_pid", MB_OKCANCEL);
  if (ret == IDOK) {
    // carry_pid
  } else {
    return;
  }
  /// RUN the static load, the direction is up and the speed is 2.0f / 60.0f
  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_200(
          2, 2, speed, 5, 1, true, DIR_NO, 0, 1, 0)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("carry_200_error"),
               _T("carry_200"), MB_OK);
  }

  st_load_is_running_ = true;
}

void WorkWindowSecondPage::OnButtonStaticAircraftDown() {
  int machineType = 4;
  int nDTCType = 2;
  int nCommport = 4;
  int nChannelNo = 2;
  int rate = 30;
  int sensorPosition = 0;
  int TestSpace = 0;
  int nDataBlockSize = 2;
  bool isAE = false;
  float speed = 2.0f / 60.0f;

  // 力传感器P值
  int lLoad_P = 20;
  int lLoad_I = 0;
  int lLoad_D = 0;

  // 位移传感器P值
  int lPosi_P = 20;
  int lPosi_I = 0;
  int lPosi_D = 0;

  // 引伸计P值
  int lExt_P = 30;
  int lExt_I = 0;
  int lExt_D = 0;
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("静载机配置文件加载失败"),
               _T("错误"), MB_OK);
    return;
  }
  if (lss->direct_ == 1) {
    // up
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(1);
  } else if (lss->direct_ == 2) {
    // down
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(0);
  } else {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("静载机配置文件方向配置错误"),
               _T("错误"), MB_OK);
  }
  speed = lss->speed_ / 60.0f;
  BOOL bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.on_line(
          nChannelNo, 0, 0, 0, rate, machineType, nDTCType, sensorPosition,
          TestSpace, nDataBlockSize, isAE)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"on_line_error", L"on_line",
               MB_OK);
    return;
  }

  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_LOAD, lLoad_P, lLoad_I, lLoad_D)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid-CH_LOAD_error",
               L"carry_pid-CH_LOAD", MB_OK);
    return;
  }

  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_POSI, lPosi_P, lPosi_I, lPosi_D)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid-CH_POSI_error",
               L"carry_pid-CH_POSI", MB_OK);
    return;
  }

  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_EXTN, lExt_P, lExt_I, lExt_D)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid-CH_EXTN_error",
               L"carry_pid-CH_EXTN", MB_OK);
    return;
  }
  int ret = MessageBox(this->pWorkWindow_->GetHWND(), L"carry_pid",
                       L"carry_pid", MB_OKCANCEL);
  if (ret == IDOK) {
    // carry_pid
  } else {
    return;
  }
  /// RUN the static load, the direction is up and the speed is 2.0f / 60.0f
  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_200(
          2, 2, speed, 5, 1, true, DIR_NO, 0, 1, 0)
          ? true
          : false;
  if (!bSuccess) {
    MessageBox(this->pWorkWindow_->GetHWND(), _T("carry_200_error"),
               _T("carry_200"), MB_OK);
  }

  st_load_is_running_ = true;
}

void WorkWindowSecondPage::OnButtonStaticAircraftStop() {
  if (st_load_is_running_) {
    // anx::device::stload::STLoadHelper::st_load_loader_.st_api_.end_read();
    anx::device::stload::STLoadHelper::st_load_loader_.st_api_.stop_run();
    st_load_is_running_ = false;
  }
}

void WorkWindowSecondPage::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool): process data
  if (is_exp_state_) {
    std::string hex_str;
    if (device == device_com_ul_.get()) {
      // process the data from ultrasound device
      // 1. parse the data
      // 2. update the data to the graph
      // 3. update the data to the data table
      // output the data as hex to the std::string
      hex_str = anx::common::ByteArrayToHexString(data, size);
      std::cout << hex_str << std::endl;
    }
    int64_t current_time_ms = anx::common::GetCurrentTimeMillis();
    int64_t time_diff = current_time_ms - exp_data_info_.exp_start_time_ms_;
    int64_t time_interval_num =
        time_diff / exp_data_info_.exp_sample_interval_ms_;
    if (time_interval_num > exp_data_info_.exp_time_interval_num_) {
      exp_data_info_.exp_time_interval_num_ = time_interval_num;
      exp_data_info_.exp_data_table_no_++;
      // update the data to the database table amp, stress, um
      // TODO(hhool):
      int64_t cycle_count = exp_data_info_.exp_data_table_no_ * 500;
      double KHz = 208.230f * kMultiFactor;
      // TODO(hhool): random KHz, um, MPa
      double um = (rand() % 15) * kMultiFactor;
      double MPa = (rand() % 8) * kMultiFactor;
      double date = anx::common::GetCurrrentDateTime() * kMultiFactor;
      exp_data_info_.amp_freq_ = KHz;
      exp_data_info_.amp_um_ = um;
      exp_data_info_.stress_value_ = MPa;
      // TODO(hhool): save to database
      // format cycle_count, KHz, MPa, um to the sql string and insert to the
      // database
      std::string sql_str = ("INSERT INTO ");
      sql_str.append(anx::db::helper::kTableExpData);
      sql_str.append((" (cycle, KHz, MPa, um, date) VALUES ("));
      sql_str.append(std::to_string(cycle_count));
      sql_str.append(", ");
      sql_str.append(std::to_string(KHz));
      sql_str.append(", ");
      sql_str.append(std::to_string(MPa));
      sql_str.append(", ");
      sql_str.append(std::to_string(um));
      sql_str.append(", ");
      sql_str.append(std::to_string(date));
      sql_str.append(");");
      LOG_F(LG_INFO) << "\r\n";
      LOG_F(LG_INFO) << "no:" << exp_data_info_.exp_data_table_no_ << " "
                     << "cycle:" << cycle_count << " " << "um:" << um << " "
                     << "MPa:" << MPa << " " << "KHz:" << KHz << " "
                     << "sql:" << sql_str;
      anx::db::helper::InsertDataTable(
          anx::db::helper::kDefaultDatabasePathname,
          anx::db::helper::kTableExpData, sql_str);
    }
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
  }
}

}  // namespace ui
}  // namespace anx
