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

////////////////////////////////////////////////////////////////////////////////
/// @brief clz ComSettings
ComSettings::ComSettings(int32_t device_com_type,
                         const std::string& com_name,
                         const ComPort& com_port)
    : ComPortDevice(com_name, com_port), device_com_type_(device_com_type) {}

ComSettings::~ComSettings() {}

int32_t ComSettings::GetDeviceComType() const {
  return device_com_type_;
}

std::string ComSettings::ToXml(bool close_tag) const {
  std::stringstream ss;
  if (close_tag) {
    ss << "<com_settings>\r\n";
  }
  ss << "<device_com_type>" << device_com_type_ << "</device_com_type>\r\n";
  ss << "<com_name>" << com_name_ << "</com_name>\r\n";
  ss << "<com_port>\r\n";
  ss << "<baud_rate>" << com_port_.ValueBaudRateToString()
     << "</baud_rate>\r\n";
  ss << "<data_bits>" << com_port_.ValueDataBitsToString()
     << "</data_bits>\r\n";
  ss << "<stop_bits>" << com_port_.ValueStopBitsToString()
     << "</stop_bits>\r\n";
  ss << "<parity>" << com_port_.ValueParityToString() << "</parity>\r\n";
  ss << "<flow_control>" << com_port_.ValueFlowControlToString()
     << "</flow_control>\r\n";
  ss << "<timeout>" << com_port_.timeout << "</timeout>\r\n";
  ss << "</com_port>\r\n";
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
  tinyxml2::XMLElement* ele_parity = ele_com_port->FirstChildElement("parity");
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
  ComPort com_port;
  com_port.baud_rate =
      anx::device::ComPort::ValueBaudRateFromString(ele_baud_rate->GetText());
  com_port.data_bits =
      anx::device::ComPort::ValueDataBitsFromString(ele_data_bits->GetText());
  com_port.stop_bits =
      anx::device::ComPort::ValueStopBitsFromString(ele_stop_bits->GetText());
  com_port.parity =
      anx::device::ComPort::ValueParityFromString(ele_parity->GetText());
  com_port.flow_control = anx::device::ComPort::ValueFlowControlFromString(
      ele_flow_control->GetText());
  com_port.timeout = std::stoi(ele_timeout->GetText());

  int32_t device_com_type = std::stoi(ele_device_com_type->GetText());
  return std::make_unique<ComSettings>(device_com_type, ele_name->GetText(),
                                       com_port);
}

}  // namespace device
}  // namespace anx
