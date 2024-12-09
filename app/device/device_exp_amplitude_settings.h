/**
 * @file device_exp_amplitude_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief  device exp amplitude settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_EXP_AMPLITUDE_SETTINGS_H_
#define APP_DEVICE_DEVICE_EXP_AMPLITUDE_SETTINGS_H_

#include <map>
#include <memory>
#include <string>

namespace anx {
namespace device {
class DeviceExpAmplitude {
 public:
  DeviceExpAmplitude();
  explicit DeviceExpAmplitude(
      const std::map<int32_t, float>& exp_power2amp_map);
  virtual ~DeviceExpAmplitude();

 public:
  std::map<int32_t, float> exp_power2amp_map_;
};

class DeviceExpAmplitudeSettings : public DeviceExpAmplitude {
 public:
  DeviceExpAmplitudeSettings();
  DeviceExpAmplitudeSettings(
      const std::map<int32_t, float>& exp_power2amp_map);
  virtual ~DeviceExpAmplitudeSettings();

 public:
  /// @brief  ToXml function
  /// @param close_tag
  /// @return  std::string
  virtual std::string ToXml(bool close_tag = true) const;

 public:
  static std::unique_ptr<DeviceExpAmplitudeSettings> FromXml(
      const std::string& xml);
};

///////////////////////////////////////////////////////////////////////////////
/// helper function
/// @brief LoadDeviceExpAmplitudeSettingsDefaultResource function
/// load device exp amplitude settings from default resource file
/// that stored in the application data directory with the file name
/// device_exp_amplitude_settings.xml
/// @return std::unique_ptr<DeviceExpAmplitudeSettings>
std::unique_ptr<DeviceExpAmplitudeSettings>
LoadDeviceExpAmplitudeSettingsDefaultResource();

/// @brief  SaveDeviceExpAmplitudeSettingsDefaultResource function
/// save device exp amplitude settings to default resource file
/// that stored in the application data directory with the file name
/// device_exp_amplitude_settings.xml
int32_t SaveDeviceExpAmplitudeSettingsDefaultResource(
    const DeviceExpAmplitudeSettings& settings);

/// @brief  ResetDeviceExpAmplitudeSettingsDefaultResource function
/// reset device exp amplitude settings to default resource file
/// that stored in the application data directory with the file name
/// device_exp_amplitude_settings.xml from origin resource file at application
/// path
int32_t ResetDeviceExpAmplitudeSettingsDefaultResource();

/// @brief  amp to device amp
/// @param amp
/// @param device_amp
/// @return int32_t 0 success, -1 failed
int32_t Amp2DeviceExpPower(const DeviceExpAmplitudeSettings& settings,
                           float exp_amp,
                           int32_t* exp_power);

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_EXP_AMPLITUDE_SETTINGS_H_
