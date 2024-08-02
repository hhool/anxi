/**
 * @file work_window_tab_main_second_page_base.h
 * @author hhool (hhool@outlook.com)
 * @brief work window second page ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_BASE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_BASE_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;

namespace anx {
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {

class WorkWindowSecondPageBase {
 public:
  WorkWindowSecondPageBase(WorkWindow* pOwner,
                           DuiLib::CPaintManagerUI* paint_manager_ui);
  virtual ~WorkWindowSecondPageBase();

 public:
  virtual void InitPage();

 protected:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_BASE_H_
