/**
 * @file device_com_settings_helper.cc
 * @author hhool (hhool@outlook.com)
 * @brief com settings helper implementation
 * @version 0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_com_settings_helper.h"

#include <memory>
#include <sstream>
#include <string>

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"
#include "app/device/device_com_settings.h"
#include "app/ui/app_config.h"

namespace anx {
namespace device {

std::string DefaultDeviceComSettingsXmlFilePath(int32_t device_com_type) {
  std::string default_xml = "default";
  default_xml += anx::common::kPathSeparator;
  if (device_com_type == kDeviceCom_Ultrasound) {
    default_xml += "com_settings_ua.xml";
    return default_xml;
  } else if (device_com_type == kDeviceCom_StaticLoad) {
    default_xml += "com_settings_sl.xml";
    return default_xml;
  } else if (device_com_type == kDeviceLan_StaticLoad) {
    default_xml += "com_settings_sl2.xml";
    return default_xml;
  }
  return "";
}

std::unique_ptr<ComSettings> LoadDeviceComSettingsWithFilePath(
    const std::string& file_path) {
  // Read the file content
  std::string file_content;
  if (!anx::common::ReadFile(file_path, &file_content, true)) {
    LOG_F(LG_ERROR) << "Failed to read file: " << file_path;
    return nullptr;
  }
  // Parse the file content
  std::unique_ptr<ComSettings> com_settings =
      anx::device::ComSettings::FromXml(file_content);
  if (com_settings.get() == nullptr) {
    return nullptr;
  }
  return com_settings;
}

std::unique_ptr<ComSettings> LoadDeviceComSettingsDefaultResourceWithType(
    int32_t device_com_type) {
  std::string default_xml =
      DefaultDeviceComSettingsXmlFilePath(device_com_type);
  // get app data path
  std::string app_data_dir =
      anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;

  std::unique_ptr<ComSettings> com_setting =
      LoadDeviceComSettingsWithFilePath(default_xml);
  return com_setting;
}

int32_t SaveDeviceComSettingsFile(const std::string& file_path,
                                  const ComSettings& settings) {
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

int32_t SaveDeviceComSettingsFileDefaultPath(const ComSettings& settings) {
  std::string default_xml =
      DefaultDeviceComSettingsXmlFilePath(settings.GetDeviceComType());
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;

  return SaveDeviceComSettingsFile(default_xml, settings);
}

}  // namespace device
}  // namespace anx
