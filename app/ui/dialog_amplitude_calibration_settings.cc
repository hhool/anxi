/**
 * @file dialog_amplitude_calibration_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief amplitude calibration settings dialog class implementation
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_amplitude_calibration_settings.h"

#include <map>
#include <string>

#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "app/device/device_exp_amplitude_settings.h"
#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogAmplitudeCalibrationSettings,
                      DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogAmplitudeCalibrationSettings::DialogAmplitudeCalibrationSettings() {}

DialogAmplitudeCalibrationSettings::~DialogAmplitudeCalibrationSettings() {}

void DialogAmplitudeCalibrationSettings::InitWindow() {
  __super::InitWindow();
  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_ok_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("btn_amplitude_calibration_ok")));

  edit_amp_level_one_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_one")));
  edit_amp_level_two_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_two")));
  edit_amp_level_third_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_third")));
  edit_amp_level_fourth_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_fourth")));

  UpdateControlFromSettings();
}

void DialogAmplitudeCalibrationSettings::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    OnClick(msg);
  }
}

void DialogAmplitudeCalibrationSettings::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogAmplitudeCalibrationSettings::ResponseDefaultKeyEvent(
    WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogAmplitudeCalibrationSettings::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_ok_) {
    SaveSettingsFromControl();
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  }
}

void DialogAmplitudeCalibrationSettings::OnPrepare(
    const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

void DialogAmplitudeCalibrationSettings::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceExpAmplitudeSettings> exp_amp_settings =
      anx::device::LoadDeviceExpAmplitudeSettingsDefaultResource();
  if (exp_amp_settings == nullptr) {
    return;
  }
  std::map<int32_t, int32_t> exp_power2amp_map =
      exp_amp_settings->exp_power2amp_map_;
  edit_amp_level_one_->SetText(std::to_wstring(exp_power2amp_map[20]).c_str());
  edit_amp_level_two_->SetText(std::to_wstring(exp_power2amp_map[40]).c_str());
  edit_amp_level_third_->SetText(
      std::to_wstring(exp_power2amp_map[60]).c_str());
  edit_amp_level_fourth_->SetText(
      std::to_wstring(exp_power2amp_map[80]).c_str());
}

void DialogAmplitudeCalibrationSettings::SaveSettingsFromControl() {
  std::map<int32_t, int32_t> exp_power2amp_map;
  exp_power2amp_map[20] = std::stoi(
      anx::common::wstring2string(edit_amp_level_one_->GetText().GetData())
          .c_str());
  exp_power2amp_map[40] = std::stoi(
      anx::common::wstring2string(edit_amp_level_two_->GetText().GetData())
          .c_str());
  exp_power2amp_map[60] = std::stoi(
      anx::common::wstring2string(edit_amp_level_third_->GetText().GetData())
          .c_str());
  exp_power2amp_map[80] = std::stoi(
      anx::common::wstring2string(edit_amp_level_fourth_->GetText().GetData())
          .c_str());

  anx::device::SaveDeviceExpAmplitudeSettingsDefaultResource(exp_power2amp_map);
}

}  // namespace ui
}  // namespace anx
