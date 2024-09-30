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
#include "app/device/device_com.h"
#include "app/device/device_com_settings.h"

namespace anx {
namespace device {
class ComPortDevice;
class UltraDevice : public DeviceNode {
 public:
  explicit UltraDevice(DeviceComInterface* com_port_device);
  ~UltraDevice();

 public:
  /// @brief  Get port device interface
  /// @return port device interface
  DeviceComInterface* GetPortDevice();
  /// @brief  Open device utrasonic
  /// @param com_settings  com settings
  /// @return success 0, failed -1
  int32_t Open(const anx::device::ComSettings& com_settings);
  /// @brief  Close device utrasonic
  /// @return void
  void Close();
  /// @brief  Check device is open
  /// @return true is open, false is close
  bool isOpened();
  /// @brief  Start ultra
  /// send: 01 05 00 02 FF 00 2D FA
  /// response: 01 05 00 02 FF 00 2D FA
  /// @return success 0, failed -1
  int32_t StartUltra();
  /// @brief  Stop ultra
  /// send: 01 05 00 02 00 00 8C 0A
  /// response: 01 05 00 02 00 00 8C 0A
  /// @return success 0, failed -1
  int32_t StopUltra();
  /// @brief  Check ultra is started
  /// @return true is started, false is stoped
  bool IsUltraStarted();
  /// @brief  Get fault code
  /// send: 01 04 00 02 00 01 90 0A
  /// response: 01 04 02 00 00 B9 30
  /// value is 0x0000 means no fault
  /// @return fault code or 0 means no fault
  int32_t GetFaultCode();
  /// @brief  Get current freq
  /// send: 01 04 00 01 00 01 60 0A
  /// response: 01 04 02 4D 97 CD CE
  /// value is 0x4D97 = 19863
  int32_t GetCurrentFreq();
  /// @brief  Get current power
  /// send: 01 04 00 00 00 01 31 CA
  /// response: 01 04 02 00 2A 38 EF
  /// value is 0x002A = 42
  /// @return current power or error < 0
  int32_t GetCurrentPower();
  /// @brief  Set ultra amplitude [20, 100]
  /// send: 01 06 00 18 00 14 90 C2
  /// receive: 01 06 00 18 00 14 90 C2
  /// amplitude is 0x14 = 20
  /// @param amplitude  [20, 100]
  /// @return success 0, failed -1
  int32_t SetAmplitude(int32_t amplitude);
  /// @brief  Get ultra amplitude [20, 100]
  /// send: 01 03 00 18 00 01 04 0D
  /// receive: 01 03 02 00 14 B8 4B
  /// amplitude is 0x14 = 20
  /// @return amplitude [20, 100] or error < 0
  int32_t GetAmplitude();
  /// @brief  Set ultra work time [1, 999]
  /// send: 01 06 00 19 00 C8 59 9B
  /// receive: 01 06 00 19 00 C8 59 9B
  /// value is 0xC8 = 200
  /// send: 01 06 00 19 01 90 59 f1
  /// response: 01 06 00 19 01 90 59 f1
  /// time is 0x0190 = 400
  /// send: 01 06 00 19 02 58 58 97
  /// response: 01 06 00 19 02 58 58 97
  /// time is 0x0258 = 600
  /// @param time_sec  [1, 99]
  /// @return success 0, failed -1
  int32_t SetWedingTime(int32_t time_sec);
  /// @brief  Get ultra Weding time [1, 999] mean [0.1,99.9]S
  /// send: 01 03 00 19 00 01 55 CD
  /// receive: 01 03 02 00 C8 59 9B
  /// value is 0xC8 = 200
  /// @return Weding time [1, 999] mean [0.1,99.9]S or error < 0
  int32_t GetWedingTime();
  /// @brief  Get max freq
  /// send: 01 03 00 03 00 01 74 0A
  /// receive: 01 03 02 50 14 84 4B
  /// value: 0x5014 = 20404
  /// @return max freq or error < 0
  int32_t GetMaxFreq();
  /// @brief  Get min freq
  /// send: 01 03 00 04 00 01 C5 CB
  /// response: 01 03 02 4B 00 8E B4
  /// value: 0x4B00 = 19200
  /// @return min freq or error < 0
  int32_t GetMinFreq();
  /// @brief  Get max power
  /// send: 01 03 00 02 00 01 25 CA
  /// receive: 01 03 02 05 DC BA 8D
  /// value: 0x05DC = 1500
  /// @return max power or error < 0
  int32_t GetMaxPower();
  /// @brief  Get freq at machine on
  /// send: 01 03 00 00 00 01 84 0A
  /// response: 01 03 02 4D 62 0C FD
  /// value: 0x4D62 = 19810
  /// @return freq at machine on or error < 0
  int32_t GetFreqAtMachineOn();
  /// @brief  Get soft time at machine on
  /// send: 01 03 00 01 00 01 D5 CA
  /// response: 01 03 02 00 64 B9 AF
  /// value: 0x0064 = 100
  int32_t GetSoftTimeAtMachineOn();

 private:
  DeviceComInterface* port_device_;
  bool is_ultra_started_;
};

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_ULTRASONIC_ULTRA_DEVICE_H_
