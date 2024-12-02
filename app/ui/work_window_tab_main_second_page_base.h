/**
 * @file work_window_tab_main_second_page_base.h
 * @author hhool (hhool@outlook.com)
 * @brief work window second page ui
 * @version 0.1
 * @date 2024-08-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_BASE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_BASE_H_

#include <map>
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
class DeviceExpDataSampleSettings;
}  // namespace device
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {

const float kMultiFactor = 1000.0f;

typedef enum {
  /// @brief real time view mode of graph ctrl for exp data display
  view_mode_real = 0,
  /// @brief history view mode of graph ctrl for exp data display
  view_mode_history = 1
} e_view_mode;

class ExpDataInfo {
 public:
  ExpDataInfo() = default;
  ~ExpDataInfo() = default;

 public:
  /// @brief current mode, 0: real time, 1: history
  int32_t mode_ = view_mode_real;
  /// @brief exp data table no of history view start position.
  /// 0: is not initialized: !0: is initialized and valid.
  /// exp_data_view_current_start_no_ % 10 must be 1
  /// for graph ctrl display
  int32_t exp_data_view_current_start_no_ = 1;
  /// @brief exp start time. in miliseconds
  /// @note exp_start_time_ms_ is the start time of exp data sample,
  /// exp_start_time_ms_ = 0, means exp data sample is not started
  /// is not initialized: !0: is initialized and valid.
  /// for data list ctrl display and exp data sample time calculation
  /// when hard real time exp data sample is started, exp_start_time_ms_ is
  /// updated to current time.
  int64_t exp_start_time_ms_ = 0;
  /// @brief exp data sample interval in miliseconds, 2000ms default
  int32_t exp_sample_interval_ms_ = 2000;
  /// @brief exp data sample count since exp start time, 0: is not initialized:
  /// !0: is initialized and valid.
  /// used to sync time period of exp data sample when data list ctrl display
  int64_t exp_time_interval_num_ = 0;
  int64_t exp_pre_sample_timestamp_ms_ = 0;
  int64_t exp_freq_total_count_ = 0;
  /// @brief exp data sample store in database exp table count since exp start
  /// time, 0: is not initialized: !0: is initialized and valid.
  /// valid from exp start time and exp stop time
  int32_t exp_data_table_no_ = 0;
  /// @brief real value
  ///  store in database REAL
  double amp_freq_ = 0.0f;
  /// @brief real value
  ///  store in database REAL
  double amp_um_ = 0.0f;
  /// @brief real value
  ///  store in database REAL
  double stress_value_ = 0.0f;

 public:
  std::string ToString() const {
    return "mode: " + std::to_string(mode_) +
           ", exp_data_view_current_start_no_: " +
           std::to_string(exp_data_view_current_start_no_) +
           ", exp_start_time_ms_: " + std::to_string(exp_start_time_ms_) +
           ", exp_sample_interval_ms_: " +
           std::to_string(exp_sample_interval_ms_) +
           ", exp_time_interval_num_: " +
           std::to_string(exp_time_interval_num_) +
           ", exp_data_table_no_: " + std::to_string(exp_data_table_no_) +
           ", amp_freq_: " + std::to_string(amp_freq_) +
           ", amp_um_: " + std::to_string(amp_um_) +
           ", stress_value_: " + std::to_string(stress_value_);
  }
};

/// @brief exp status
/// 0 - stop, 1 - start, 2 - pause, <0 - unvalid
enum {
  kExpStateStop = 0,
  kExpStateStart = 1,
  kExpStatePause = 2,
  kExpStateUnvalid = -1
};

inline const std::string ExpStateToString(int32_t exp_state) {
  switch (exp_state) {
    case kExpStateStop:
      return "stop";
    case kExpStateStart:
      return "start";
    case kExpStatePause:
      return "pause";
    default:
      return "unvalid";
  }
}

/// @brief enum exp_state pause and stop state reason code
/// 0 - none, 1 - out frequency range, 2 - reach max cycle, 3 - reach range
/// time end pos, 4 - system standby, 100 - unkown
/// @note exp_pause_stop_reason_ for record the pause reason code
/// if the exp is paused, then the pause reason code will record the pause
/// reason code value and update the button state with the pause reason code
/// of message box
enum {
  kExpPauseStopReasonNone = 0,
  kExpPauseStopReasonOutFrequecyRange = 1,
  kExpPauseStopReasonReachMaxCycle = 2,
  kExpPauseStopReasonReachRangeTimeEndPos = 3,
  kExpPauseStopReasonSystemStandby = 4,
  kExpPauseStopReasonUnkown = 100
};

inline const std::string ExpPauseStopReasonToString(
    int32_t exp_pause_stop_reason) {
  switch (exp_pause_stop_reason) {
    case kExpPauseStopReasonNone:
      return "none";
    case kExpPauseStopReasonOutFrequecyRange:
      return "out frequency range";
    case kExpPauseStopReasonReachMaxCycle:
      return "reach max cycle";
    case kExpPauseStopReasonReachRangeTimeEndPos:
      return "reach range time end pos";
    case kExpPauseStopReasonSystemStandby:
      return "system standby";
    case kExpPauseStopReasonUnkown:
      return "unkown";
    default:
      return "unkown";
  }
}
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_BASE_H_
