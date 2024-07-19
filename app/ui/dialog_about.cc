/**
 * @file dialog_about.cc
 * @author hhool (hhool@outlook.com)
 * @brief dialog about
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_about.h"
#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogAbout, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogAbout::DialogAbout() {}

DialogAbout::~DialogAbout() {}

void DialogAbout::InitWindow() {}

void DialogAbout::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == kCloseButtonControlName) {
      this->Close();
      return;
    }
  }
}

void DialogAbout::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT anx::ui::DialogAbout::ResponseDefaultKeyEvent(WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

DuiLib::CDuiString DialogAbout::GetSkinFolder() {
#ifdef _DEBUG
  return _T("skin\\anxi\\used\\");
#else
  return _T("skin\\");
#endif
}

DuiLib::CDuiString DialogAbout::GetSkinFile() {
  return _T("dialog_about.xml");
}

DuiLib::UILIB_RESOURCETYPE DialogAbout::GetResourceType() const {
#ifdef _DEBUG
  return DuiLib::UILIB_FILE;
#else
  return DuiLib::UILIB_ZIP;
#endif
}

LPCTSTR DialogAbout::GetWindowClassName(void) const {
  return _T("dialog_about");
}

}  // namespace ui
}  // namespace anx
