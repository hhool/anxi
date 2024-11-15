/**
 * @file work_window_tab_main_second_page.h
 * @author hhool (hhool@outlook.com)
 * @brief work window second page ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_H_

#include "app/ui/work_window_tab_main_second_page_base.h"

#include <map>
#include <memory>
#include <string>

#include "app/device/device_com.h"
#include "app/device/device_exp_load_static_settings.h"
#include "app/device/device_exp_ultrasound_settings.h"
#include "app/device/stload/stload_helper.h"
#include "app/device/ultrasonic/ultra_device.h"
#include "app/ui/ui_virtual_wnd_base.h"

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace device {
class DeviceComInterface;
class DeviceComListener;
class DeviceExpDataSampleSettings;
}  // namespace device
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {

class WorkWindowSecondPage : public DuiLib::CNotifyPump,
                             public UIVirtualWndBase,
                             public anx::device::DeviceComListener {
 public:
  WorkWindowSecondPage(WorkWindow* pWorkWindow,
                       DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowSecondPage();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);         // NOLINT
  void OnTimer(TNotifyUI& msg);         // NOLINT
  void OnValueChanged(TNotifyUI& msg);  // NOLINT
  bool OnExpClipChanged(void* msg);

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 public:
  /// @brief static load aircraft up
  void OnButtonStaticAircraftUp();
  /// @brief static load aircraft down
  void OnButtonStaticAircraftDown();
  /// @brief static load aircraft stop
  void OnButtonStaticAircraftStop();
  /// @brief static load aircraft clear
  void OnButtonStaticAircraftClear();
  /// @brief static load aircraft setting
  void OnButtonStaticAircraftKeepLoad();
  /// @brief static load aircraft reset
  void OnButtonExpReset();
  void OnExpStart();
  void OnExpStop();
  void OnExpPause();
  void OnExpResume();

 protected:
  void CheckDeviceComConnectedStatus();
  void RefreshExpClipTimeControl();
  void UpdateControlFromSettings();
  void SaveExpClipSettingsFromControl();
  void UpdateExpClipTimeFromControl();

 protected:
  int32_t exp_start();
  /// @brief exp pause
  /// @note if the exp is not started, then do nothing
  /// if the exp is started, then pause the exp and update the button state
  /// stop the ultrasound hard ware and update the button state
  void exp_pause();
  void exp_stop();
  void exp_resume();
  /// @brief static load aircraft do move up
  bool StaticAircraftDoMoveUp();
  /// @brief static load aircraft do move down
  bool StaticAircraftDoMoveDown();
  /// @brief static load aircraft stop
  bool StaticAircraftStop();
  /// @brief get the max exp cycle count from the control
  int64_t MaxExpCycleCountFromControl();
  /// @brief  Update buttons state with device connected status
  /// work state, exp state, static aircraft states
  void UpdateUIButton();

 protected:
  // impliment anx::device::DeviceComListener;
  void OnDataReceived(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override;
  void OnDataOutgoing(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override {
    // TODO(hhool): do nothing
  }
  void ProcessDataGraph();
  void ProcessDataListModeLinear();
  void ProcessDataListModeExponential();
  void StoreDataListItem(int64_t cycle_count, double date_time);

 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::unique_ptr<DuiLib::CNotifyPump>
      work_window_second_page_graph_notify_pump_;
  UIVirtualWndBase* work_window_second_page_graph_virtual_wnd_;
  std::unique_ptr<DuiLib::CNotifyPump>
      work_window_second_page_data_notify_pump_;
  UIVirtualWndBase* work_window_second_page_data_virtual_wnd_;
  //////////////////////////////////////////////////////////////////////////
  /// @brief ultrasound exp state related
  int32_t is_exp_state_ = kExpStateUnvalid;
  int32_t exp_pause_stop_reason_ = kExpPauseStopReasonNone;
  int32_t user_exp_state_ = kExpStateUnvalid;
  anx::device::UltraDevice* ultra_device_;
  //////////////////////////////////////////////////////////////////////////
  /// @brief ultrasound exp start working initial frequency and power,
  /// used to detect frequency and power change, if the frequency over the
  /// initial frequency at
  /// DeviceUltrasoundSettings::exp_frequency_fluctuations_range_ value, then
  /// stop the exp.
  int32_t initial_frequency_;
  /// @brief ultrasound exp start working initial frequency and power,
  /// only record the power value.
  int32_t initial_power_;
  /// @brief ultrasound current frequency
  int32_t cur_freq_;
  /// @brief ultrasound current power
  int32_t cur_power_;
  //////////////////////////////////////////////////////////////////////////
  /// @brief ultrasound current total cycle count
  /// if the value is -1, then the exp is not started
  /// if the value is >= 0, then the exp is started
  /// cur_total_cycle_count_ will increase with the cycle count value and
  /// increase with pre_total_cycle_count_ value and current exp cycle count
  /// value
  int64_t cur_total_cycle_count_ = -1;
  /// @brief pre cycle count value for record total cycle count value
  /// when the exp is paused. then the cycle count value will not increase
  /// and the pre cycle count value will record the last cycle count value
  int64_t pre_exp_start_time_ms_ = 0;
  int64_t pre_total_cycle_count_ = 0;
  /// @brief pre total data table no for record the last data table no
  /// when the exp is paused. then the data table no will not increase.
  /// table related to the exp start time and exp stop time. database table
  /// name is exp_data_list_
  int32_t pre_total_data_table_no_ = 0;

  anx::device::DeviceUltrasoundSettings dus_;
  double exp_amplitude_;
  double exp_statc_load_mpa_;
  double exp_max_stress_MPa_;
  /// @note state_ultrasound_exp_clip_ for exp clip time control
  /// 0 - stop, 1 - start, 2 - pause, <0 - unvalid
  int32_t state_ultrasound_exp_clip_;
  ExpDataInfo exp_data_graph_info_;
  ExpDataInfo exp_data_list_info_;
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings> dedss_;
  int64_t exp_data_pre_duration_exponential_ = 0;
  int64_t pre_clip_paused_ms_ = 0;
  bool start_time_pos_has_deal_ = false;

  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss_;
  /// @note load achieve target keep duration time in ms
  /// 20s default value
  int64_t st_load_achieve_target_keep_duration_ms_ = 200 * 1000;
  /// @note record first time achieve the target load time point in ms
  /// value -1 is not achieve the target load, value > 0 is achieve the
  /// target load first time point.
  int64_t st_load_achieve_target_time_ = -1;
  /// @note load keep load value actual state.
  /// value -1 is not achieve the target load, value == 1 is achieve the target
  /// load first time point. will detect the load value with target load value
  /// periodically. if the load value is not equal to the target load value,
  /// then keep do the action for target load value. eg up or down.
  int64_t st_load_keep_load_ = -1;
  /// @note Aircraft static load event from who.
  /// value -1 no event from, value 0 is from button (up and down), value 1 is
  /// from button (keep load button)
  enum {
    kSTLoadEventNone = -1,
    kSTLoadEventFromButtonUpDown = 0,
    kSTLoadEventFromKeepLoadButton = 1
  };
  int32_t st_load_event_from_ = kSTLoadEventNone;
  /// @brief static load aircraft position reach first time point in ms
  /// value -1 is not reach the first time point, value > 0 is reach the first
  /// time point. delay 1000ms for the aircraft stop action.
  int64_t st_posi_reach_first_time_ = 0;
  DuiLib::CTabLayoutUI* btn_tablayout_;
  DuiLib::CButtonUI* btn_tab_graph_;
  DuiLib::CButtonUI* btn_tab_data_;

  /// @brief Static aircraft releated button
  ///
  /// @brief Static aircraft clear button
  DuiLib::CButtonUI* btn_sa_clear_;
  /// @brief Static aircraft setting button
  DuiLib::CButtonUI* btn_sa_keep_load_;
  /// @brief Static aircraft reset button
  DuiLib::CButtonUI* btn_exp_reset_;

  bool st_load_is_running_ = false;
  int64_t st_start_time_ = 0;
  int64_t st_run_time_ = 0;
  anx::device::stload::STResult st_load_result_;
  /// @brief exp action releated button
  ///
  /// @brief exp action start button
  DuiLib::CButtonUI* btn_exp_start_;
  /// @brief exp action stop button
  DuiLib::CButtonUI* btn_exp_stop_;
  /// @brief exp action pause button
  DuiLib::CButtonUI* btn_exp_pause_;
  /// @brief exp action resume button
  DuiLib::CButtonUI* btn_exp_resume_;
  /// @brief exp pause layout
  DuiLib::CHorizontalLayoutUI* layout_exp_pause_;
  /// @brief exp resume layout
  DuiLib::CHorizontalLayoutUI* layout_exp_resume_;

  /// @brief Static aircraft action releated button
  ///
  /// @brief Static aircraft action up button
  DuiLib::CButtonUI* btn_sa_up_;
  /// @brief Static aircraft action down button
  DuiLib::CButtonUI* btn_sa_down_;
  /// @brief Static aircraft action stop button
  DuiLib::CButtonUI* btn_sa_stop_;

  /// @brief amplitude align releated button
  ///
  /// @brief amplitude align setting button
  DuiLib::CButtonUI* btn_aa_setting_;

  /// @brief exp clip set checkbox
  DuiLib::CCheckBoxUI* chk_exp_clip_set_;
  DuiLib::CEditUI* edit_exp_clip_time_duration_;
  DuiLib::CEditUI* edit_exp_clip_time_paused_;
  DuiLib::CTextUI* text_exp_clip_time_duration_;
  DuiLib::CTextUI* text_exp_clip_time_paused_;

  /// @brief max cycle count edit
  DuiLib::CEditUI* edit_max_cycle_count_;
  DuiLib::CEditUI* edit_max_cycle_power_;
  DuiLib::CEditUI* edit_frequency_fluctuations_range_;
  DuiLib::CTextUI* text_max_cycle_duration_;

  /// @brief exp data sample settings
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_H_
