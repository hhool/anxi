/**
 * @file dialog_com_port_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief com port select dialog class implementation
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_com_port_settings.h"

#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogComPortSettings, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogComPortSettings::DialogComPortSettings() {}

DialogComPortSettings::~DialogComPortSettings() {}

void DialogComPortSettings::InitWindow() {}

void DialogComPortSettings::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == kCloseButtonControlName) {
      this->Close();
      return;
    }
  }
}

void DialogComPortSettings::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT anx::ui::DialogComPortSettings::ResponseDefaultKeyEvent(WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

DuiLib::CDuiString DialogComPortSettings::GetSkinFolder() {
#ifdef _DEBUG
  return _T("skin\\anxi\\used\\");
#else
  return _T("skin\\");
#endif
}

DuiLib::CDuiString DialogComPortSettings::GetSkinFile() {
  return _T("dialog_com_port_settings.xml");
}

DuiLib::UILIB_RESOURCETYPE DialogComPortSettings::GetResourceType() const {
#ifdef _DEBUG
  return DuiLib::UILIB_FILE;
#else
  return DuiLib::UILIB_ZIP;
#endif
}

LPCTSTR DialogComPortSettings::GetWindowClassName(void) const {
  return _T("dialog_com_port_settings");
}
}  // namespace ui
}  // namespace anx
