/**
 * @file dialog_com_record_2000c.h
 * @author hhool (hhool@outlook.com)
 * @brief dialog com record 2000c
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_COM_RECORD_2000C_H_
#define APP_UI_DIALOG_COM_RECORD_2000C_H_

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
class DialogComRecord2000C : public DuiLib::WindowImplBase {
 public:
  DialogComRecord2000C();
  ~DialogComRecord2000C();

  DUI_DECLARE_MESSAGE_MAP()

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DuiLib::CDuiString GetSkinFolder() override {
#ifdef _DEBUG
    return _T("skin\\");
#else
    return _T("skin\\");
#endif
  }
  DuiLib::CDuiString GetSkinFile() override {
    return _T("dialog_com_record_2000c.xml");
  }
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override {
#ifdef _DEBUG
    return DuiLib::UILIB_FILE;
#else
    return DuiLib::UILIB_ZIP;
#endif
  }
  LPCTSTR GetWindowClassName(void) const override {
    return _T("dialog_com_record_2000c");
  }

 protected:
  void OnPrepare(const TNotifyUI& msg);

 private:
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_COM_RECORD_2000C_H_
