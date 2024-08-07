/**
 * @file device_exp_ultrasound_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief settings struct for device ultrasound with exp settings
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_EXP_ULTRASOUND_SETTINGS_H_
#define APP_DEVICE_DEVICE_EXP_ULTRASOUND_SETTINGS_H_

#include <memory>
#include <string>

namespace anx {
namespace device {

class DeviceUltrasound {
 public:
  DeviceUltrasound();
  DeviceUltrasound(int32_t sample_mode,
                   int32_t sampling_start_pos,
                   int32_t sampling_end_pos,
                   int32_t sampling_interval,
                   int32_t exp_clipping_enable,
                   int32_t exp_clip_time_duration,
                   int32_t exp_clip_time_paused,
                   int64_t exp_max_cycle_count,
                   int32_t exp_max_cycle_power,
                   int32_t exp_frequency_fluctuations_range);
  virtual ~DeviceUltrasound();

 public:
  /// @brief  sample mode 0: exponent, 1: linear.
  int32_t sample_mode_;
  /// @brief  sampling frequency start position.
  int32_t sampling_start_pos_;
  /// @brief  sampling frequency end position.
  int32_t sampling_end_pos_;
  /// @brief  sampling frequency step.
  int32_t sampling_interval_;

  /// @brief  exp clipping enable.
  int32_t exp_clipping_enable_;

  /// @brief  exp clipping time duration.
  int32_t exp_clip_time_duration_;
  /// @brief  exp clipping time paused.
  int32_t exp_clip_time_paused_;

  /// @brief  exp max cycle count.
  int64_t exp_max_cycle_count_;
  /// @brief  exp max cycle power.
  int32_t exp_max_cycle_power_;
  /// @brief  exp max cycle power.
  int32_t exp_frequency_fluctuations_range_;

 public:
  std::string ValueSampleModeToString() const;
  std::string ValueExpClippingEnableToString() const;
  std::string ValueExpFrequencyFluctuationsRangeToString() const;
  std::string ValueExpMaxCyclePowerToString() const;
  std::string ValueExpMaxCycleCountToString() const;
  std::string ValueExpClipTimePausedToString() const;
  std::string ValueExpClipTimeDurationToString() const;
  std::string ValueSamplingIntervalToString() const;
  std::string ValueSamplingEndPosToString() const;
  std::string ValueSamplingStartPosToString() const;

  static int32_t ValueSampleModeFromString(const std::string& sample_mode_str);
};

class DeviceUltrasoundSettings : public DeviceUltrasound {
 public:
  DeviceUltrasoundSettings();
  virtual ~DeviceUltrasoundSettings();

 public:
  /// @brief  ToXml function
  /// @param close_tag
  /// @return  std::string
  virtual std::string ToXml(bool close_tag = true) const;

 public:
  /// @brief  FromXml function
  /// @param xml
  /// @return  std::unique_ptr<DeviceUltrasoundSettings>
  static std::unique_ptr<DeviceUltrasoundSettings> FromXml(
      const std::string& xml);
};

////////////////////////////////////////////////////////
/// helper function

std::unique_ptr<DeviceUltrasoundSettings>
LoadDeviceUltrasoundSettingsDefaultResource();

int32_t SaveDeviceUltrasoundSettingsDefaultResource(
    const DeviceUltrasoundSettings& settings);
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_EXP_ULTRASOUND_SETTINGS_H_
