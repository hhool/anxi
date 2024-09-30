/**
 * @file work_window_tab_main_page_base.h
 * @author hhool (hhool@outlook.com)
 * @brief work window second page ui
 * @version 0.1
 * @date 2024-08-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_PAGE_BASE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_PAGE_BASE_H_

#include <memory>
#include <string>

#include "app/device/device_com.h"
#include "app/ui/ui_virtual_wnd_base.h"
#include "app/ui/work_window_tab_main_second_page_base.h"
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

typedef enum ENMsgType {
  enmsg_type_stload_max,
  enmsg_type_stload_value_cur,
  enmsg_type_amp_value_max,
  enmsg_type_amp_value_cur,
  enmsg_type_exp_stress_amp,
} ENMsgType;

typedef struct ENMsgStruct {
  ENMsgType type_;
  void* ptr_;
} ENMsgStruct;

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_PAGE_BASE_H_
