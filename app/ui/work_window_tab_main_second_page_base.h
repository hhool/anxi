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

class ExpDataInfo {
 public:
  ExpDataInfo() = default;
  ~ExpDataInfo() = default;

 public:
  /// @brief exp start time. in miliseconds
  int64_t exp_start_time_ms_ = 0;
  /// @brief exp data sample interval in miliseconds, 2000ms default
  int64_t exp_sample_interval_ms_ = 0;
  /// @brief exp data sample count since exp start time
  int64_t exp_time_interval_num_ = 0;
  /// @brief exp data sample store in database exp table count since exp start
  /// time
  uint32_t exp_data_table_no_ = 0;
  /// @brief real value multiply by kMultiFactor,
  ///  store in database REAL
  double amp_freq_ = 0.0f;
  /// @brief real value multiply by kMultiFactor,
  ///  store in database REAL
  double amp_um_ = 0.0f;
  /// @brief real value multiply by kMultiFactor,
  ///  store in database REAL
  double stress_value_ = 0.0f;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_H_
