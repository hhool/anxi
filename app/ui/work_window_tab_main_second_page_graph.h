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

class WorkWindowSecondPageGraph : public DuiLib::CNotifyPump,
                                  public UIVirtualWndBase,
                                  public anx::device::DeviceComListener {
 public:
  WorkWindowSecondPageGraph(WorkWindow* pWorkWindow,
                            DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowSecondPageGraph();

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

  /// @brief graph time mode pre hour
  DuiLib::COptionUI* opt_graph_time_mode_pre_hour_;
  /// @brief graph time mode now
  DuiLib::COptionUI* opt_graph_time_mode_now_;

  /// @brief graph time alway show new
  DuiLib::CCheckBoxUI* chk_graph_always_show_new_;

  /// @brief graph time range option
  DuiLib::COptionUI* opt_graph_time_range_5_mnitues_;
  DuiLib::COptionUI* opt_graph_time_range_10_mnitues_;
  DuiLib::COptionUI* opt_graph_time_range_30_mnitues_;
  DuiLib::COptionUI* opt_graph_time_range_60_mnitues_;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_H_
