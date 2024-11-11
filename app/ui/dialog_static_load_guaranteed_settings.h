/**
 * @file dialog_static_load_guaranteed_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief staic load guaranteed settings dialog class implementation
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_STATIC_LOAD_GUARANTEED_SETTINGS_H_
#define APP_UI_DIALOG_STATIC_LOAD_GUARANTEED_SETTINGS_H_

#include "app/ui/dialog_common.h"

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
namespace ui {
class DialogCommon;
}
}  // namespace anx

namespace anx {
namespace ui {
class DialogStaticLoadGuaranteedSettings : public anx::ui::DialogCommon {
 public:
  explicit DialogStaticLoadGuaranteedSettings(int32_t* result);
  ~DialogStaticLoadGuaranteedSettings();

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(DuiLib::TNotifyUI& msg) override;

  DuiLib::CDuiString GetSkinFolder() override { return _T("skin\\"); }
  DuiLib::CDuiString GetSkinFile() override {
    return _T("dialog_static_load_guaranteed_settings.xml");
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
    return _T("dialog_static_load_guaranteed_settings");
  }

 protected:
  void OnPrepare(const DuiLib::TNotifyUI& msg);
  bool OnCtrlTypeChanged(void* param);
  bool OnCtrlMoveChanged(void* param);
  void UpdateControlFromSettings();
  void SaveSettingsFromControl();

 private:
  CButtonUI* btn_close_;
  CButtonUI* btn_ok_;
  CButtonUI* btn_cancel_;
  COptionUI* opt_displacement_;
  COptionUI* opt_retention_;
  COptionUI* opt_up_;
  COptionUI* opt_down_;
  CEditUI* edit_speed_;
  CEditUI* edit_retention_;
  CEditUI* edit_displacement_;
  CEditUI* edit_keep_load_duration_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_STATIC_LOAD_GUARANTEED_SETTINGS_H_
