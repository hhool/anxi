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

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
////////////////////////////////////////////////////////////
// constants
const int32_t kDeviceCom_Ultrasound = 0;
const int32_t kDeviceCom_StaticLoad = 1;
const int32_t kDeviceCom_Air_compressor = 2;
////////////////////////////////////////////////////////////////////////////////
/// @brief  clz ComPort
ComPort::ComPort()
    : baud_rate(9600),
      data_bits(8),
      stop_bits(1),
      parity(0),
      flow_control(0),
      timeout(1000) {}

////////////////////////////////////////////////////////////////////////////////
/// @brief clz ComSettings
ComSettings::ComSettings() {}
ComSettings::ComSettings(std::string name,
                         int32_t device_com_type,
                         const ComPort& com_port)
    : name_(name), device_com_type_(device_com_type), com_port_(com_port) {}

ComSettings::~ComSettings() {}

std::string ComSettings::ToXml(bool close_tag) const {
  std::stringstream ss;
  if (close_tag) {
    ss << "<com_settings>\r\n";
  }
  ss << "<name>" << name_ << "</name>\r\n";
  ss << "<com_port>\r\n";
  ss << "<baud_rate>" << com_port_.baud_rate << "</baud_rate>\r\n";
  ss << "<data_bits>" << com_port_.data_bits << "</data_bits>\r\n";
  ss << "<stop_bits>" << com_port_.stop_bits << "</stop_bits>\r\n";
  ss << "<parity>" << com_port_.parity << "</parity>\r\n";
  ss << "<flow_control>" << com_port_.flow_control << "</flow_control>\r\n";
  ss << "<timeout>" << com_port_.timeout << "</timeout>\r\n";
  ss << "</com_port>\r\n";
  if (close_tag) {
    ss << "</com_settings>\r\n";
  }
  return ss.str();
}

int32_t ComSettings::FromXml(const std::string& xml,
                             ComSettings* com_settings) {
  tinyxml2::XMLDocument doc;
  if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
    return -1;
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return -1;
  }
  tinyxml2::XMLElement* name = root->FirstChildElement("name");
  if (name == nullptr) {
    return -1;
  }
  com_settings->name_ = name->GetText();
  tinyxml2::XMLElement* com_port = root->FirstChildElement("com_port");
  if (com_port == nullptr) {
    return -1;
  }
  tinyxml2::XMLElement* baud_rate = com_port->FirstChildElement("baud_rate");
  if (baud_rate == nullptr) {
    return -1;
  }
  com_settings->com_port_.baud_rate = std::stoi(baud_rate->GetText());
  tinyxml2::XMLElement* data_bits = com_port->FirstChildElement("data_bits");
  if (data_bits == nullptr) {
    return -1;
  }
  com_settings->com_port_.data_bits = std::stoi(data_bits->GetText());
  tinyxml2::XMLElement* stop_bits = com_port->FirstChildElement("stop_bits");
  if (stop_bits == nullptr) {
    return -1;
  }
  com_settings->com_port_.stop_bits = std::stoi(stop_bits->GetText());
  tinyxml2::XMLElement* parity = com_port->FirstChildElement("parity");
  if (parity == nullptr) {
    return -1;
  }
  com_settings->com_port_.parity = std::stoi(parity->GetText());
  tinyxml2::XMLElement* flow_control =
      com_port->FirstChildElement("flow_control");
  if (flow_control == nullptr) {
    return -1;
  }
  tinyxml2::XMLElement* timeout = com_port->FirstChildElement("timeout");
  if (timeout == nullptr) {
    return -1;
  }
  return 0;
}

}  // namespace device
}  // namespace anx
