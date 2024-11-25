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
using namespace DuiLib;  // NOLINT

using DuiLib::CControlUI;
using DuiLib::CPaintManagerUI;
using DuiLib::CWindowWnd;
using DuiLib::INotifyUI;

namespace anx {
namespace ui {}
namespace device {
class DeviceExpAmplitudeSettings;
}
}  // namespace anx

namespace anx {
namespace ui {
class DialogAmplitudeCalibrationSettings : public DuiLib::WindowImplBase {
 public:
  DialogAmplitudeCalibrationSettings();
  ~DialogAmplitudeCalibrationSettings();

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(DuiLib::TNotifyUI& msg) override;

  DuiLib::CDuiString GetSkinFolder() override { return _T("skin\\"); }
  DuiLib::CDuiString GetSkinFile() override {
    return _T("dialog_amplitude_calibration_settings.xml");
  }
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override {
#if _DEBUG
    return DuiLib::UILIB_FILE;
#else
    return DuiLib::UILIB_ZIP;
#endif
  }
  DuiLib::CDuiString GetZIPFileName() const override {
    return _T("skin_default.zip");
  }
  LPCTSTR GetWindowClassName(void) const override {
    return _T("dialog_amplitude_calibration_settings");
  }

 protected:
  void OnPrepare(const DuiLib::TNotifyUI& msg);
  bool OnEditControlChanged(void* param);
  void LoadSettingsFromResource();
  void UpdateControlFromSettings();
  void SaveSettingsToResource();

 private:
  std::unique_ptr<anx::device::DeviceExpAmplitudeSettings> deas_;
  CButtonUI* btn_close_;
  CButtonUI* btn_ok_;
  CButtonUI* btn_cancel_;

  CEditUI* edit_amp_level_one_;
  CEditUI* edit_amp_level_two_;
  CEditUI* edit_amp_level_third_;
  CEditUI* edit_amp_level_fourth_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_AMPLITUDE_CALIBRATION_SETTINGS_H_
