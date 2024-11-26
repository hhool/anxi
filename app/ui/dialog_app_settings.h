/**
 * @file dialog_app_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief  dialog app settings
 * @version 0.1
 * @date 2024-11-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_APP_SETTINGS_H_
#define APP_UI_DIALOG_APP_SETTINGS_H_

#include <map>
#include <memory>
#include <string>
#include <vector>


#include "app/ui/dialog_app_settings_helper.h"
#include "app/ui/ui_virtual_wnd_base.h"

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

using DuiLib::CControlUI;
using DuiLib::CPaintManagerUI;
using DuiLib::CWindowWnd;
using DuiLib::INotifyUI;

namespace anx {
namespace ui {
class DialogAppSettingsBase {
 public:
  virtual int32_t SaveSettingsToResource() = 0;
};
class DialogAppSettings;
class DialogAppSettingsCommon : public DuiLib::CNotifyPump,
                                public DuiLib::INotifyUI,
                                public anx::ui::UIVirtualWndBase,
                                public DialogAppSettingsBase {
 public:
  DialogAppSettingsCommon(DialogAppSettings* pOwner,
                          DuiLib::CPaintManagerUI* paint_manager_ui);
  ~DialogAppSettingsCommon();

 public:
  void Notify(DuiLib::TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnSelectChanged(TNotifyUI& msg);  // NOLINT
  void OnKillFocus(TNotifyUI& msg);      // NOLINT
  void OnBtnThirdAppReset();

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 public:
  int32_t SaveSettingsToResource() override;

 protected:
  void UpdateControlFromAppSettings();

 private:
  void LoadFileWithDialog();

 private:
  DialogAppSettings* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::vector<anx::settings::SettingSTLoad::STLoadItem> stloads_;
  std::string third_app_path_;
  std::string third_app_name_;
};

class DialogAppSettingsAdvanced : public DuiLib::CNotifyPump,
                                  public DuiLib::INotifyUI,
                                  public anx::ui::UIVirtualWndBase,
                                  public DialogAppSettingsBase {
 public:
  DialogAppSettingsAdvanced(DialogAppSettings* pOwner,
                            DuiLib::CPaintManagerUI* paint_manager_ui);
  ~DialogAppSettingsAdvanced();

 public:
  void Notify(DuiLib::TNotifyUI& msg) override;

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 public:
  int32_t SaveSettingsToResource() override;

 protected:
  void UpdateControlFromAppSettings();

 private:
  DialogAppSettings* pOwner_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
};

class DialogAppSettings : public DuiLib::WindowImplBase {
 public:
  explicit DialogAppSettings(DuiLib::WindowImplBase* pOwner);
  ~DialogAppSettings();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(DuiLib::TNotifyUI& msg) override;

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DuiLib::CDuiString GetSkinFolder() override { return _T("skin\\"); }
  DuiLib::CDuiString GetSkinFile() override {
    return _T("dialog_app_settings.xml");
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
    return _T("dialog_app_settings");
  }

 protected:
  void OnPrepare(const DuiLib::TNotifyUI& msg);

 private:
  DuiLib::WindowImplBase* pOwner_;
  UIVirtualWndBase* page_common_wnd_;
  UIVirtualWndBase* page_advanced_wnd_;
  std::map<std::string, std::unique_ptr<DuiLib::CNotifyPump>> tab_pages_;
  CButtonUI* btn_ok_ = nullptr;
  CButtonUI* btn_close_ = nullptr;
  CButtonUI* btn_cancel_ = nullptr;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_APP_SETTINGS_H_
