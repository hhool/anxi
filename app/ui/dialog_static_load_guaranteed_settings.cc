/**
 * @file dialog_static_load_guaranteed_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief staic load guaranteed settings dialog class implementation
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_static_load_guaranteed_settings.h"

#include <string>

#include "app/common/string_utils.h"
#include "app/device/device_exp_load_static_settings.h"
#include "app/device/stload/stload_common.h"
#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogStaticLoadGuaranteedSettings,
                      DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogStaticLoadGuaranteedSettings::DialogStaticLoadGuaranteedSettings() {}

DialogStaticLoadGuaranteedSettings::~DialogStaticLoadGuaranteedSettings() {}

void DialogStaticLoadGuaranteedSettings::InitWindow() {
  __super::InitWindow();
  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_ok_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("okbtn")));
  btn_cancel_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));
  opt_displacement_ = static_cast<COptionUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_opt_displacement")));
  opt_retention_ = static_cast<COptionUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_opt_retention")));
  edit_speed_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_speed")));
  edit_retention_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_retention")));
  edit_displacement_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_displacement")));
  edit_keep_load_duration_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_keep_load_duration")));

  /// bind event
  opt_displacement_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnCtrlTypeChanged);
  opt_retention_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnCtrlTypeChanged);
  UpdateControlFromSettings();
}

void DialogStaticLoadGuaranteedSettings::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    OnClick(msg);
  }
}

void DialogStaticLoadGuaranteedSettings::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogStaticLoadGuaranteedSettings::ResponseDefaultKeyEvent(
    WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogStaticLoadGuaranteedSettings::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_ok_) {
    SaveSettingsFromControl();
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  } else if (msg.pSender == btn_cancel_) {
    this->Close();
  }
}

void DialogStaticLoadGuaranteedSettings::OnPrepare(
    const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

bool DialogStaticLoadGuaranteedSettings::OnCtrlTypeChanged(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender == opt_displacement_) {
    edit_displacement_->SetEnabled(true);
    edit_retention_->SetEnabled(false);
    edit_keep_load_duration_->SetEnabled(false);
  } else if (pMsg->pSender == opt_retention_) {
    edit_displacement_->SetEnabled(false);
    edit_retention_->SetEnabled(true);
    edit_keep_load_duration_->SetEnabled(true);
  }
  return true;
}

void DialogStaticLoadGuaranteedSettings::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    return;
  }
  if (lss->ctrl_type_ == CTRL_LOAD) {
    opt_displacement_->Selected(false);
    opt_retention_->Selected(true);
    edit_displacement_->SetEnabled(false);
  } else {
    opt_displacement_->Selected(true);
    opt_retention_->Selected(false);
    edit_retention_->SetEnabled(false);
  }
  edit_speed_->SetText(
      anx::common::string2wstring(std::to_string(lss->speed_).c_str()).c_str());
  edit_retention_->SetText(
      anx::common::string2wstring(std::to_string(lss->retention_).c_str())
          .c_str());
  edit_displacement_->SetText(
      anx::common::string2wstring(std::to_string(lss->displacement_).c_str())
          .c_str());
  edit_keep_load_duration_->SetText(
      anx::common::string2wstring(
          std::to_string(lss->keep_load_duration_).c_str())
          .c_str());
}

void DialogStaticLoadGuaranteedSettings::SaveSettingsFromControl() {
  // std::string direct = opt_direct_up_->IsSelected() ? "up" : "down";
  int32_t speed = _ttoi(edit_speed_->GetText().GetData());
  int32_t retention = _ttoi(edit_retention_->GetText().GetData());
  anx::device::DeviceLoadStaticSettings lss;
  // lss.direct_ = anx::device::DeviceLoadStatic::ValueDirectFromString(direct);
  lss.ctrl_type_ = opt_displacement_->IsSelected() ? CTRL_POSI : CTRL_LOAD;
  lss.speed_ = speed;
  lss.retention_ = retention;
  lss.displacement_ = _ttoi(edit_displacement_->GetText().GetData());
  lss.keep_load_duration_ =
      _ttoi(edit_keep_load_duration_->GetText().GetData());
  anx::device::SaveDeviceLoadStaticSettingsDefaultResource(lss);
}

}  // namespace ui
}  // namespace anx
