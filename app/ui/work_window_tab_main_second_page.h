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

#include "app/ui/ui_virtual_wnd_base.h"

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;

namespace anx {
namespace device {
class DeviceComInterface;
}
namespace esolution {
class SolutionDesign;
}  // namespace esolution
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {

class WorkWindowSecondPage : public DuiLib::CNotifyPump,
                             public UIVirtualWndBase {
 public:
  WorkWindowSecondPage(WorkWindow* pWorkWindow,
                       DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowSecondPage();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);
  void OnTimer(TNotifyUI& msg);

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 protected:
  void CheckDeviceComConnectedStatus();

 protected:
  int32_t exp_start();
  void exp_pause();
  void exp_stop();
  void exp_resume();

  void UpdateExpClipTimeFromControl();

 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  bool is_exp_running_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_sl_;
  int32_t exp_clip_time_duration_;
  int32_t exp_clip_time_paused_;
  int64_t exp_cycle_count_;
  int32_t exp_freq_fluctuations_range_;

  int64_t sample_start_pos_;
  int64_t sample_end_pos_;
  int64_t sample_time_interval_;

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

  /// @brief exp action releated button
  ///
  /// @brief exp action start button
  DuiLib::CButtonUI* btn_exp_start_;
  /// @brief exp action stop button
  DuiLib::CButtonUI* btn_exp_stop_;
  /// @brief exp action pause button
  DuiLib::CButtonUI* btn_exp_pause_;

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

  /// @brief max cycle count edit
  DuiLib::CEditUI* edit_max_cycle_count_;
  DuiLib::CEditUI* edit_max_cycle_power_;
  DuiLib::CEditUI* edit_frequency_fluctuations_range_;

  /// @brief sample start pos edit
  DuiLib::CEditUI* edit_sample_start_pos_;
  /// @brief sample end pos edit
  DuiLib::CEditUI* edit_sample_end_pos_;
  /// @brief sample time duration edit
  DuiLib::CEditUI* edit_sample_interval_;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_H_
