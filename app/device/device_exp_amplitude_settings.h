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
  DeviceExpAmplitude(const std::map<int32_t, int32_t>& exp_power2amp_map);
  virtual ~DeviceExpAmplitude();

 public:
  std::map<int32_t, int32_t> exp_power2amp_map_;
};

class DeviceExpAmplitudeSettings : public DeviceExpAmplitude {
 public:
  DeviceExpAmplitudeSettings();
  DeviceExpAmplitudeSettings(
      const std::map<int32_t, int32_t>& exp_power2amp_map);
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

/// helper function
std::unique_ptr<DeviceExpAmplitudeSettings>
LoadDeviceExpAmplitudeSettingsDefaultResource();

int32_t SaveDeviceExpAmplitudeSettingsDefaultResource(
    const DeviceExpAmplitudeSettings& settings);

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_EXP_AMPLITUDE_SETTINGS_H_
