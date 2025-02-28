/**
 * @file device_exp_data_sample_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief  device exp data_sample settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_exp_data_sample_settings.h"

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
// clz DeviceExpDataSample
DeviceExpDataSample::DeviceExpDataSample()
    : sample_mode_(kSampleModeLinear),
      sampling_start_pos_(0),
      sampling_end_pos_(0),
      sampling_interval_(20) {}

DeviceExpDataSample::DeviceExpDataSample(int32_t sample_mode,
                                         int64_t sampling_start_pos,
                                         int64_t sampling_end_pos,
                                         int32_t sampling_interval)
    : sample_mode_(sample_mode),
      sampling_start_pos_(sampling_start_pos),
      sampling_end_pos_(sampling_end_pos),
      sampling_interval_(sampling_interval) {}

DeviceExpDataSample::~DeviceExpDataSample() {}

std::string DeviceExpDataSample::ValueSampleModeToString() const {
  if (sample_mode_ == kSampleModeExponent) {
    return "exponent";
  } else if (sample_mode_ == kSampleModeLinear) {
    return "linear";
  } else {
    return "linear";
  }
}

std::string DeviceExpDataSample::ValueSamplingIntervalToString() const {
  return std::to_string(sampling_interval_);
}

std::string DeviceExpDataSample::ValueSamplingEndPosToString() const {
  return std::to_string(sampling_end_pos_);
}

std::string DeviceExpDataSample::ValueSamplingStartPosToString() const {
  return std::to_string(sampling_start_pos_);
}

int32_t DeviceExpDataSample::ValueSampleModeFromString(
    const std::string& sample_mode_str) {
  if (sample_mode_str == "exponent") {
    return 0;
  } else if (sample_mode_str == "linear") {
    return 1;
  } else {
    return 1;
  }
}

////////////////////////////////////////////////////////////////////
// clz DeviceExpDataSampleSettings
DeviceExpDataSampleSettings::DeviceExpDataSampleSettings() {}

DeviceExpDataSampleSettings::~DeviceExpDataSampleSettings() {}

std::string DeviceExpDataSampleSettings::ToXml(bool close_tag) const {
  std::stringstream ss;
  if (close_tag) {
    ss << "<exp_data_sample_settings>\r\n";
  }
  ss << "<sample_mode>" << ValueSampleModeToString() << "</sample_mode>\r\n";
  ss << "<sampling_start_pos>" << ValueSamplingStartPosToString()
     << "</sampling_start_pos>\r\n";
  ss << "<sampling_end_pos>" << ValueSamplingEndPosToString()
     << "</sampling_end_pos>\r\n";
  ss << "<sampling_interval>" << ValueSamplingIntervalToString()
     << "</sampling_interval>\r\n";
  if (close_tag) {
    ss << "</exp_data_sample_settings>\r\n";
  }
  return ss.str();
}

std::unique_ptr<DeviceExpDataSampleSettings>
DeviceExpDataSampleSettings::FromXml(const std::string& xml) {
  tinyxml2::XMLDocument doc;
  if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
    return nullptr;
  }
  auto root = doc.FirstChildElement("exp_data_sample_settings");
  if (!root) {
    return nullptr;
  }
  auto ele_sample_mode = root->FirstChildElement("sample_mode");
  if (!ele_sample_mode) {
    return nullptr;
  }
  auto ele_sampling_start_pos = root->FirstChildElement("sampling_start_pos");
  if (!ele_sampling_start_pos) {
    return nullptr;
  }
  auto ele_sampling_end_pos = root->FirstChildElement("sampling_end_pos");
  if (!ele_sampling_end_pos) {
    return nullptr;
  }
  auto ele_sampling_interval = root->FirstChildElement("sampling_interval");
  if (!ele_sampling_interval) {
    return nullptr;
  }

  std::unique_ptr<DeviceExpDataSampleSettings> settings(
      new DeviceExpDataSampleSettings());
  settings->sample_mode_ =
      DeviceExpDataSampleSettings::ValueSampleModeFromString(
          ele_sample_mode->GetText());
  settings->sampling_start_pos_ = std::atoll(ele_sampling_start_pos->GetText());
  settings->sampling_end_pos_ = std::atoll(ele_sampling_end_pos->GetText());
  settings->sampling_interval_ = std::atoi(ele_sampling_interval->GetText());
  return settings;
}

////////////////////////////////////////////////////////////////////
// helper function
std::string DefaultDeviceExpDataSampleSettingsXmlFilePath() {
  std::string default_xml = "default";
  default_xml += anx::common::kPathSeparator;
  default_xml += "device_exp_data_sample_settings.xml";
  return default_xml;
}

std::unique_ptr<DeviceExpDataSampleSettings>
LoadDeviceExpDataSampleSettingsDefaultWithFilePath(
    const std::string& file_path) {
  std::string file_content;
  if (!anx::common::ReadFile(file_path, &file_content, true)) {
    LOG_F(LG_ERROR) << "Failed to read file: " << file_path;
    return nullptr;
  }

  // Parse the file content
  std::unique_ptr<DeviceExpDataSampleSettings> settings =
      DeviceExpDataSampleSettings::FromXml(file_content);
  if (settings.get() == nullptr) {
    return nullptr;
  }
  return settings;
}

std::unique_ptr<DeviceExpDataSampleSettings>
LoadDeviceExpDataSampleSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceExpDataSampleSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;

  std::unique_ptr<DeviceExpDataSampleSettings> settings =
      LoadDeviceExpDataSampleSettingsDefaultWithFilePath(default_xml);
  return settings;
}

int32_t SaveDeviceExpDataSampleSettingsFile(
    const std::string& file_path,
    const DeviceExpDataSampleSettings& settings) {
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

int32_t SaveDeviceExpDataSampleSettingsDefaultResource(
    const DeviceExpDataSampleSettings& settings) {
  std::string default_xml = DefaultDeviceExpDataSampleSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;

  return SaveDeviceExpDataSampleSettingsFile(default_xml, settings);
}

int32_t ResetDeviceExpDataSampleSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceExpDataSampleSettingsXmlFilePath();
  // get app data path"
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  std::string dst_xml =
      app_data_dir + anx::common::kPathSeparator + default_xml;
  std::string origin_xml =
      anx::common::GetModuleDir() + anx::common::kPathSeparator + default_xml;

  if (!anx::common::FileExists(origin_xml)) {
    LOG_F(LG_ERROR) << "origin file not exists: " << origin_xml;
    return -1;
  }
  if (!anx::common::CCopyFile(origin_xml, dst_xml)) {
    LOG_F(LG_ERROR) << "copy file failed: " << origin_xml << " to " << dst_xml;
    return -2;
  }
  return 0;
}

}  // namespace device
}  // namespace anx
