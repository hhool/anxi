/**
 * @file device_exp_graph_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief  device exp graph settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_exp_graph_settings.h"

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
// clz DeviceExpGraph
DeviceExpGraph::DeviceExpGraph()
    : exp_graph_show_time_type_(0), exp_graph_range_minitues_(0) {}

DeviceExpGraph::DeviceExpGraph(int32_t exp_graph_show_time_type,
                               int32_t exp_graph_range_minitues)
    : exp_graph_show_time_type_(exp_graph_show_time_type),
      exp_graph_range_minitues_(exp_graph_range_minitues) {}

DeviceExpGraph::~DeviceExpGraph() {}

////////////////////////////////////////////////////////////////////
// clz DeviceExpGraphSettings
DeviceExpGraphSettings::DeviceExpGraphSettings() {}

DeviceExpGraphSettings::~DeviceExpGraphSettings() {}

std::string DeviceExpGraphSettings::ToXml(bool close_tag) const {
  std::stringstream ss;
  if (close_tag) {
    ss << "<exp_graph_settings>\r\n";
  }
  ss << "<exp_graph_show_time_type>" << exp_graph_show_time_type_
     << "</exp_graph_show_time_type>\r\n";
  ss << "<exp_graph_range_minitues>" << exp_graph_range_minitues_
     << "</exp_graph_range_minitues>\r\n";
  if (close_tag) {
    ss << "</exp_graph_settings>\r\n";
  }
  return ss.str();
}

std::unique_ptr<DeviceExpGraphSettings> DeviceExpGraphSettings::FromXml(
    const std::string& xml) {
  tinyxml2::XMLDocument doc;
  if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
    return nullptr;
  }
  auto root = doc.FirstChildElement("exp_graph_settings");
  if (!root) {
    return nullptr;
  }
  auto exp_graph_show_time_type =
      root->FirstChildElement("exp_graph_show_time_type");
  if (!exp_graph_show_time_type) {
    return nullptr;
  }
  auto exp_graph_range_minitues =
      root->FirstChildElement("exp_graph_range_minitues");
  if (!exp_graph_range_minitues) {
    return nullptr;
  }
  std::unique_ptr<DeviceExpGraphSettings> settings(
      new DeviceExpGraphSettings());
  settings->exp_graph_show_time_type_ = exp_graph_show_time_type->IntText(0);
  settings->exp_graph_range_minitues_ = exp_graph_range_minitues->IntText(0);
  return settings;
}

////////////////////////////////////////////////////////////////////
// helper function
std::string DefaultDeviceExpGraphSettingsXmlFilePath() {
  std::string default_xml = "default";
  default_xml += anx::common::kPathSeparator;
  default_xml += "device_exp_graph_settings.xml";
  return default_xml;
}

std::unique_ptr<DeviceExpGraphSettings>
LoadDeviceExpGraphSettingsDefaultWithFilePath(const std::string& file_path) {
  std::string file_content;
  if (!anx::common::ReadFile(file_path, &file_content, true)) {
    LOG_F(LG_ERROR) << "Failed to read file: " << file_path;
    return nullptr;
  }

  // Parse the file content
  std::unique_ptr<DeviceExpGraphSettings> settings =
      DeviceExpGraphSettings::FromXml(file_content);
  if (settings.get() == nullptr) {
    return nullptr;
  }
  return settings;
}

std::unique_ptr<DeviceExpGraphSettings>
LoadDeviceExpGraphSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceExpGraphSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;

  std::unique_ptr<DeviceExpGraphSettings> settings =
      LoadDeviceExpGraphSettingsDefaultWithFilePath(default_xml);
  return settings;
}

int32_t SaveDeviceExpGraphSettingsFile(const std::string& file_path,
                                       const DeviceExpGraphSettings& settings) {
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

int32_t SaveDeviceExpGraphSettingsDefaultResource(
    const DeviceExpGraphSettings& settings) {
  std::string default_xml = DefaultDeviceExpGraphSettingsXmlFilePath();
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;
  return SaveDeviceExpGraphSettingsFile(default_xml, settings);
}

}  // namespace device
}  // namespace anx
