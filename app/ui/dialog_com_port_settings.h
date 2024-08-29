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

namespace anx {
namespace device {
class ComPort;
}  // namespace device
}  // namespace anx

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

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

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam, bool& bHandled) override;

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
    return _T("dialog_com_port_settings.xml");
  }
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override {
#ifdef _DEBUG
    return DuiLib::UILIB_FILE;
#else
    return DuiLib::UILIB_ZIP;
#endif
  }
  LPCTSTR GetWindowClassName(void) const override {
    return _T("dialog_com_port_settings");
  }

 protected:
  void OnPrepare(const DuiLib::TNotifyUI& msg);

 protected:
  void SaveComInfoFromControlAll();
  void SaveComInfoFromControl(std::string tail_prefix);

  std::string GetComPortNameFromControl(std::string tail_prefix);
  std::string GetBaudRateFromControl(std::string tail_prefix);
  std::string GetDataBitsFromControl(std::string tail_prefix);
  std::string GetStopBitsFromControl(std::string tail_prefix);
  std::string GetParityFromControl(std::string tail_prefix);

  void UpdateControlFromComInfoAll();
  void UpdateControlFromComInfo(std::string tail_prefix);

  void SetComPortNameToControl(std::string tail_prefix,
                               std::string com_port_name);
  void SetBaudRateToControl(std::string tail_prefix, std::string baud_rate);
  void SetDataBitsToControl(std::string tail_prefix, std::string data_bits);
  void SetStopBitsToControl(std::string tail_prefix, std::string stop_bits);
  void SetParityToControl(std::string tail_prefix, std::string parity);

 private:
  CButtonUI* btn_close_;
  CButtonUI* btn_ok_;
  CButtonUI* btn_cancel_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_COM_PORT_SETTINGS_H_
