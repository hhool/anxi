/**
 * @file work_window_tab_main_second_page_data.h
 * @author hhool (hhool@outlook.com)
 * @brief work window second page data ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_DATA_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_DATA_H_

#include <time.h>
#include <memory>
#include <string>
#include <vector>

#include "app/device/device_com.h"
#include "app/expdata/experiment_data_base.h"
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
class WorkWindowSecondPageData : public DuiLib::CNotifyPump,
                                 public DuiLib::INotifyUI,
                                 public UIVirtualWndBase,
                                 public anx::device::DeviceComListener {
 public:
  WorkWindowSecondPageData(WorkWindow* pWorkWindow,
                           DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowSecondPageData();

 public:
  void Notify(TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT

 protected:
  bool OnTimer(void* param);

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 protected:
  void RefreshSampleTimeControl();
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings>
  UpdateExpClipTimeFromControl();
  void UpdateControlFromSettings();
  void SaveSettingsFromControl();
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
                      int32_t size) override;

 protected:
  void OnExpStart();
  void OnExpStop();
  void OnExpPause();
  void OnExpResume();

 protected:
  friend class WorkWindowSecondPage;
  void ClearExpData();

 protected:
  class ListVirtalDataView;

 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_sl_;
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings>
      device_exp_data_settings_;

  /// @brief sample mode option button
  DuiLib::COptionUI* option_sample_mode_exp_;
  DuiLib::COptionUI* option_sample_mode_linear_;
  /// @brief sample start pos edit
  DuiLib::CEditUI* edit_sample_start_pos_;
  /// @brief sample end pos edit
  DuiLib::CEditUI* edit_sample_end_pos_;
  /// @brief sample time duration edit
  DuiLib::CEditUI* edit_sample_interval_;
  /// @brief sample interval text
  DuiLib::CTextUI* text_sample_interval_;

  /// @brief data list control
  DuiLib::CListUI* list_data_;
  std::unique_ptr<DuiLib::IListVirtalCallbackUI> list_data_view_;

  /// @brief experiment related data
  /// @brief exp status
  /// 0 - stop, 1 - start, 2 - pause, <0 - unvalid
  int32_t is_exp_state_ = -1;
  int64_t exp_start_time_ms_ = 0;
  int64_t exp_start_date_time_ = 0;
  uint64_t exp_data_incoming_num_ = 0;
  int64_t exp_time_interval_num_ = 0;
  uint32_t exp_data_table_no_ = 0;
  int64_t exp_sample_interval_ms_ = 0;

  /// @brief experiment data  for the data table view
  std::vector<anx::expdata::ExperimentData> exp_datas_;
  /// @brief start row no for the data table view
  int32_t exp_data_table_start_row_no_ = 0;
  /// @brief limit row no for the data table view
  int32_t exp_data_table_limit_row_no_ = 100;
  /// @brief data table count for the data table view
  int32_t exp_data_table_count_ = 0;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_DATA_H_
