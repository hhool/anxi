/**
 * @file device_exp_ultrasound_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief device ultrasound settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_exp_ultrasound_settings.h"

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
// clz DeviceUltrasound
DeviceUltrasound::DeviceUltrasound()
    : exp_clipping_enable_(false),
      exp_clip_time_duration_(50),
      exp_clip_time_paused_(50),
      exp_max_cycle_count_(1),
      exp_max_cycle_power_(9),
      exp_frequency_fluctuations_range_(300) {}

DeviceUltrasound::DeviceUltrasound(int32_t exp_clipping_enable,
                                   int64_t exp_clip_time_duration,
                                   int64_t exp_clip_time_paused,
                                   double exp_max_cycle_count,
                                   int32_t exp_max_cycle_power,
                                   int32_t exp_frequency_fluctuations_range)
    : exp_clipping_enable_(exp_clipping_enable),
      exp_clip_time_duration_(exp_clip_time_duration),
      exp_clip_time_paused_(exp_clip_time_paused),
      exp_max_cycle_count_(exp_max_cycle_count),
      exp_max_cycle_power_(exp_max_cycle_power),
      exp_frequency_fluctuations_range_(exp_frequency_fluctuations_range) {}

DeviceUltrasound::~DeviceUltrasound() {}

std::string DeviceUltrasound::ValueExpClippingEnableToString() const {
  if (exp_clipping_enable_ == 0) {
    return "disable";
  } else if (exp_clipping_enable_ == 1) {
    return "enable";
  } else {
    return "disable";
  }
}

std::string DeviceUltrasound::ValueExpFrequencyFluctuationsRangeToString()
    const {
  return std::to_string(exp_frequency_fluctuations_range_);
}

std::string DeviceUltrasound::ValueExpMaxCyclePowerToString() const {
  return std::to_string(exp_max_cycle_power_);
}

std::string DeviceUltrasound::ValueExpMaxCycleCountToString() const {
  return std::to_string(exp_max_cycle_count_);
}

std::string DeviceUltrasound::ValueExpClipTimePausedToString() const {
  return std::to_string(exp_clip_time_paused_);
}

std::string DeviceUltrasound::ValueExpClipTimeDurationToString() const {
  return std::to_string(exp_clip_time_duration_);
}

////////////////////////////////////////////////////////////////////
// clz DeviceUltrasoundSettings
DeviceUltrasoundSettings::DeviceUltrasoundSettings() {}

DeviceUltrasoundSettings::~DeviceUltrasoundSettings() {}

std::string DeviceUltrasoundSettings::ToXml(bool close_tag) const {
  std::stringstream ss;
  if (close_tag) {
    ss << "<device_exp_ultrasound_settings>\r\n";
  }
  ss << "<exp_clipping_enable>" << ValueExpClippingEnableToString()
     << "</exp_clipping_enable>\r\n";
  ss << "<exp_clip_time_duration>" << ValueExpClipTimeDurationToString()
     << "</exp_clip_time_duration>\r\n";
  ss << "<exp_clip_time_paused>" << ValueExpClipTimePausedToString()
     << "</exp_clip_time_paused>\r\n";
  ss << "<exp_max_cycle_count>" << ValueExpMaxCycleCountToString()
     << "</exp_max_cycle_count>\r\n";
  ss << "<exp_max_cycle_power>" << ValueExpMaxCyclePowerToString()
     << "</exp_max_cycle_power>\r\n";
  ss << "<exp_frequency_fluctuations_range>"
     << ValueExpFrequencyFluctuationsRangeToString()
     << "</exp_frequency_fluctuations_range>\r\n";
  if (close_tag) {
    ss << "</device_exp_ultrasound_settings>\r\n";
  }
  return ss.str();
}

std::unique_ptr<DeviceUltrasoundSettings> DeviceUltrasoundSettings::FromXml(
    const std::string& xml) {
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError error = doc.Parse(xml.c_str());
  if (error != tinyxml2::XML_SUCCESS) {
    return nullptr;
  }

  tinyxml2::XMLElement* root =
      doc.FirstChildElement("device_exp_ultrasound_settings");
  if (root == nullptr) {
    return nullptr;
  }

  int32_t exp_clipping_enable = 0;
  int64_t exp_clip_time_duration = 0;
  int64_t exp_clip_time_paused = 0;
  double exp_max_cycle_count = 0;
  int32_t exp_max_cycle_power = 0;
  int32_t exp_frequency_fluctuations_range = 0;

  tinyxml2::XMLElement* element =
      root->FirstChildElement("exp_clipping_enable");
  if (element != nullptr) {
    std::string str = element->GetText();
    if (str == "enable") {
      exp_clipping_enable = 1;
    }
  }

  element = root->FirstChildElement("exp_clip_time_duration");
  if (element != nullptr) {
    exp_clip_time_duration = std::stoll(element->GetText());
  }

  element = root->FirstChildElement("exp_clip_time_paused");
  if (element != nullptr) {
    exp_clip_time_paused = std::stoll(element->GetText());
  }

  element = root->FirstChildElement("exp_max_cycle_count");
  if (element != nullptr) {
    exp_max_cycle_count = std::stod(element->GetText());
  }

  element = root->FirstChildElement("exp_max_cycle_power");
  if (element != nullptr) {
    exp_max_cycle_power = std::stoi(element->GetText());
  }

  element = root->FirstChildElement("exp_frequency_fluctuations_range");
  if (element != nullptr) {
    exp_frequency_fluctuations_range = std::stoi(element->GetText());
  }

  std::unique_ptr<DeviceUltrasoundSettings> settings(
      new DeviceUltrasoundSettings());
  settings->exp_clipping_enable_ = exp_clipping_enable;
  settings->exp_clip_time_duration_ = exp_clip_time_duration;
  settings->exp_clip_time_paused_ = exp_clip_time_paused;
  settings->exp_max_cycle_count_ = exp_max_cycle_count;
  settings->exp_max_cycle_power_ = exp_max_cycle_power;
  settings->exp_frequency_fluctuations_range_ =
      exp_frequency_fluctuations_range;
  return settings;
}
////////////////////////////////////////////////////////////////////
// helper function

std::string DefaultDeviceUltrasoundSettingsXmlFilePath() {
#if defined(_WIN32) || defined(_WIN64)
  return "default\\device_exp_ultrasound_settings.xml";
#else
  return "default/device_exp_ultrasound_settings.xml";
#endif
}

std::unique_ptr<DeviceUltrasoundSettings>
LoadDeviceUltrasoundSettingsWithFilePath(const std::string& file_path) {
  std::string file_content;
  if (!anx::common::ReadFile(file_path, &file_content, true)) {
    LOG_F(LG_ERROR) << "Failed to read file: " << file_path;
    return nullptr;
  }

  // Parse the file content
  std::unique_ptr<DeviceUltrasoundSettings> ultrasound_settings =
      DeviceUltrasoundSettings::FromXml(file_content);
  if (ultrasound_settings.get() == nullptr) {
    return nullptr;
  }
  return ultrasound_settings;
}

std::unique_ptr<DeviceUltrasoundSettings>
LoadDeviceUltrasoundSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceUltrasoundSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif

  std::unique_ptr<DeviceUltrasoundSettings> ultrasound_settings =
      LoadDeviceUltrasoundSettingsWithFilePath(default_xml);
  return ultrasound_settings;
}

int32_t SaveDeviceUltrasoundSettingsFile(
    const std::string& file_path,
    const DeviceUltrasoundSettings& settings) {
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

int32_t SaveDeviceUltrasoundSettingsDefaultResource(
    const DeviceUltrasoundSettings& settings) {
  std::string default_xml = DefaultDeviceUltrasoundSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif

  return SaveDeviceUltrasoundSettingsFile(default_xml, settings);
}
}  // namespace device
}  // namespace anx
