/**
 * @file work_window_tab_main_third_page.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window tab main third page class definition
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_third_page.h"

#include <utility>

#include "app/common/defines.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowThirdPage, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
WorkWindowThirdPage::WorkWindowThirdPage(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pOwner_(pOwner), paint_manager_ui_(paint_manager_ui) {}

WorkWindowThirdPage::~WorkWindowThirdPage() {}

void WorkWindowThirdPage::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
  } else if (msg.sType == kSelectChanged) {
  }
}
void anx::ui::WorkWindowThirdPage::OnPrepare(DuiLib::TNotifyUI& msg) {}
}  // namespace ui
}  // namespace anx
