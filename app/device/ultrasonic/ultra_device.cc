/**
 * @file ultra_device.cc
 * @author hhool (hhool@outlook.com)
 * @brief ultrasonic device class implementation
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/ultrasonic/ultra_device.h"

#include <iostream>

namespace anx {
namespace device {

UltraDevice::UltraDevice(DeviceComInterface* port_device)
    : port_device_(port_device) {}

UltraDevice::~UltraDevice() {
  Close();
}

int32_t UltraDevice::Open(const anx::device::ComSettings& com_settings) {
  if (port_device_ == nullptr) {
    return -1;
  }

  if (port_device_->Open(com_settings) != 0) {
    return -1;
  }

  return 0;
}

void UltraDevice::Close() {
  if (port_device_ != nullptr) {
    port_device_->Close();
  }
}

}  // namespace device
}  // namespace anx
