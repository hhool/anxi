/**
 * @file device_com_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief com settings struct implementation
 * @version 0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_com_settings.h"

#include <assert.h>
#include <sstream>
#include <string>

#include "app/device/device_com.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
////////////////////////////////////////////////////////////
// constants
const int32_t kDeviceCom_Ultrasound = 0;
const int32_t kDeviceCom_StaticLoad = 1;
const int32_t kDeviceLan_StaticLoad = 2;

////////////////////////////////////////////////////////////////////////////////
/// @brief clz ComSettings
ComSettings::ComSettings(int32_t device_com_type,
                         const std::string& com_name,
                         ComBase* com_base)
    : device_com_type_(device_com_type) {
  assert(com_base != nullptr);
  // ComPortDevice(com_name, com_port),
  device_com_port_ = std::make_unique<ComPortDevice>(com_name, com_base);
}

ComSettings::~ComSettings() {}

int32_t ComSettings::GetDeviceComType() const {
  return device_com_type_;
}

std::string ComSettings::ToXml(bool close_tag) const {
  std::stringstream ss;
  if (close_tag) {
    ss << "<com_settings>\r\n";
  }
  ComBase* com_base = device_com_port_->GetComPort();
  if (com_base->adrtype == 1) {
    ComAddressPort* com_port = static_cast<ComAddressPort*>(com_base);
    ss << "<adr_type>" << com_port->adrtype << "</adr_type>\r\n";
    ss << "<device_com_type>" << device_com_type_ << "</device_com_type>\r\n";
    ss << "<com_name>" << device_com_port_->GetComName() << "</com_name>\r\n";
    ss << "<com_port>\r\n";
    ss << "<baud_rate>" << com_port->ValueBaudRateToString()
       << "</baud_rate>\r\n";
    ss << "<data_bits>" << com_port->ValueDataBitsToString()
       << "</data_bits>\r\n";
    ss << "<stop_bits>" << com_port->ValueStopBitsToString()
       << "</stop_bits>\r\n";
    ss << "<parity>" << com_port->ValueParityToString() << "</parity>\r\n";
    ss << "<flow_control>" << com_port->ValueFlowControlToString()
       << "</flow_control>\r\n";
    ss << "<timeout>" << com_port->timeout << "</timeout>\r\n";
    ss << "</com_port>\r\n";
  } else {
    ComAddressLan* com_port = static_cast<ComAddressLan*>(com_base);
    ss << "<adr_type>" << com_port->adrtype << "</adr_type>\r\n";
    ss << "<device_com_type>" << device_com_type_ << "</device_com_type>\r\n";
    ss << "<ip>" << com_port->ip << "</ip>\r\n";
    ss << "<port>" << com_port->port << "</port>\r\n";
  }

  if (close_tag) {
    ss << "</com_settings>\r\n";
  }
  return ss.str();
}

std::unique_ptr<ComSettings> ComSettings::FromXml(const std::string& xml) {
  tinyxml2::XMLDocument doc;
  if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
    return nullptr;
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_device_com_type =
      root->FirstChildElement("device_com_type");
  if (ele_device_com_type == nullptr) {
    return nullptr;
  }

  tinyxml2::XMLElement* ele_adrtype = root->FirstChildElement("adr_type");
  if (ele_adrtype == nullptr) {
    return nullptr;
  }
  int32_t adr_type = std::stoi(ele_adrtype->GetText());
  if (adr_type == 0 || adr_type > 2) {
    return nullptr;
  }
  if (adr_type == 2) {
    tinyxml2::XMLElement* ele_ip = root->FirstChildElement("ip");
    if (ele_ip == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_port = root->FirstChildElement("port");
    if (ele_port == nullptr) {
      return nullptr;
    }
    ComAddressLan com_port_lan(ele_ip->GetText(),
                               std::stoi(ele_port->GetText()));
    int32_t device_com_type = std::stoi(ele_device_com_type->GetText());
    return std::make_unique<ComSettings>(device_com_type, "", &com_port_lan);
  } else if (adr_type == 1) {
    tinyxml2::XMLElement* ele_name = root->FirstChildElement("com_name");
    if (ele_name == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_com_port = root->FirstChildElement("com_port");
    if (ele_com_port == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_baud_rate =
        ele_com_port->FirstChildElement("baud_rate");
    if (ele_baud_rate == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_data_bits =
        ele_com_port->FirstChildElement("data_bits");
    if (ele_data_bits == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_stop_bits =
        ele_com_port->FirstChildElement("stop_bits");
    if (ele_stop_bits == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_parity =
        ele_com_port->FirstChildElement("parity");
    if (ele_parity == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_flow_control =
        ele_com_port->FirstChildElement("flow_control");
    if (ele_flow_control == nullptr) {
      return nullptr;
    }
    tinyxml2::XMLElement* ele_timeout =
        ele_com_port->FirstChildElement("timeout");
    if (ele_timeout == nullptr) {
      return nullptr;
    }
    ComAddressPort com_port;
    com_port.baud_rate = anx::device::ComAddressPort::ValueBaudRateFromString(
        ele_baud_rate->GetText());
    com_port.data_bits = anx::device::ComAddressPort::ValueDataBitsFromString(
        ele_data_bits->GetText());
    com_port.stop_bits = anx::device::ComAddressPort::ValueStopBitsFromString(
        ele_stop_bits->GetText());
    com_port.parity = anx::device::ComAddressPort::ValueParityFromString(
        ele_parity->GetText());
    com_port.flow_control =
        anx::device::ComAddressPort::ValueFlowControlFromString(
            ele_flow_control->GetText());
    com_port.timeout = std::stoi(ele_timeout->GetText());

    int32_t device_com_type = std::stoi(ele_device_com_type->GetText());
    return std::make_unique<ComSettings>(device_com_type, ele_name->GetText(),
                                         &com_port);
  }
  return nullptr;
}

}  // namespace device
}  // namespace anx
