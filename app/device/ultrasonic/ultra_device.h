/**
 * @file ultra_device.h
 * @author hhool (hhool@outlook.com)
 * @brief  ultrasonic device class
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_ULTRASONIC_ULTRA_DEVICE_H_
#define APP_DEVICE_ULTRASONIC_ULTRA_DEVICE_H_

#include <string>
#include "app/device/device_com_settings.h"
#include "app/device/device_com.h"

namespace anx {
namespace device {
class ComPortDevice;
class UltraDevice {
 public:
  explicit UltraDevice(DeviceComInterface* com_port_device);
  ~UltraDevice();

 public:
  int32_t Open(const anx::device::ComSettings& com_settings);
  void Close();
  
 private:
  DeviceComInterface* port_device_;
  ComPortDevice* com_port_device_;
};

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_ULTRASONIC_ULTRA_DEVICE_H_
