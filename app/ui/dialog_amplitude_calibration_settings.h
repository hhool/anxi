/**
 * @file dialog_amplitude_calibration_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief amplitude calibration settings dialog class implementation
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_AMPLITUDE_CALIBRATION_SETTINGS_H_
#define APP_UI_DIALOG_AMPLITUDE_CALIBRATION_SETTINGS_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;

using DuiLib::CControlUI;
using DuiLib::CPaintManagerUI;
using DuiLib::CWindowWnd;
using DuiLib::INotifyUI;

namespace anx {
namespace ui {
class DialogAmplitudeCalibrationSettings : public DuiLib::WindowImplBase {
 public:
  DialogAmplitudeCalibrationSettings();
  ~DialogAmplitudeCalibrationSettings();

  void InitWindow() override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(DuiLib::TNotifyUI& msg) override;

  DuiLib::CDuiString GetSkinFolder() override {
#ifdef _DEBUG
    return _T("skin\\");
#else
    return _T("skin\\");
#endif
  }
  DuiLib::CDuiString GetSkinFile() override {
    return _T("dialog_amplitude_calibration_settings.xml");
  }
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override {
#ifdef _DEBUG
    return DuiLib::UILIB_FILE;
#else
    return DuiLib::UILIB_ZIP;
#endif
  }
  LPCTSTR GetWindowClassName(void) const override {
    return _T("dialog_amplitude_calibration_settings");
  }

 private:
  CButtonUI* btn_close_;
  CButtonUI* btn_ok_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_AMPLITUDE_CALIBRATION_SETTINGS_H_
