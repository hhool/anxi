/**
 * @file device_exp_load_static_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief device load static settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_EXP_LOAD_STATIC_SETTINGS_H_
#define APP_DEVICE_DEVICE_EXP_LOAD_STATIC_SETTINGS_H_

#include <memory>
#include <string>

namespace anx {
namespace device {
class DeviceLoadStatic {
 public:
  DeviceLoadStatic();
  DeviceLoadStatic(int32_t direct,
                   int32_t action,
                   int32_t speed,
                   int32_t retention);
  virtual ~DeviceLoadStatic();

 public:
  /// @brief speed
  int32_t speed_;
  /// @brief retention
  int32_t retention_;
};

class DeviceLoadStaticSettings : public DeviceLoadStatic {
 public:
  DeviceLoadStaticSettings();
  virtual ~DeviceLoadStaticSettings();

 public:
  /// @brief  ToXml function
  /// @param close_tag
  /// @return  std::string
  virtual std::string ToXml(bool close_tag = true) const;

 public:
  static std::unique_ptr<DeviceLoadStaticSettings> FromXml(
      const std::string& xml);
};

////////////////////////////////////////////////////////////
/// helper function

/// @brief  LoadDeviceLoadStaticSettingsDefaultResource function load
/// default resource from file @see DeviceLoadStaticSettings @see
/// DeviceLoadStatic
/// @return  std::unique_ptr<DeviceLoadStaticSettings> DeviceLoadStaticSettings
/// ptr or nullptr if failed @see DeviceLoadStaticSettings @see DeviceLoadStatic
std::unique_ptr<DeviceLoadStaticSettings>
LoadDeviceLoadStaticSettingsDefaultResource();
/// @brief  SaveDeviceLoadStaticSettingsDefaultResource function save
/// default resource to file @see DeviceLoadStaticSettings @see
/// DeviceLoadStatic
/// @param settings DeviceLoadStaticSettings
/// @return  int32_t 0: success, -1: failed
int32_t SaveDeviceLoadStaticSettingsDefaultResource(
    const DeviceLoadStaticSettings& settings);
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_EXP_LOAD_STATIC_SETTINGS_H_
