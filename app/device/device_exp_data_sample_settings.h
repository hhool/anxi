/**
 * @file device_exp_data_sample_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief  device exp data sample settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_EXP_DATA_SAMPLE_SETTINGS_H_
#define APP_DEVICE_DEVICE_EXP_DATA_SAMPLE_SETTINGS_H_

#include <memory>
#include <string>

namespace anx {
namespace device {
class DeviceExpDataSample {
 public:
  DeviceExpDataSample();
  DeviceExpDataSample(int32_t sample_mode,
                      int32_t sampling_start_pos,
                      int32_t sampling_end_pos,
                      int32_t sampling_interval);
  virtual ~DeviceExpDataSample();

 public:
  /// @brief  sample mode 0: exponent, 1: linear.
  int32_t sample_mode_;
  /// @brief  sampling frequency start position.
  int32_t sampling_start_pos_;
  /// @brief  sampling frequency end position.
  int32_t sampling_end_pos_;
  /// @brief  sampling frequency step.
  int32_t sampling_interval_;

 public:
  std::string ValueSampleModeToString() const;
  std::string ValueSamplingIntervalToString() const;
  std::string ValueSamplingEndPosToString() const;
  std::string ValueSamplingStartPosToString() const;

 public:
  static int32_t ValueSampleModeFromString(const std::string& sample_mode_str);
};

class DeviceExpDataSampleSettings : public DeviceExpDataSample {
 public:
  DeviceExpDataSampleSettings();
  virtual ~DeviceExpDataSampleSettings();

 public:
  /// @brief  ToXml function
  /// @param close_tag
  /// @return  std::string
  virtual std::string ToXml(bool close_tag = true) const;

 public:
  static std::unique_ptr<DeviceExpDataSampleSettings> FromXml(
      const std::string& xml);
};

/// helper function
std::unique_ptr<DeviceExpDataSampleSettings>
LoadDeviceExpDataSampleSettingsDefaultResource();

int32_t SaveDeviceExpDataSampleSettingsDefaultResource(
    const DeviceExpDataSampleSettings& settings);

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_EXP_DATA_SAMPLE_SETTINGS_H_
