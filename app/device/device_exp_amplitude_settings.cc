/**
 * @file device_exp_amplitude_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief  device exp amplitude settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_exp_amplitude_settings.h"

#include <sstream>
#include <string>

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
////////////////////////////////////////////////////////////////////
// clz DeviceExpAmplitude
DeviceExpAmplitude::DeviceExpAmplitude() {
  exp_power2amp_map_ = {{20, 30}, {40, 38}, {60, 47}, {80, 54}};
}

DeviceExpAmplitude::DeviceExpAmplitude(
    const std::map<int32_t, int32_t>& exp_power2amp_map) {
  exp_power2amp_map_ = exp_power2amp_map;
}

DeviceExpAmplitude::~DeviceExpAmplitude() {}

////////////////////////////////////////////////////////////////////
// clz DeviceExpAmplitudeSettings
DeviceExpAmplitudeSettings::DeviceExpAmplitudeSettings() {}

DeviceExpAmplitudeSettings::DeviceExpAmplitudeSettings(
    const std::map<int32_t, int32_t>& exp_power2amp_map)
    : DeviceExpAmplitude(exp_power2amp_map) {}

DeviceExpAmplitudeSettings::~DeviceExpAmplitudeSettings() {}

std::string DeviceExpAmplitudeSettings::ToXml(bool close_tag) const {
  std::stringstream ss;
  if (close_tag) {
    ss << "<exp_amplitude_settings>\r\n";
  }
  for (const auto& item : exp_power2amp_map_) {
    ss << "<exp_power2amp_item>\r\n";
    ss << "<exp_power>" << item.first << "</exp_power>\r\n";
    ss << "<exp_amp>" << item.second << "</exp_amp>\r\n";
    ss << "</exp_power2amp_item>\r\n";
  }
  if (close_tag) {
    ss << "</exp_amplitude_settings>\r\n";
  }
  return ss.str();
}

std::unique_ptr<DeviceExpAmplitudeSettings> DeviceExpAmplitudeSettings::FromXml(
    const std::string& xml) {
  tinyxml2::XMLDocument doc;
  if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
    return nullptr;
  }
  auto root = doc.FirstChildElement("exp_amplitude_settings");
  if (!root) {
    return nullptr;
  }

  std::map<int32_t, int32_t> exp_power2amp_map;
  for (auto item = root->FirstChildElement("exp_power2amp_item"); item;
       item = item->NextSiblingElement("exp_power2amp_item")) {
    int32_t exp_power = 0;
    int32_t exp_amp = 0;
    auto sub = item->FirstChildElement("exp_power");
    if (sub) {
      exp_power = std::stoi(sub->GetText());
    }
    sub = item->FirstChildElement("exp_amp");
    if (sub) {
      exp_amp = std::stoi(sub->GetText());
    }
    exp_power2amp_map[exp_power] = exp_amp;
  }
  return std::unique_ptr<DeviceExpAmplitudeSettings>(
      new DeviceExpAmplitudeSettings(exp_power2amp_map));
}

////////////////////////////////////////////////////////////////////
// helper function
std::string DefaultDeviceExpAmplitudeSettingsXmlFilePath() {
#if defined(_WIN32) || defined(_WIN64)
  return "default\\device_exp_amp_settings.xml";
#else
  return "default/device_exp_amp_settings.xml";
#endif
}

std::unique_ptr<DeviceExpAmplitudeSettings>
LoadDeviceExpAmplitudeSettingsDefaultWithFilePath(
    const std::string& file_path) {
  std::string file_content;
  if (!anx::common::ReadFile(file_path, &file_content, true)) {
    LOG_F(LG_ERROR) << "Failed to read file: " << file_path;
    return nullptr;
  }

  // Parse the file content
  std::unique_ptr<DeviceExpAmplitudeSettings> settings =
      DeviceExpAmplitudeSettings::FromXml(file_content);
  if (settings.get() == nullptr) {
    return nullptr;
  }
  return settings;
}

std::unique_ptr<DeviceExpAmplitudeSettings>
LoadDeviceExpAmplitudeSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceExpAmplitudeSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif

  std::unique_ptr<DeviceExpAmplitudeSettings> settings =
      LoadDeviceExpAmplitudeSettingsDefaultWithFilePath(default_xml);
  return settings;
}

int32_t SaveDeviceExpAmplitudeSettingsFile(
    const std::string& file_path,
    const DeviceExpAmplitudeSettings& settings) {
  std::string xml = settings.ToXml();
  if (xml.empty()) {
    LOG_F(LG_ERROR) << "ToXml failed";
    return -1;
  }

  if (!anx::common::WriteFile(file_path, xml, true)) {
    LOG_F(LG_ERROR) << "write file failed:" << file_path;
    return -2;
  }
  return 0;
}

int32_t SaveDeviceExpAmplitudeSettingsDefaultResource(
    const DeviceExpAmplitudeSettings& settings) {
  std::string default_xml = DefaultDeviceExpAmplitudeSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif
  return SaveDeviceExpAmplitudeSettingsFile(default_xml, settings);
}

}  // namespace device
}  // namespace anx
