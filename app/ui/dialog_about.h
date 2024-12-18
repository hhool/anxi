/**
 * @file dialog_about.h
 * @author hhool (hhool@outlook.com)
 * @brief dialog about
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_ABOUT_H_
#define APP_UI_DIALOG_ABOUT_H_

#include <map>
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
class DialogAbout : public DuiLib::WindowImplBase {
 public:
  DialogAbout();
  ~DialogAbout();

  DUI_DECLARE_MESSAGE_MAP()

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DuiLib::CDuiString GetSkinFolder() override { return _T("skin\\"); }
  DuiLib::CDuiString GetSkinFile() override { return _T("dialog_about.xml"); }
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
  LPCTSTR GetWindowClassName(void) const override { return _T("dialog_about"); }

 protected:
  void OnPrepare(const TNotifyUI& msg);
  std::map<std::string, std::string> LoadAboutConfig();

 private:
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_ABOUT_H_
