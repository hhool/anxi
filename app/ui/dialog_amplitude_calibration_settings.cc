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

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "app/device/device_exp_amplitude_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/ui_num_string_convert.hpp"

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
  btn_ok_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("okbtn")));
  btn_cancel_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));

  edit_amp_level_one_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_one")));
  edit_amp_level_two_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_two")));
  edit_amp_level_third_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_third")));
  edit_amp_level_fourth_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_fourth")));

  LoadSettingsFromResource();

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
    SaveSettingsToResource();
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  } else if (msg.pSender == btn_cancel_) {
    this->Close();
  }
}

void DialogAmplitudeCalibrationSettings::OnPrepare(
    const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

void DialogAmplitudeCalibrationSettings::LoadSettingsFromResource() {
  deas_ = anx::device::LoadDeviceExpAmplitudeSettingsDefaultResource();
  if (deas_ == nullptr) {
    deas_ = std::unique_ptr<anx::device::DeviceExpAmplitudeSettings>(
        new anx::device::DeviceExpAmplitudeSettings());
  }
}

void DialogAmplitudeCalibrationSettings::UpdateControlFromSettings() {
  if (deas_ == nullptr) {
    LOG_F(LG_ERROR) << "deas_ is nullptr";
    return;
  }
  std::map<int32_t, int32_t> exp_power2amp_map = deas_->exp_power2amp_map_;
  set_value_to_control(edit_amp_level_one_, exp_power2amp_map[20]);
  set_value_to_control(edit_amp_level_two_, exp_power2amp_map[40]);
  set_value_to_control(edit_amp_level_third_, exp_power2amp_map[60]);
  set_value_to_control(edit_amp_level_fourth_, exp_power2amp_map[80]);
}

void DialogAmplitudeCalibrationSettings::SaveSettingsToResource() {
  if (deas_ == nullptr) {
    LOG_F(LG_INFO) << "deas_ is nullptr";
    return;
  }
  std::map<int32_t, int32_t> exp_power2amp_map = deas_->exp_power2amp_map_;
  anx::device::SaveDeviceExpAmplitudeSettingsDefaultResource(exp_power2amp_map);
}

}  // namespace ui
}  // namespace anx
