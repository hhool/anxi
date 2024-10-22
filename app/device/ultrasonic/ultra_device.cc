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

#include "app/common/crc16.h"
#include "app/common/time_utils.h"

namespace anx {
namespace device {

UltraDevice::UltraDevice(DeviceComInterface* port_device)
    : port_device_(port_device), is_ultra_started_(false) {
  port_device_->AttachDeviceNode(this);
}

UltraDevice::~UltraDevice() {
  Close();
  port_device_->DetachDeviceNode(this);
}

DeviceComInterface* UltraDevice::GetPortDevice() {
  return port_device_;
}

int32_t UltraDevice::Open(const anx::device::ComSettings& com_settings) {
  if (port_device_ == nullptr) {
    return -1;
  }

  if (port_device_->Open(com_settings) != 0) {
    return -2;
  }

  return 0;
}

void UltraDevice::Close() {
  if (port_device_ != nullptr) {
    port_device_->Close();
  }
  is_ultra_started_ = false;
}

bool UltraDevice::isOpened() {
  if (port_device_ == nullptr) {
    return false;
  }

  return port_device_->isOpened();
}

int32_t UltraDevice::StartUltra() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x05, 0x00, 0x02, 0xFF, 0x00, 0x2D, 0xFA};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 8) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 8) {
    return -3;
  }
  if (memcmp(hex, hex_res, 8) != 0) {
    return -4;
  }
  is_ultra_started_ = true;
  return 0;
}

int32_t UltraDevice::StopUltra() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x05, 0x00, 0x02, 0x00, 0x00, 0x6C, 0x0A};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 8) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 8) {
    return -3;
  }
  if (memcmp(hex, hex_res, 8) != 0) {
    return -4;
  }
  is_ultra_started_ = false;
  return 0;
}

bool UltraDevice::IsUltraStarted() {
  if (port_device_ == nullptr) {
    return false;
  }

  return is_ultra_started_;
}

int32_t UltraDevice::GetFaultCode() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x04, 0x00, 0x02, 0x00, 0x01, 0x90, 0x0A};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  /// get fault code
  int32_t fault_code = hex_res[3] * 256 + hex_res[4];
  return fault_code;
}

int32_t UltraDevice::GetCurrentFreq() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0A};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  /// get freq
  int32_t freq = hex_res[3] * 256 + hex_res[4];
  return freq;
}

int32_t UltraDevice::GetCurrentPower() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }
  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  /// get power
  int32_t power = hex_res[3] * 256 + hex_res[4];
  return power;
}

int32_t UltraDevice::SetAmplitude(int32_t amplitude) {
  if (port_device_ == nullptr) {
    return -1;
  }
  // amplitude [20, 100]
  if (amplitude < 20 || amplitude > 100) {
    return -2;
  }
  // trans to hex
  uint8_t hex[8] = {0x01, 0x06, 0x00, 0x18, 0x00};
  hex[5] = amplitude;
  uint16_t crc = anx::common::crc16(hex, 6);
  hex[6] = crc & 0xFF;
  hex[7] = (crc & 0xFF00) >> 8;
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written != 8) {
    return -3;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 8) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 8) {
    return -4;
  }
  if (memcmp(hex, hex_res, 8) != 0) {
    return -5;
  }
  return 0;
}

int32_t UltraDevice::GetAmplitude() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x03, 0x00, 0x18, 0x00, 0x01, 0x04, 0x0D};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written != 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  if (hex_res[0] != 0x01 || hex_res[1] != 0x03 || hex_res[2] != 0x02) {
    return -4;
  }
  int32_t value = hex_res[4];
  if (value < 1 || value > 100) {
    return -5;
  }
  return value;
}

int32_t UltraDevice::SetWedingTime(int32_t time_sec) {
  if (port_device_ == nullptr) {
    return -1;
  }
  if (time_sec < 0 || time_sec > 99) {
    return -2;
  }
  uint8_t hex[8] = {0x01, 0x06, 0x00, 0x19, 0x00};
  hex[5] = time_sec;
  uint16_t crc = anx::common::crc16(hex, 6);
  hex[6] = crc & 0xFF;
  hex[7] = (crc & 0xFF00) >> 8;
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -3;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 8) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 8) {
    return -4;
  }
  if (memcmp(hex, hex_res, 8) != 0) {
    return -5;
  }
  return 0;
}

int32_t UltraDevice::GetWedingTime() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x03, 0x00, 0x19, 0x00, 0x01, 0x55, 0xCD};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  if (hex_res[0] != 0x01 || hex_res[1] != 0x03 || hex_res[2] != 0x02) {
    return -4;
  }
  // value is pos 3 and 4
  int32_t value = hex_res[3] * 256 + hex_res[4];
  if (value < 0 || value > 999) {
    return -5;
  }
  return value;
}

int32_t UltraDevice::GetMaxFreq() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  if (hex_res[0] != 0x01 || hex_res[1] != 0x03 || hex_res[2] != 0x02) {
    return -4;
  }
  // value is pos 3 and 4
  int32_t value = hex_res[3] * 256 + hex_res[4];
  if (value < 1 || value > 0xEFFF) {
    return -5;
  }
  return value;
}

int32_t UltraDevice::GetMinFreq() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC5, 0xCB};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  if (hex_res[0] != 0x01 || hex_res[1] != 0x03 || hex_res[2] != 0x02) {
    return -4;
  }
  // value is pos 3 and 4
  int32_t value = hex_res[3] * 256 + hex_res[4];
  if (value < 1 || value > 0xEFFF) {
    return -5;
  }
  return value;
}

int32_t UltraDevice::GetMaxPower() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xCA};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written <= 0) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  if (hex_res[0] != 0x01 || hex_res[1] != 0x03 || hex_res[2] != 0x02) {
    return -4;
  }
  // value is pos 3 and 4
  int32_t value = hex_res[3] * 256 + hex_res[4];
  if (value < 1 || value > 0xEFFF) {
    return -5;
  }
  return value;
}

int32_t UltraDevice::GetFreqAtMachineOn() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  if (hex_res[0] != 0x01 || hex_res[1] != 0x03 || hex_res[2] != 0x02) {
    return -4;
  }
  // value is pos 3 and 4
  int32_t value = hex_res[3] * 256 + hex_res[4];
  if (value < 1 || value > 0xEFFF) {
    return -5;
  }
  return value;
}

int32_t UltraDevice::GetSoftTimeAtMachineOn() {
  if (port_device_ == nullptr) {
    return -1;
  }

  uint8_t hex[8] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA};
  int32_t written = port_device_->Write(hex, sizeof(hex));
  if (written < 8) {
    return -2;
  }

  uint8_t hex_res[64] = {0};
  int32_t try_count = 0;
  int32_t readed = 0;
  while (try_count < 10) {
    int32_t r = port_device_->Read(hex_res + readed, sizeof(hex_res) - readed);
    if (r > 0) {
      readed += r;
    }
    if (readed >= 7) {
      break;
    }
    try_count++;
    anx::common::sleep_ms(10);
  }
  if (readed < 7) {
    return -3;
  }
  if (hex_res[0] != 0x01 || hex_res[1] != 0x03 || hex_res[2] != 0x02) {
    return -4;
  }
  // value is pos 3 and 4
  int32_t value = hex_res[3] * 256 + hex_res[4];
  if (value < 1 || value > 0xEFFF) {
    return -5;
  }
  return value;
}
}  // namespace device
}  // namespace anx
