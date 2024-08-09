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
  return "default/device_exp_amp_settings.xml";
}

std::unique_ptr<DeviceExpAmplitudeSettings>
LoadDeviceExpAmplitudeSettingsDefaultWithFilePath(
    const std::string& file_path) {
  FILE* file = fopen(file_path.c_str(), "rb");
  if (file == nullptr) {
    return nullptr;
  }
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  // Read the file content
  std::unique_ptr<char[]> file_content(new char[file_size]);
  size_t size = fread(file_content.get(), 1, file_size, file);
  if (size != file_size) {
    return nullptr;
  }
  fclose(file);

  // Parse the file content
  std::string content(file_content.get(), file_size);
  std::unique_ptr<DeviceExpAmplitudeSettings> settings =
      DeviceExpAmplitudeSettings::FromXml(content);
  if (settings.get() == nullptr) {
    return nullptr;
  }
  return settings;
}

std::unique_ptr<DeviceExpAmplitudeSettings>
LoadDeviceExpAmplitudeSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceExpAmplitudeSettingsXmlFilePath();
  // get module path
  std::string module_dir = anx::common::GetModuleDir();
  default_xml = module_dir + "\\" + default_xml;

  std::unique_ptr<DeviceExpAmplitudeSettings> settings =
      LoadDeviceExpAmplitudeSettingsDefaultWithFilePath(default_xml);
  return settings;
}

int32_t SaveDeviceExpAmplitudeSettingsFile(
    const std::string& file_path,
    const DeviceExpAmplitudeSettings& settings) {
  std::string xml = settings.ToXml();
  if (xml.empty()) {
    return -1;
  }

  FILE* file = fopen(file_path.c_str(), "wb");
  if (file == nullptr) {
    return -1;
  }
  size_t size = xml.size();
  size_t written = fwrite(xml.c_str(), 1, size, file);
  if (written != size) {
    fclose(file);
    return -1;
  }
  fclose(file);
  return 0;
}

int32_t SaveDeviceExpAmplitudeSettingsDefaultResource(
    const DeviceExpAmplitudeSettings& settings) {
  std::string default_xml = DefaultDeviceExpAmplitudeSettingsXmlFilePath();
  std::string module_dir = anx::common::GetModuleDir();
  default_xml = module_dir + "\\" + default_xml;
  return SaveDeviceExpAmplitudeSettingsFile(default_xml, settings);
}

}  // namespace device
}  // namespace anx
