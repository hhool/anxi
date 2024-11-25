/**
 * @file dialog_app_settings_helper.cc
 * @author hhool (hhool@outlook.com)
 * @brief  dialog app settings base class for app, help to manage app settings
 * @version 0.1
 * @date 2024-11-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_app_settings_helper.h"

#include <string>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace settings {

int32_t SettingSTLoad::LoadStloadList(
    std::vector<SettingSTLoad::STLoadItem>* stload_list) {
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  if (app_data_dir.empty()) {
    return -1;
  }
#if defined(WIN32)
  app_data_dir += "\\anxi\\";
  std::string file_pathname = app_data_dir + "default\\app_settings_stload.xml";
#else
  app_data_dir += "/anxi/";
  std::string file_pathname = app_data_dir + "default/app_settings_stload.xml";
#endif
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(file_pathname.c_str()) != tinyxml2::XML_SUCCESS) {
    return -2;
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return -3;
  }
  tinyxml2::XMLElement* devices_item = root->FirstChildElement("devices");
  if (devices_item == nullptr) {
    return -4;
  }
  tinyxml2::XMLElement* device_item = devices_item->FirstChildElement("device");
  if (device_item == nullptr) {
    return -5;
  }
  do {
    if (device_item == nullptr) {
      break;
    }
    const char* attr_name = device_item->Attribute("name");
    if (attr_name == nullptr) {
      break;
    }
    const char* attr_id = device_item->Attribute("id");
    if (attr_id == nullptr) {
      break;
    }
    const char* attr_enabled = device_item->Attribute("enabled");
    if (attr_enabled == nullptr) {
      break;
    }
    std::string val_enabled(attr_enabled);
    bool enabled =
        (val_enabled == "true" || val_enabled == "1" || val_enabled == "True" ||
         val_enabled == "TRUE" || val_enabled == "yes" ||
         val_enabled == "Yes" || val_enabled == "YES");
    stload_list->push_back(
        SettingSTLoad::STLoadItem(attr_name, attr_id, enabled));
  } while (device_item = device_item->NextSiblingElement("device"));

  return 0;
}

int32_t SettingSTLoad::SaveStloadList(
    const std::vector<SettingSTLoad::STLoadItem>& stload_list) {
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  if (app_data_dir.empty()) {
    return -1;
  }
#if defined(WIN32)
  app_data_dir += "\\anxi\\";
  std::string file_pathname = app_data_dir + "default\\app_settings_stload.xml";
#else
  app_data_dir += "/anxi/";
  std::string file_pathname = app_data_dir + "default/app_settings_stload.xml";
#endif
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertFirstChild(root);
  tinyxml2::XMLElement* devices_item = doc.NewElement("devices");
  root->InsertEndChild(devices_item);
  for (const auto& stload : stload_list) {
    tinyxml2::XMLElement* device_item = doc.NewElement("device");
    device_item->SetAttribute("name", stload.name_.c_str());
    device_item->SetAttribute("id", stload.id_.c_str());
    device_item->SetAttribute("enabled", stload.enable_);
    devices_item->InsertEndChild(device_item);
  }
  auto err = doc.SaveFile(file_pathname.c_str());
  if (err != tinyxml2::XML_SUCCESS) {
    return -2;
  }

  return 0;
}

int32_t SettingSTLoad::GetEnableStloadVersionFromList(
    const std::vector<SettingSTLoad::STLoadItem>& stload_list) {
  for (const auto& stload : stload_list) {
    if (stload.enable_) {
      if (stload.id_ == "1") {
        return 1;
      } else if (stload.id_ == "2") {
        return 2;
      }
    }
  }
  return 1;
}

int32_t SettingSTLoad::GetEnableStloadVersion() {
  std::vector<SettingSTLoad::STLoadItem> stload_list;
  int32_t ret = LoadStloadList(&stload_list);
  if (ret != 0) {
    return -1;
  }
  return GetEnableStloadVersionFromList(stload_list);
}
//////////////////////////////////////////////////////////////////////////
/// SettingAppThird
std::vector<std::string> SettingAppThird::LoadThirdApp() {
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  if (app_data_dir.empty()) {
    return std::vector<std::string>();
  }
#if defined(WIN32)
  app_data_dir += "\\anxi\\";
  std::string file_pathname =
      app_data_dir + "default\\app_settings_third_app.xml";
#else
  app_data_dir += "/anxi/";
  std::string file_pathname =
      app_data_dir + "default/app_settings_third_app.xml";
#endif
  std::vector<std::string> third_app_list;
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(file_pathname.c_str()) != tinyxml2::XML_SUCCESS) {
    return std::vector<std::string>();
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return std::vector<std::string>();
  }
  tinyxml2::XMLElement* ele_item = root->FirstChildElement("item");
  if (ele_item == nullptr) {
    return std::vector<std::string>();
  }
  do {
    if (ele_item == nullptr) {
      break;
    }
    tinyxml2::XMLElement* ele_name = ele_item->FirstChildElement("name");
    if (ele_name == nullptr) {
      break;
    }
    tinyxml2::XMLElement* ele_path = ele_item->FirstChildElement("path");
    if (ele_path == nullptr) {
      break;
    }
    third_app_list.push_back(ele_name->GetText());
    third_app_list.push_back(ele_path->GetText());
  } while (ele_item = ele_item->NextSiblingElement("item"));
  return third_app_list;
}

int32_t SettingAppThird::SaveThirdApp(const std::string& name,
                                      const std::string& third_app_path) {
  std::string module_dir = anx::common::GetApplicationDataPath();
  if (module_dir.empty()) {
    return -1;
  }
#if defined(WIN32)
  module_dir += "\\anxi\\";
  std::string module_path = module_dir + "default\\app_settings_third_app.xml";
#else
  module_dir += "/anxi/";
  std::string module_path = module_dir + "default/app_settings_third_app.xml";
#endif
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLElement* root = doc.NewElement("root");
  doc.InsertFirstChild(root);
  tinyxml2::XMLElement* ele_item = doc.NewElement("item");
  root->InsertEndChild(ele_item);
  tinyxml2::XMLElement* ele_name = doc.NewElement("name");
  ele_name->SetText(name.c_str());
  ele_item->InsertEndChild(ele_name);
  tinyxml2::XMLElement* ele_path = doc.NewElement("path");
  ele_path->SetText(third_app_path.c_str());
  ele_item->InsertEndChild(ele_path);
  auto err = doc.SaveFile(module_path.c_str());
  if (err != tinyxml2::XML_SUCCESS) {
    return -2;
  }
  return 0;
}

std::string SettingAppThird::GetThirdAppPath() {
  std::vector<std::string> infos = LoadThirdApp();
  if (infos.size() < 2) {
    return "";
  }
  return infos[1];
}

std::string SettingAppThird::GetThirdAppName() {
  std::vector<std::string> infos = LoadThirdApp();
  if (infos.size() < 2) {
    return "";
  }
  return infos[0];
}

}  // namespace settings
}  // namespace anx
