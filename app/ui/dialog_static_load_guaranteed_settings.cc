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
#include "app/ui/ui_constants.h"

#include "app/device/device_exp_load_static_settings.h"

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
  /*
    opt_direct_up_ = static_cast<COptionUI*>(
        m_PaintManager.FindControl(_T("tab_page_three_left_move_up")));
    opt_direct_down_ = static_cast<COptionUI*>(
        m_PaintManager.FindControl(_T("tab_page_three_left_move_down")));
  */
  edit_speed_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_speed")));

  edit_retention_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_retention")));

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

void DialogStaticLoadGuaranteedSettings::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    return;
  }
  /*opt_direct_up_->Selected(lss->direct_ == 1);
  opt_direct_down_->Selected(lss->direct_ == 2);*/
  edit_speed_->SetText(
      anx::common::string2wstring(std::to_string(lss->speed_).c_str()).c_str());
  edit_retention_->SetText(
      anx::common::string2wstring(std::to_string(lss->retention_).c_str())
          .c_str());
}

void DialogStaticLoadGuaranteedSettings::SaveSettingsFromControl() {
  // std::string direct = opt_direct_up_->IsSelected() ? "up" : "down";
  int32_t speed = _ttoi(edit_speed_->GetText().GetData());
  int32_t retention = _ttoi(edit_retention_->GetText().GetData());
  anx::device::DeviceLoadStaticSettings lss;
  // lss.direct_ = anx::device::DeviceLoadStatic::ValueDirectFromString(direct);
  lss.speed_ = speed;
  lss.retention_ = retention;
  anx::device::SaveDeviceLoadStaticSettingsDefaultResource(lss);
}

}  // namespace ui
}  // namespace anx
