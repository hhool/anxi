/**
 * @file dialog_common.h
 * @author hhool (hhool@outlook.com)
 * @brief dialog about
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_COMMON_H_
#define APP_UI_DIALOG_COMMON_H_

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
class DialogCommon : public DuiLib::WindowImplBase {
 public:
  enum DialogCommonStyle {
    kDialogCommonStyleOk = 0,
    kDialogCommonStyleOkCancel = 1,
  };

  enum {
    DC_Cancel = 0,
    DC_Ok,
  };

  /// @brief show dialog common dialog with title and content and style
  /// @param hwnd parent window handle
  /// @param title dialog title
  /// @param content dialog content
  /// @param style dialog style kDialogCommonStyleOk or
  /// kDialogCommonStyleOkCancel
  /// @return int32_t DC_Cancel or DC_Ok
  static int32_t ShowDialog(HWND hwnd,
                            const std::string& title,
                            const std::string& content,
                            DialogCommonStyle style);

 public:
  DialogCommon() {}
  DialogCommon(const std::string& title,
               const std::string& content,
               int32_t* result,
               DialogCommonStyle style = kDialogCommonStyleOkCancel);
  ~DialogCommon();

  DUI_DECLARE_MESSAGE_MAP()

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DuiLib::CDuiString GetSkinFolder() override { return _T("skin\\"); }
  DuiLib::CDuiString GetSkinFile() override { return _T("dialog_common.xml"); }
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
    return _T("dialog_common");
  }

 protected:
  void OnPrepare(const DuiLib::TNotifyUI& msg);

 private:
  std::string title_;
  std::string content_;
  int32_t* result_;
  DialogCommonStyle style_;

  CButtonUI* btn_close_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_COMMON_H_
