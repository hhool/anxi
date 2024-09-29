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
#include "app/device/device_exp_ultrasound_settings.h"
#include "app/device/stload/stload_helper.h"
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

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 protected:
  void CheckDeviceComConnectedStatus();
  void RefreshExpClipTimeControl();
  void UpdateControlFromSettings();
  void SaveSettingsFromControl();
  void UpdateExpClipTimeFromControl();

 protected:
  int32_t exp_start();
  void exp_pause();
  void exp_stop();
  void exp_resume();

  /// @brief static load aircraft clear
  void OnButtonStaticAircraftClear();
  /// @brief static load aircraft setting
  void OnButtonStaticAircraftSetting();
  /// @brief static load aircraft reset
  void OnButtonStaticAircraftReset();
  /// @brief static load aircraft up
  void OnButtonStaticAircraftUp();
  /// @brief static load aircraft down
  void OnButtonStaticAircraftDown();
  /// @brief static load aircraft stop
  void OnButtonStaticAircraftStop();

  /// @brief  Update button with exp status
  /// @param status  0 - stop, 1 - start, 2 - pause
  void UpdateUIWithExpStatus(int status);

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
  void ProcessDataList();
 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  // std::map<std::string, std::unique_ptr<DuiLib::CNotifyPump>>
  // tab_second_pages_;
  std::unique_ptr<DuiLib::CNotifyPump>
      work_window_second_page_graph_notify_pump_;
  UIVirtualWndBase* work_window_second_page_graph_virtual_wnd_;
  std::unique_ptr<DuiLib::CNotifyPump>
      work_window_second_page_data_notify_pump_;
  UIVirtualWndBase* work_window_second_page_data_virtual_wnd_;
  /// @brief exp status
  /// 0 - stop, 1 - start, 2 - pause, <0 - unvalid
  int32_t is_exp_state_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul_;
  anx::device::DeviceUltrasoundSettings dus_;
  /// @note state_ultrasound_exp_clip_ for exp clip time control
  /// 0 - stop, 1 - start, 2 - pause, <0 - unvalid
  int32_t state_ultrasound_exp_clip_;
  ExpDataInfo exp_data_graph_info_;
  ExpDataInfo exp_data_list_info_;
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings> dedss_;

  DuiLib::CTabLayoutUI* btn_tablayout_;
  DuiLib::CButtonUI* btn_tab_graph_;
  DuiLib::CButtonUI* btn_tab_data_;

  /// @brief Static aircraft releated button
  ///
  /// @brief Static aircraft clear button
  DuiLib::CButtonUI* btn_sa_clear_;
  /// @brief Static aircraft setting button
  DuiLib::CButtonUI* btn_sa_setting_;
  /// @brief Static aircraft reset button
  DuiLib::CButtonUI* btn_sa_reset_;

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

  /// @brief exp data sample settings
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_H_
