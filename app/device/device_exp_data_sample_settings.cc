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
#if defined(_WIN32) || defined(_WIN64)
  return "default\\device_exp_data_sample_settings.xml";
#else
  return "default/device_exp_data_sample_settings.xml";
#endif
}

std::unique_ptr<DeviceExpDataSampleSettings>
LoadDeviceExpDataSampleSettingsDefaultWithFilePath(
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
    fclose(file);
    return nullptr;
  }
  fclose(file);

  // Parse the file content
  std::string content(file_content.get(), file_size);
  std::unique_ptr<DeviceExpDataSampleSettings> settings =
      DeviceExpDataSampleSettings::FromXml(content);
  if (settings.get() == nullptr) {
    return nullptr;
  }
  return settings;
}

std::unique_ptr<DeviceExpDataSampleSettings>
LoadDeviceExpDataSampleSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceExpDataSampleSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif

  std::unique_ptr<DeviceExpDataSampleSettings> settings =
      LoadDeviceExpDataSampleSettingsDefaultWithFilePath(default_xml);
  return settings;
}

int32_t SaveDeviceExpDataSampleSettingsFile(
    const std::string& file_path,
    const DeviceExpDataSampleSettings& settings) {
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

int32_t SaveDeviceExpDataSampleSettingsDefaultResource(
    const DeviceExpDataSampleSettings& settings) {
  std::string default_xml = DefaultDeviceExpDataSampleSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif
  return SaveDeviceExpDataSampleSettingsFile(default_xml, settings);
}

}  // namespace device
}  // namespace anx
