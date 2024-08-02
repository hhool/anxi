/**
 * @file device_com_settings_helper.h
 * @author hhool (hhool@outlook.com)
 * @brief com settings struct helper
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_COM_SETTINGS_HELPER_H_
#define APP_DEVICE_DEVICE_COM_SETTINGS_HELPER_H_

#include <memory>
#include <string>

namespace anx {
namespace device {
class ComSettings;
}  // namespace device
}  // namespace anx

namespace anx {
namespace device {
/// @brief Default device com settings xml file path with device type
/// for device com settings created with default value, the device type
/// is the device type defined in the device type enum.
/// @param device_type the device type
/// ultrasound - ua
/// static_load - sl
/// Air compressor -ac
/// @return the device com settings xml file path
std::string DefaultDeviceComSettingsXmlFilePath(int32_t device_com_type);

/// @brief Load the device com settings with the file path
/// @param file_path the file path of the device com settings file
/// @return the device com settings pointer
std::unique_ptr<ComSettings> LoadDeviceComSettingsWithFilePath(
    const std::string& file_path);

/// @brief Load default the device com settings from resource with the device
/// type
/// @param device_type the device type
/// ultrasound - ua
/// static_load - sl
/// Air compressor -ac
/// @return the device com settings pointer
std::unique_ptr<ComSettings> LoadDeviceComSettingsDefaultResourceWithType(
    int32_t device_com_type);

/// @brief Save the device com settings file
/// @param file_path the file path of the device com settings file
/// @param settings the device com settings
/// @return 0 if success, -1 if failed
int32_t SaveDeviceComSettingsFile(const std::string& file_path,
                                  const ComSettings& settings);
/// @brief  Save the device com settings file with the default path
/// @param settings the device com settings
/// @return 0 if success, -1 if failed
int32_t SaveDeviceComSettingsFileDefaultPath(const ComSettings& settings);

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_COM_SETTINGS_HELPER_H_
