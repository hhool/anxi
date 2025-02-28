/**
 * @file work_window_tab_main_second_page_graph.h
 * @author hhool (hhool@outlook.com)
 * @brief work window second page graph ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_H_

#include "app/ui/work_window_tab_main_second_page_base.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "app/device/device_com.h"
#include "app/device/ultrasonic/ultra_device.h"
#include "app/ui/ui_virtual_wnd_base.h"
#include "app/ui/work_window_tab_main_second_page_graph_ctrl.h"
#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace device {
class DeviceComInterface;
class DeviceComListener;
}  // namespace device
namespace esolution {
class SolutionDesign;
}  // namespace esolution
namespace ui {
class WorkWindow;
class WorkWindowSecondWorkWindowSecondPageGraphCtrl;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {

extern const int32_t kGraphCtrlSampleTotalMinutesOne;
extern const int32_t kGraphCtrlSampleTotalMinutesFive;
extern const int32_t kGraphCtrlSampleTotalMinutesTen;
extern const int32_t kGraphCtrlSampleTotalMinutesThirty;
extern const int32_t kGraphCtrlSampleTotalMinutesSixty;

class WorkWindowSecondPageGraph : public DuiLib::CNotifyPump,
                                  public DuiLib::INotifyUI,
                                  public UIVirtualWndBase,
                                  public anx::device::DeviceComListener {
 public:
  WorkWindowSecondPageGraph(WorkWindow* pWorkWindow,
                            DuiLib::CPaintManagerUI* paint_manager_ui,
                            ExpDataInfo* exp_base);
  ~WorkWindowSecondPageGraph() override;

 public:
  void Notify(TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);         // NOLINT
  void OnValueChanged(TNotifyUI& msg);  // NOLINT

 protected:
  bool OnChkGraphAlwaysShowNewChange(void* param);
  bool OnOptGraphTimeRangeChange(void* param);
  bool OnPagePre(void* param);
  bool OnPageNext(void* param);
  bool OnTimer(void* param);

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 public:
  void ClearGraphData();

 protected:
  void UpdateGraphCtrl(
      std::string name,
      const std::vector<std::map<std::string, std::string>>& result);
  void RefreshExpGraphTitleControl(double vartime);
  void RefreshPreNextAlwaysShowNewControl(bool is_first_page,
                                          bool is_last_page);
  void RefreshPreNextControl();
  void UpdateControlFromSettings();
  void SaveSettingsFromControl();

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

 protected:
  void OnExpStart();
  void OnExpStop();
  void OnExpPause() {
    // TODO(hhool): do nothing
  }
  void OnExpResume();

  class GraphCtrlEvent;

 private:
  WorkWindow* pWorkWindow_;
  std::unique_ptr<GraphCtrlEventInterface> graph_ctrl_event_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul_;
  anx::device::UltraDevice* ultra_device_;
  /// @brief exp related members
  ExpDataInfo* exp_data_graph_info_;
  /// @brief amp start time, stress start time.
  int64_t exp_time_interval_num_ = -1;
  /// @brief y axsi for amptitude value unused or not.
  bool y_axsi_amp_value_unused_ = false;
  /// @brief y axsi max value for amptitude.
  int32_t y_axsi_amp_value_max_ = 10;
  /// @brief y axsi min value for amptitude.
  int32_t y_axsi_amp_value_min_ = 0;

  /// @brief y axsi for stress value unused or not.
  bool y_axsi_stload_value_unused_ = false;
  /// @brief y axsi max value for stress.
  int32_t y_axsi_stload_value_max_ = 10;
  /// @brief y axsi min value for stress.
  int32_t y_axsi_stload_value_min_ = 0;

  /// @brief graph time alway show new
  DuiLib::CCheckBoxUI* chk_graph_always_show_new_;

  /// @brief graph time range option
  DuiLib::COptionUI* opt_graph_time_range_1_minitues_;
  DuiLib::COptionUI* opt_graph_time_range_5_minitues_;
  DuiLib::COptionUI* opt_graph_time_range_10_minitues_;
  DuiLib::COptionUI* opt_graph_time_range_30_minitues_;
  DuiLib::COptionUI* opt_graph_time_range_60_minitues_;
  int32_t graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesOne;

  /// @brief graph amplitude title and canvas
  DuiLib::CButtonUI* btn_graph_amplitude_title_;
  DuiLib::CButtonUI* btn_graph_amplitude_title_detail_;
  DuiLib::CButtonUI* btn_graph_amplitude_canvas_;

  /// @brief graph stress title and canvas
  DuiLib::CButtonUI* btn_graph_stress_title_;
  DuiLib::CButtonUI* btn_graph_stress_title_detail_;
  DuiLib::CButtonUI* btn_graph_stress_canvas_;

  /// @brief graph amplitude control for the page
  std::unique_ptr<WorkWindowSecondWorkWindowSecondPageGraphCtrl>
      page_graph_amplitude_ctrl_;
  std::unique_ptr<WorkWindowSecondWorkWindowSecondPageGraphCtrl>
      page_graph_stress_ctrl_;

  /// pre and next page button
  /// @brief pre page button
  DuiLib::CButtonUI* btn_pre_page_;
  /// @brief next page button
  DuiLib::CButtonUI* btn_next_page_;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_H_
