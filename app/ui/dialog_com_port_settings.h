/**
 * @file dialog_com_port_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief com port select dialog class implementation
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_COM_PORT_SETTINGS_H_
#define APP_UI_DIALOG_COM_PORT_SETTINGS_H_

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
class DialogComPortSettings : public DuiLib::WindowImplBase {
 public:
  DialogComPortSettings();
  ~DialogComPortSettings();

  DUI_DECLARE_MESSAGE_MAP()

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DuiLib::CDuiString GetSkinFolder() override;
  DuiLib::CDuiString GetSkinFile() override;
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override;
  LPCTSTR GetWindowClassName(void) const override;

 private:
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_COM_PORT_SETTINGS_H_
