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

#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
////////////////////////////////////////////////////////////////////////
// clz DeviceLoadStatic
DeviceLoadStatic::DeviceLoadStatic()
    : direct_(0), action_(0), speed_(0), retention_(0) {}

DeviceLoadStatic::DeviceLoadStatic(int32_t direct,
                                   int32_t action,
                                   int32_t speed,
                                   int32_t retention)
    : direct_(direct), action_(action), speed_(speed), retention_(retention) {}

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

std::string DeviceLoadStatic::ValueActionToString() const {
  if (action_ == 0) {
    return "none";
  } else if (action_ == 1) {
    return "pull";
  } else if (action_ == 2) {
    return "push";
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

int32_t DeviceLoadStatic::ValueActionFromString(const std::string& action_str) {
  if (action_str == "none") {
    return 0;
  } else if (action_str == "pull") {
    return 1;
  } else if (action_str == "push") {
    return 2;
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
  xml += "<action>";
  xml += ValueActionToString();
  xml += "</action>\r\n";
  xml += "<speed>";
  xml += std::to_string(speed_);
  xml += "</speed>\r\n";
  xml += "<retention>";
  xml += std::to_string(retention_);
  xml += "</retention>\r\n";
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
  tinyxml2::XMLElement* ele_action = root->FirstChildElement("action");
  if (ele_action == nullptr) {
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
  std::unique_ptr<DeviceLoadStaticSettings> settings(
      new DeviceLoadStaticSettings());
  settings->direct_ = ValueDirectFromString(ele_direct->GetText());
  settings->action_ = ValueActionFromString(ele_action->GetText());
  settings->speed_ = std::stoi(ele_speed->GetText());
  settings->retention_ = std::stoi(ele_retention->GetText());
  return settings;
}

////////////////////////////////////////////////////////////////////////
// helper function
std::string DefaultDeviceLoadStaticSettingsXmlFilePath() {
  return "default/device_exp_load_static_settings.xml";
}

std::unique_ptr<DeviceLoadStaticSettings>
LoadDeviceLoadStaticSettingsWithFilePath(const std::string& file_path) {
  FILE* file = fopen(file_path.c_str(), "rb");
  if (file == nullptr) {
    return nullptr;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Read the file content
  std::unique_ptr<char[]> file_content(new char[file_size]);
  fread(file_content.get(), 1, file_size, file);
  fclose(file);

  // Parse the file content
  std::string content(file_content.get(), file_size);
  std::unique_ptr<DeviceLoadStaticSettings> settings =
      DeviceLoadStaticSettings::FromXml(content);
  if (settings.get() == nullptr) {
    return nullptr;
  }
  return settings;
}

std::unique_ptr<DeviceLoadStaticSettings>
LoadDeviceLoadStaticSettingsDefaultResource() {
  std::string default_xml = DefaultDeviceLoadStaticSettingsXmlFilePath();
  std::string module_dir = anx::common::GetModuleDir();
  default_xml = module_dir + "\\" + default_xml;

  std::unique_ptr<DeviceLoadStaticSettings> settings =
      LoadDeviceLoadStaticSettingsWithFilePath(default_xml);
  return settings;
}

int32_t SaveDeviceLoadStaticSettingsFile(
    const std::string& file_path,
    const DeviceLoadStaticSettings& settings) {
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

int32_t SaveDeviceLoadStaticSettingsDefaultResource(
    const DeviceLoadStaticSettings& settings) {
  std::string default_xml = DefaultDeviceLoadStaticSettingsXmlFilePath();
  std::string module_dir = anx::common::GetModuleDir();
  default_xml = module_dir + "\\" + default_xml;
  return SaveDeviceLoadStaticSettingsFile(default_xml, settings);
}

}  // namespace device
}  // namespace anx
