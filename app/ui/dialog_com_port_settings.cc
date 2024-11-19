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

#include <memory>
#include <string>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_com_settings_helper.h"
#include "app/ui/dialog_app_settings_helper.h"
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
  } else if (tail_prefix == "sl2") {
    return anx::device::kDeviceLan_StaticLoad;
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
  int32_t ver_stload = anx::settings::SettingSTLoad::GetEnableStloadVersion();
  if (ver_stload == 2) {
    SaveComInfoFromControl("sl2", 2);
  } else if (ver_stload == 1) {
    SaveComInfoFromControl("sl");
  } else {
    // do nothing
  }
}

void DialogComPortSettings::SaveComInfoFromControl(std::string tail_prefix,
                                                   int32_t adrtype) {
  if (adrtype == 1) {
    std::string com_port_name = GetComPortNameFromControl(tail_prefix);
    std::string baud_rate = GetBaudRateFromControl(tail_prefix);
    std::string data_bits = GetDataBitsFromControl(tail_prefix);
    std::string stop_bits = GetStopBitsFromControl(tail_prefix);
    std::string parity = GetParityFromControl(tail_prefix);
    std::string flow_control = GetFlowControlFromControl(tail_prefix);

    // update com settings
    // convert string to int
    anx::device::ComAddressPort com_port;
    com_port.baud_rate = std::stoi(baud_rate);
    com_port.data_bits =
        anx::device::ComAddressPort::ValueDataBitsFromString(data_bits);
    com_port.stop_bits =
        anx::device::ComAddressPort::ValueStopBitsFromString(stop_bits);
    com_port.parity =
        anx::device::ComAddressPort::ValueParityFromString(parity);
    com_port.flow_control =
        anx::device::ComAddressPort::ValueFlowControlFromString(flow_control);
    int32_t device_com_type = TailPrefixToDeviceComType(tail_prefix);

    anx::device::ComSettings com_settings(device_com_type, com_port_name,
                                          &com_port);
    anx::device::SaveDeviceComSettingsFileDefaultPath(com_settings);
  } else if (adrtype == 2) {
    std::string ip_adr = GetIpAdrFromControl(tail_prefix);
    int32_t port = GetPortFromControl(tail_prefix);

    // update com settings
    anx::device::ComAddressLan com_lan(ip_adr, port);
    int32_t device_com_type = TailPrefixToDeviceComType(tail_prefix);

    anx::device::ComSettings com_settings(device_com_type, "", &com_lan);
    anx::device::SaveDeviceComSettingsFileDefaultPath(com_settings);
  } else {
    // do nothing
    LOG_F(LG_ERROR) << "adrtype is not 1 or 2";
  }
}

std::string DialogComPortSettings::GetComPortNameFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("com_port_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* com_port_name_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = com_port_name_edit->GetText().GetData();
  return anx::common::WString2String(s.GetData());
}

std::string DialogComPortSettings::GetBaudRateFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("baud_rate_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* baud_rate_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = baud_rate_edit->GetText().GetData();
  return anx::common::WString2String(s.GetData());
}

std::string DialogComPortSettings::GetDataBitsFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("data_bits_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* data_bits_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = data_bits_edit->GetText().GetData();
  return anx::common::WString2String(s.GetData());
}

std::string DialogComPortSettings::GetStopBitsFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("stop_bits_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* stop_bits_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = stop_bits_edit->GetText().GetData();
  return anx::common::WString2String(s.GetData());
}

std::string DialogComPortSettings::GetParityFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("parity_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* parity_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = parity_edit->GetText().GetData();
  return anx::common::WString2String(s.GetData());
}

std::string DialogComPortSettings::GetFlowControlFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("flow_control_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* flow_control_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = flow_control_edit->GetText().GetData();
  return anx::common::WString2String(s.GetData());
}

void DialogComPortSettings::UpdateControlFromComInfoAll() {
  UpdateControlFromComInfo("ua");
  int32_t ver_stload = anx::settings::SettingSTLoad::GetEnableStloadVersion();
  /// hide or show static load com port settings
  DuiLib::CControlUI* sl_static_ver_1 =
      m_PaintManager.FindControl(_T("body.com_port_setting_b"));
  DuiLib::CControlUI* sl_static_ver_2 =
      m_PaintManager.FindControl(_T("body.com_port_setting_c"));
  if (ver_stload == 1) {
    sl_static_ver_1->SetVisible(true);
    sl_static_ver_2->SetVisible(false);
    UpdateControlFromComInfo("sl");
  } else {
    sl_static_ver_1->SetVisible(false);
    sl_static_ver_2->SetVisible(true);
    UpdateControlFromComInfo("sl2");
  }
}

void DialogComPortSettings::UpdateControlFromComInfo(std::string tail_prefix) {
  std::unique_ptr<anx::device::ComSettings> com_settings =
      anx::device::LoadDeviceComSettingsDefaultResourceWithType(
          TailPrefixToDeviceComType(tail_prefix));
  if (com_settings != nullptr) {
    SetComPortNameToControl(tail_prefix,
                            com_settings->GetComPortDevice()->GetComName());

    anx::device::ComBase* combase =
        com_settings->GetComPortDevice()->GetComPort();
    if (combase->adrtype == 1) {
      anx::device::ComAddressPort* com_port =
          static_cast<anx::device::ComAddressPort*>(combase);
      SetBaudRateToControl(tail_prefix, com_port->ValueBaudRateToString());
      SetBaudRateToControl(tail_prefix, com_port->ValueBaudRateToString());
      SetDataBitsToControl(tail_prefix, com_port->ValueDataBitsToString());
      SetStopBitsToControl(tail_prefix, com_port->ValueStopBitsToString());
      SetParityToControl(tail_prefix, com_port->ValueParityToString());
      SetFlowControlToControl(tail_prefix,
                              com_port->ValueFlowControlToString());
    } else if (combase->adrtype == 2) {
      anx::device::ComAddressLan* com_port =
          static_cast<anx::device::ComAddressLan*>(combase);
      SetIpAdrToControl(tail_prefix, com_port->ip);
      SetPortToControl(tail_prefix, com_port->port);
    } else {
      assert(false && "Invalid com base type");
    }
  }
}

void DialogComPortSettings::SetComPortNameToControl(std::string tail_prefix,
                                                    std::string com_port_name) {
  DuiLib::CDuiString name = _T("com_port_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* com_port_name_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (com_port_name_edit == nullptr) {
    return;
  }
  int count = com_port_name_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = com_port_name_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::String2WString(com_port_name.c_str()).c_str()) == 0) {
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
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* baud_rate_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (baud_rate_edit == nullptr) {
    return;
  }
  int count = baud_rate_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = baud_rate_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::String2WString(baud_rate.c_str()).c_str()) == 0) {
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
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* data_bits_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (data_bits_edit == nullptr) {
    return;
  }
  int count = data_bits_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = data_bits_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::String2WString(data_bits.c_str()).c_str()) == 0) {
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
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* stop_bits_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (stop_bits_edit == nullptr) {
    return;
  }
  int count = stop_bits_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = stop_bits_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::String2WString(stop_bits.c_str()).c_str()) == 0) {
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
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* parity_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (parity_edit == nullptr) {
    return;
  }
  int count = parity_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = parity_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(anx::common::String2WString(parity.c_str()).c_str()) ==
        0) {
      parity_edit->SelectItem(i);
      return;
    }
  }
}

void DialogComPortSettings::SetFlowControlToControl(std::string tail_prefix,
                                                    std::string flow_control) {
  DuiLib::CDuiString name = _T("flow_control_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CComboUI* flow_control_edit =
      static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl(name + prefix));
  if (flow_control_edit == nullptr) {
    return;
  }
  int count = flow_control_edit->GetCount();
  for (int i = 0; i < count; i++) {
    DuiLib::CDuiString s = flow_control_edit->GetItemAt(i)->GetText();
    if (s.CompareNoCase(
            anx::common::String2WString(flow_control.c_str()).c_str()) == 0) {
      flow_control_edit->SelectItem(i);
      return;
    }
  }
}

std::string DialogComPortSettings::GetIpAdrFromControl(
    std::string tail_prefix) {
  DuiLib::CDuiString name = _T("ip_adr_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* ip_adr_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = ip_adr_edit->GetText().GetData();
  return anx::common::WString2String(s.GetData());
}

int32_t DialogComPortSettings::GetPortFromControl(std::string tail_prefix) {
  DuiLib::CDuiString name = _T("port_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* port_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  DuiLib::CDuiString s = port_edit->GetText().GetData();
  return std::stoi(anx::common::WString2String(s.GetData()));
}

void DialogComPortSettings::SetIpAdrToControl(std::string tail_prefix,
                                              std::string ip_adr) {
  DuiLib::CDuiString name = _T("ip_adr_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* ip_adr_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  ip_adr_edit->SetText(anx::common::UTF8ToUnicode(ip_adr).c_str());
}

void DialogComPortSettings::SetPortToControl(std::string tail_prefix,
                                             int32_t port) {
  DuiLib::CDuiString name = _T("port_");
  DuiLib::CDuiString prefix;
  prefix.Append(
      (LPCTSTR)(anx::common::String2WString(tail_prefix.c_str()).c_str()));
  DuiLib::CEditUI* port_edit =
      static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name + prefix));
  port_edit->SetText(anx::common::UTF8ToUnicode(std::to_string(port)).c_str());
}

}  // namespace ui
}  // namespace anx
