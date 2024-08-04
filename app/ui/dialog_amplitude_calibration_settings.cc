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
    // SaveComInfoFromControlAll();
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  }
}

}  // namespace ui
}  // namespace anx
