/**
 * @file dialog_com_port_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief com port select dialog class implementation
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_com_port_settings.h"

#include "app/common/string_utils.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_com_settings_helper.h"
#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogComPortSettings, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

/*
 * tail_prefix:
 * ultrasound - ua
 * static_load - sl
 * Air compressor - ac
 */
namespace anx {
namespace ui {

namespace {
int32_t TailPrefixToDeviceComType(std::string tail_prefix) {
  if (tail_prefix == "ua") {
    return anx::device::kDeviceCom_Ultrasound;
  } else if (tail_prefix == "sl") {
    return anx::device::kDeviceCom_StaticLoad;
  } else if (tail_prefix == "ac") {
    return anx::device::kDeviceCom_Air_compressor;
  }
  return -1;
}

}  // namespace
DialogComPortSettings::DialogComPortSettings() {}

DialogComPortSettings::~DialogComPortSettings() {}

void DialogComPortSettings::InitWindow() {
  __super::InitWindow();
  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_ok_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("okbtn")));
  btn_cancel_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));
  UpdateControlFromComInfoAll();
}

void DialogComPortSettings::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    OnClick(msg);
  }
}

void DialogComPortSettings::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogComPortSettings::ResponseDefaultKeyEvent(WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogComPortSettings::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_ok_) {
    SaveComInfoFromControlAll();
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  } else if (msg.pSender == btn_cancel_) {
    this->Close();
  }
}

void DialogComPortSettings::OnPrepare(const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

void DialogComPortSettings::SaveComInfoFromControlAll() {
  SaveComInfoFromControl("ua");
  SaveComInfoFromControl("sl");
}

void DialogComPortSettings::SaveComInfoFromControl(std::string tail_prefix) {
  std::string com_port_name = GetComPortNameFromControl(tail_prefix);
  std::string baud_rate = GetBaudRateFromControl(tail_prefix);
  std::string data_bits = GetDataBitsFromControl(tail_prefix);
  std::string stop_bits = GetStopBitsFromControl(tail_prefix);
  std::string parity = GetParityFromControl(tail_prefix);

  // update com settings
  // convert string to int
  anx::device::ComPort com_port;
  com_port.baud_rate = std::stoi(baud_rate);
  com_port.data_bits = anx::device::ComPort::ValueDataBitsFromString(data_bits);
  com_port.stop_bits = anx::device::ComPort::ValueStopBitsFromString(stop_bits);
  com_port.parity = anx::device::ComPort::ValueParityFromString(parity);
  // TODO(hhool):
  com_port.flow_control =
      anx::device::ComPort::ValueFlowControlFromString("hardware");
  int32_t device_com_type = TailPrefixToDeviceComType(tail_prefix);

  anx::device::ComSettings com_settings(device_com_type, com_port_name,
                                        com_port);
  anx::device::SaveDeviceComSettingsFileDefaultPath(com_settings);
}

std::string DialogComPortSettings::GetComPortNameFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("com_port_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* com_port_name_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = com_port_name_edit->GetText().GetData();
  return anx::common::wstring2string(s.GetData());
}

std::string DialogComPortSettings::GetBaudRateFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("baud_rate_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* baud_rate_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = baud_rate_edit->GetText().GetData();
  return anx::common::wstring2string(s.GetData());
}

std::string DialogComPortSettings::GetDataBitsFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("data_bits_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* data_bits_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = data_bits_edit->GetText().GetData();
  return anx::common::wstring2string(s.GetData());
}

std::string DialogComPortSettings::GetStopBitsFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("stop_bits_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* stop_bits_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = stop_bits_edit->GetText().GetData();
  return anx::common::wstring2string(s.GetData());
}

std::string DialogComPortSettings::GetParityFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("parity_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* parity_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = parity_edit->GetText().GetData();
  return anx::common::wstring2string(s.GetData());
}

void DialogComPortSettings::UpdateControlFromComInfoAll() {
  UpdateControlFromComInfo("ua");
  UpdateControlFromComInfo("sl");
}

void DialogComPortSettings::UpdateControlFromComInfo(std::string tail_prefix) {
  std::unique_ptr<anx::device::ComSettings> com_settings =
      anx::device::LoadDeviceComSettingsDefaultResourceWithType(
          TailPrefixToDeviceComType(tail_prefix));
  if (com_settings != nullptr) {
    SetComPortNameToControl(tail_prefix, com_settings->GetComName());
    SetBaudRateToControl(tail_prefix,
                         com_settings->GetComPort().ValueBaudRateToString());
    SetDataBitsToControl(tail_prefix,
                         com_settings->GetComPort().ValueDataBitsToString());
    SetStopBitsToControl(tail_prefix,
                         com_settings->GetComPort().ValueStopBitsToString());
    SetParityToControl(tail_prefix,
                       com_settings->GetComPort().ValueParityToString());
  }
}

void DialogComPortSettings::SetComPortNameToControl(std::string tail_prefix,
                                                    std::string com_port_name) {
  DuiLib::CDuiString name = _T("com_port_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* com_port_name_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (com_port_name_edit == nullptr) {
    return;
  }
  int count = com_port_name_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = com_port_name_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::string2wstring(com_port_name.c_str()).c_str()) == 0) {
      com_port_name_edit->SelectItem(i);
      return;
    }
  }
}

void DialogComPortSettings::SetBaudRateToControl(std::string tail_prefix,
                                                 std::string baud_rate) {
  DuiLib::CDuiString name = _T("baud_rate_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* baud_rate_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (baud_rate_edit == nullptr) {
    return;
  }
  int count = baud_rate_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = baud_rate_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::string2wstring(baud_rate.c_str()).c_str()) == 0) {
      baud_rate_edit->SelectItem(i);
      return;
    }
  }
}

void DialogComPortSettings::SetDataBitsToControl(std::string tail_prefix,
                                                 std::string data_bits) {
  DuiLib::CDuiString name = _T("data_bits_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* data_bits_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (data_bits_edit == nullptr) {
    return;
  }
  int count = data_bits_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = data_bits_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::string2wstring(data_bits.c_str()).c_str()) == 0) {
      data_bits_edit->SelectItem(i);
      return;
    }
  }
}

void DialogComPortSettings::SetStopBitsToControl(std::string tail_prefix,
                                                 std::string stop_bits) {
  DuiLib::CDuiString name = _T("stop_bits_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* stop_bits_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (stop_bits_edit == nullptr) {
    return;
  }
  int count = stop_bits_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = stop_bits_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::string2wstring(stop_bits.c_str()).c_str()) == 0) {
      stop_bits_edit->SelectItem(i);
      return;
    }
  }
}

void DialogComPortSettings::SetParityToControl(std::string tail_prefix,
                                               std::string parity) {
  DuiLib::CDuiString name = _T("parity_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* parity_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (parity_edit == nullptr) {
    return;
  }
  int count = parity_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = parity_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(anx::common::string2wstring(parity.c_str()).c_str()) ==
        0) {
      parity_edit->SelectItem(i);
      return;
    }
  }
}

}  // namespace ui
}  // namespace anx
