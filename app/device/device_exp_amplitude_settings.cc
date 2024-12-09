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

#include <assert.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "app/esolution/algorithm/alg.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
////////////////////////////////////////////////////////////////////
// clz DeviceExpAmplitude
DeviceExpAmplitude::DeviceExpAmplitude() {
  exp_power2amp_map_ = {{20, 37.8f}, {40, 46.8f}, {60, 55.6f}, {80, 63.f}};
}

DeviceExpAmplitude::DeviceExpAmplitude(
    const std::map<int32_t, float>& exp_power2amp_map) {
  exp_power2amp_map_ = exp_power2amp_map;
}

DeviceExpAmplitude::~DeviceExpAmplitude() {}

////////////////////////////////////////////////////////////////////
// clz DeviceExpAmplitudeSettings
DeviceExpAmplitudeSettings::DeviceExpAmplitudeSettings() {}

DeviceExpAmplitudeSettings::DeviceExpAmplitudeSettings(
    const std::map<int32_t, float>& exp_power2amp_map)
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
    ss << "<exp_amp>" << std::to_string(item.second) << "</exp_amp>\r\n";
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

  std::map<int32_t, float> exp_power2amp_map;
  for (auto item = root->FirstChildElement("exp_power2amp_item"); item;
       item = item->NextSiblingElement("exp_power2amp_item")) {
    int32_t exp_power = 0;
    float exp_amp = 0;
    auto sub = item->FirstChildElement("exp_power");
    if (sub) {
      exp_power = std::stoi(sub->GetText());
    }
    sub = item->FirstChildElement("exp_amp");
    if (sub) {
      exp_amp = std::stof(sub->GetText());
    }
    exp_power2amp_map[exp_power] = exp_amp;
  }
  return std::unique_ptr<DeviceExpAmplitudeSettings>(
      new DeviceExpAmplitudeSettings(exp_power2amp_map));
}

////////////////////////////////////////////////////////////////////
// helper function
std::string DefaultDeviceExpAmplitudeSettingsXmlFilePath() {
  std::string default_xml = "default";
  default_xml += anx::common::kPathSeparator;
  default_xml += "device_exp_amp_settings.xml";
  return default_xml;
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
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;

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
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;

  return SaveDeviceExpAmplitudeSettingsFile(default_xml, settings);
}

int32_t ResetDeviceExpAmplitudeSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceExpAmplitudeSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  std::string dst_xml =
      app_data_dir + anx::common::kPathSeparator + default_xml;
  std::string origin_xml =
      anx::common::GetModuleDir() + anx::common::kPathSeparator + default_xml;

  if (!anx::common::FileExists(origin_xml)) {
    LOG_F(LG_ERROR) << "origin xml file not exists: " << origin_xml;
    return -1;
  }
  if (!anx::common::CCopyFile(origin_xml, dst_xml)) {
    LOG_F(LG_ERROR) << "copy file failed: " << origin_xml << " to " << dst_xml;
    return -2;
  }
  return 0;
}

namespace {
static int32_t Interpolation(const std::vector<float>& x,
                             const std::vector<float>& y,
                             float x0) {
  assert(x.size() == y.size());
  float* _x = new float[x.size()];
  float* _y = new float[y.size()];
  for (size_t i = 0; i < x.size(); i++) {
    _x[i] = x[i];
    _y[i] = y[i];
  }
  float a = 0;
  float b = 0;
  anx::esolution::algorithm::LineFit(_x, _y, x.size(), &a, &b);
  delete[] _x;
  delete[] _y;
  return static_cast<int32_t>(a * x0 + b);
}
}

int32_t Amp2DeviceExpPower(const DeviceExpAmplitudeSettings& settings,
                           float exp_amp,
                           int32_t* exp_power) {
  if (exp_power == nullptr) {
    return -1;
  }
  // define vector with exp_power2amp_map_ size
  std::vector<float> exp_power2amp_map(settings.exp_power2amp_map_.size());
  // copy the exp_power2amp_map_ to the vector
  int32_t i = 0;
  for (const auto& item : settings.exp_power2amp_map_) {
    exp_power2amp_map[i++] = item.second;
  }
  // define vector with exp_power2amp_map_ size
  std::vector<float> exp_power2amp_map_key(settings.exp_power2amp_map_.size());
  // copy the exp_power2amp_map_ to the vector
  i = 0;
  for (const auto& item : settings.exp_power2amp_map_) {
    exp_power2amp_map_key[i++] = item.first * 1.0f;
  }
  // call the algorithm function
  *exp_power = Interpolation(exp_power2amp_map, exp_power2amp_map_key, exp_amp);
  return 0;
}

}  // namespace device
}  // namespace anx
