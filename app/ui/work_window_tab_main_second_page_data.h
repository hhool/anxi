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

#include <memory>
#include <string>

#include "app/device/device_com.h"
#include "app/ui/ui_virtual_wnd_base.h"

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
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {

class WorkWindowSecondPageData : public DuiLib::CNotifyPump,
                                 public UIVirtualWndBase,
                                 public anx::device::DeviceComListener {
 public:
  WorkWindowSecondPageData(WorkWindow* pWorkWindow,
                           DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowSecondPageData();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnTimer(TNotifyUI& msg);  // NOLINT

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 protected:
  void CheckDeviceComConnectedStatus();
  void RefreshExpClipTimeControl();
  void RefreshSampleTimeControl();
  void UpdateControlFromSettings();
  void SaveSettingsFromControl();

 protected:
  void UpdateExpClipTimeFromControl();

 protected:
  // impliment anx::device::DeviceComListener;
  void OnDataReceived(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override;
  void OnDataOutgoing(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override;

 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_sl_;

  int64_t sample_start_pos_;
  int64_t sample_end_pos_;
  int64_t sample_time_interval_;

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
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_DATA_H_
