/**
 * @file device_exp_graph_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief  device exp graph settings class implementation
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_EXP_GRAPH_SETTINGS_H_
#define APP_DEVICE_DEVICE_EXP_GRAPH_SETTINGS_H_

#include <memory>
#include <string>

namespace anx {
namespace device {
class DeviceExpGraph {
 public:
  DeviceExpGraph();
  DeviceExpGraph(int32_t exp_graph_show_time_type,
                 bool exp_graph_always_new,
                 int32_t exp_graph_range_minitues);
  virtual ~DeviceExpGraph();

 public:
  /// @brief  exp graph type 0: current, 1: pre hour.
  // TODO(hhool): unused will be remove
  int32_t exp_graph_show_time_type_;

  /// @brief  exp graph range minitues.
  /// @note  0: 5, 1: 10, 2: 30, 3: 60.
  int32_t exp_graph_range_minitues_;
};

class DeviceExpGraphSettings : public DeviceExpGraph {
 public:
  DeviceExpGraphSettings();
  virtual ~DeviceExpGraphSettings();

 public:
  /// @brief  ToXml function
  /// @param close_tag
  /// @return  std::string
  virtual std::string ToXml(bool close_tag = true) const;

 public:
  static std::unique_ptr<DeviceExpGraphSettings> FromXml(
      const std::string& xml);
};

/// helper function
std::unique_ptr<DeviceExpGraphSettings>
LoadDeviceExpGraphSettingsDefaultResource();

int32_t SaveDeviceExpGraphSettingsDefaultResource(
    const DeviceExpGraphSettings& settings);

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_EXP_GRAPH_SETTINGS_H_
