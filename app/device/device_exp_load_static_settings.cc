/**
 * @file device_exp_load_static_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief device load static settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_exp_load_static_settings.h"

#include <sstream>
#include <string>

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
////////////////////////////////////////////////////////////////////////
// clz DeviceLoadStatic
DeviceLoadStatic::DeviceLoadStatic()
    : direct_(0),
      ctrl_type_(0),
      speed_(50),
      retention_(10),
      displacement_(50),
      keep_load_duration_(120) {}

DeviceLoadStatic::DeviceLoadStatic(int32_t direct,
                                   int32_t ctrl_type,
                                   int32_t speed,
                                   int32_t retention,
                                   int32_t displacement,
                                   int32_t keep_load_duration)
    : direct_(direct),
      ctrl_type_(ctrl_type),
      speed_(speed),
      retention_(retention),
      displacement_(displacement),
      keep_load_duration_(keep_load_duration) {}

DeviceLoadStatic::~DeviceLoadStatic() {}

std::string DeviceLoadStatic::ValueDirectToString() const {
  if (direct_ == 0) {
    return "none";

  } else if (direct_ == 1) {
    return "up";

  } else if (direct_ == 2) {
    return "down";

  } else if (direct_ == 3) {
    return "left";

  } else if (direct_ == 4) {
    return "right";

  } else {
    return "none";
  }
}
int32_t DeviceLoadStatic::ValueDirectFromString(const std::string& direct_str) {
  if (direct_str == "none") {
    return 0;

  } else if (direct_str == "up") {
    return 1;

  } else if (direct_str == "down") {
    return 2;

  } else if (direct_str == "left") {
    return 3;

  } else if (direct_str == "right") {
    return 4;

  } else {
    return 0;
  }
}
////////////////////////////////////////////////////////////////////////
// clz DeviceLoadStaticSettings
DeviceLoadStaticSettings::DeviceLoadStaticSettings() {}

DeviceLoadStaticSettings::~DeviceLoadStaticSettings() {}

std::string DeviceLoadStaticSettings::ToXml(bool close_tag) const {
  std::string xml;
  xml += "<device_exp_load_static_settings>\r\n";
  xml += "<direct>";
  xml += ValueDirectToString();
  xml += "</direct>\r\n";
  xml += "<ctrl_type>";
  xml += std::to_string(ctrl_type_);
  xml += "</ctrl_type>\r\n";
  xml += "<speed>";
  xml += std::to_string(speed_);
  xml += "</speed>\r\n";
  xml += "<retention>";
  xml += std::to_string(retention_);
  xml += "</retention>\r\n";
  xml += "<displacement>";
  xml += std::to_string(displacement_);
  xml += "</displacement>\r\n";
  xml += "<keep_load_duration>";
  xml += std::to_string(keep_load_duration_);
  xml += "</keep_load_duration>\r\n";
  if (close_tag) {
    xml += "</device_exp_load_static_settings>\r\n";
  }
  return xml;
}

std::unique_ptr<DeviceLoadStaticSettings> DeviceLoadStaticSettings::FromXml(
    const std::string& xml) {
  tinyxml2::XMLDocument doc;
  doc.Parse(xml.c_str());
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_direct = root->FirstChildElement("direct");
  if (ele_direct == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_ctrltype = root->FirstChildElement("ctrl_type");
  if (ele_ctrltype == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_speed = root->FirstChildElement("speed");
  if (ele_speed == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_retention = root->FirstChildElement("retention");
  if (ele_retention == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_displacement =
      root->FirstChildElement("displacement");
  if (ele_displacement == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_keep_load_duration =
      root->FirstChildElement("keep_load_duration");
  if (ele_keep_load_duration == nullptr) {
    return nullptr;
  }

  std::unique_ptr<DeviceLoadStaticSettings> settings(
      new DeviceLoadStaticSettings());
  settings->direct_ = ValueDirectFromString(ele_direct->GetText());
  settings->ctrl_type_ = std::stoi(ele_ctrltype->GetText());
  settings->speed_ = std::stoi(ele_speed->GetText());
  settings->retention_ = std::stoi(ele_retention->GetText());
  settings->displacement_ = std::stoi(ele_displacement->GetText());
  settings->keep_load_duration_ = std::stoi(ele_keep_load_duration->GetText());
  return settings;
}

////////////////////////////////////////////////////////////////////////
// helper function
std::string DefaultDeviceLoadStaticSettingsXmlFilePath() {
#if defined(_WIN32) || defined(_WIN64)
  return "default\\device_exp_load_static_settings.xml";
#else
  return "default/device_exp_load_static_settings.xml";
#endif
}

std::unique_ptr<DeviceLoadStaticSettings>
LoadDeviceLoadStaticSettingsWithFilePath(const std::string& file_path) {
  std::string file_content;
  if (!anx::common::ReadFile(file_path, &file_content, true)) {
    LOG_F(LG_ERROR) << "Failed to read file: " << file_path;
    return nullptr;
  }

  // Parse the file content
  std::unique_ptr<DeviceLoadStaticSettings> settings =
      DeviceLoadStaticSettings::FromXml(file_content);
  if (settings.get() == nullptr) {
    return nullptr;
  }
  return settings;
}

std::unique_ptr<DeviceLoadStaticSettings>
LoadDeviceLoadStaticSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceLoadStaticSettingsXmlFilePath();
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#if defined(_WIN32)
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/" + default_xml;
#endif

  std::unique_ptr<DeviceLoadStaticSettings> settings =
      LoadDeviceLoadStaticSettingsWithFilePath(default_xml);
  return settings;
}

int32_t SaveDeviceLoadStaticSettingsFile(
    const std::string& file_path,
    const DeviceLoadStaticSettings& settings) {
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

int32_t SaveDeviceLoadStaticSettingsDefaultResource(
    const DeviceLoadStaticSettings& settings) {
  std::string default_xml = DefaultDeviceLoadStaticSettingsXmlFilePath();
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#if defined(_WIN32)
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/" + default_xml;
#endif
  return SaveDeviceLoadStaticSettingsFile(default_xml, settings);
}

int32_t ResetDeviceLoadStaticSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceLoadStaticSettingsXmlFilePath();
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#if defined(_WIN32)
  std::string dst_xml = app_data_dir + "\\anxi\\" + default_xml;
  std::string origin_xml = anx::common::GetModuleDir() + "\\" + default_xml;
#else
  std::string dst_xml = app_data_dir + "/anxi/" + default_xml;
  std::string origin_xml = anx::common::GetModuleDir() + "/" + default_xml;
#endif
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

}  // namespace device
}  // namespace anx
