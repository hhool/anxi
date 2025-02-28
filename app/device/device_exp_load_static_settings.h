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
                   int32_t ctrl_type,
                   float speed,
                   float threshold,
                   float retention,
                   float displacement,
                   float keep_load_duration);
  virtual ~DeviceLoadStatic();

 public:
  /// @brief direct 0: none, 1: up, 2: down, 3: left, 4: right
  int32_t direct_;
  /// @brief  control type 0: load, 1: extn, 2: posi
  int32_t ctrl_type_;
  /// @brief speed
  float speed_;
  /// @brief threshold
  float threshold_;
  /// @brief retention
  float retention_;
  /// @brief  displacement unit: mm
  float displacement_;
  /// @brief keep_load_duration in seconds unit: second
  float keep_load_duration_;

 public:
  std::string ValueDirectToString() const;

 public:
  /// @brief  direct string value to bool
  /// @param direct_str  none, up, down, left, right
  /// @return  int32_t 0: none, 1: up, 2: down, 3: left, 4: right
  static int32_t ValueDirectFromString(const std::string& direct_str);
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
/// @brief ResetDeviceLoadStaticSettingsDefaultResource function reset
/// reset device load static settings to default resource file
/// that stored in the application data directory with the file name
/// device_exp_load_static_settings.xml from origin resource file at application
/// path

int32_t ResetDeviceLoadStaticSettingsDefaultResource();

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_EXP_LOAD_STATIC_SETTINGS_H_
