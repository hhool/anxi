/**
 * @file dialog_app_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief  dialog app settings
 * @version 0.1
 * @date 2024-11-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_app_settings.h"

#include <utility>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/ui/main_window.h"
#include "app/ui/ui_constants.h"
#include "app/ui/ui_num_string_convert.hpp"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogAppSettingsCommon, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogAppSettingsAdvanced, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogAppSettings, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

////////////////////////////////////////////////////////////////////////////////
/// DialogAppSettingsCommon
DialogAppSettingsCommon::DialogAppSettingsCommon(
    DialogAppSettings* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pOwner_(pOwner), paint_manager_ui_(paint_manager_ui) {
  paint_manager_ui_->AddNotifier(this);
}

DialogAppSettingsCommon::~DialogAppSettingsCommon() {
  paint_manager_ui_->RemoveNotifier(this);
}

void DialogAppSettingsCommon::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    OnClick(msg);
  } else if (msg.sType == kItemSelect) {
    OnSelectChanged(msg);
  } else if (msg.sType == kKillFocus) {
    OnKillFocus(msg);
  } else {
    // TODO(hhool): do nothing
  }
}

void DialogAppSettingsCommon::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  if (msg.pSender->GetName() == _T("btn_e10c_pilot_path")) {
    // open pilot e10c
    LoadFileWithDialog();
  } else if (msg.pSender->GetName() == _T("okbtn")) {
    if (third_app_name_.empty() || third_app_path_.empty()) {
      return;
    }
    SaveSettingsToResource();
  } else if (msg.pSender->GetName() == _T("btn_e10c_pilot_params_reset")) {
    OnBtnThirdAppReset();
  } else {
    // TODO(hhool): do nothing
  }
}

void DialogAppSettingsCommon::OnSelectChanged(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  if (msg.pSender->GetName() == _T("combo_stload_version")) {
    DuiLib::CComboUI* combo_stload = static_cast<DuiLib::CComboUI*>(
        paint_manager_ui_->FindControl(_T("combo_stload_version")));
    int32_t index = combo_stload->GetCurSel();
    if (index < 0) {
      return;
    }
    DuiLib::CControlUI* control = combo_stload->GetItemAt(index);
    if (control == nullptr) {
      return;
    }
    DuiLib::CDuiString text = control->GetText();
    std::string stload_name = anx::common::UnicodeToUTF8(text.GetData());
    for (auto& stload : stloads_) {
      if (stload.name_ == stload_name) {
        stload.enable_ = true;
      } else {
        stload.enable_ = false;
      }
    }
  }
}

void DialogAppSettingsCommon::OnKillFocus(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  if (msg.pSender->GetName() == _T("edit_e10c_pilot_path")) {
    DuiLib::CEditUI* edit_third_app_path = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(_T("edit_e10c_pilot_path")));
    DuiLib::CDuiString path = edit_third_app_path->GetText();
    third_app_path_ = anx::common::UnicodeToUTF8(path.GetData());
  } else if (msg.pSender->GetName() == _T("edit_e10c_pilot_name")) {
    DuiLib::CEditUI* edit_third_app_name = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(_T("edit_e10c_pilot_name")));
    DuiLib::CDuiString name = edit_third_app_name->GetText();
    third_app_name_ = anx::common::UnicodeToUTF8(name.GetData());
  }
}

void DialogAppSettingsCommon::OnBtnThirdAppReset() {
  int32_t ret = anx::settings::SettingAppThird::ResetThirdApp();
  if (ret != 0) {
    LOG_F(LG_ERROR) << "ResetThirdApp failed";
    return;
  }
  third_app_name_ = anx::settings::SettingAppThird::GetThirdAppName();
  DuiLib::CEditUI* edit_third_app_name = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_e10c_pilot_name")));
  edit_third_app_name->SetText(
      anx::common::UTF8ToUnicode(third_app_name_).c_str());

  third_app_path_ = anx::settings::SettingAppThird::GetThirdAppPath();
  DuiLib::CEditUI* edit_third_app = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_e10c_pilot_path")));
  edit_third_app->SetText(anx::common::UTF8ToUnicode(third_app_path_).c_str());
}

void DialogAppSettingsCommon::Bind() {
  UpdateControlFromAppSettings();
}

void DialogAppSettingsCommon::Unbind() {}

int32_t DialogAppSettingsCommon::SaveSettingsToResource() {
  anx::settings::SettingSTLoad::SaveStloadList(stloads_);
  anx::settings::SettingAppThird::SaveThirdApp(third_app_name_,
                                               third_app_path_);
  return 0;
}

void DialogAppSettingsCommon::UpdateControlFromAppSettings() {
  /// update control from app settings
  anx::settings::SettingSTLoad::LoadStloadList(&stloads_);
  DuiLib::CComboUI* combo_stload = static_cast<DuiLib::CComboUI*>(
      paint_manager_ui_->FindControl(_T("combo_stload_version")));
  int32_t i = 0;
  int32_t select_index = 0;
  for (const auto& stload : stloads_) {
    combo_stload->GetItemAt(i++)->SetText(
        anx::common::UTF8ToUnicode(stload.name_).c_str());
    if (stload.enable_) {
      select_index = i - 1;
    }
  }
  combo_stload->SelectItem(select_index);

  /// load third app list
  std::string third_app_name =
      anx::settings::SettingAppThird::GetThirdAppName();
  DuiLib::CEditUI* edit_third_app_name = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_e10c_pilot_name")));
  edit_third_app_name->SetText(
      anx::common::UTF8ToUnicode(third_app_name).c_str());
  third_app_name_ = third_app_name;

  std::string third_app_path =
      anx::settings::SettingAppThird::GetThirdAppPath();
  DuiLib::CEditUI* edit_third_app = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_e10c_pilot_path")));
  edit_third_app->SetText(anx::common::UTF8ToUnicode(third_app_path).c_str());
  third_app_path_ = third_app_path;
}

void DialogAppSettingsCommon::LoadFileWithDialog() {
  OPENFILENAME ofn;
  TCHAR FileName[MAX_PATH];
  memset(&ofn, 0, sizeof(OPENFILENAME));
  memset(FileName, 0, sizeof(char) * MAX_PATH);
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = *pOwner_;
  ofn.lpstrFilter = _T("*.exe");
  ofn.lpstrFile = FileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_FILEMUSTEXIST;
  if (!GetOpenFileName(&ofn)) {
    return;
  }
  third_app_path_ = anx::common::UnicodeToUTF8(ofn.lpstrFile);
  DuiLib::CEditUI* edit_third_app = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_e10c_pilot_path")));
  edit_third_app->SetText(anx::common::UTF8ToUnicode(third_app_path_).c_str());
}

////////////////////////////////////////////////////////////////////////////////
/// DialogAppSettingsAdvanced
DialogAppSettingsAdvanced::DialogAppSettingsAdvanced(
    DialogAppSettings* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pOwner_(pOwner), paint_manager_ui_(paint_manager_ui) {
  paint_manager_ui_->AddNotifier(this);
}

DialogAppSettingsAdvanced::~DialogAppSettingsAdvanced() {
  paint_manager_ui_->RemoveNotifier(this);
}

void DialogAppSettingsAdvanced::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    OnClick(msg);
  } else {
  }
}

void DialogAppSettingsAdvanced::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  if (msg.pSender->GetName() == _T("okbtn")) {
    SaveSettingsToResource();
  } else {
    // TODO(hhool): do nothing
  }
}

void DialogAppSettingsAdvanced::Bind() {}

void DialogAppSettingsAdvanced::Unbind() {}

void DialogAppSettingsAdvanced::UpdateControlFromAppSettings() {}

int32_t DialogAppSettingsAdvanced::SaveSettingsToResource() {
  return 0;
}

//////////////////////////////////////////////////////////////////////////
/// DialogAppSettings
DialogAppSettings::DialogAppSettings(DuiLib::WindowImplBase* pOwner)
    : pOwner_(pOwner) {
  DialogAppSettingsCommon* dialog_app_settings_common =
      new DialogAppSettingsCommon(this, &m_PaintManager);
  page_common_wnd_ = dialog_app_settings_common;
  tab_pages_["app_settings_common"].reset(dialog_app_settings_common);
  this->AddVirtualWnd(_T("app_settings_common"),
                      tab_pages_["app_settings_common"].get());
  DialogAppSettingsAdvanced* dialog_app_settings_advanced =
      new DialogAppSettingsAdvanced(this, &m_PaintManager);
  page_advanced_wnd_ = dialog_app_settings_advanced;
  tab_pages_["app_settings_advanced"].reset(dialog_app_settings_advanced);
  this->AddVirtualWnd(_T("app_settings_advanced"),
                      tab_pages_["app_settings_advanced"].get());
}

DialogAppSettings::~DialogAppSettings() {
  for (auto& tab_page : tab_pages_) {
    DuiLib::CDuiString name(anx::common::UTF8ToUnicode(tab_page.first).c_str());
    this->RemoveVirtualWnd(name);
  }

  page_common_wnd_->Unbind();
  page_common_wnd_ = nullptr;

  DialogAppSettingsCommon* dialog_app_settings_common =
      reinterpret_cast<DialogAppSettingsCommon*>(
          tab_pages_["app_settings_common"].release());
  delete dialog_app_settings_common;
  {
    auto it = tab_pages_.find("app_settings_common");
    tab_pages_.erase(it);
  }

  page_advanced_wnd_->Unbind();
  page_advanced_wnd_ = nullptr;

  DialogAppSettingsAdvanced* dialog_app_settings_advanced =
      reinterpret_cast<DialogAppSettingsAdvanced*>(
          tab_pages_["app_settings_advanced"].release());
  delete dialog_app_settings_advanced;
  {
    auto it = tab_pages_.find("app_settings_advanced");
    tab_pages_.erase(it);
  }

  // remove tab_pages_
  tab_pages_.clear();
}

void DialogAppSettings::InitWindow() {
  __super::InitWindow();
  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_ok_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("okbtn")));
  btn_cancel_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));
}

void DialogAppSettings::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    OnClick(msg);
  }
}

void DialogAppSettings::OnFinalMessage(HWND hWnd) {
  __super::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogAppSettings::ResponseDefaultKeyEvent(WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogAppSettings::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_ok_) {
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  } else if (msg.pSender == btn_cancel_) {
    this->Close();
  }
}

void DialogAppSettings::OnPrepare(const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
  page_common_wnd_->Bind();
  page_advanced_wnd_->Bind();
}

}  // namespace ui
}  // namespace anx
