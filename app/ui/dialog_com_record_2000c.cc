/**
 * @file dialog_com_record_2000c.cc
 * @author hhool (hhool@outlook.com)
 * @brief dialog com record 2000c
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_com_record_2000c.h"

#include <map>
#include <string>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"
#include "app/ui/ui_constants.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogComRecord2000C, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogComRecord2000C::DialogComRecord2000C() {}

DialogComRecord2000C::~DialogComRecord2000C() {}

void DialogComRecord2000C::InitWindow() {
  __super::InitWindow();
}

void DialogComRecord2000C::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == kCloseButtonControlName) {
      this->Close();
      return;
    }
  }
}

void DialogComRecord2000C::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogComRecord2000C::ResponseDefaultKeyEvent(WPARAM wParam,
                                                      bool& bHandled) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogComRecord2000C::OnPrepare(const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

}  // namespace ui
}  // namespace anx
