/**
 * @file work_window_tab_main_third_page.h
 * @author hhool (hhool@outlook.com)
 * @brief work window third page ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;

namespace anx {
namespace esolution {
class SolutionDesign;
}  // namespace esolution
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {
class WorkWindowThirdPage : public DuiLib::CNotifyPump {
 public:
  WorkWindowThirdPage(WorkWindow* pOwner,
                      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowThirdPage();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);

 protected:
  void OnPrepare(DuiLib::TNotifyUI& msg);

 private:
  WorkWindow* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::unique_ptr<esolution::SolutionDesign> solution_design_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_
